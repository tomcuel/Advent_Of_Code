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

// Comparison function for qsort
int compare(const void *a, const void *b) {
    // Cast pointers to uint64_t and compare
    uint64_t x = *((uint64_t*)a);
    uint64_t y = *((uint64_t*)b);
    
    if (x < y) return -1;
    else if (x > y) return 1;
    else return 0;
}

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
    // storing the final scores, it will be 0 if the line isn't valid, and the score of the line to complete otherwise
    u_int64_t* final_scores = (u_int64_t*)malloc(nb_lines * sizeof(u_int64_t));
    for (int i = 0; i < nb_lines; i++){
        final_scores[i] = 0;
    }

    for (int i = 0; i < nb_lines; i++){
        // if the line isn't valid, we add the score of the line to the answer
        char* str_score = false_line_result(lines[i]);
        u_int64_t int_score = atoi(str_score);
        // if the line isn't completed, the final score array at the line i will be the score of the non completed line 
        if (int_score == 0){
            final_scores[i] = non_completed_line_result(lines[i]);
        }
    }

    // we need to get the middle value of the array, without considering the 0 values
    // we need to count the number of zero values (there will be p)
    // we need to get the value at the index p+len(final_scores)/2
    int q = 0;
    for (int i = 0; i < nb_lines; i++){
        if (final_scores[i] == 0){
            q++;
        }
    }

    // making the final score list without the 0 values
    u_int64_t* final_scores_without_0 = (u_int64_t*)malloc((nb_lines-q) * sizeof(u_int64_t));
    int k = 0;
    for (int i = 0; i < nb_lines; i++){
        if (final_scores[i] != 0){
            final_scores_without_0[k] = final_scores[i];
            k++;
        }
    }

    // Sorting the array
    qsort(final_scores_without_0, k, sizeof(uint64_t), compare);

    // printing the final sorted scores
    printf("final_scores_without_0 :\n");
    for (int i = 0; i < k; i++){
        printf("%llu\n",final_scores_without_0[i]);
    }

    int middle_index = k/2;
    printf("middle_index = %d\n", middle_index);
    u_int64_t middle_value = final_scores_without_0[middle_index];
    // print the answer
    printf("\nThe answer is %llu\n\n", middle_value);


    // free the memory
    for (int i = 0; i < nb_lines; i++){
        free(lines[i]);
    }
    free(lines);

    free(final_scores);

    free(final_scores_without_0);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 288957 for the test.txt file as expected