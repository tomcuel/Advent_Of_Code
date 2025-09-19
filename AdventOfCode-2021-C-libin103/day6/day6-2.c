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




int main (){

    FILE* file=fopen("input6.txt", "r");
    char line[1000];
    fgets(line, sizeof(line), file);
    int nb_numbers=number_of_times(line, ',')+1;
    // printf("number of numbers for the input : %d\n", nb_fishes_type);
    char** char_numbers=split_line(line, nb_numbers, ",");
    // give the number of fishes for each type
    int nb_fishes_type=9;
    // we initialize the array of fishes with 0
    long int fishes[nb_fishes_type];
    for (int i=0; i<nb_fishes_type; i++){
        fishes[i]=0;
    }
    // we fill the array of fishes with the input
    for (int i=0; i<nb_numbers; i++){
        int fish= atoi(char_numbers[i]);
        // increase the number of fishes for the type fish
        fishes[fish]++;
    }
    
    /*
    printf("Number of fishes type : %d\n", nb_fishes_type);
    // just to see the input is correctly taken
    printf("Input : \n");
    for (int i=0; i<nb_fishes_type; i++){
        printf("%d ", fishes[i]);
        
    }
    */
    
    int days=256;
    for (int i=0; i<days; i++){
        // we create a new array of fishes, to get the state of the fishes after this day
        long int new_fishes[nb_fishes_type];
        for (int j=0; j<nb_fishes_type; j++){
            new_fishes[j]=0;
        }

        for (int j=0; j<nb_fishes_type; j++){
            // if we're on a 0, we add the fish to the type 6 and we add a type 8 fish
            if (j==0){
                new_fishes[6]+=fishes[j];
                new_fishes[8]=fishes[j];
            }
            // otherwise we add the fish to the previous type
            else{
                new_fishes[j-1]+=fishes[j];
            }
        }
        // we update the fishes array
        for (int j=0; j<nb_fishes_type; j++){
            fishes[j]=new_fishes[j];
            // printf("%ld ", fishes[j]);
        }
        // printf("\n");
    }

    long int result=0;
    for (int i=0; i<nb_fishes_type; i++){
        result+=fishes[i];
    }

    // printing the result
    printf("\nResult : %ld\n\n", result);

    // free the memory

    for (int i=0; i<nb_fishes_type; i++){
        free(char_numbers[i]);
    }
    free(char_numbers);


    fclose(file);
    return EXIT_SUCCESS;
}


// we found 26984457539 fishes for 256days for the first test.txt file as expected 