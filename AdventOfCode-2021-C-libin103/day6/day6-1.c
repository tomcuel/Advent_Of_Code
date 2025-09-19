#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// function that returns the number of times a character appears in a string
int number_of_times(char* ch, char x){
    int n=0;
    int i=0;
    while (ch[n] != 0){
        if (ch[n]==x){
            i=i+1;
        }
        n=n+1;
    }
    return(i);
}


// function that splits a string into an array of strings using a delimiter string (here i will use ',')
// I'm using malloc allocation to allocate memory for the array of strings
// so there is a need to free the memory after using the array of strings
char** split_line(char* line, int split_line, char* delimiter){
    char** result = malloc(split_line * sizeof(char*));
    for (int i=0; i<split_line; i++){
        result[i] = malloc(5 * sizeof(char));
    }
    char* token = strtok(line, delimiter);
    int i = 0;
    while (token != NULL){
        strcpy(result[i],token);
        token = strtok(NULL, delimiter);
        i++;
    }
    return result;

}


// function that returns the length of an array
// in our case the array if full until the first -1
int get_lenght(int* array, int nb_fishes){
    int i=0;
    while (array[i] != -1){
        i=i+1;
    }
    return i;
}


void append(int **array, int *size, int value) {
    // Increase the size of the array by 1
    (*size)++;
    // Reallocate memory for the resized array
    *array = (int *)realloc(*array, (*size) * sizeof(int));
    if (*array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Append the new value to the end of the array
    (*array)[(*size) - 1] = value;
}


int main (){

    FILE* file=fopen("input6.txt", "r");
    char line[1000];
    fgets(line, sizeof(line), file);
    int nb_numbers=number_of_times(line, ',')+1;
    // printf("number of numbers : %d\n", nb_fishes);
    char** char_numbers=split_line(line, nb_numbers, ",");
    int* fishes = NULL;
    int nb_fishes=0;
    for (int i=0; i<nb_numbers; i++){
        int fish= atoi(char_numbers[i]);
        append(&fishes, &nb_fishes,fish);
    }
    
    /*
    printf("Number of fishes : %d\n", nb_fishes);
    // just to see the input is correctly taken
    printf("Input : \n");
    for (int i=0; i<nb_fishes; i++){
        printf("%d ", fishes[i]);
        
    }
    */
    

    // doing the loop
    // =18 for the test.txt fil for the first test, =80 after for the other tests
    int nb_loops=80;
    for (int i=0; i<nb_loops; i++){
        int temp=nb_fishes;
        for (int j=0; j<temp; j++){
            // if there is a 0 in the array, we replace it by 1 and add an 8 at the end of the array as asked in the puzzle 
            if (fishes[j] == 0){
                fishes[j]=6;
                append(&fishes, &nb_fishes, 8);
            }
            // otherwise we decrease the value by 1
            else {
                fishes[j]--;
            }
        }
        /*
        printf("Number of fishes : %d\n", nb_fishes);
        // just to see the input is correctly taken
        printf("Input : \n");
        for (int i=0; i<nb_fishes; i++){
            printf("%d ", fishes[i]);
        }
        */
    }

    // printing the result
    printf("\nResult : %d\n\n", nb_fishes);

    // free the memory

    for (int i=0; i<nb_fishes; i++){
        free(char_numbers[i]);
    }
    free(char_numbers);

    free(fishes);

    fclose(file);
    return EXIT_SUCCESS;
}


// we found 26 fishes for 18 loops and 5934 fishes for 80loops for the first test.txt file as expected 