#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#include "integer_set.h"

/*
Problem : a seven-line display where lines are represented by letters 
but they've been mixed up
input : represents the segment that are turned on when each number is displayed (mixed up)
output : the number that represents a unique code 
seris of 2 segments == 1
series of 7 segments == 8
………
We're searching for the number of times 2,3,4,7 appear in the input
*/

int main (){

    FILE* file;
    file = fopen("input8.txt", "r");
    char line[100];

    // reading the file
    int nb_lines = 0;
    while (fgets(line, 100, file) != NULL){
        nb_lines++;
    }
    // printf("Number of lines: %d\n", nb_lines);
    rewind(file);

    // left part of the file
    char*** input=malloc(nb_lines*sizeof(char**));
    for (int i = 0; i < nb_lines; i++){
        input[i] = malloc(10*sizeof(char*));
        for (int j = 0; j < 10; j++){
            input[i][j] = malloc(20*sizeof(char));
        }
    }

    // right part of the file
    char*** output=malloc(nb_lines*sizeof(char**));
    for (int i = 0; i < nb_lines; i++){
        output[i] = malloc(4*sizeof(char*));
        for (int j = 0; j < 10; j++){
            output[i][j] = malloc(20*sizeof(char));
        }
    }
    

    // getting the data from the file
    for (int i = 0; i < nb_lines; i++){
        fscanf(file, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s\n", input[i][0], input[i][1], input[i][2], input[i][3], input[i][4], input[i][5], input[i][6], input[i][7], input[i][8], input[i][9], output[i][0], output[i][1], output[i][2], output[i][3]);
    }

    /*
    // printing the data
    printf("Data:\n");
    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < 10; j++){
            printf("%s ", input[i][j]);
        }
        printf("| ");
        for (int j = 0; j < 4; j++){
            printf("%s ", output[i][j]);
        }
        printf("\n");
    }
    */

    integer_set_t set;
    integer_set_init(&set);
    integer_set_insert(&set, 2);
    integer_set_insert(&set, 3);
    integer_set_insert(&set, 4);
    integer_set_insert(&set, 7);

    // going trough the output data
    int answer=0;
    // for each line
    for (int i = 0; i < nb_lines; i++){
        // for each number of the output
        // since we've taken those numbers who are the only one with that number of segments, they're unique, there isn't any need to check if they're unique
        for (int j = 0; j < 4; j++){
            if (integer_set_is_member(&set, strlen(output[i][j]))){
                answer++;
            }
        }
    }

    // printing the answer
    printf("\nThe answer is : %d\n\n", answer);


    // freeing the memory
    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < 10; j++){
            free(input[i][j]);
        }
        free(input[i]);
    }
    free(input);

    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < 4; j++){
            free(output[i][j]);
        }
        free(output[i]);
    }

    integer_set_destroy(&set);

    // closing the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 26 with the test.txt file as expected