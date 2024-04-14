#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// function that returns the minimum of two numbers
int min(int a, int b){
    if (a<b){
        return a;
    }
    else{
        return b;
    }
}
// function that returns the maximum of two numbers
int max(int a, int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}


// function that wether the position is in the target area 
bool is_in_target(int x, int y, char** grid, int height_grid, int width_grid){
    if (grid[y][x]=='T' && x<width_grid && x>=0 && y<height_grid && y>=0){
        return true;
    }
    else{
        return false;
    }
}


// function that returns wether we've past the target area, depending on the velocity, after one more velocity step
bool is_past_target(int x, int y, int vx, int vy, char** grid, int height_grid, int width_grid){
    if (is_in_target(x+vx,y+vy,grid,height_grid,width_grid)){
        return true;
    }
    else{
        return false;
    }
}

int main (){

    // getting the input not by opening the file this time 

    // for the test.txt file
    int x1=20;
    int x2=30;
    int y1=-10;
    int y2=-5;

    /*
    // for the input17.txt file
    int x1=139;
    int x2=187;
    int y1=-148;
    int y2=-89;
    */

    int min_x=min(x1,x2);
    int max_x=max(x1,x2);

    int min_y=min(y1,y2);
    int max_y=max(y1,y2);

    int height_grid = 100-min_y+1;
    int width_grid = max_x+1;

    // what I will do : creating a grid : 
/*

...............................
...............................
...............................
...............................
...............................
...............................
...............................
.............#....#............
.......#..............#........
...............................
S........................#.....
...............................
...............................
...........................#...
...............................
....................TTTTTTTTTTT
....................TTTTTTTTTTT
....................TTTTTTTT#TT
....................TTTTTTTTTTT
....................TTTTTTTTTTT
....................TTTTTTTTTTT

*/

    // creating the grid and initializing it with the target area and the starting point
    char** grid=malloc(height_grid*sizeof(char*));
    for (int i=0; i<height_grid; i++){
        grid[i]=malloc(width_grid*sizeof(char));
    }
    for (int i=0; i<height_grid; i++){
        for (int j=0; j<width_grid; j++){
            if (i > 100-max_y-1 && j > min_x-1){
                grid[i][j]='T';
            }
            else if (i==100 && j==0){
                grid[i][j]='S';
            }
            else {
                grid[i][j]='.';
            }
        }
    }

    
    // printing the grid
    for (int i=0; i<height_grid; i++){
        for (int j=0; j<width_grid; j++){
            printf("%c",grid[i][j]);
        }
        printf("\n");
    }
    

    int target_x[2];
    target_x[0]=min_x;
    target_x[1]=max_x;

    printf("target_x[0] : %d\n",target_x[0]);
    printf("target_x[1] : %d\n",target_x[1]);

    int target_y[2];
    target_y[0]=100-max_y;
    target_y[1]=100-min_y;

    printf("target_y[0] : %d\n",target_y[0]);
    printf("target_y[1] : %d\n",target_y[1]);



    // free the memory
    for (int i=0; i<height_grid; i++){
        free(grid[i]);
    }
    free(grid);

    return EXIT_SUCCESS;
}


// we need to remove the last empty line from the file