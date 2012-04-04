#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "parse.h"
#include "trie.h"
#include <arpa/inet.h>



int main(int argc, char *argv[]){
    
    trie_t *top = trie_new();
	
	char *lookstr;
	uint8_t max = 32;

    if(argc >= 2){
    	lookstr = argv[1];
    	if(argc >= 3){
    		max = atoi(argv[2]);
    	}
    }else {
    	lookstr = "123.4.5.6";
    }

    parse("bgpsample.txt", top, max);

    
	uint32_t lookaddr;
	inet_pton(AF_INET, lookstr, &lookaddr);

    uint32_t look = htonl(trie_lookup(top, lookaddr));

    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &look, buf, INET_ADDRSTRLEN);

    printf("%s - %s\n", lookstr, buf);
    // trie_print_graph(top);

    trie_free(top);
    return EXIT_SUCCESS;
}
