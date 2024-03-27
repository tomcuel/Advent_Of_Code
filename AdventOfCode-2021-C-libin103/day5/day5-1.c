#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// function to get the min of two numbers 
int min (int a, int b){
    if (a<b){
        return(a);
    }
    else {
        return(b);
    }
}

// function to get the max of two numbers 
int max (int a, int b){
    if (a>b){
        return(a);
    }
    else {
        return(b);
    }
}



int main (){

    // open the file
    FILE *file;
    file = fopen("input5.txt", "r");
    char line[100];

    // check if the file is opened correctly
    if (file == NULL){
        printf("Error in opening the file\n");
        return EXIT_FAILURE;
    }

    // get the number of lines in the file
    int nb_lines = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        nb_lines++;
    }

    rewind(file);
    // making a 2D array to store the coordinates of the input
    int** lines=(int**)malloc(nb_lines*sizeof(int*));
    for (int i=0; i<nb_lines; i++){
        lines[i]=(int*)malloc(4*sizeof(int));
    }

    // read the file line by line and get the coordinates
    int i=0;
    while (fgets(line, sizeof(line), file) != NULL) {
        int num1_x, num1_y, num2_x, num2_y;
        sscanf(line, "%d,%d -> %d,%d", &num1_x, &num1_y, &num2_x, &num2_y);
        // printf("Coordinates :" "%d,%d -> %d,%d\n", num1_x, num1_y, num2_x, num2_y);
        lines[i][0]=num1_x;
        lines[i][1]=num1_y;
        lines[i][2]=num2_x;
        lines[i][3]=num2_y;
        ++i;
    }
    /*
    // print the lines
    printf("lines : \n");  
    for (int i=0; i<nb_lines; i++){
        printf("%d,%d -> %d,%d\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
    }
    */

    // supress the lines where the movements isn't a vertical or horizontal line
    // in reality, we put -1 in the coordinates of the lines that are not vertical or horizontal

    for (int i=0; i<nb_lines; i++){
        if (lines[i][0]!=lines[i][2] && lines[i][1]!=lines[i][3]){
            lines[i][0]=-1;
            lines[i][1]=-1;
            lines[i][2]=-1;
            lines[i][3]=-1;
        }
    }
    /*
    // print the lines
    printf("lines : \n");  
    for (int i=0; i<nb_lines; i++){
        printf("%d,%d -> %d,%d\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
    }
    */


    // get the maximum x and y coordinates remaining
    // ie the boundaries of the grid
    int max_x=0, max_y=0;
    for (int i=0; i<nb_lines; i++){

        if (lines[i][0]>max_x){
            max_x=lines[i][0];
        }

        if (lines[i][1]>max_y){
            max_y=lines[i][1];
        }

        if (lines[i][2]>max_x){
            max_x=lines[i][2];
        }

        if (lines[i][3]>max_y){
            max_y=lines[i][3];
        }

    }
    // printf("max_x, max_y : %d, %d\n", max_x, max_y);
   
    // we also create a cover array to keep track of how many lines are covering a point
    // we initialize it to 0
    int** cover=(int**)malloc((max_x+1)*sizeof(int*));
    for (int i=0; i<max_x+1; i++){
        cover[i]=(int*)malloc((max_y+1)*sizeof(int));
    }
    for (int i=0; i<max_x+1; i++){
        for (int j=0; j<max_y+1; j++){
            cover[i][j]=0;
        }
    }


    // we now fill the cover array by going through the lines
    for (int i=0; i<nb_lines; i++){
        // if the line is not a -1 line, it means it is a vertical or horizontal line
        if (lines[i][0]!=-1){
            // printing the line to be sure this is an horizontal of vertical line
            // printf("line : %d,%d -> %d,%d\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3]);

            // looking wether the line is vertical or horizontal
            // if the line is horizontal, we go through the y coordinates (the columns)
            if (lines[i][0]==lines[i][2]){
                // the bottom is the lines[i][1] and lines[i][3]
                // the top is the max lines[i][1] and lines[i][3]
                int bottom=min(lines[i][1],lines[i][3]);
                int top=max(lines[i][1],lines[i][3]);
                for (int j=bottom; j<top+1;j++){
                    cover[lines[i][0]][j]++;
                }
            }

            // if the line is vertical, we go through the x coordinates (the rows)
            if (lines[i][1]==lines[i][3]){
                // the bottom is the lines[i][0] and lines[i][2]
                // the top is the max lines[i][0] and lines[i][2]
                int bottom=min(lines[i][0],lines[i][2]);
                int top=max(lines[i][0],lines[i][2]);
                for (int j=bottom; j<top+1;j++){
                    cover[j][lines[i][1]]++;
                }
            }
          
        }
    }
    /*
    // print the cover array
    printf("cover : \n");
    for (int i=0; i<max_x+1; i++){
        for (int j=0; j<max_y+1; j++){
            printf("%d ", cover[i][j]);
        }
        printf("\n");
    }
    */

    // getting the answer by counting the number of times there is more than one line that covers the area 
    int answer=0;
    for (int i=0; i<max_x+1; i++){
        for (int j=0; j<max_y+1; j++){
            if (cover[i][j] > 1){
                answer++;
            }
        }
    }


    // printing the answer 
    printf("\nThe answer is : %d\n\n",answer);

    // free the memory
    for (int i=0; i<nb_lines; i++){
        free(lines[i]);
    }
    free(lines);

  
    for (int i=0; i<nb_lines; i++){
        free(cover[i]);
    }
    free(cover);

    fclose(file);
    return EXIT_SUCCESS;
}


// I found 5 for the test.txt as expected 