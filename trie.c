#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

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

void trie_add(trie_t *top, uint32_t prefix, uint8_t class, uint8_t oc, uint32_t value){
    if(class == 0){
        if(oc > top->class){
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
                trie_add(top, duprefix | i, STRIDE, oc, value);
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
uint32_t trie_lookup(trie_t *top, uint32_t address){
    return 0;
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
	}
	printf("%ld [label=\"%s\"];\n", thisid, vstring);

	for(int i = 0; i < 1<<STRIDE; i++){
		if(top->children[i] != NULL){
			int c = trie_print(top->children[i]);
			if(c >= 0)
				printf("%ld--%d [label=\"%d%d\"];\n",thisid, c, i&2, i&1);
		}
	}
	return thisid;
}
