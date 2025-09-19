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


// function that returns the minimum of two integers
int min(int a, int b){
    if (a<b){
        return a;
    }
    return b;
}


int main (){

    FILE* file=fopen("input7.txt", "r");
    char line[4000];
    fgets(line, sizeof(line), file);
    int nb_numbers=number_of_times(line, ',')+1;
    // printf("number of numbers for the input : %d\n", nb_fishes_type);
    char** char_numbers=split_line(line, nb_numbers, ",");
    // getting the different crabe heights
    int* numbers = malloc(nb_numbers * sizeof(int));
    for (int i=0; i<nb_numbers; i++){
        numbers[i]=atoi(char_numbers[i]);
    }

    // getting the maximum crabe height
    int max_number=0;
    for (int i=0; i<nb_numbers; i++){
        if (numbers[i]>max_number){
            max_number=numbers[i];
        }
    }

    // doing some brute force to get the minimum number of jumps
    // getting the minimum number of jumps, we initialize the result to a very large number
    int result=1<<20; // 2^20
    // going through all our possible heights
    for (int i=0; i<max_number; i++){
        int temp=0;
        // calculating the number of jumps for each height
        for (int j=0; j<nb_numbers; j++){
            temp+=abs(numbers[j]-i);
        }
        // updating the result
        result=min(result, temp);
    }


    // print the result
    printf("\nThe minimum number of jumps is : %d\n\n", result);


    // free the memory allocated 
    for (int i=0; i<nb_numbers; i++){
        free(char_numbers[i]);
    }
    free(char_numbers);

    free(numbers);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 37 as expected for the result of the test.txt file