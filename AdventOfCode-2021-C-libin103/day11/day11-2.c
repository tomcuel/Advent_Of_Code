#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// directions used to determine the neighbours of a location
int directions[8][2] = {{-1,0}, // up
                        {1,0}, // down
                        {0,-1}, // left
                        {0,1}, // right
                        {-1,-1}, // up left
                        {1,1}, // down right
                        {-1,1}, // up right
                        {1,-1}}; // down left


int main (){

    FILE *file = fopen("input11.txt", "r");
    if (file == NULL){
        printf("Error opening file\n");
        return EXIT_FAILURE;
    }
    char line[100];
    int nb_lines = 0;
    int nb_col = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        ++nb_lines;
        nb_col = strlen(line);
    }
    // printf("Nombre de lignes : %d\n", nb_lines);
    // printf("Nombre de colonnes : %d\n", nb_col);
    rewind(file);
    int **octo = malloc(nb_lines * sizeof(int *));
    for (int i = 0; i < nb_lines; ++i){
        octo[i] = malloc(nb_col * sizeof(int));
    }
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        for (int j = 0; j < nb_col; ++j){
            octo[i][j] = line[j]-48;
        }
        ++i;
    }
    
    // printing the octo
    printf("Octo at the beggining : \n");
    for (int i = 0; i < nb_lines; ++i){
        for (int j = 0; j < nb_col; ++j){
            printf("%d", octo[i][j]);
        }
        printf("\n");
    }
    

    // count the number of rounds : 
    int round_all_flashes = 0;
    // while there's not a stopping condition
    while(true){
        // we make one more round
        round_all_flashes += 1;
        // we flash all the octo
        for (int i=0; i<nb_lines; i++){
            for (int j=0; j<nb_col; j++){
                octo[i][j] +=1;
            }
        }

        // making an array of flashed locations 
        // 0  when the location is not flashed, 1 when it is flashed
        int **flashed = malloc(nb_lines * sizeof(int *));
        for (int i = 0; i < nb_lines; ++i){
            flashed[i] = malloc(nb_col * sizeof(int));
            // initialize the array to -1
            for (int j = 0; j < nb_col; ++j){
                flashed[i][j] = 0;
            }
        }

        // we're going to flash the neighbours of the flashes, and if they're over 9, we're going to flash them too
        bool can_flash = true;
        // we continue until we can't flash anymore
        while (can_flash == true){
            // setting the can_flash to false, we will update it to true if we can flash again at any point
            can_flash = false;
            // going through the octo
            for (int i=0; i<nb_lines; i++){
                for (int j=0; j<nb_col; j++){
                    // if the location has to be flashed but hasn't been flashed yet
                    if (flashed[i][j] == 0 && octo[i][j] > 9){
                        // we mark the location as flashed
                        flashed[i][j] = 1;
                        // we will need to flash some neighbours
                        can_flash = true;
                        // we need to flash the neighbours 
                        for (int d=0; d<8; d++){
                            // getting the coordinates of the neighbour
                            int new_i = i + directions[d][0];
                            int new_j = j + directions[d][1];
                            // if the neighbour is in the octo, if it's not outside of the map
                            if (new_i >= 0 && new_i < nb_lines && new_j >= 0 && new_j < nb_col){
                                // we flash the neighbour
                                octo[new_i][new_j] += 1;
                                // if the neighbour is over 9, we will need to flash it too, but it will be down by the next round
                            }
                        }
                    }
                }
            }
        }


        // making the places where there's been a flash to 0
        for (int i=0; i<nb_lines; i++){
            for (int j=0; j<nb_col; j++){
                if (flashed[i][j] == 1){
                    octo[i][j] = 0;
                }
            }
        }

        // check if all the octo has been flashed
        // we supopose that all the octo has been flashed, and if we find one that hasn't been flashed, we change the value of the variable and break the loop
        bool all_flashed = true;
        for (int i=0; i<nb_lines; i++){
            for (int j=0; j<nb_col; j++){
                if (octo[i][j] != 0){
                    all_flashed = false;
                    break;
                }
            }
        }

        // free the memory of the flashed array
        for (int i = 0; i < nb_lines; ++i){
            free(flashed[i]);
        }
        free(flashed);


        // if all the octo has been flashed, we stop the while loop
        if (all_flashed == true){
            break;
        }


        /*
        // print the octo after this round
        printf("Printing the octo after steps n°%d\n",round_all_flashes);
        for (int i = 0; i < nb_lines; ++i){
            for (int j = 0; j < nb_col; ++j){
                printf("%d", octo[i][j]);
            }
        printf("\n");
        }
        */

    }

    // print the octo after all the rounds
    printf("The octo after all the rounds :\n");
    for (int i = 0; i < nb_lines; ++i){
        for (int j = 0; j < nb_col; ++j){
            printf("%d", octo[i][j]);
        }
        printf("\n");
    }


    // print the number of flashes
    printf("\nRound where all flashes : %d\n\n", round_all_flashes);


    // free the memory
    for (int i = 0; i < nb_lines; ++i){
        free(octo[i]);
    }
    free(octo);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// we found the good octo after 193, 194, 195 rounds, the number of step for the octo to be only 0 is 195 as expected for test.txt
// you need no to have an empty line that end your file, otherwise it will not work