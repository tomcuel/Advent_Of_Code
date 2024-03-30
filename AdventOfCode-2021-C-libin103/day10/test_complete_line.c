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

char* bad_score[4][2]= {
    {")", "3"},
    {"]", "57"},
    {"}", "1197"},
    {">", "25137"}
};

char* good_score[4][2]= {
    {"(", "1"},
    {"[]", "2"},
    {"{}", "3"},
    {"<", "4"}
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
                    return bad_score[i][1];
                }
            }
        }
    }
    
    // if the character is a pair, we need to return 0 since it will need not to affect the final score
    // we need to destroy the stack before returning to free the memory
    character_stack_destroy(&stack);
    return "0";
}


// function to score a good line
uint64_t non_completed_line_result(const char* line){
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
            }
        }
    }
    // we've done the same process as the false line, to put the entrance into the stack and if there is a corresponding closing, we pop the entrance so it's not in memory anymore
    // we're left only with the non completed entrance in the stack 
    
    uint64_t answer = 0;
    // we iterate over the stack to get the answer
    while (character_stack_size(&stack) > 0){
        // we pop the character on top of the stack to treat it
        char popped;
        character_stack_pop(&stack, &popped);
        // formula of the puzzle 
        answer*=5;
        // we need to get the score of the character "popped" and add it to the answer
        for (int i = 0; i < 4; i++){
            if (popped == good_score[i][0][0]){
                answer += atoi(good_score[i][1]);
                // we need to break the "for" loop since we found the score of the character
                break;
            }
        }
    }


    // if the character is a pair, we need to return 0 since it will need not to affect the final score
    // we need to destroy the stack before returning to free the memory
    character_stack_destroy(&stack);
    return answer;

}

int main() {
    char* line = "{<[[]]>}<{[{[{[]{()[[[]";

    char* str_score = false_line_result(line);
    u_int64_t int_score = atoi(str_score);
    // if the line isn't completed, the final score array at the line i will be the score of the non completed line 
    uint64_t result = 0;
    if (int_score == 0){
        result= non_completed_line_result(line);
    }
    
    printf("Result: %llu\n", result);
    return 0;
}


// everything is working fine for the differents tests provided in the puzzle of day 10
// I was using it since my main function didn't work, but now it's working fine
// the problem was on how i was sorting my final array to get the middle number of the array that had an odd length