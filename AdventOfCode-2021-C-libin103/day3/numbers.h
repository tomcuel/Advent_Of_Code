#ifndef __NUMBERS_H__
#define __NUMBERS_H__


// function that converts a decimal number to binary
int convert_to_decimal(int* binary, int len);

// function that returns the number of 1 in the column j of the board
int number_of_1(int** board, int nb_lines, int j);


// function that returns the number of 0 in the column j of the board
int number_of_0(int** board, int nb_lines, int j);


// function that get the number of lines that not yet have -1 in them
// will be used to stop the loop when we've only one line left
int number_of_lines_without(int** board, int nb_lines, int len);

#endif
