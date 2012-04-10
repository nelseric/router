#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <arpa/inet.h>
#include "trie.h"

void parse(char *filename, trie_t *top, int maxclass){
    FILE *f = fopen(filename, "r");

    char buf[128];

    while(!feof(f)){
        if(fgets(buf, 128, f) == NULL){
            if(ferror(f)){
                perror("file");
                exit(1);
            } else {
                break;
            }
        }
        if(strlen(buf) < 7){
            break;
        }

        char *tmp_prefix = strtok(buf, " ");
        char *tmp_dest = strtok(NULL, " ");
        tmp_dest = strtok(tmp_dest, "\n");


        tmp_prefix = strtok(tmp_prefix, "/");
        char *tmp_class = strtok(NULL, "/");


        uint32_t dest, prefix;
        inet_pton(AF_INET, tmp_prefix, &prefix);
        inet_pton(AF_INET, tmp_dest, &dest);
        uint8_t class = atoi(tmp_class);
        prefix = ntohl(prefix);
        dest = ntohl(dest);
        // int a = 0;
        // for(int bit = 31; bit >= 0; bit--){
        //     printf("%d", (prefix>>bit) & 1);
        //     if(++a%8 == 0)
        //         printf(" ");
        // }

        // printf("\t%s/%d", tmp_prefix, class);

        // puts("");
        // printf("%s/%s %s\t", tmp_prefix, tmp_class, tmp_dest);
        // printf("%u/%u %u\n", prefix, class, dest);
        if(class <= maxclass){
            trie_add(top, prefix, class, class, dest);
        }

    }



    fclose(f);    
}
