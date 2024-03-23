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
    int sum =0; 
    // getting the first product of 3 numbers that will be used to compare with the next products
    int numbers[3];
    for (int i = 0; i < 3; i++){
        fgets(line, sizeof(line), file);
        numbers[i] = atoi(line);
    }
    int product;
    int previous;
    int last;

    while (fgets(line, sizeof(line), file) != NULL) {
        // geting the new number from the input
        last = atoi(line);
        // nexw product
        product = numbers[1]+numbers[2]+last;
        // previous product
        previous = numbers[0]+numbers[1]+numbers[2];
        // looking if the product is increasing
        if (product > previous){
            ++sum;            
        }

        // changing the numbers to get the next previous product
        numbers[0] = numbers[1];
        numbers[1] = numbers[2];
        numbers[2] = last;
    }


    printf("\n\nThe number of increasing numbers is %d\n\n", sum);


    fclose(file);
    return 0;
}


// I found 5 as expected with the test.txt file.
// don't need the library for this one 