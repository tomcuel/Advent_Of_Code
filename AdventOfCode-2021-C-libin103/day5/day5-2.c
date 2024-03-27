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

// function to get the sign of a number
int sign(int num) {
    if (num < 0)
        return -1;
    else if (num == 0)
        return 0;
    else
        return 1;
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
        lines[i]=(int*)malloc(6*sizeof(int));
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
        // getting the direction we're heading to
        lines[i][4]=sign(lines[i][2]-lines[i][0]);
        lines[i][5]=sign(lines[i][3]-lines[i][1]);

        ++i;
    }

    /*
    // print the lines and the direction associated 
    printf("lines : \n");  
    for (int i=0; i<nb_lines; i++){
        printf("%d,%d -> %d,%d : direction : (%d,%d)\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3], lines[i][4], lines[i][5]);
    }
    */
    

    // supress the lines where the movements isn't a vertical or horizontal line
    // in reality, we put -1 in the coordinates of the lines that are not vertical or horizontal or real diagonals
    // by using the directions part in lines array

    for (int i=0; i<nb_lines; i++){
        bool is_diagonal=false;
        if (abs(lines[i][4])==abs(lines[i][5])){
            is_diagonal=true;
        }

        if (lines[i][4]!=0 && lines[i][5]!=0 && !is_diagonal){
            lines[i][0]=-1;
            lines[i][1]=-1;
            lines[i][2]=-1;
            lines[i][3]=-1;
        }
    }
    
    /*
    // print the lines and the direction associated 
    printf("lines : \n");  
    for (int i=0; i<nb_lines; i++){
        printf("%d,%d -> %d,%d : direction : (%d,%d)\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3], lines[i][4], lines[i][5]);
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
        // if the line is not a -1 line, it means it is a vertical or horizontal line or diagonal 
        printf("line : %d,%d -> %d,%d\n, in direction : %d,%d\n", lines[i][0], lines[i][1], lines[i][2], lines[i][3], lines[i][4], lines[i][5]);
        if (lines[i][0]!=-1){

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
                continue;
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
                continue;
            }

            // if the line is a diagonal, we go through the direction of the line
            int temp_x=lines[i][0];
            int temp_y=lines[i][1];
            // while we haven't reached the end of the line (lines[i][2], lines[i][3])
            // we go through the direction (lines[i][4],lines[i][5]) and increment the cover array
            while (temp_x!=lines[i][2] && temp_y!=lines[i][3]){
                cover[temp_x][temp_y]++;
                temp_x+=lines[i][4];
                temp_y+=lines[i][5];
            }
            cover[temp_x][temp_y]++;
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
    printf("\n");   
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


// I found 12 for the test.txt as expected 