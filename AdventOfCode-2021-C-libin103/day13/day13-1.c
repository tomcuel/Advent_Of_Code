#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// I will use the integer_set.h file to store the dots, so that I have a unique representation of the dots positions, without having to go trhough an array 
// but every time, I need to convert the position of the dot in the grid to the position in the array with formula : row * width + col and the other way around
#include "integer_set.h"

// function that get the posistion of a character in a string
int get_pos(char* string, char c){
    for (int i = 0; i < strlen(string); i++){
        if (string[i] == c){
            return i;
        }
    }
    return -1;
}


int main (){

    FILE *file = fopen("input13.txt", "r");
    char line[25];

    // getting the index of the empty line
    int empty_line = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '\n'){
            break;
        }
        empty_line++;
    }
    printf("\nempty_line index : %d\n", empty_line);


    rewind(file);
    int height = 0; // will be the number of the dot that is the lowest 
    int width = 0; // will be the number of the dot that is the most on the right
    while (fgets(line, sizeof(line), file) != NULL) {
        // splitting the line into two parts
        int pos = get_pos(line, ',');

        // getting the height and looking if it's the highest
        char* str_height = malloc(5 * sizeof(char));
        for (int i = 0; i < pos; i++){
            str_height[i] = line[i];
        }
        if (atoi(str_height)+1 > height){
            height = atoi(str_height)+1;
        }
        // printf("height : %d / ", atoi(str_height));

        // getting the width and looking if it's the most on the right
        char* str_width = malloc(5 * sizeof(char));
        for (int i = pos + 1; i < strlen(line); i++){
            str_width[i - pos - 1] = line[i];
        }
        if (atoi(str_width)+1 > width){
            width = atoi(str_width)+1;
        }
        // printf("width : %d\n", atoi(str_width));

    }
    // printing the max height and width
    printf("max_height : %d / max_width : %d\n\n", height, width);

  

    // making an array of dots
    // the dots at (row, col) will be at the position row * width + col
    integer_set_t dots;
    integer_set_init(&dots);

    rewind(file);
    // here we're getting where the dots are and storing the into the set
    for (int i = 0; i < empty_line; i++){
        fgets(line, sizeof(line), file);
        // printf("line : %s", line);
        char* token = strtok(line, ",");
        int row = atoi(token);
        // printf("token 1: %s\n", token);
        token = strtok(NULL, ",");
        // printf("token 2 : %s", token);
        int col = atoi(token);        

        // printf("row : %d / ", row);
        // printf("col : %d\n", col);

        int in_set=row * width + col;
        // printf("in_set : %d\n", in_set);

        integer_set_insert(&dots, in_set);
        // printf("\n");
    }
    printf("\n");

    rewind(file);
    // we now need to get the commands 
    char* commands_str= malloc(25 * sizeof(char));
    int* commands_int = malloc(25 * sizeof(int));
    for (int i = 0; i < empty_line+1; i++){
        fgets(line, sizeof(line), file);
    }
    int index_command = strlen("fold along ");
    // getting the number of commands
    int number_of_commands=0;
    while(fgets(line, sizeof(line), file) != NULL){
        // printf("line : %s for ", line);
        commands_str[number_of_commands] = line[index_command];
        // printf("command_grid[%d] : %c\n", k,commands_str[k]);
        // now we need to get the number associated with the command
        int pos_nb=get_pos(line, '=');
        char* str_nb = malloc(5 * sizeof(char));
        for (int i = pos_nb+1; i < strlen(line); i++){
            str_nb[i - pos_nb - 1] = line[i];
        }
        commands_int[number_of_commands] = atoi(str_nb);
        ++number_of_commands;
        // printf("command_int[%d] : %d\n", k,commands_int[k]);
    }
    printf("Number of commands : %d\n",number_of_commands);



    // for the folding we need to create a new set of dots
    // the dots that were already on the part wthe folding will cover will remain visible
    // if two dots are overlapping, we will it as one dot

    // doing only the first command
    integer_set_t new_dots[number_of_commands];
    for (int k = 0; k < number_of_commands; k++){
        integer_set_init(&new_dots[k]);
    }

    // making the loop for the commands
    // k<1 if we want to do only the first command
    for (int k=0; k<2; k++){

        // we have the parsing line : command= x or y, value = row or col affected
        char command = commands_str[k]; 
        int value = commands_int[k];
        // printf("command : %c / value : %d\n", command, value);

        // we need to fold the bottom half of the grid up if the line in the command is x (row)    
        if (command == 'y'){
            int new_height = height;
            int new_width = value;
            int old_height = height;
            int old_width = width;
            // going trought the height that hasn't changed
            for (int i = 0; i < new_height; i++){
                // going trought the part that will not be fold
                for (int j = 0; j < new_width; j++){
                    int old_point = i*old_width+j;
                    int new_point=i*new_width+j;
                    
                    // if the old point is in the old set of dots, we add the new point to the new set
                    // just need to check if we're at the first iteration, we need to consider the input, not the previous output
                    if (k==0){
                        if (integer_set_is_member(&dots, old_point)){
                            integer_set_insert(&new_dots[k], new_point);
                            // printf("point : (%d,%d)\n", i,j);
                        }
                    }
                    else {
                        if (integer_set_is_member(&new_dots[k-1], old_point)){
                            integer_set_insert(&new_dots[k], new_point);
                            // printf("point : (%d,%d)\n", i,j);
                        }
                    }
                    
                }
                // going trought the part that will be fold
                for (int j = new_width+1; j < old_width; j++){
                    // we need to make the difference between the dots that are on the left and the dots that are on the right
                    int point_to_be_folded=i*old_width+j;
                    int point_after_folding=i*new_width+(2*value-j);
                    // if the old point is in the old set of dots, we add the new point to the new set, but it needs not to already be in the new set
                    // just need to check if we're at the first iteration, we need to consider the input, not the previous output
                    if (k==0){
                        if (integer_set_is_member(&dots, point_to_be_folded) && !integer_set_is_member(&new_dots[k], point_after_folding)){
                            integer_set_insert(&new_dots[k], point_after_folding);
                            // printf("point : (%d,%d)\n", i,width-1-j);
                        }
                    }
                    else {
                        if (integer_set_is_member(&new_dots[k-1], point_to_be_folded) && !integer_set_is_member(&new_dots[k], point_after_folding)){
                            integer_set_insert(&new_dots[k], point_after_folding);
                            //printf("point : (%d,%d)\n", i,width-1-j);
                        }
                    }
                }
            }
            height = new_height;
            width = new_width;
            // printf("after y command : new_height : %d / new_width : %d\n", height, width);
        }

        // we need to fold the right half of the grid to the left if the line in the command is y (col)
        if (command == 'x'){
            int new_height = value;
            int new_width = width;
            int old_height = height;
            int old_width = width;
            // going trought the width that hasn't changed
            for (int j=0; j<new_width; j++){
                // going trought the part that will not be fold
                for (int i=0; i<new_height; i++){
                    int old_point = i*old_width+j;
                    int new_point=i*new_width+j;
                    // if the point is in the set of dots, we add it to the new set
                    // just need to check if we're at the first iteration, we need to consider the input, not the previous output
                    if (k==0){
                        if (integer_set_is_member(&dots, old_point)){
                            integer_set_insert(&new_dots[k], new_point);
                            // printf("point : (%d,%d)\n", i,j);
                        }
                    }
                    else {
                        if (integer_set_is_member(&new_dots[k-1], old_point)){
                            integer_set_insert(&new_dots[k], new_point);
                            // printf("point : (%d,%d)\n", i,j);
                        }
                    }
                }
                // going trought the part that will be fold
                for (int i=new_height+1; i<old_height; i++){
                    // we need to make the difference between the dots that are on the top and the dots that are on the bottom
                    int point_to_be_folded=i*old_width+j;
                    int point_after_folding=(2*value-i)*new_width+j;
                    // if the point is in the set of dots, we add it to the new set, but it needs not to already be in the new set
                    // just need to check if we're at the first iteration, we need to consider the input, not the previous output
                    if (k==0){
                        if (integer_set_is_member(&dots, point_to_be_folded) && !integer_set_is_member(&new_dots[k], point_after_folding)){
                            integer_set_insert(&new_dots[k], point_after_folding);
                            // printf("point : (%d,%d)\n", height-1-i,j);
                        }
                    }
                    else {
                        if (integer_set_is_member(&new_dots[k-1], point_to_be_folded) && !integer_set_is_member(&new_dots[k], point_after_folding)){
                            integer_set_insert(&new_dots[k], point_after_folding);
                            // printf("point : (%d,%d)\n", height-1-i,j);
                        }
                    }
                }
            }
            height = new_height;
            width = new_width;
            // printf("after x command : new_height : %d / new_width : %d\n", height, width);
        }
        

        // printf("\n");
        
    
    }


    // printing the answer : the first one for part one and the last one for part two
    printf("\nThe number of dots that are visible after the first step is : %d\n\n", integer_set_size(&new_dots[0]));
    printf("\nThe number of dots that are visible after the second step is : %d\n\n", integer_set_size(&new_dots[1]));


    // free the memory
    for (int k = 0; k < number_of_commands; k++){
        integer_set_destroy(&new_dots[k]);
    }

    integer_set_destroy(&dots);
    free(commands_str);
    free(commands_int);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// we found 17 dots visible for the test.txt file
// we found 16 for the second round for test.txt file where we processed all the commands