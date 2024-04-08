#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  

// struct to represent a cube by its ranges in x, y and z
typedef struct {
    int x1, x2, y1, y2, z1, z2;
} Cube;

// function that returns wether two cubes are equal 
bool are_cubes_equal(Cube cube1, Cube cube2){
    return (cube1.x1 == cube2.x1 && cube1.x2 == cube2.x2 && cube1.y1 == cube2.y1 && cube1.y2 == cube2.y2 && cube1.z1 == cube2.z1 && cube1.z2 == cube2.z2);
}


// function to get the maximum of two numbers
int max(int a, int b){
    if (a>b){
        return a;
    }
    return b;
}
// function to get the minimum of two numbers
int min(int a, int b){
    if (a<b){
        return a;
    }
    return b;
}


// function that find the indexs of the cube to_find in the Cubes_count array
// it returns a -1 array if it is not found
int* find_index (Cube* Cube_count, int num, Cube to_find){
    int* index=malloc(num*sizeof(int));
    for (int i=0; i<num; i++){
        index[i]=-1;
    }
    int k=0;
    for (int i=0; i<num; i++){
        if (are_cubes_equal(Cube_count[i], to_find)){
            index[i]=i;
            k++;
        }
    }
    return (index);
}


// function that returns the intersection of two cubes
// it returns (2,2,2,2,2,2) if the intersection is empty
// cube=(x1,x2,y1,y2,z1,z2)
// need to free the memory after using it
Cube overlap(Cube cube1, Cube cube2){
    Cube answer;

    // testing if the cubes are overlapping
    if (cube1.x1 > cube2.x2 || cube2.x1 > cube1.x2 || cube1.y1 > cube2.y2 || cube2.y1 > cube1.y2 || cube1.z1 > cube2.z2 || cube2.z1 > cube1.z2){
        answer.x1 = 2;
        answer.x2 = 2;
        answer.y1 = 2;
        answer.y2 = 2;
        answer.z1 = 2;
        answer.z2 = 2;
        return answer;
    }
    
    answer.x1 = max(cube1.x1, cube2.x1);
    answer.x2 = min(cube1.x2, cube2.x2);
    answer.y1 = max(cube1.y1, cube2.y1);
    answer.y2 = min(cube1.y2, cube2.y2);
    answer.z1 = max(cube1.z1, cube2.z1);
    answer.z2 = min(cube1.z2, cube2.z2);

    return answer;
}



int main (){

    Cube cube1;
    cube1.x1=1;
    cube1.x2=1;
    cube1.y1=1;
    cube1.y2=1;
    cube1.z1=1;
    cube1.z2=1;

    Cube cubed[3];

    cubed[0].x1=1;
    cubed[0].x2=1;
    cubed[0].y1=1;
    cubed[0].y2=-1;
    cubed[0].z1=4;
    cubed[0].z2=1;

    cubed[1].x1=1;
    cubed[1].x2=1;
    cubed[1].y1=1;
    cubed[1].y2=1;
    cubed[1].z1=1;
    cubed[1].z2=1;

    cubed[2].x1=1;
    cubed[2].x2=1;
    cubed[2].y1=1;
    cubed[2].y2=1;
    cubed[2].z1=1;
    cubed[2].z2=3;

    int* found_index=find_index(cubed, 3, cube1);
    for (int i=0; i<3; i++){
        printf("index n°%d : %d\n", i, found_index[i]);
    }
    free(found_index);

    Cube rep=overlap(cubed[0], cubed[1]);
    printf("%d %d %d %d %d %d\n", rep.x1, rep.x2, rep.y1, rep.y2, rep.z1, rep.z2);

    return 2;
}