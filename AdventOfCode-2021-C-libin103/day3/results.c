#include "results.h"

// function that returns the oxygen generating rating
int find_oxygen_generating_rating(int** board_real, int nb_lines, int len){
    
    int** board=copy_board(board_real, nb_lines, len);
    // printf("board copied\n");
    // print_board(board, nb_lines, len);

    int j=0;
    // process the board to make lines with -1 if there aren't the one we want
    for (; j< len; j++){
        int numbers_of_1=number_of_1(board, nb_lines, j);
        // printf("The number of 1 in the column %d is: %d\n", j, numbers_of_1);
        int numbers_of_0=number_of_0(board, nb_lines, j);
        // printf("The number of 0 in the column %d is: %d\n", j, numbers_of_0);
        // if we've more 1 than 0, we remove the lines with 0 in the column j
        if (numbers_of_1>=numbers_of_0){
            remove_element(board, nb_lines, len,j, 0);
        }
        // if we've more 0 than 1, we remove the lines with 1 in the column j
        else{
            remove_element(board, nb_lines, len, j, 1);
        }

        if (number_of_lines_without(board, nb_lines, len)<=2){
            break;
        }
        // print_board(board, nb_lines, len);
    }

    // print_board(board, nb_lines, len);
    // getting the index of the row where there isn't a -1, where our oxygen generating rating is
    // there can be two at the end of the board, we need to choose the one with the 1 at the end
    int indexs[2];
    for (int i=0; i< 2; i++){
        indexs[i]=-1;
    }
    int k=0;

    for (int i=0; i< nb_lines; i++){
        int nb_minus_1=0;
        for (int j=0; j< len; j++){
            if (board[i][j]==-1){
                nb_minus_1++;
            }
        }
        if (nb_minus_1==0){
            indexs[k]=i;
            ++k;
        }
        nb_minus_1=0;
    }
    printf("The lines possible are:\n");
    for (int i=0; i< 2; i++){
        printf("%d\n", indexs[i]+1);
    }
    printf("\n");

    int index;
    // if there's two different lines, we choose the one with the 1 at the end
    if (board[indexs[0]][j-1]==1){
        index=indexs[0];
    }
    else{
        index=indexs[1];
    }
  
    
    // store the binary number in an array, it represents the oxygen generating rating
    int* binary = malloc(len*sizeof(int));
    printf("The binary number of the oxygen generating rating is: ");
    for (int j=0; j< len; j++){
        binary[j]=board[index][j];
        printf("%d", binary[j]);
    }
    printf("\n");


    // convert the binary number to decimal
    int decimal = convert_to_decimal(binary, len);
    for (int i = 0; i < nb_lines; i++) {
        free(board[i]);
    }
    free(board);
    free(binary);
    return decimal;
}


// function that returns the oxygen generating rating
int find_co2_generating_rating(int** board_real, int nb_lines, int len){
    
    int** board=copy_board(board_real, nb_lines, len);
    // printf("board copied\n");
    // print_board(board, nb_lines, len);

    int j=0;
    // process the board to make lines with -1 if there aren't the one we want
    for (; j< len; j++){
        int numbers_of_1=number_of_1(board, nb_lines, j);
        // printf("The number of 1 in the column %d is: %d\n", j, numbers_of_1);
        int numbers_of_0=number_of_0(board, nb_lines, j);
        // printf("The number of 0 in the column %d is: %d\n", j, numbers_of_0);
        // if we've more 1 than 0, we remove the lines with 0 in the column j
        if (numbers_of_0<=numbers_of_1){
            remove_element(board, nb_lines, len,j, 1);
        }
        // if we've more 0 than 1, we remove the lines with 1 in the column j
        else{
            remove_element(board, nb_lines, len, j, 0);
        }

        if (number_of_lines_without(board, nb_lines, len)<=2){
            break;
        }
        // print_board(board, nb_lines, len);
    }

    // print_board(board, nb_lines, len);
    // getting the index of the row where there isn't a -1, where our oxygen generating rating is
    // there can be two at the end of the board, we need to choose the one with the 1 at the end
    int indexs[2];
    for (int i=0; i< 2; i++){
        indexs[i]=-1;
    }
    int k=0;

    for (int i=0; i< nb_lines; i++){
        int nb_minus_1=0;
        for (int j=0; j< len; j++){
            if (board[i][j]==-1){
                nb_minus_1++;
            }
        }
        if (nb_minus_1==0){
            indexs[k]=i;
            ++k;
        }
        nb_minus_1=0;
    }
    printf("The lines possible are:\n");
    for (int i=0; i< 2; i++){
        printf("%d\n", indexs[i]+1);
    }
    printf("\n");


    int index;
    // if there's two different lines, we choose the one with the 0 at the end
    if (board[indexs[0]][j-1]==0){
        index=indexs[0];
    }
    else{
        index=indexs[1];
    }
    
    // store the binary number in an array, it represents the oxygen generating rating
    int* binary = malloc(len*sizeof(int));
    printf("The binary number of the co2 generating rating is: ");
    for (int j=0; j< len; j++){
        binary[j]=board[index][j];
        printf("%d", binary[j]);
    }
    printf("\n");


    // convert the binary number to decimal
    int decimal = convert_to_decimal(binary, len);
    for (int i = 0; i < nb_lines; i++) {
        free(board[i]);
    }
    free(board);
    free(binary);
    return decimal;
}
