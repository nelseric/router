#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

#include "test.h"
#include "trie.h"


void test(trie_t *top, char * testfile){
    FILE * tests = fopen(testfile, "r");

    char buf[128];

    int passed = 0;
    int failed = 0;

    while(!feof(tests)){
        if(fgets(buf, 128, tests) == NULL){
            if(ferror(tests)){
                perror("file");
                exit(1);
            } else {
                break;
            }
        }

        if(strlen(buf) < 7){
            break;
        }

        char *  input_temp;
        char *  dest_temp;
        input_temp = strtok(buf, "\t");
        dest_temp = strtok(NULL, "\n");        

        uint32_t dest, input;
        inet_pton(AF_INET, input_temp, &input);
        inet_pton(AF_INET, dest_temp, &dest);

        input = ntohl(input);
        dest = ntohl(dest);

        uint32_t res;
        res = trie_lookup(top, input);  
        if(dest != res){
            char ipbuf[INET_ADDRSTRLEN];
            res = htonl(res);
            inet_ntop(AF_INET, &res, ipbuf, INET_ADDRSTRLEN);

            printf("lookup(%s)=\"%s\", expected \"%s\"\n", input_temp, ipbuf, dest_temp );
            failed++;
        } else {
            passed++;
        }
    }

    fclose(tests);
    printf("%d of %d tests passed, %d tests failed.\n", passed, passed+failed, failed);
}
