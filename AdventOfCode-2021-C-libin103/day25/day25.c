#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// function to do one step of the puzzle 
// It modifies the grid and returns if others moves are possible with the boolean
bool do_steps(char** grid, int nb_lines, int nb_columns, char type_of_arrows){
    // copying the grid to not modify the original one, we will do that after 
    char** grid_copy = (char**)malloc(nb_lines*sizeof(char*));
    for (int i = 0; i < nb_lines; i++){
        grid_copy[i] = (char*)malloc(nb_columns*sizeof(char));
        for (int j = 0; j < nb_columns; j++){
            grid_copy[i][j] = grid[i][j];
        }
    }

    // making the boolean to check if we can move in the direction
    bool can_move_direction = false;

    // going through the grid to make the moves
    for (int j = 0; j < nb_columns; j++){
        for (int i = 0; i < nb_lines; i++){
            // if the cell is not the type of arrow we are looking for, we continue
            if (type_of_arrows != grid_copy[i][j]){
                continue;
            }

            // depending on the type of arrow, we check if we can move in the direction after getting the direction 
            int dest_i = -1;
            int dest_j = -1;
            if (grid_copy[i][j] == 'v'){
                dest_i = (i+1)%nb_lines;
                dest_j = j;
            }
            else if (grid_copy[i][j] == '>'){
                dest_i = i;
                dest_j = (j+1)%nb_columns;
            }

            // if the grid at the destination is empty, we can move in the direction
            if (grid_copy[dest_i][dest_j] == '.' && dest_i != -1 && dest_j != -1){
                can_move_direction = true;
                grid[dest_i][dest_j] = grid_copy[i][j];
                grid[i][j] = '.';
            }
            
        }
    }


    // free the memory before returning
    for (int i = 0; i < nb_lines; i++){
        free(grid_copy[i]);
    }
    free(grid_copy);

    // return the boolean to check if we can move in the direction
    return can_move_direction;
}


int main (){
  
    FILE* file = fopen("input25.txt", "r");
    char line[200];
    int nb_lines = 0;
    int nb_columns = 0;
    while (fgets(line, sizeof(line), file)) {
        nb_lines++;
        nb_columns = strlen(line);
    }
    // printf("nb_lines: %d\n", nb_lines);
    // printf("nb_columns: %d\n", nb_columns);
    rewind(file); 
    char** map = (char**)malloc(nb_lines*sizeof(char*));
    for (int i = 0; i < nb_lines; i++){
        map[i] = (char*)malloc(nb_columns*sizeof(char));
    }
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int j = 0; j < nb_columns; j++){
            map[i][j] = line[j];
        }
        i++;
    }
    /*
    // print the map to check if it's correct
    printf("nb_lines: %d\n", nb_lines);
    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < nb_columns; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    */


    // Count the number of steps before it can't move anymore
    int steps=0;
    bool test = true; 
    // while we didn't break the loop
    while (test==true){
        // we update the map and check if we can move south or east
        bool can_move_east = do_steps(map, nb_lines, nb_columns, '>');
        bool can_move_south = do_steps(map, nb_lines, nb_columns, 'v');
        // we increment the number of steps made 
        steps=steps+1;

        /*
        // print the map to check after 4 steps for small_test.txt
        if (steps == 4){
            printf("map after %d steps:\n", steps);
            for (int i = 0; i < nb_lines; i++){
                for (int j = 0; j < nb_columns; j++){
                    printf("%c", map[i][j]);
                }
                printf("\n");
            }
            printf("\n");
            break;
        }
        */

        // if no moves are possible, we break the loop
        if (!can_move_south && !can_move_east){
            test=false;
        }

    }


    // printing the answer
    printf("\nThe number of steps made before it can't move is : %d\n", steps);


    // free the memory
    for (int i = 0; i < nb_lines; i++){
        free(map[i]);
    }
    free(map);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// need to remove the last line of the file so it doesn't mess up the number of lines we calculate
// I found 58 as expected for the test.txt file
// I found the right map after 4 steps for the small_test.txt file example 