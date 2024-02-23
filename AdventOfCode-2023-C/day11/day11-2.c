#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

int compter_lettre(char* ch, char x){
    int n=0;
    int i=0;
    while (ch[n] != 0){
        if (ch[n]==x){
            i=i+1;
        }
        n=n+1;
    }
    return(i);
}

int min(int a, int b){
    if (a < b){
        return (a);
    }
    else {
        return (b);
    }
}
int max(int a, int b){
    if (a > b){
        return (a);
    }
    else {
        return (b);
    }
}

// renvoie 1 si la ligne est vide, 0 sinon 
int is_empty_ligne(int **galaxies, int nrows, int i){
    int test=1;
    for (int j=0; j<nrows; j++){
        if (galaxies[i][j]==1){
            test=0;
        }
    }
    return(test);
}
// renvoie 1 si la colonne est vide, 0 sinon 
int is_empty_colonne(int **galaxies, int nline, int j){
    int test=1;
    for (int i=0; i<nline; i++){
        if (galaxies[i][j]==1){
            test=0;
        }
    }
    return(test);
}

u_int32_t distance_entre_deux_galaxies(int i1, int j1, int i2, int j2, int* emptyligne, int* emptycolonne){
    u_int32_t dist=0;

    // la distance sera la même, on parcout alors l'input de gauche à droite, de haut en bas
    int x1 = min(i1,i2);
    int y1 = min(j1,j2);

    int x2 = max(i1,i2);
    int y2 = max(j1,j2);

    // on regarde si on passe par une ligne vide 
    for (int i=x1; i<x2; i++){
        dist=dist+1;
        if (emptyligne[i]==1){
            dist=dist+1000000-1;
        }
    }
    // on regarde si on passe par une colonne vide 
    for (int j=y1; j<y2; j++){
        dist=dist+1;
        if (emptycolonne[j]==1){
            dist=dist+1000000-1;
        }
    }

    return(dist);
}


int main(){

    FILE* file;
    file = fopen("test.txt", "r");
    char ligne[256]; 
    int nline=0;
    int nrows=0;
    int ncoords=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        nrows=strlen(ligne); //on a compter le sautage de ligne à la fin mais on s'en fou, si ça marche pas on décale de -1
        nline=nline+1; 
        int j=compter_lettre(ligne, '#');
        ncoords=ncoords+j;
    }

    printf("\n");
    printf("nombre de lignes : %d\n",nline);
    printf("nombre de colonnes : %d\n",nrows);
    printf("nombre de galaxies : %d\n", ncoords);
    printf("\n");

    // Allocation de mémoire pour le tableau de l'univers
    int **galaxies = (int **)malloc((nline) * sizeof(int *));
    // Allocation de mémoire pour chaque ligne
    for (int i = 0; i < nline; i++) {
        galaxies[i] = (int *)malloc((nrows) * sizeof(int));
    }

    // Allocation de mémoire pour stocker les coordonnées des galaxies
    int **coords = (int **)malloc((ncoords) * sizeof(int *));
    // Allocation de mémoire pour chaque ligne
    for (int i = 0; i < nline; i++) {
        coords[i] = (int *)malloc(2 * sizeof(int));
    }

    rewind(file);
    int i=0;
    int p=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for (int j=0; j < nrows; j++){
            if (ligne[j]=='#'){
                galaxies[i][j]=1;
                coords[p][0]=i; 
                coords[p][1]=j; 
                p=p+1;
            }
            if (ligne[j]=='.'){
                galaxies[i][j]=0;
            }
        }
        i=i+1;
    }

     // Tableau pour dire si la ligne correspondant au némuro du tableau est vide 
    int* emptyligne=(int *)malloc(sizeof(int)*nrows);
    for (int i=0; i < nline; i++){
        emptyligne[i]=is_empty_ligne(galaxies, nrows, i);
    }

    // Tableau pour dire si la colonne correspondant au némuro du tableau est vide 
    int* emptycolonne=(int *)malloc(sizeof(int)*nline);
    for (int j=0; j < nrows; j++){
        emptycolonne[j]=is_empty_colonne(galaxies, nline, j);
    }

    u_int32_t sum=0;
    u_int32_t d=0;
    for (int i=0; i < ncoords; i++){
        for (int j=i+1; j<ncoords; j++){
            d = distance_entre_deux_galaxies(coords[i][0], coords[i][1], coords[j][0], coords[j][1], emptyligne, emptycolonne);
            sum=sum+d;
        }
    }


    for (int i = 0; i < ncoords; i++) {
        printf("(%d,%d)\n", coords[i][0], coords[i][1]);
        free(coords[i]);
    }
    free(coords);

    for (int i = 0; i < nline; i++) {
        for (int j=0; j<nrows; j++){
            printf("%d,",galaxies[i][j]);
        }
        printf("\n");
        free(galaxies[i]);
    }
    printf("\n");
    free(galaxies);
    for (int j=0; j<nline; j++){
        printf("%d,",emptyligne[j]);
    }
    printf("\n");
    free(emptyligne);
     for (int j=0; j<nrows; j++){
        printf("%d,",emptycolonne[j]);
    }
    printf("\n");
    free(emptycolonne);

    printf("\nLe résultat est : %d\n", sum);


    fclose(file);
    return(0);
}

// fait bien 8410 si on réduit les 100000 à 1000 pour test.txt
// marche en théorie, mais problème de longueur et de segmentation fault
// fait en python pour le faire marcher 
