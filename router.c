#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <time.h>
#include "parse.h"
#include "trie.h"
#include "test.h"

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}


int main(int argc, char *argv[]){

    trie_t *top = trie_new();

    int max = 1000;

    if(argc >= 2){
        max = atoi(argv[1]);
    }

    parse("bgpsample.txt", top, max);
#ifndef _UNIT_TEST    
    srand(time(0));
    long it = 0, r = 0, h=0, m=0;
    for(int i = 0; i < max; i++){
        struct timespec start, stop, iter, rec;
        uint32_t lkup = rand();
        uint32_t rx, ix;
        uint32_t x = trie_lookup(top, lkup);
/*/
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        ix = trie_lookupr(top, lkup);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        rec = diff(start, stop);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        rx = trie_lookupi(top, lkup);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        iter = diff(start, stop);
/*///

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        rx = trie_lookupi(top, lkup);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        iter = diff(start, stop);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        ix = trie_lookupr(top, lkup);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        rec = diff(start, stop);
//*/
        if(x==(96<<24|4<<16|55))

            // printf("m");
            m++;
        else{
            // printf("h");
            h++;
        }
        if(iter.tv_nsec > rec.tv_nsec){
            r++;
        } else {
            it++;
        }
        //printf("%s %ld %ld %s\n",iter.tv_nsec > rec.tv_nsec?"R":"I", iter.tv_nsec, rec.tv_nsec , (x == rx && x== ix)?"":"FAIL");
    }
    printf("%f%% hit rate, %.1f%% iterative wins, %.1f%% recursive wins.\n", (100.0*h)/(h+m), (100.0*it)/(r+it), (100.0*r)/(r+it));
#else

    test(top, "samples.txt");

#endif
    // trie_print_graph(top);

    trie_free(top);
    return EXIT_SUCCESS;
}
