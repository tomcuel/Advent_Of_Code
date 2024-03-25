#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// function that returns the decimal number of the binary number
int convert_to_decimal(int* binary, int len){
    int decimal = 0;
    int power = 1;
    for (int j=0; j< len; j++){
       decimal += binary[len-1-j]*power;
       power*=2;
    }
    return decimal;
}


// function that returns the decimal number of the binary number of the gamma rate
int gamma_rate(int** board, int nb_lines, int len){
    int* binary = malloc(len*sizeof(int));

    int number_of_1=0;
    int number_of_0=0;
    for (int j=0; j< len; j++){
        // for each column, count the number of 1 and 0
        for (int i=0; i< nb_lines; i++){
            if (board[i][j]==1){
                number_of_1++;
            }
            else if (board[i][j]==0){
                number_of_0++;
            }
        }
        // printf("Number of 1s: %d\n", number_of_1);
        // printf("Number of 0s: %d\n", number_of_0);
        // if the number of 1 is greater than the number of 0, then the binary number is 1
        if (number_of_1>=number_of_0){
            binary[j]=1;
        }
        // otherwise, the binary number is 0
        else{
            binary[j]=0;
        }
        number_of_1=0;
        number_of_0=0;
    }

    printf("The binary number is: ");
    for (int j=0; j< len; j++){
        printf("%d", binary[j]);
    }
    printf("\n");
    // we have 10110 as the binary number for test.txt

    // convert the binary number to decimal
    int decimal = convert_to_decimal(binary, len);

    printf("The decimal number is: %d\n", decimal);
    // we have 22 as the decimal number for test.txt

    free(binary);
    return decimal;
}


// function that returns the decimal number of the binary number of the epsilon rate
int espilon_rate(int** board, int nb_lines, int len){
    int* binary = malloc(len*sizeof(int));

    int number_of_1=0;
    int number_of_0=0;
    for (int j=0; j< len; j++){
        // for each column, count the number of 1 and 0
        for (int i=0; i< nb_lines; i++){
            if (board[i][j]==1){
                number_of_1++;
            }
            else if (board[i][j]==0){
                number_of_0++;
            }
        }
        // printf("Number of 1s: %d\n", number_of_1);
        // printf("Number of 0s: %d\n", number_of_0);
        // if the number of 1 is greater than the number of 0, then the binary number is 0
        if (number_of_1>=number_of_0){
            binary[j]=0;
        }
        // otherwise, the binary number is 1
        else{
            binary[j]=1;
        }
        number_of_1=0;
        number_of_0=0;
    }

    printf("The binary number is: ");
    for (int j=0; j< len; j++){
        printf("%d", binary[j]);
    }
    printf("\n");

    // convert the binary number to decimal
    int decimal = convert_to_decimal(binary, len);

    printf("The decimal number is: %d\n", decimal);

    free(binary);
    return decimal;
}


int main(){

    FILE* file;
    file = fopen("input3.txt", "r");
    char line[20];

    // get the number of lines and the length of the lines
    int nb_lines = 0;
    int len = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        nb_lines++;
        len = strlen(line);
    }
    // store the board in a 2D array
    int** board=malloc(nb_lines*sizeof(int*));
    for (int i = 0; i < nb_lines; i++) {
        board[i] = malloc(len*sizeof(int));
    }

    // read the file again, to start from the beginning
    rewind(file);

    // making the storage of the board
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int j = 0; j < len; j++) {
            board[i][j] = line[j]-48; // 48 is the ASCII code of 0
        }
        ++i;
    }
    
    /*
    // print the board to see it is stored correctly
    printf("The board is stored as follows:\n");
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    */

    int gamma = gamma_rate(board, nb_lines, len);
    // we have 10110 as the binary number for test.txt
    // we have 22 as the decimal number for test.txt
    int epsilon = espilon_rate(board, nb_lines, len);
    // we have 01001 as the binary number for test.txt
    // we have 9 as the decimal number for test.txt


    int answer = gamma * epsilon;
    printf("\nThe answer is: %d\n\n", answer);

    // free the memory and close the file
    fclose(file);
    for (int i = 0; i < nb_lines; i++) {
        free(board[i]);
    }
    free(board);
    return 0;
}


// we found the answer to be 22*9=198 for test.txt, as expected
// you need to remove the last empty line in the input file, otherwise you will do one extra iteration that will mess up the results