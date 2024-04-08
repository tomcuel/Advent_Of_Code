#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// non successful attempt to solve the problem


// here we want to count the number of pixels that are on
// to do so, we will do it not by brute force, but by figuring out the number of pixels that are on in each cube of the input 
// the difficulty here is to find the intersection of two cubes and deals with them properly
// Id doesn't work for me, even if it does in Python 
// I didn't figure how to make all the different crosses of cubes that overlap in C even if I succeeded in Python to do it 


// function to says if a character is in a string
bool is_in(char c, char* string){
    for (int i=0; i<strlen(string); i++){
        if (c==string[i]){
            return true;
        }
    }
    return false;
}


// struct to represent a cube by its ranges in x, y and z
typedef struct {
    long long int x1, x2, y1, y2, z1, z2;
} Cube;


// structure to represent a cube and the number of times it is counted
typedef struct {
    Cube cube;
    int count;
} CubeCount;


// function to get the volume of the cube
long long int volume_cube(Cube cube){
    return (cube.x2 - cube.x1 + 1) * (cube.y2 - cube.y1 + 1) * (cube.z2 - cube.z1 + 1);
}


// function to get the maximum of two numbers
long long int max(long long int a, long long int b){
    if (a>b){
        return a;
    }
    return b;
}
// function to get the minimum of two numbers
long long int min(long long int a, long long int b){
    if (a<b){
        return a;
    }
    return b;
}


// function that returns wether two cubes are equal 
bool are_cubes_equal(Cube cube1, Cube cube2){
    return (cube1.x1 == cube2.x1 && cube1.x2 == cube2.x2 && cube1.y1 == cube2.y1 && cube1.y2 == cube2.y2 && cube1.z1 == cube2.z1 && cube1.z2 == cube2.z2);
}


// function that find the indexs of the cube to_find in the Cubes_count array
// it returns -1 if it is not found
// it returns the number of times it is found if it is found more than once
int nb_index (Cube* Cube_count, int num, Cube to_find){
    int nb_index=0;
    for (int i=0; i<num; i++){
        if (are_cubes_equal(Cube_count[i], to_find)){
            nb_index++;
        }
    }
    if (nb_index==0){
        return -1;
    }
    return (nb_index);
}


// function that returns the first index of the cubes that are equal to the cube to_find
// it allows us to make operations only on one cube if two are the same
int get_first_index (CubeCount* Cube_count, int num, Cube to_find){
    int indexes=-1;
    for (int i=0; i<num; i++){
        if (are_cubes_equal(Cube_count[i].cube, to_find)){
            indexes=i;
            return indexes;
        }
    }
    return indexes;
}


// function that returns the intersection of two cubes
// it returns (0,0,0,0,0,0) if the intersection is empty
// cube=(x1,x2,y1,y2,z1,z2)
// need to free the memory after using it
Cube overlap(Cube cube1, Cube cube2){
    Cube answer;

    // testing if the cubes are overlapping
    if (cube1.x1 > cube2.x2 || cube2.x1 > cube1.x2 || cube1.y1 > cube2.y2 || cube2.y1 > cube1.y2 || cube1.z1 > cube2.z2 || cube2.z1 > cube1.z2){
        answer.x1 = 0;
        answer.x2 = 0;
        answer.y1 = 0;
        answer.y2 = 0;
        answer.z1 = 0;
        answer.z2 = 0;
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

    // opening the file and getting the data 
    FILE *file = fopen("large_test.txt", "r");
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
    long long int** positions=malloc(nb_lines*sizeof(long long int*));
    for (int i=0; i<nb_lines; i++){
        positions[i]=malloc(6*sizeof(long long int));
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
            positions[i][k]=atoi(tokenjpos);
            // printf("%d\n", positions[i][k]);
            k=k+1;
            tokenjpos = strtok(NULL, "..");
            positions[i][k]=atoi(tokenjpos);
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
            printf("%lld ", positions[i][j]);
            if (j%2 == 1){
                printf(", ");
            }
        }
        printf("\n");
    }
    */
    Cube* cubes = malloc(nb_lines * sizeof(Cube));
    for (int i = 0; i < nb_lines; i++) {
        cubes[i].x1 = positions[i][0];
        cubes[i].x2 = positions[i][1];
        cubes[i].y1 = positions[i][2];
        cubes[i].y2 = positions[i][3];
        cubes[i].z1 = positions[i][4];
        cubes[i].z2 = positions[i][5];
    }
    for (int i=0; i<nb_lines; i++){
        free(positions[i]);
    }
    free(positions);
    /*
    // printing the cubes and the buttons
    for (int i=0; i<nb_lines; i++){
        printf("button %d : %c for cube : (%lld, %lld, %lld, %lld, %lld, %lld)\n", i, button[i], cubes[i].x1, cubes[i].x2, cubes[i].y1, cubes[i].y2, cubes[i].z1, cubes[i].z2);
    }
    */
    // Size of the dict count 
    // this dict will get us to know which cube are on or off 
    // on_off[i] = 1 if the cube is on, 0 if not 
    // Cube_count[i] give us the cube that is considered at this rank 
    int num=1000;
    int nb_cubes_filled=0;

    // give the cubes we've gone through and marked if they are open (1) or not (-1), because we need to do operations on them
    // equivalent of a defaultdict in python
    // this array give a unique count for each cube we've gone through
    CubeCount* Cube_counts=malloc(num*sizeof(Cube));
    for (int i=0; i<num; i++){
        Cube_counts[i].cube.x1=0;
        Cube_counts[i].cube.x2=0;
        Cube_counts[i].cube.y1=0;
        Cube_counts[i].cube.y2=0;
        Cube_counts[i].cube.z1=0;
        Cube_counts[i].cube.z2=0;
        Cube_counts[i].count=0;
    }
    // making the cubes of light 
    for (int p=0; p<nb_lines; p++){
        char button_p = button[p];
        Cube cubek = cubes[p];

        // creating the equivalent of a new_count of the cubes as a defaultdict
        CubeCount* new_cube_count=malloc(num*sizeof(Cube));
        for (int i=0; i<num; i++){
            new_cube_count[i].cube.x1=0;
            new_cube_count[i].cube.x2=0;
            new_cube_count[i].cube.y1=0;
            new_cube_count[i].cube.y2=0;
            new_cube_count[i].cube.z1=0;
            new_cube_count[i].cube.z2=0;
            new_cube_count[i].count=0;
        }

        // making the equivalent of a set of cubes, so that they are unique in it 
        Cube* set_cube=malloc(num*sizeof(Cube));
        // initializing the set of cubes
        for (int i=0; i<num; i++){
            set_cube[i].x1=0;
            set_cube[i].x2=0;
            set_cube[i].y1=0;
            set_cube[i].y2=0;
            set_cube[i].z1=0;
            set_cube[i].z2=0;
        }
        int k=0;
        // we go through the cube_count array
        for (int i=0; i<num; i++){
            // we get the index of this cube in set_cube
            int indexs = nb_index(set_cube, num, Cube_counts[i].cube);
            // if this cube isn't in the set, we can add it in there 
            if (indexs == -1){
                set_cube[k].x1=Cube_counts[i].cube.x1;
                set_cube[k].x2=Cube_counts[i].cube.x2;
                set_cube[k].y1=Cube_counts[i].cube.y1;
                set_cube[k].y2=Cube_counts[i].cube.y2;
                set_cube[k].z1=Cube_counts[i].cube.z1;
                set_cube[k].z2=Cube_counts[i].cube.z2;
                ++k;
            }
        }

    
        if (p==5){
            // printing the count of the cubes if there are not Cube with only 0 as coordinates
            for (int i=0; i<num; i++){
                if (Cube_counts[i].cube.x1!=0 || Cube_counts[i].cube.x2!=0 || Cube_counts[i].cube.y1!=0 || Cube_counts[i].cube.y2!=0 || Cube_counts[i].cube.z1!=0 || Cube_counts[i].cube.z2!=0){
                    printf("cube %d : (%lld, %lld, %lld, %lld, %lld, %lld) with count %d\n", i, Cube_counts[i].cube.x1, Cube_counts[i].cube.x2, Cube_counts[i].cube.y1, Cube_counts[i].cube.y2, Cube_counts[i].cube.z1, Cube_counts[i].cube.z2, Cube_counts[i].count);
                }
            }
        }


        int nb_new_cubes_filled_after=0;
        // we go through the set of cubes
        for (int i=0; i<k; i++){
            int index_set_cube_k = get_first_index(Cube_counts, num, set_cube[i]);
            bool set_switch = false;
            if (Cube_counts[index_set_cube_k].count > 0){
                set_switch = true;
            }
            if (set_switch==false){
               continue;
            }
            Cube overlapped = overlap(cubek, set_cube[i]);
            // if the cube is not overlapping, we skip the rest of the process
            if (overlapped.x1 == 0 && overlapped.x2 == 0 && overlapped.y1 == 0 && overlapped.y2 == 0 && overlapped.z1 == 0 && overlapped.z2 == 0){
                continue;
            }

            // adding the overlapped cube to the new_count_cube
            new_cube_count[nb_new_cubes_filled_after].cube.x1 = overlapped.x1;
            new_cube_count[nb_new_cubes_filled_after].cube.x2 = overlapped.x2;
            new_cube_count[nb_new_cubes_filled_after].cube.y1 = overlapped.y1;
            new_cube_count[nb_new_cubes_filled_after].cube.y2 = overlapped.y2;
            new_cube_count[nb_new_cubes_filled_after].cube.z1 = overlapped.z1;
            new_cube_count[nb_new_cubes_filled_after].cube.z2 = overlapped.z2;
            new_cube_count[nb_new_cubes_filled_after].count = 0;

            new_cube_count[nb_new_cubes_filled_after].count -= Cube_counts[index_set_cube_k].count; // reset to 0
            ++nb_new_cubes_filled_after;

        }

        // if the button is on, we increment the count of the new_count_cube, 
        if (button_p == 'n'){
            // adding the cubek into the count_cube
            new_cube_count[nb_new_cubes_filled_after].cube.x1 = cubek.x1;
            new_cube_count[nb_new_cubes_filled_after].cube.x2 = cubek.x2;
            new_cube_count[nb_new_cubes_filled_after].cube.y1 = cubek.y1;
            new_cube_count[nb_new_cubes_filled_after].cube.y2 = cubek.y2;
            new_cube_count[nb_new_cubes_filled_after].cube.z1 = cubek.z1;
            new_cube_count[nb_new_cubes_filled_after].cube.z2 = cubek.z2;
            new_cube_count[nb_new_cubes_filled_after].count =0;
            // incrementing the number of cubes filled
            nb_new_cubes_filled_after++;

            int indexk = get_first_index(new_cube_count, num, cubek);
            new_cube_count[indexk].count += 1;
        }
        
        // updating the counts with the new counts, by changing the count of the cubes that are already in the count_cube, and adding the new cubes that are not in the count_cube otherwise
        int nb_new = 0;
        for (int i=0; i<nb_new_cubes_filled_after; i++){
            int index_i = get_first_index(Cube_counts, num, new_cube_count[i].cube);
            if (index_i == -1){
                Cube_counts[nb_cubes_filled+i].cube.x1 = new_cube_count[i].cube.x1;
                Cube_counts[nb_cubes_filled+i].cube.x2 = new_cube_count[i].cube.x2;
                Cube_counts[nb_cubes_filled+i].cube.y1 = new_cube_count[i].cube.y1;
                Cube_counts[nb_cubes_filled+i].cube.y2 = new_cube_count[i].cube.y2;
                Cube_counts[nb_cubes_filled+i].cube.z1 = new_cube_count[i].cube.z1;
                Cube_counts[nb_cubes_filled+i].cube.z2 = new_cube_count[i].cube.z2;
                Cube_counts[nb_cubes_filled+i].count = new_cube_count[i].count;
                nb_new++;
            }
            else{
                Cube_counts[index_i].count += new_cube_count[i].count;
            }
           
        }
        nb_cubes_filled += nb_new;


        free(set_cube);
        free(new_cube_count);
    }

    
    // Calculate the answer
    long long int ans = 0;
    for (int i = 0; i < num; i++) {
        ans += volume_cube(cubes[i]) * Cube_counts[i].count;
    }


    // print the answer
    printf("\nThe answer is : %lld\n\n", ans); // Print the result
    


    // free the memory
    free(button);
    
    free(cubes);

    free(Cube_counts);

    // closing the file
    fclose(file);

    return EXIT_SUCCESS;
}

// on the test.txt file, the result is 39 as expected
// on the larger_test.txt file, the result is 590784 as expected
// need to supress the last empty line of the input file to get the right result
// I had a seg fault because I used strcpy rather than strdup for the memory allocation of the tokens