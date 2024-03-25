#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


#include "board.h"
#include "numbers.h"
# include "results.h"


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
    print_board(board, nb_lines, len);
    */

    int oxygen_generating_rating=find_oxygen_generating_rating(board, nb_lines, len);
    printf("The oxygen generating rating is: %d\n", oxygen_generating_rating);

    printf("\n\n");

    int co2_generating_rating=find_co2_generating_rating(board, nb_lines, len);
    printf("The co2 generating rating is: %d\n", co2_generating_rating);
   
    int answer=oxygen_generating_rating*co2_generating_rating;
    printf("\nThe answer is: %d\n\n", answer);

    // free the memory and close the file
    fclose(file);
    for (int i = 0; i < nb_lines; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}


// we found the answer to be 23*10=230 for test.txt, as expected
// you need to remove the last empty line in the input file, otherwise you will do one extra iteration that will mess up the results
// I don't think it's really optimized, but it works fine 
// I did'nt know how to use the library one this one, so it dit without it 
// It was really hard for me to code this one, since it was difficult to figure out what to do at some stages of the process 


// I tried doing a bit of separated compilation
// here are the commands I used to compile the code
/*
gcc -o numbers.o -c numbers.c             
gcc -o board.o -c board.c                 
gcc -o results.o -c results.c
gcc -o day3-2.o -c day3-2.c 
gcc -o day3-2.x numbers.o board.o results.o day3-2.o 
./day3-2.x 
*/

// I made a makefile for this one