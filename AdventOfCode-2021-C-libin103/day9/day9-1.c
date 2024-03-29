#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


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
    
    // printing the input to check
    printf("The input : \n"); 
    for (int i = 0; i < nb_line; i++) {
        for (int j = 0; j < strlen(line); j++) {
            printf("%d", input[i][j]);
        }
        printf("\n");
    }
    

    int direction[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}}; // right, left, up, down

    // watching if the location is under its neighbors 
    int answer = 0;
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
                answer+=1+input[i][j];
            }
        }
    }


    // printing the answer
    printf("The number of locations that are under their neighbors is %d\n", answer);


    // free the memory
    for (int i = 0; i < nb_line; i++) {
        free(input[i]);
    }
    free(input);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// I found 15 for the test.txt file as expected