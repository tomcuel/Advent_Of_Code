#include "numbers.h"


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


// function that returns the number of 1 in the column j of the board
int number_of_1(int** board, int nb_lines, int j){
    int number_of_1=0;

    // for each column, count the number of 1 and 0
    for (int i=0; i< nb_lines; i++){
        if (board[i][j]==1){
            number_of_1++;
        }
    }
    return number_of_1;
}


// function that returns the number of 0 in the column j of the board
int number_of_0(int** board, int nb_lines, int j){
    int number_of_0=0;

    // for each column, count the number of 1 and 0
    for (int i=0; i< nb_lines; i++){
        if (board[i][j]==0){
            number_of_0++;
        }
    }
    return number_of_0;
}


// function that get the number of lines that not yet have -1 in them
// will be used to stop the loop when we've only one line left
int number_of_lines_without(int** board, int nb_lines, int len){
    int number_of_lines=0;
    // going through the rows to see if there is a line without -1
    for (int i=0; i< nb_lines; i++){
        // getting the number of -1 in the line
        int nb_minus_1=0;
        for (int j=0; j< len; j++){
            if (board[i][j]==-1){
                nb_minus_1++;
            }
        }
        // if there is no -1 in the line, we increment the number of lines
        if (nb_minus_1==0){
            number_of_lines++;
        }
        nb_minus_1=0;
    }
    return number_of_lines;
}