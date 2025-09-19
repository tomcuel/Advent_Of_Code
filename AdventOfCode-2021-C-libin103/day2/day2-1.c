#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


int main(){

    FILE* file;
    file = fopen("input2.txt", "r");
    char line[50];

    int horizontal=0;
    int depth=0;
   
    while (fgets(line, sizeof(line), file) != NULL) {
        char* direction=strtok(line, " ");
        char* number_char=strtok(NULL, " ");
        int number=atoi(number_char); 

        // looking for the direction and then making the right operation
        if (strcmp(direction, "forward")==0){
            horizontal+=number;
            // printf("horizontal: %d\n", horizontal);
            continue;
        }
        if (strcmp(direction, "down")==0){
            depth+=number;
            // printf("depth: %d\n", depth);
            continue;
        }
        if (strcmp(direction, "up")==0){
            depth-=number;
            // printf("depth: %d\n", depth);
            continue;
        }
    }


    // formula to calculate the answer
    int answer=horizontal*depth;
    printf("\nThe answer is %d\n\n", answer);
   

    fclose(file);
    return 0;
}


// I found 150 as expected with the test.txt file.
// don't need the library for this one 