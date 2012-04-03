#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "parse.h"
#include "trie.h"



int main(int argc, char *argv[]){
    
    trie_t *top = trie_new();

	parse(argv[1], top);

    trie_print_graph(top);
    trie_free(top);
    return EXIT_SUCCESS;
}
