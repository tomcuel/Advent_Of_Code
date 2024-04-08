#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


long int size_grid=101;
long int translation=50;


// function to says if a character is in a string
bool is_in(char c, char* string){
    for (int i=0; i<strlen(string); i++){
        if (c==string[i]){
            return true;
        }
    }
    return false;
}


// function to tell if the x-range, y-range and z-range are in the limits of the grid 
bool is_within_bounds_range(long int x1, long int x2, long int y1, long int y2, long int z1, long int z2){
    if (x1>=0 && x2<size_grid && y1>=0 && y2<size_grid && z1>=0 && z2<size_grid){
        return true;
    }
    return false;
}


int main (){

    // opening the file and getting the data 
    FILE *file = fopen("input22.txt", "r");
    char line[100];

    // counting the number of lines in the file
    int nb_lines = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        ++nb_lines;
    }
    // printf("number of lines : %d\n", nb_lines);
    rewind(file);

    // button[i]=n means on and button[i]=f means off
    char* button=malloc(nb_lines*sizeof(char));

    // array to get the points that will be enligthened or turned off
    // they will be represented by a 6tuple (x1,x2,y1,y2,z1,z2) that get the ranges of the x,y,z coordinates on which we apply the command
    long int** positions=malloc(nb_lines*sizeof(long int*));
    for (int i=0; i<nb_lines; i++){
        positions[i]=malloc(6*sizeof(long int));
        // initializing the array
        for (int j=0; j<6; j++){
            positions[i][j]=0;
        }
    }
    int i=0;
    while (fgets(line, sizeof(line), file) != NULL) {
        
        // getting what type of button this is
        if (line[1]=='n'){
            button[i]='n';
        }
        else if (line[1]=='f'){
            button[i]='f';
        }

        char* token_command = strtok(line, " ");
        // we've the part with the position
        token_command = strtok(NULL, " ");
        // printf("%s\n", token_command);

        // we can now split token on ".." to get the ranges 
        char** tokens = malloc(3*sizeof(char*));
        for (int j=0; j<3; j++){
            tokens[j]=malloc(10*sizeof(char));
        }
        char* token = strtok(token_command, ",");
        int j=0;
        while (token != NULL){
            // printf("%s\n", token);
            // better than strcpy for memory allocation
            tokens[j]=strdup(token);
            j++;
            token = strtok(NULL, ",");
        }
        // I've already made the translation to have the right values of the puzzle, so it's easier afterwards
        int k=0;
        for (int j=0; j<3; j++){
            char* tokenj = strtok(tokens[j], "=");
            tokenj = strtok(NULL, "=");
            char* tokenjpos = strtok(tokenj, "..");
            positions[i][k]=atoi(tokenjpos)+translation;
            // printf("%d\n", positions[i][k]);
            k=k+1;
            tokenjpos = strtok(NULL, "..");
            positions[i][k]=atoi(tokenjpos)+translation;
            // printf("%d\n", atoi(tokenjpos));
            k=k+1;
        }

        // free the memory
        for (int j=0; j<3; j++){
            free(tokens[j]);
        }
        free(tokens);

        // incrementing the counter of lines we've read
        i++;
    }
    /*
    // printing everything
    for (int i=0; i<nb_lines; i++){
        printf("button %d : %c for pos_range : ", i, button[i]);
        for (int j=0; j<6; j++){
            printf("%ld ", positions[i][j]);
            if (j%2 == 1){
                printf(", ");
            }
        }
        printf("\n");
    }
    */
    
    // creating the grid
    // it will be a 101x101 grid to cover the -50 to 50 range, I will make translation to be sure to get the right things
    // there will be 1 if the light is on and 0 if the light is off
    long int*** grid = malloc(size_grid*sizeof(long int**));
    for (int i=0; i<size_grid; i++){
        grid[i]=malloc(size_grid*sizeof(long int*));
        for (int j=0; j<size_grid; j++){
            grid[i][j]=malloc(size_grid*sizeof(long int));
            for (int k=0; k<size_grid; k++){
                grid[i][j][k]=0;
            }
        }
    }

    // applying the commands for each line 
    for (int p=0; p<nb_lines; p++){
        // verifying that the ranges are within the grid
        if (is_within_bounds_range(positions[p][0], positions[p][1], positions[p][2], positions[p][3], positions[p][4], positions[p][5])==false){
            continue;
        }
        
        // going through the x ranges
        for (int i=positions[p][0]; i<=positions[p][1]; i++){
            // going through the y ranges
            for (int j=positions[p][2]; j<=positions[p][3]; j++){
                // going through the z ranges
                for (int k=positions[p][4]; k<=positions[p][5]; k++){
                    // if the button is on, we turn on the light
                    if (button[p]=='n'){
                        grid[i][j][k]=1;
                    }
                    // if the button is off, we turn off the light
                    else if (button[p]=='f'){
                        grid[i][j][k]=0;
                    }
                }
            }
        }
    }


    // counting the number of lights that are on
    long int count=0;
    for (int i=0; i<size_grid; i++){
        for (int j=0; j<size_grid; j++){
            for (int k=0; k<size_grid; k++){
                // if the light is on, we increment the counter
                if (grid[i][j][k]==1){
                    count++;
                }
            }
        }
    }


    // printing the result
    printf("\nThe number of lights that are on is : %ld\n\n", count);


    // free the memory
    free(button);
    for (int i=0; i<nb_lines; i++){
        free(positions[i]);
    }
    free(positions);

    for (int i=0; i<size_grid; i++){
        for (int j=0; j<size_grid; j++){
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);

    // closing the file
    fclose(file);

    return EXIT_SUCCESS;
}

// on the small_test.txt file, the result is 39 as expected
// on the medium_test.txt file, the result is 590784 as expected
// need to supress the last empty line of the input file to get the right result
// I had a seg fault because I used strcpy rather than strdup for the memory allocation of the tokens