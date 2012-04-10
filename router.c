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
    
    srand(time(0));
    for(int i = 0; i < max; i++){
        struct timespec start, stop;

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        trie_lookup(top, rand());
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        printf("%ld\n", diff(start, stop).tv_nsec);
    }

    // test(top, "samples.txt");
    // trie_print_graph(top);

    trie_free(top);
    return EXIT_SUCCESS;
}
