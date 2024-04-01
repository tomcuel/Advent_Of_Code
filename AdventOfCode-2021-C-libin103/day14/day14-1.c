#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  

#define MAX_LENGTH 100000

int main (){

    FILE* file= fopen("input14.txt", "r");

    char line[30];
    int nb_lines=0;
    while(fgets(line, sizeof(line), file)){
        nb_lines++;
    }

    rewind(file);
    // getting the polymer we will start with
    fgets(line, sizeof(line), file);
    char* polymer=strdup(line);
    printf("Starting Polymer: %s\n", polymer);
    fgets(line, sizeof(line), file);

    // number of insertions
    int nb_insertions=nb_lines-2;
    printf("nb_insertions: %d\n", nb_insertions);

    // getting the commands for the insertions
    char** insertions=malloc(nb_insertions*sizeof(char*));
    for(int i=0; i<nb_insertions; i++){
        insertions[i]=malloc(3*sizeof(char));
    }
    int k=0;
    while(fgets(line, sizeof(line), file)){
        insertions[k][0]=line[0];
        insertions[k][1]=line[1];
        insertions[k][2]=line[6];
        // printf("Insertion : %c bewteen %c and %c\n", insertions[k][2], insertions[k][0], insertions[k][1]);
        ++k;
    }

    // number of times we need to do the insertions
    int nb_steps=10;
    for (int i=0; i<nb_steps; i++){

        // at each step, we will insert the characters, but we need to consider all the possible insertions at the same time 
        // we will look if something can be inserted between polymer[j] and polymer[j+1] for all j
        // so we will create a new polymer that will contain all the insertions at the end

        char* new_polymer = malloc(MAX_LENGTH * sizeof(char)); // Allocate memory for the new polymer, allocate enough memory for the new polymer to contain all the insertions
        int new_polymer_index = 0; // Index for the new polymer

        for (int i = 0; i < strlen(polymer) - 1; i++) {
            new_polymer[new_polymer_index++] = polymer[i]; // Copy the current character in index i to the new polymer
            // it will copy the one in index i+1, the next step

            // going throught the insertions to see if we can insert something between polymer[i] and polymer[i+1]
            // we consider that theren't any overlapping insertions
            for (int j = 0; j < nb_insertions; j++) {
                if (polymer[i] == insertions[j][0] && polymer[i + 1] == insertions[j][1]) {
                    new_polymer[new_polymer_index++] = insertions[j][2]; // Insert the character between polymer[i] and polymer[i+1]
                    break; // Break the loop, we don't need to check the other insertions, and we gain time complexity
                }
            }

        }
        new_polymer[new_polymer_index++] = polymer[strlen(polymer) - 1]; // Copy the last character of the polymer to the new polymer
        new_polymer[new_polymer_index] = '\0'; // Add null terminator to the new polymer so that it's a string 

        free(polymer); // Free the memory allocated for the old polymer
        polymer=strdup(new_polymer); // Update the polymer pointer to point to the new polymer

        free(new_polymer); // Free the memory allocated for the new polymer
        if (i<4) {
            printf("Polymer after step %d: %sof length : %lu\n", i+1, polymer, strlen(polymer)); // Print the polymer after the current step
        }
    }
    // printf("Final Polymer: %s", polymer);
    printf("length of the polymer: %lu\n", strlen(polymer));

    // we now need to count the number of time each character appear in the polymer
    // then we calculate answer=most_common_character-least_common_character
    int* count=malloc(26*sizeof(int));
    for(int i=0; i<26; i++){
        count[i]=0;
    }
    for(int i=0; i<strlen(polymer); i++){
        count[polymer[i]-'A']++;
    }

    int min=INT_MAX;
    int index_min=0;
    int max=INT_MIN;
    int index_max=0;
    for(int i=0; i<26; i++){
        if(count[i]>max){
            max=count[i];
            index_max=i+1;
        }
        if(count[i]<min && count[i]>0){
            min=count[i];
            index_min=i+1;
        }
    }

    printf("Least represented: %d, %d times\n", index_min, min);
    printf("Most represented: %d, %d times\n", index_max, max);
    printf("\nAnswer: %d\n\n", max-min);


    // free the memory
    for(int i=0; i<nb_insertions; i++){
        free(insertions[i]);
    }
    free(insertions);

    free(polymer);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}

/*
we found that as expected as in the puzzle : 

Polymer after step 1: NCNBCHB

Polymer after step 2: NBCCNBBBCBHCB

Polymer after step 3: NBBBCNCCNBBNBNBBCHBHHBCHB

Polymer after step 4: NBBNBNBBCCNBCNCCNBBNBBNBBBNBBNBBCBHCBHHNHCBBCBHCB

I found 1588 as expected for the test.txt file

will need to be change for way more steps

*/