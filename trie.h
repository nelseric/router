#ifndef _ROUTER_H_
#define _ROUTER_H_
#include <stdint.h>

#define STRIDE 2
#define TRIE_PAGE_SIZE 1000000
#define TRIE_MAX_PAGES 4


typedef struct trie {
    uint32_t value;
    uint8_t class;
    struct trie *children[1<<STRIDE];
} trie_t;

void trie_add(trie_t *top, uint32_t prefix, uint8_t class, uint8_t oc, uint32_t value);
uint32_t trie_lookup(trie_t *top, uint32_t address);
trie_t * trie_new(); //Dynamically allocate a new trie
void trie_free(trie_t * top); // free all the trie's children
void trie_print_graph(trie_t * top);
long trie_print(trie_t *top);

#endif
