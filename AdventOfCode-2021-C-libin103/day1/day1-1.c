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
    file = fopen("input1.txt", "r");
    char line[6];
    // starting at -1 because the first number is not considered and my algorithm will add the first number
    int sum =-1; 
    // keeping the previous number to make the comparison
    int previous = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        int number = atoi(line);
        // printf("number : %d, ", number);
        // looking if the number is increasing
        if (number > previous){
            ++sum;
            // printf("increasing, ");
            // printf("sum = %d\n", sum);
        }
        // changing the previous number
        previous = number;
    }


    printf("\nThe number of increasing numbers is %d\n\n", sum);


    fclose(file);
    return 0;
}


// I found 7 as expected with the test.txt file.
// don't need the library for this one. 