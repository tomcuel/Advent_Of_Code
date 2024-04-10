#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// the only change I've made on this one is invert the order of the digits we need to consider (cf line 100)

// I've parse the input file after each input to make it a bit easier to read
// After looking manually at the input file, I've noticed something 

// every step is organized in the following way (actually it's the same, but with some number that changes)(and two different types of modifications)

// 1) z is maintained all the way through the input file
// 2) every digit modified z in some way
// 3) they are two types of modifcations: 
//      - z= 26 z +w + ?? when we have the instruction "div z 1" and at the end "add z y" by going through "mul y 0" and some "add y something"
//      - z= integer part of z/26 when we have the instruction "div z 26" and "add something -something"

// I manually annalysed the input file to make this solution easier to implement
// You will need to do the same for your input file if you want to use this solution

int mul26_add[14]={1, 9, 11, -1, 6, 13, -1, 5, -1, 2, -1, -1, -1, -1};
int div26_substract[14]={-1, -1, -1, 13, -1, -1, 14, -1, 8, -1, 9, 11, 6, 5};

int* processing (int* digits){
    int z=0; 
    int* result=malloc(14*sizeof(int));

    // the index of the digit we're currently processing
    int digit_idx=0;

    for (int i=0; i<14; i++){
        // we can konw the type of modification we're going to do by looking at the values of mul26_add and div26_substract at the index i
        int increment=mul26_add[i];
        int modulo=div26_substract[i];

        // if we're on the type 2 modification
        if (increment==-1 && modulo!=-1){
            // apply the modification
            result[i]=(z%26)-modulo;
            z=z/26;
            // if the result is not between 1 and 9, then it's not a valid result, number (0 isn't a valid number)
            if (result[i]<1 || result[i]>9){
                return NULL;
            }
        }

        // if we're on the type 1 modification
        else if (increment!=-1 && modulo==-1){
            // apply the modification
            z=26*z+increment+digits[digit_idx]; // digits[digit_idx] correspond to w
            result[i]=digits[digit_idx];
            digit_idx=digit_idx+1;
        }
    }

    return result;
}


int main (){
    
    // it will not be needed afterwall
    FILE* file = fopen("input24.txt", "r");

    // close the file since we don't need it at all 
    fclose(file);


    long int nb_permutations=9*9*9*9*9*9*9;
    int** range_test=malloc(nb_permutations*sizeof(int*));
    for (long int i=0; i<nb_permutations; i++){
        range_test[i]=malloc(7*sizeof(int));
    }
    /* range_test does look like that : 
    
    i=nb_permutations-1 : 9 9 9 9 9 9 9
    i=nb_permutations-2 : 9 9 9 9 9 9 8
    i=nb_permutations-3 : 9 9 9 9 9 9 7
    …
    …
    …
    …
    …
    i=0 : 1 1 1 1 1 1 1
    the same as before but reverse 
    */

    // making the range_test array
    int index = 0;
    for (int a =1; a <= 9; a++) {
        for (int b =1; b <= 9; b++) {
            for (int c =1; c <= 9; c++) {
                for (int d =1; d <= 9; d++) {
                    for (int e =1; e <= 9; e++) {
                        for (int f =1; f <= 9; f++) {
                            for (int g =1; g <= 9; g++) {
                                range_test[index][0] = a;
                                range_test[index][1] = b;
                                range_test[index][2] = c;
                                range_test[index][3] = d;
                                range_test[index][4] = e;
                                range_test[index][5] = f;
                                range_test[index][6] = g;
                                ++index;
                            }
                        }
                    }
                }
            }
        }
    }
    /*
    // printing the range_test array (the 20 first elements and the 20 last elements)
    printf("\nFirst 20 elements:\n");
    for (int i = 0; i < 20; i++) {
        printf("%d %d %d %d %d %d %d\n", range_test[i][0], range_test[i][1], range_test[i][2], range_test[i][3], range_test[i][4], range_test[i][5], range_test[i][6]);
    }
    printf("\nLast 20 elements:\n");
    for (int i = nb_permutations - 20; i < nb_permutations; i++) {
        printf("%d %d %d %d %d %d %d\n", range_test[i][0], range_test[i][1], range_test[i][2], range_test[i][3], range_test[i][4], range_test[i][5], range_test[i][6]);
    }
    printf("\n");
    */


    // digits represent a 7 digit array in which we're going to add digit bewteen, depending on the instructions
    for (long int i=0; i<nb_permutations; i++){
        // gettting the digits
        int* digits=malloc(7*sizeof(int));
        for (int j=0; j<7; j++){
            digits[j]=range_test[i][j];
        }

        // printing the digits
        printf("Digits: ");
        for (int j=0; j<7; j++){
            printf("%d", digits[j]);
        }
        printf("\n");
        
        // processing the digits
        int* result=processing(digits);
        
        // if the result is valid, then we print it and break the loop
        if (result!=NULL){
            printf("\nThe result is: ");
            for (int i=0; i<14; i++){
                printf("%d", result[i]);
            }
            printf("\n\n");
            // free the memory
            free(digits);
            free(result);
            break;
        }

        // free the memory if the result is not valid
        free(digits);
        free(result);
    }


    // free the memory
    for (long int i=0; i<nb_permutations; i++){
        free(range_test[i]);
    }
    free(range_test);

    return EXIT_SUCCESS;
}