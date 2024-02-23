#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

int calc_colonne(char** grid, int nline, int tailleline, int j){
    int sum=0;
    int i=0; 
    while (i<nline){
        while (i<nline && grid[i][j]=='#'){
            ++i;
        }

        // on est maintenant à un 'O' ou un '.'
        int nb=0; // nombre de 0 jusqu'à la prochaine pierre fixe '#'
        int p=i; // on commence à partir de cette ligne, donne notre point de départ pour ensuite faire les calculs
        while (i<nline && grid[i][j]!='#'){
            if (grid[i][j]=='O'){
                ++nb;
            }
            ++i;
        }

        for (int k=p;k<p+nb; k++){
            sum=sum+nline-k;
        }


    }
    return(sum);
}


int main(){

    FILE* file;
    file=fopen("input14.txt", "r");

    int nline=0;
    int tailleline=0; 
    char ligne[120];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        tailleline=strlen(ligne);
        ++nline;
    }

    rewind(file);

    char** grid=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    int i=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for (int j=0; j<tailleline; j++){
            grid[i][j]=ligne[j];
        }
        ++i;
    }

    // affichage de la grille 
    /*
    printf("\n");
    for (int i=0; i<nline; i++){
        printf("%s\n",grid[i]);
    }
    printf("\n");
    */

    int sum=0; 
    for (int j=0; j<tailleline; j++){
        sum=sum+calc_colonne(grid, nline, tailleline, j);
    }

    printf("\nle résultat est : %d\n\n", sum);


    for (int i=0; i<nline; i++){
        free(grid[i]);
    }
    free(grid);
    fclose(file);
    return(0);
}

// on trouve bien 136 pour le test.txt