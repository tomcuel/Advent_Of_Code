#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// function searching around a basin the neighbors that are over the basin
// we then do it recursively but we need the difference between the height of the basin and the height of the neighbor to be 1 
// and also for the neighbor of the neighbor that match the condition


// including the libin structure we will use to do the dfs 
#include "integer_stack.h"
#include "integer_set.h"


int main (){

    FILE *file = fopen("input9.txt", "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier input.txt\n");
        return EXIT_FAILURE;
    }

    char line[105];
    // counting the number of lines in the file
    int nb_line = 0;
    int nb_col = 0;
    while (fgets(line, sizeof(line), file)) {
        ++nb_line;
        nb_col = strlen(line);
    }
    // getting the input in an array
    int** input = malloc(nb_line * sizeof(int*));
    for (int i = 0; i < nb_line; i++) {
        input[i] = malloc(105 * sizeof(int));
    }

    // reoppening the file to read from the beginning
    rewind(file);
    // getting the input in an array
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int j= 0; j < strlen(line); j++) {
            input[i][j] = line[j]-'0';
        }
        ++i;
    }
    /*
    // printing the input to check
    printf("The input : \n"); 
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < strlen(line); j++) {
            printf("%d", input[i][j]);
        }
        printf("\n");
    }
    */

    int direction[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}}; // right, left, up, down
    
    // getting 
    int** pos_basin= malloc(nb_line * sizeof(int*));
    for (int i = 0; i < nb_line; i++) {
        pos_basin[i] = malloc(nb_col * sizeof(int));
        // initializing the array with 0
        for (int j = 0; j < nb_col; j++) {
            pos_basin[i][j] = 0;
        }
    }

    // watching if the location is under its neighbors 
    // adding it to the pos_basin array, to give the basins
    for (int i=0; i<nb_line; i++) {
        for (int j=0; j<nb_col; j++) {
            bool is_under = true;
            for (int k=0; k<4; k++) {
                int new_i = i + direction[k][0];
                int new_j = j + direction[k][1];
                
                // if the location is on the border, we continue if we go out of the border
                if (!(0 <= new_i && new_i < nb_line) || !(0 <= new_j && new_j < nb_col)) {
                    continue;
                }

                // if the location is not under its neighbors, we break the loop
                if (input[i][j] >= input[new_i][new_j]) {
                    is_under = false;
                    break;
                }
                
            }

            // if the location is under its neighbors, we add 1 + the height of the location
            if (is_under == true) {
                pos_basin[i][j] = 1;
            }
        }
    }

    /*
    // printing the basin array to check
    printf("The basins are here : \n");
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < nb_col; j++) {
            printf("%d", pos_basin[i][j]);
        }
        printf("\n");
    }
    */

    // we're going to do a dfs to find the neighbors that are over the basin
    // using a queue from the library integer_queue.h
    // since we can't put a 2D array in a queue, we're going to put the position of the neighbors in the queue
    // the position is for (i,j) pos=i*nb_col+j
    // we can do the reverse operation to get the position in the 2D array : i = pos/nb_col and j = pos%nb_col
    
    // getting the number of basins
    int nb_basins = 0;
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < nb_col; j++) {
            if (pos_basin[i][j] == 1) {
                ++nb_basins;
            }
        }
    }
    // doing a queue for each basin
    integer_stack_t queue[nb_basins];
    for (int i = 0; i < nb_basins; i++) {
        integer_stack_init(&queue[i]);
    }

    // putting the basins in each queue, to do a dfs for each basin
    int p=0;
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < nb_col; j++) {
            if (pos_basin[i][j] == 1) {
                int pos = i*nb_col+j;
                integer_stack_push(&queue[p], pos);
                ++p;
            }
        }
    }

    // initialize the size of each basin
    int basin_size[nb_basins];
    for (int i = 0; i < nb_basins; i++) {
        basin_size[i] = 0;
    }

    // we do the array of the basins, numeroting them by 1, 2, 3, ...
    int** basins= malloc(nb_line * sizeof(int*));
    for (int i = 0; i < nb_line; i++) {
        basins[i] = malloc(nb_col * sizeof(int));
        // initializing the array with 0
        for (int j = 0; j < nb_col; j++) {
            basins[i][j] = 0;
        }
    }


    for (int k=0; k<nb_basins; k++){
        // printf("current_basin : %d\n", k+1);

        integer_set_t visited;
        integer_set_init(&visited);

        // we're going to do a dfs to find the neighbors that are over the basin by only 1 and then add +1 to the size of the basin
        while (integer_stack_size(&queue[k]) > 0) {
            int pos=0;
            integer_stack_pop(&queue[k], &pos);
            int i = pos/nb_col;
            int j = pos%nb_col;

            // if the position is already visited, we continue
            if (integer_set_is_member(&visited, pos)) {
                continue;
            }
            // otherwise we add the position to the visited set
            integer_set_insert(&visited, pos);


            // fulling the basin array with the current basin
            basins[i][j] = k+1;
            // printf("basins[%d][%d] : %d\n", i, j, basins[i][j]);    


            for (int p=0; p<4; p++){
                int new_i = i + direction[p][0];
                int new_j = j + direction[p][1];
                int new_pos = new_i*nb_col+new_j;

                // if the location is on the border, we continue if we go out of the border
                if (!(0 <= new_i && new_i < nb_line) || !(0 <= new_j && new_j < nb_col)) {
                    continue;
                }

                // if the location is 9 height we continue
                if (input[new_i][new_j] ==9) {
                    continue;
                }

                // if the location is over the basin, we add it to the queue
                integer_stack_push(&queue[k], new_pos);
                
            }
        }

        // destroying the visited set
        integer_set_destroy(&visited);
    }

    /*
    // printing what the basins are, we see them by their number
    printf("the basins are : \n");
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < nb_col; j++) {
            printf("%d", basins[i][j]);
        }
        printf("\n");
    }
    */
    

    // we need to get the size of the three largest bassins and then multiplie them together
    int* occurrences = malloc((nb_basins+1) * sizeof(int));

    // Go through each list of the basins matrix
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < nb_col; j++) {
            // imcrement the counter for each element
            occurrences[basins[i][j]]++;
        }
    }
    int* size_basins= malloc(nb_basins * sizeof(int));
    for (int i = 0; i < nb_basins; i++) {
        size_basins[i] = occurrences[i+1];
    }

    /*
    // printing the occurrences    
    printf("The occurrences of each number : \n");
    for (int i = 0; i < nb_basins; i++) {
        printf("%d, ", i+1, size_basins[i]);
    }
    printf("\n");
    */
    
    
    // we need to get the three largest basins
    int max1 = 0;
    int max2 = 0;
    int max3 = 0;
    for (int i = 0; i < nb_basins; i++) {
        if (size_basins[i] > max1) {
            max3 = max2;
            max2 = max1;
            max1 = size_basins[i];
        } 
        else if (size_basins[i] > max2) {
            max3 = max2;
            max2 = size_basins[i];
        } 
        else if (size_basins[i] > max3) {
            max3 = size_basins[i];
        }
    }
    // printing the three largest basins
    printf("\nThe three largest basins are : %d, %d, %d\n", max1, max2, max3);
    // printing the multiplication of the three largest basins
    printf("\nThe multiplication of the three largest basins is : %d\n\n", max1*max2*max3);


    // free the memomry of the used structures 
    for (int i = 0; i < nb_line; i++){
        integer_stack_destroy(&queue[i]);
    }

    // free the memory
    for (int i = 0; i < nb_line; i++) {
        free(input[i]);
    }
    free(input);

    for (int i = 0; i < nb_line; i++) {
        free(pos_basin[i]);
    }
    free(pos_basin);

    for (int i = 0; i < nb_line; i++) {
        free(basins[i]);
    }
    free(basins);

    free(occurrences);
    free(size_basins);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 1134 for the test.txt file as expected