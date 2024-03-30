#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#include "character_stack.h"

char pairs[4][2]={"()", "[]", "{}", "<>"};

char* scores[4][2]= {
    {")", "3"},
    {"]", "57"},
    {"}", "1197"},
    {">", "25137"}
};


// function to score a false line
char* false_line_result(const char* line) {
    character_stack_t stack;
    character_stack_init(&stack);
    // append in the stack is putting a character in the stack and incrementing the top
    // pop in the stack is decrementing the top while returning the character at the previous top

    // we iterate over the line, character by character
    for (const char* ptr = line; *ptr != '\0'; ptr++) {
        // going through the different pairs, good is true if the character is a pair, stay false otherwise
        for (int i = 0; i < 4; i++) {
            // if the character is an opening of a pair, we add it to the stack and set good to true
            if (*ptr == pairs[i][0]) {
                character_stack_push(&stack, *ptr);
            } 
            // if the character is a closing of a pair, we check if the top of the stack is the corresponding opening
            // and set good to true if it is the case
            else if (*ptr == pairs[i][1]) {
                if (character_stack_peek(&stack) == pairs[i][0]) {
                    char popped;
                    character_stack_pop(&stack, &popped);
                } 
                // otherwise, we return the score of the pair since it is a false line
                else {
                    // we need to destroy the stack before returning to free the memory
                    character_stack_destroy(&stack);
                    return scores[i][1];
                }
            }
        }
    }
    
    // if the character is a pair, we need to return 0 since it will need not to affect the final score
    // we need to destroy the stack before returning to free the memory
    character_stack_destroy(&stack);
    return "0";
}


int main (){

    FILE* file = fopen("input10.txt", "r");
    char line[256];
    int nb_lines = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        nb_lines++;
    }
    // printf("nb_lines = %d\n", nb_lines);

    // storing the lines in an array
    char** lines = (char**)malloc(nb_lines * sizeof(char*));
    for (int i = 0; i < nb_lines; i++){
        lines[i] = (char*)malloc(256 * sizeof(char));
    }
    rewind(file);
    int p=0;
    while (fgets(line, sizeof(line), file) != NULL){
        strcpy(lines[p], line);
        p++;
    }
    /*
    // printing the lines
    printf("lines :\n");
    for (int i = 0; i < nb_lines; i++){
        printf("lines[%d] = %s\n", i, lines[i]);
    }
    */

   long int answer = 0;
   for (int i = 0; i < nb_lines; i++){
        // if the line isn't valid, we add the score of the line to the answer
        char* score = false_line_result(lines[i]);
        // printf("score = %s\n", score);
        answer += atoi(score);
    }

    // print the answer
    printf("\nThe answer is %ld\n\n", answer);


    // free the memory
    for (int i = 0; i < nb_lines; i++){
        free(lines[i]);
    }
    free(lines);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 26397 for the test.txt file as expected