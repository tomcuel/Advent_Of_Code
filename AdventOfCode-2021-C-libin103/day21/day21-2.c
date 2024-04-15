#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// in this part, the tree of possibilities is too big 

// we can create an array that give us the number_of ways to have a number between 1 and 9 given, three dice to are either 1 or 2 or 3
int number_of_ways[7][2]={
    {3, 1},
    {4, 3},
    {5, 6},
    {6, 7},
    {7, 6},
    {8, 3},
    {9, 1}
};






int main (){

    FILE* file=fopen("test.txt", "r");
    char line[100];

    fgets(line, sizeof(line),file);
    int len=strlen(line);
    int start_pos_player_1=line[len-2]-'0';
    fgets(line, sizeof(line),file);
    int start_pos_player_2=line[len-2]-'0';

    // printf("start_pos_player_1: %d\n", start_pos_player_1);
    // printf("start_pos_player_2: %d\n", start_pos_player_2);

    for (int i=0; i<7; i++){
        printf("amount : %d and freq[amoount]: %d\n", number_of_ways[i][0], number_of_ways[i][1]);
    }

    
    // free the memory


    // close the file 
    fclose(file);

    return EXIT_SUCCESS;
}

// didn't manage to make it works
// I didn't find a way that could make it optimized without brute forcing it (it doens't work for the moment, it's too slow)