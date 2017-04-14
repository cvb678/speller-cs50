/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

#define TRIELENGTH 27

typedef struct node {
    bool isWord;
    struct node * next;
} node_t;

bool initialize (node_t * arr) {
    
    int i;
    
    for(i=0; i<TRIELENGTH; i++) {
        arr[i].isWord = false;
        arr[i].next = NULL;
    }
    
    return true;
}

void recursive (node_t * arr) {
    
    int i;
    
    for(i=0; i<TRIELENGTH; i++) {
        if(arr[i].next != NULL) {
            recursive(arr[i].next);
        }
    }
    
    free(arr);
}

node_t * trie;
int words = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int index, i = 0;
    node_t * tempArray = trie;
    
    while(word[i] != '\00') {
        
        if(word[i] == '\'') {
                index = 26;
        }
        else {
            index = (int)(toupper(word[i])-'A');  //get trie index out of character   
        }
        
        if(tempArray[index].next == NULL) {
            return false;
        }
        tempArray = tempArray[index].next;
        i++;
    }
    
    if(!tempArray[index].isWord) {
        return false;
    }
    
    return true;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    trie = malloc(sizeof(node_t)*TRIELENGTH);
    FILE* inptr = fopen(dictionary, "r");
    
    if(inptr == NULL) {
        printf("Couldn't open a file!\n");
        return false;
    }
    
    if(!initialize(trie)) {
        printf("Couldn't initialize trie!\n");
        return false;
    }
    
    char line[LENGTH+2];  //+2 for terminating sequence \n\00
    int i, index;
    node_t * tempArray;
    
    while(fgets(line, sizeof(line), inptr)) {
        tempArray = trie;
        i=0;
        
        while(line[i] != '\n')  //lines ending with \n\00
        {
            if(line[i] == '\'') {
                index = 26;
            }
            else {
                index = (int)(toupper(line[i])-'A');  //get trie index out of character   
            }
            
            if(tempArray[index].next == NULL) { 
                tempArray[index].next = malloc(TRIELENGTH*sizeof(node_t));
                initialize(tempArray[index].next);
            }
            
            tempArray = tempArray[index].next;
            i++;
        }
        
        tempArray[index].isWord = true;
        words++;
    
    }
    fclose(inptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    recursive(trie);   
    return true;
}
