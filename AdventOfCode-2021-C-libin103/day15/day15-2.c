#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// using set to have the unicity of each point 
// point (i,j) will be i*nb_columns+j
// the other way around 
// heaps will use the same the same methods
#include "integer_set.h"
#include "integer_heap.h"


// array of directions
int directions[4][2] = {
        {0, 1}, // right 
        {1, 0}, // down
        {0, -1}, // left 
        {-1, 0} // down
};


// function that get the value in the big map by going to the input map by making some conversion
int get_map(int i, int j, int** map, int N, int M){
    int x= (map[i%N][j%M] + (i/N) + (j/M)); // taking into account the number of increments, how far we are in the map from the original one
    return ((x-1) %9 +1);

}


int main (){

    // getting the input 
    FILE* file = fopen("test.txt", "r");
    char line[200];
    int nb_lines = 0;
    int nb_columns = 0;
    while (fgets(line, sizeof(line), file)) {
        nb_lines++;
        nb_columns = strlen(line);
    }
    int nb_lines_real = 5*nb_lines;
    int nb_columns_real = 5*nb_columns;

    rewind(file);
    int** grid = (int**)malloc(nb_lines * sizeof(int*));
    for (int i = 0; i < nb_lines; i++) {
        grid[i] = (int*)malloc(nb_columns * sizeof(int));
    }
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int j = 0; j < nb_columns; j++) {
            grid[i][j] = line[j]-'0';
        }
        i++;
    }

    // print the grid to see if it was read correctly
    printf("grid %dx%d:\n", nb_lines, nb_columns);
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    grid[0][0] = 0;

    // making a cost array 
    int** cost = (int**)malloc(nb_lines_real * sizeof(int*));
    for (int i = 0; i < nb_lines_real; i++) {
        cost[i] = (int*)malloc(nb_columns_real * sizeof(int));
        for (int j = 0; j < nb_columns_real; j++) {
            cost[i][j] = 0;
        }
    }

    // we will use a set to keep track of the points we have visited
    // they will be represented by their index in the grid
    integer_set_t visited;
    integer_set_init(&visited);

    // we will use two heaps to keep track of the points we have visited
    // one will represent by their index in the grid
    // the other will represent their value
    // a point=value*1000+ i*nb_columns+j
    // value=point/1000
    // i=(point%1000)/nb_columns
    // j=(point%1000)%nb_columns
    integer_heap_t points;
    integer_heap_init(&points, integer_MIN_HEAP);
    // start at the top left corner
    integer_heap_insert(&points, 0);
    while (integer_heap_size(&points) > 0){
        
        // getting the next point in our graph path
        int point = 0;
        integer_heap_extract(&points, &point);
        int value = point / 1000;
        int i = (point % 1000) / nb_columns;
        int j = (point % 1000) % nb_columns;
        int point_idx = i * nb_columns + j;

        printf("point %d %d %d\n", value, i, j);

        // if it has already been visited, skip
        if (integer_set_is_member(&visited, point_idx)){
            continue;
        }
        // otherwise, mark it as visited
        integer_set_insert(&visited, point_idx);

        // update the cost of the point
        cost[i][j] = value;

        // if we've reach the end of the grid, we're done
        if (i == nb_lines_real - 1 && j == nb_columns_real - 1){
            break;
        }

        // otherwise, visit the neighbors and add them to the heap to repeat the process
        for (int k=0; k<4; k++){
            int new_i = i + directions[k][0];
            int new_j = j + directions[k][1];

            // if the new point is in the bounds, we add it to the heap
            if (new_i >= 0 || new_i < nb_lines_real || new_j >= 0 || new_j < nb_columns_real){
                int new_point = (value + get_map(new_i, new_j, grid, nb_lines, nb_columns)) * 1000 + new_i * nb_columns + new_j;
                integer_heap_insert(&points, new_point);
            }
        }

    }

    // for the answer 
    int answer = cost[nb_lines_real-1][nb_columns_real-1];
    printf("\nThe answer : %d\n\n", answer);


    // free the memory
    for (int i = 0; i < nb_lines; i++) {
        free(grid[i]);
    }
    free(grid);

    for (int i = 0; i < nb_lines; i++) {
        free(cost[i]);
    }
    free(cost);

    integer_set_destroy(&visited);
    integer_heap_destroy(&points);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// could have worked but it doesn't like that 
// I didn't wanted to make a x25 graph, but I could have done it, it could have been difficult to manage the memory
// but mainly to know which node to connect to which node
// was to difficult for me, so I did it in python 