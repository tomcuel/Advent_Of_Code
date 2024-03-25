#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <string.h> // pour les chaînes de caractères, strlen par exemple


// copy the board to a new board, to avoid modifying the original board
int** copy_board(int** board, int nb_lines, int len);


// function to print the board, the real one which is processed with -1 and the one without the line we don't want
void print_board(int** board, int nb_lines, int len);


// function that removes the lines with the value (0 or 1) in the column j of the board
// it puts -1 in the next element of the line, to know that we've deleted this line
void remove_element(int** board, int nb_lines, int len, int j, int value);

#endif
