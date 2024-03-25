#include "board.h"

#include <stdio.h>
#include <stdlib.h>

// copy the board to a new board, to avoid modifying the original board
int** copy_board(int** board, int nb_lines, int len){
    int** new_board=malloc(nb_lines*sizeof(int*));
    for (int i = 0; i < nb_lines; i++) {
        new_board[i] = malloc(len*sizeof(int));
    }
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < len; j++) {
            new_board[i][j]=board[i][j];
        }
    }
    return new_board;
}


// function to print the board, the real one which is processed with -1 and the one without the line we don't want
void print_board(int** board, int nb_lines, int len){
    // print the board to see it is stored correctly
    printf("Real board:\n");
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("The board is stored like this right now :\n");
    for (int i = 0; i < nb_lines; i++) {
        int nb_minus_1=0;
        for (int j = 0; j < len; j++) {
            if (board[i][j]==-1){
                nb_minus_1++;
            }
        }
        if (nb_minus_1==0){
            for (int j = 0; j < len; j++) {
                printf("%d", board[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
}


// function that removes the lines with the value (0 or 1) in the column j of the board
// it puts -1 in the next element of the line, to know that we've deleted this line
void remove_element(int** board, int nb_lines, int len, int j, int value){
    for (int i=0; i< nb_lines; i++){
        if (board[i][j]==value){
            for (int l=0; l< len; l++){
                board[i][l]=-1;
            }
        }
    }
}