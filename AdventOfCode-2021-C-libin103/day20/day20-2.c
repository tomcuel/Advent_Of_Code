#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


#define LIMIT 1000


// using integer_set to be able to use the grid without having to use each time every point
#include "integer_set.h"


// function that returns the minimum of two integers
int min(int a, int b) {
    if(a<b) {
        return a;
    }
    return b;
}
// function that returns the maximum of two integers
int max(int a, int b) {
    if(a>b) {
        return a;
    }
    return b;
}


// function to convert a binary array into a decimal number
int convert_base_2_into_base_10(int* binary, int start, int end){
    int decimal = 0;
    for (int i = start; i < end; i++){
        // using horner formula for polynomial evaluation
        decimal = decimal*2 + binary[i];
    }
    return decimal;
}


// function that returns the bounds of the image, will be used to have the array we consider to make moves around the image in a certain range, without having to consider having to make a grid 
int* get_bounds(integer_set_t* image) {
    int* bounds=malloc(4*sizeof(int));
    bounds[0]=INT_MAX; // min row
    bounds[1]=INT_MIN; // max row
    bounds[2]=INT_MAX; // min col
    bounds[3]=INT_MIN; // max col
    for(int i=-200+1; i<200-1; i++) {
        for(int j=-200+1; j<200-1; j++) {
            if(integer_set_is_member(image, i*LIMIT+j)) {
                // printf("i: %d, j: %d\n", i, j);
                bounds[0]=min(bounds[0], i);
                bounds[1]=max(bounds[1], i);
                bounds[2]=min(bounds[2], j);
                bounds[3]=max(bounds[3], j);
            }
        }
    }
    return bounds;
}


// function that print the image 
void print_image(integer_set_t* image) {
    int* bounds=get_bounds(image);
    /*
    // printing the bounds to see them
    printf("bounds[0]: %d\n", bounds[0]);
    printf("bounds[1]: %d\n", bounds[1]);
    printf("bounds[2]: %d\n", bounds[2]);
    printf("bounds[3]: %d\n", bounds[3]);
    */
    int min_row=bounds[0];
    int max_row=bounds[1];
    int min_col=bounds[2];
    int max_col=bounds[3];
    free(bounds);
    for(int i=min_row; i<=max_row; i++) {
        for(int j=min_col; j<=max_col; j++) {
            if(integer_set_is_member(image, i*LIMIT+j)) {
                printf("#");
            } 
            else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}


// function that make the enhancement of the image
integer_set_t enhance(integer_set_t* image, int min_row, int max_row, int min_col, int max_col, char* command) {
    integer_set_t new_image;
    integer_set_init(&new_image);

    for (int i=min_row; i<=max_row; i++) {
        for (int j=min_col; j<=max_col; j++) {
            // we need to extract the 3x3 grid around the point (i, j)
            // we get the binary result of the 3x3 grid by going through this grid and updating the binary number with 1 if it's a # and 0 if it's a . at this point (if it is in the set or not)
            int binary_number[9];
            int index=0;
            for (int drow=-1; drow < 2; drow++){
                for (int dcol=-1; dcol < 2; dcol++){
                    int new_row=i+drow;
                    int new_col=j+dcol;
                    if (integer_set_is_member(image, new_row*LIMIT+new_col)){
                        binary_number[index]=1;
                    }
                    else{
                        binary_number[index]=0;
                    }
                    index++;
                }
            }

            // we now need to make the link with the command as the puzzle says
            int decimal_number=convert_base_2_into_base_10(binary_number, 0, 9);
            if (command[decimal_number]=='#'){
                integer_set_insert(&new_image, i*LIMIT+j);
            }
        }
    }
    
    return new_image;
}


int main() {

    FILE* file = fopen("test.txt", "r");
    char line[600];
    fgets(line, sizeof(line), file);
    char* command=strdup(line);

    // passing the empty line 
    fgets(line, sizeof(line), file);

    // the grid is a square grid, so there is no need to calculate the height of the grid and the width of the grid, the size of the grid is enough
    int size_grid=0;
    while(fgets(line, sizeof(line), file)) {
        size_grid++;
    }
    // printf("size_grid: %d\n", size_grid);

    // creating the grid and fulling it
    char** grid=malloc(size_grid*sizeof(char*));
    for(int i=0; i<size_grid; i++) {
        grid[i]=malloc(size_grid*sizeof(char));
    }
    rewind(file);
    // passing the two first lines (command + empty line)
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    int i=0;
    while(fgets(line, sizeof(line), file)) {
        for(int j=0; j<size_grid; j++) {
            grid[i][j]=line[j];
        }
        i++;
    }
    /*
    // printing the grid to see it 
    printf("Grid:\n");
    for(int i=0; i<size_grid; i++) {
        for(int j=0; j<size_grid; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    */

    // creating the set of integers
    integer_set_t image;
    integer_set_init(&image);
    // (row, col) = i*LIMIT+j

    // adding the integers to the set
    for(int i=0; i<size_grid; i++) {
        for(int j=0; j<size_grid; j++) {
            if(grid[i][j]=='#') {
                integer_set_insert(&image, i*LIMIT+j);
            }
        }
    }

    // print_image(&image);


    // getting the bounds of the image and add a margin of 200 to be able to make moves around the image
    int* bounds=get_bounds(&image);
    int min_row=bounds[0]-200;
    int max_row=bounds[1]+200;
    int min_col=bounds[2]-200;
    int max_col=bounds[3]+200;
    free(bounds);

    // we do 2 iterations of the process 
    int nb_iterations=50;
    for (int i=0; i<nb_iterations; i++){
        // enhancing the image
        image=enhance(&image, min_row, max_row, min_col, max_col, command);
        // since we add a margin of 200 and the image got bigger, we need to extend it by 3 (as it's the thing in the puzzle)
        min_row=min_row+3;
        max_row=max_row-3;
        min_col=min_col+3;
        max_col=max_col-3;
    }

    // print_image(&image);

    // print the lenght of the new_image set since we want the number of pixels that are on
    printf("\nNumber of pixels that are on: %d\n\n", integer_set_size(&image));



    // free memory
    free(command);
    for(int i=0; i<size_grid; i++) {
        free(grid[i]);
    }
    free(grid);

    integer_set_destroy(&image);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}

// need to remove the empty line at the end of the input file
// we find that 3351 pixels are on after 50 iterations for the test.txt file as expected in the puzzle
// the program is working for the input20.txt file but it takes, a very very very long time 