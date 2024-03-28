#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// function to swap two characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}


// function to permute a string between the l-th and r-th character
// it does implement the permutations of the string in the permutations array
void permute(char *str, int l, int r, char** permutations, int* test) {
    if (l == r) {
        // Perform your operation here
        strcpy(permutations[*test], str);
        *test += 1;
    } 
    else {
        for (int i = l; i <= r; i++) {
            swap((str + l), (str + i));
            permute(str, l + 1, r, permutations, test);
            swap((str + l), (str + i)); // Backtrack
        }
    }
}


/*
Problem : a seven-line display where lines are represented by letters 
but they've been mixed up
input : represents the segment that are turned on when each number is displayed (mixed up)
output : the number that represents a unique code 
seris of 2 segments == 1
series of 7 segments == 8
………
We're searching for the number of times 2,3,4,7 appear in the input in the first part 
here we're searching for every number 
we need to deduce which segment is which number
*/


// List of digits correspond to the illuminating segments
// will be used to decrypt the output
char* digits[] = {
    "abcefg", // 0
    "cf", // 1
    "acdeg", // 2
    "acdfg", // 3
    "bcdf", // 4
    "abdfg", // 5 
    "abdefg", // 6
    "acf", // 7
    "abcdefg", // 8
    "abcdfg" // 9
};


// give the number of times a segment appears in the input
// will be sorted 
char* digits_key[] = {
    "abcefg", // 0
    "cf", // 1
    "acdeg", // 2
    "acdfg", // 3
    "bcdf", // 4
    "abdfg", // 5 
    "abdefg", // 6
    "acf", // 7
    "abcdefg", // 8
    "abcdfg" // 9
};


// Comparison function for qsort
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}


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


    // sorting the digits_key list
    // 
    qsort(digits_key, 10, sizeof(char*), compare);
    /*
    for (int i = 0; i < 10; i++){
        printf("%s\n", digits_key[i]);
    }
    */


    // string on which we will perform the permutations
    char sigma[] = "abcdefg"; 
    int n = strlen(sigma);
    int nb_permutations = 5040; // 7! = 5040
    // allocating memory to store the permutations
    char** permutations = malloc(nb_permutations*sizeof(char*));
    for (int i = 0; i < nb_permutations; i++) {
        permutations[i] = malloc(n*sizeof(char));
    }
    // used for the permute function and the incrementation of the rank where we put the permutation
    int test = 0;
    // performing all the permutations and proceding to store them in the permutations array
    permute(sigma, 0, n-1, permutations, &test);
    /*
    // printing the permutations and the number of permutations
    for (int i = 0; i < nb_permutations; i++) {
        printf("%s\n", permutations[i]);
    }
    printf("Number of permutations: %d\n", test);
    */

    int winning_permutation_index = 0;
    char** winning_permutation = malloc(nb_lines*sizeof(char*));
    for (int i = 0; i < nb_lines; i++){
        winning_permutation[i] = malloc(n*sizeof(char));
    }

    // allocating memory for the new input when we will sort the clues
    // it's a kind of temporary variable that will be udpated at each permutation

    // the answer 
    int answer = 0;
    // going through the lines (of the input and output)
    for (int i = 0; i < nb_lines; i++){
        
            // trying all the way it could be substituted
        for (int j=0; j<nb_permutations; j++){

            // key[i]=["before","after"]
            char key[2][7];
            for (int k = 0; k < 7; k++){
                key[0][k] = sigma[k];
                key[1][k] = permutations[j][k];
            }

            // printing some key to see if it works
            /*
            if (j<3 && i==0){
                printf("\n");
                printf("sigma %s --> permutation %s\n", sigma, permutations[j]);
                for (int k = 0; k < 7; k++){
                    printf("old %c --> new %c\n", key[0][k], key[1][k]);
                }
            }
            */
           
            
            // allocating memory for the new input when we will sort the clues
            // it's a kind of temporary variable that will be udpated at each permutation
            char** new_input=malloc(10*sizeof(char*));


            // seeing what the permutation looks like for the line i 
            // we now need to make them as the new string to sort them and see if it corresponds to the digits_key
            for (int k = 0; k < 10; k++){
                // getting the input n°k and its size
                int size = strlen(input[i][k]);
                new_input[k] = malloc(size*sizeof(char));
                /*
                if (j<3 && i==0){
                    printf("\n");
                    printf("old input: %s\n", input[i][k]);
                    printf("size: %d\n", size);
                }
                */
                for (int l = 0; l < size; l++){
                    int position=-1;
                    for (int m = 0; m < 7; m++){
                        // we search where is input[i][k][l] in the non converted part of the key
                        if (input[i][k][l] == key[0][m]){
                            position = m;
                            break;
                        }
                    }
                    // we put the converted part in the new input
                    new_input[k][l] = key[1][position];
                    /*
                    if (j<3 && i==0){
                        printf(" input %c\n", input[i][k][l]);
                        printf(" position %d\n", position);
                        printf(" new input %c\n", new_input[k][l]);
                        printf("\n");
                    }
                    */
                }

            }

            // new input represent the current input where the clues have been permuted according to the permutation j
            // for example, if we have the permutation "abcdefg" -> "abcdegf", the input "acedgfb" will become "acedfgb"
            
            
            /*
            if (j<3 && i==0){
                printf("\nsigma %s --> permutation %s\n", sigma, permutations[j]);

                printf("\nnon sorted old input :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s ", input[i][k]);
                }
                printf("\n");

                printf("output:\n");
                for (int k = 0; k < 4; k++){
                    printf("%s ", output[i][k]);
                }

                printf("\nnew input :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s ", new_input[k]);
                }
                printf("\n");

                printf("\nsorted digits_key :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s, ", digits_key[k]);
                }
                printf("\n\n");
            }
            */
    
           
            // sorting the new input for each strings 
            for (int k = 0; k < 10; k++){
                qsort(new_input[k], strlen(new_input[k]), sizeof(char), (int (*)(const void *, const void *))strcmp);
            }

            // sorting the new input strings list 
            qsort(new_input, 10, sizeof(char*), compare);

            /*
            // print new_input
            printf("\nnew_input:\n");
            for (int i = 0; i < 10; i++){
                printf("%s ", new_input[i]);
            }
            */

        
            // checking if the new input is the same as the digits_key
            // to know if the permutation is working and if we can use it to decrypt the output
            int is_working = 0;
            for (int k = 0; k < 10; k++){
                if (strcmp(new_input[k], digits_key[k]) == 0){
                    is_working++;
                }
            }
            

            // freeing the memory
            for (int k = 0; k < 10; k++){
                free(new_input[k]);
            }
            free(new_input);


            // if the permutation is working, we increment the answer with numbers of the outputs we need to decrypt
            if (is_working==10){
                /*
                if (i==1){
                printf("\nsigma %s --> permutation %s\n", sigma, permutations[j]);

                printf("\nnon sorted old input :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s ", input[i][k]);
                }
                printf("\n");

                printf("output:\n");
                for (int k = 0; k < 4; k++){
                    printf("%s ", output[i][k]);
                }

                printf("\nnew input :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s ", new_input[k]);
                }
                printf("\n");

                printf("\nsorted digits_key :\n");
                for (int k = 0; k < 10; k++){
                    printf("%s, ", digits_key[i]);
                }
                printf("\n\n");
                */
                // storing the winning permutation to implement it in the output
                winning_permutation_index = i;
                for (int p = 0; p < 7; p++){
                    winning_permutation[i][p] = permutations[j][p];
                }
                // getting the number the output is supposed to be
                int number[4];
                // going throufh the 4 outputs
                for (int l = 0; l < 4; l++){
                    // getting the size of the output
                    int size_x = strlen(output[i][l]);
                    // getting the converted part of the key
                    char x[size_x];
                    for (int p=0; p<size_x; p++){
                        int pos=-1;
                        for (int m = 0; m < 7; m++){
                            // on cherche à quelle position se trouve output[l][p] dans la partie convertie de key : 
                            if (output[i][l][p] == key[0][m]){
                                pos = m;
                                break;
                            }
                        }
                        x[p] = key[1][pos];
                    }
                    qsort(x, size_x, sizeof(char), (int (*)(const void *, const void *))strcmp);
                    // printf("x: %s", x);
                    // I have the godd x, I verified
                    // getting the index of x (encrypted output) in the digits
                    int index_of_x_in_digit = 0;
                    for (int h = 0; h < 10; h++){
                        if (strcmp(x, digits[h]) == 0){
                            index_of_x_in_digit = h;
                            break;
                        }
                    }
                    // getting the number
                    number[l] = index_of_x_in_digit;
                    // printf("with number: %d\n", number[l]);
        
                }
                // incrementing the answer
                // printf("The final number is: %d\n\n", number[0]*1000+number[1]*100+number[2]*10+number[3]);
                answer+=number[0]*1000+number[1]*100+number[2]*10+number[3];
                break;
            }

        }
    }

    /*
    // printing the winning permutation
    printf("\nWinning permutation: \n");
    for (int i = 0; i < nb_lines; i++){
        printf("%s\n", winning_permutation[i]);
    }
    // I found the right one 
    */

    // printing the answer
    printf("\nThe answer is: %d\n\n", answer);


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

    for (int i = 0; i < nb_permutations; i++){
        free(permutations[i]);
    }
    free(permutations);

    for (int i = 0; i < nb_lines; i++){
        free(winning_permutation[i]);
    }
    free(winning_permutation);

    // closing the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 61229 with the test.txt file as expected