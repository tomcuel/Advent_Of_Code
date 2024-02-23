#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

int Hash(char* s){
    int current_hash=0;
    int i=0;
    while (s[i] != '\0'){
        current_hash=((current_hash + s[i])*17) % 256;
        ++i;
    }
    return(current_hash);
}


int main (){

    FILE* file;
    file=fopen("input15.txt", "r");

    char ligne[200];
    int sum=0;
    int par=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        
        char* token=strtok(ligne, ",");
        printf("token : %s\n",token);
        par=Hash(token);
        sum=sum+par;
        printf("par : %d\n", par);
        printf("sum : %d\n", sum);
        printf("\n");
        while (strcmp(token,"\n") != 0 && token != NULL){
            token=strtok(NULL, ",");
            printf("token : %s\n",token);
            par=Hash(token);
            printf("par : %d\n", par);
            sum=sum+par;
            printf("sum : %d\n", sum);
            printf("\n");
        }
        sum=sum-par;
    }

	printf("\nLe résultat est : %d\n\n", sum);
    
    fclose(file);
	return 0;
}

// donne bien 1320 pour le fichier test.txt