#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#ifdef CMEM
static size_t curpage = -1;
static size_t nmemb = TRIE_PAGE_SIZE;
static trie_t *pages[4];

trie_t * trie_new(){
    if(nmemb == TRIE_PAGE_SIZE){
        nmemb = 0;
        curpage++;
        pages[curpage] = calloc(TRIE_PAGE_SIZE, sizeof(trie_t));
    }


    return pages[curpage] + nmemb++;
}

void trie_free(trie_t * top){
    for(int i = 0; i <= curpage; i++){
        free(pages[i] );
    }
}

#else

trie_t * trie_new(){
    return (trie_t *) calloc(1, sizeof(trie_t));   
}

void trie_free(trie_t * top){
    for(int i = 0; i < 1<<STRIDE; i++){
        if(top->children[i] != NULL){
            trie_free(top->children[i]);
        }
    }
    free(top);
}

#endif



void trie_add(trie_t *top, uint32_t prefix, uint8_t class, uint8_t oc, uint32_t value){
    if(class == 0){
        if(oc >= top->class){
            top->value = value;
            top->class = oc;
        }
    } else {
        uint32_t mask = ~(UINT32_MAX>>STRIDE);	
        if(class < STRIDE){ // Do we need to insert duplicates?
            int dupes = 1<<(STRIDE - class);
            uint32_t duprefix = (prefix & mask)>>(32-STRIDE) //Get the top STRIDE bits
                & (UINT32_MAX<<(STRIDE-class)); //Make sure dupe non padding bits are 0
            for(int i = 0; i < dupes; i++){
                trie_add(top, (duprefix | i)<<(32-STRIDE), STRIDE, oc, value);
            }
        } else {
            int n = (prefix & mask) >> (32-STRIDE);
            if(top->children[n] == NULL){
                top->children[n] = trie_new();
            }
            trie_add(top->children[n], prefix<<STRIDE, class-STRIDE, oc, value);
        }
    }
}
#ifdef _ITLKUP
uint32_t trie_lookup(trie_t *top, uint32_t address){
    int n = address>>(32-STRIDE);
    uint32_t val = top->value;
    while(top->children[n] != NULL){
        top = top->children[n];
        val = top->value?top->value:val;
        address = address<<2;
        n = address>>(32-STRIDE);
    }
    return val;
}




#else
uint32_t trie_lookup(trie_t *top, uint32_t address){
    uint8_t n = address>>(32-STRIDE);
    if(top->children[n] != NULL){
        uint32_t ch = trie_lookup(top->children[n], address<<STRIDE);
        return ch>0?ch:top->value;
    } else {
        return top->value;	
    }
}
#endif

uint32_t trie_lookupi(trie_t *top, uint32_t address){
    int n = address>>(32-STRIDE);
    uint32_t val = top->value;
    while(top->children[n] != NULL){
        top = top->children[n];
        val = top->value?top->value:val;
        address = address<<2;
        n = address>>(32-STRIDE);
    }
    return val;
}

uint32_t trie_lookupr(trie_t *top, uint32_t address){
    uint8_t n = address>>(32-STRIDE);
    if(top->children[n] != NULL){
        uint32_t ch = trie_lookupr(top->children[n], address<<STRIDE);
        return ch>0?ch:top->value;
    } else {
        return top->value;  
    }
}

void trie_print_graph(trie_t *top){
    puts("graph trie {");
    trie_print(top);
    puts("}");
}

long trie_print(trie_t *top){
    static long id = 0;
    long thisid = id++;
    char vstring[INET_ADDRSTRLEN] = "";
    if(top->value){
        uint32_t tmp = htonl(top->value);
        inet_ntop(AF_INET, &tmp, vstring, INET_ADDRSTRLEN);
        printf("%ld [label=\"%s {%d}\"];\n", thisid, vstring, top->class);
    } else {
        printf("%ld [label=\"\"];\n", thisid);
    }

    for(int i = 0; i < 1<<STRIDE; i++){
        if(top->children[i] != NULL){
            int c = trie_print(top->children[i]);
            if(c >= 0){
                printf("%ld--%d [label=\"",thisid, c);
                for(int bit = STRIDE-1; bit >= 0; bit--){
                    printf("%d", (i>>bit) & 1);
                }
                puts("\"];");
            }
        }
    }
    return thisid;
}
