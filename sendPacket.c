//
//  sendPacket.c
//  
//
//  Created by Akmal Fadlurohman on 10/16/17.
//
//

#include "sendPacket.h"

char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    int len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(int i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}
