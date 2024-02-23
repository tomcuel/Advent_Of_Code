#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// renvoi 1 si on a un axe de symétrie horizontal, 0 sinon 
int is_horiz(char** grid, int nblignes, int nbcolonnes, int i){
    for (int j=0; j< nbcolonnes; j++){
        for (int k1=0; k1<nblignes; k1++){
            int k2=2*i+1-k1;
            if ( (k2 < 0) || (k2 >= nblignes) ){
                continue;
            }
            if (grid[k1][j] != grid[k2][j]){
                return(0);
            }

        }
    }
    return(1);
}


// renvoi 1 si on a un axe de symétrie vertical, 0 sinon 
int is_verti(char** grid, int nblignes, int nbcolonnes, int j){
    for (int i=0; i< nblignes; i++){
        for (int k1=0; k1<nbcolonnes; k1++){
            int k2=2*j+1-k1;
            if ( (k2 < 0) || (k2 >= nbcolonnes) ){
                continue;
            }
            if (grid[i][k1] != grid[i][k2]){
                return(0);
            }

        }
    }
    return(1);
}


// attention, il faut commencer à j=1 dans grid et finir à nb lignes non compris 
// nb lignes sera égal à atoi(pattern[i][0])
// fonction qui renvoi le nombre de symétries dans une structure en utilsant la méthode de calcul de l'énoncé
int summary(char** grid, int nblignes, int nbcolonnes){
    int summary=0;

    int horiz=-1;
    for (int i=0; i<nblignes-1; i++){
        if (is_horiz(grid, nblignes, nbcolonnes, i)==1){
            horiz=i;
            break;
        }
    }

    int verti=-1;
    for (int j=0; j<nbcolonnes-1; j++){
        if (is_verti(grid, nblignes, nbcolonnes, j)==1){
            verti=j;
            break;
        }
    }

    if ((verti ==-1) || (horiz ==-1)){
        summary=verti+1+100*(horiz+1);
    }
    return(summary);
}



int main(){

    FILE* file;
    file=fopen("input13.txt", "r");

    int nline=0;
    int nbline_par_struct=50;
    int tailleline=50;
    char ligne[tailleline];

    int nbstruct=1;

    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
        if (strcmp(ligne, "\n")==0){
            ++nbstruct;
        }

    }

    printf("\nil y a %d lignes\n", nline);
    printf("\nil y a %d stuctures à analyser\n", nbstruct);

    rewind(file);

    // on regroupe par strucutre et par ligne à l'intérieur de cette structure
    char ***pattern = (char ***)malloc(nbstruct * sizeof(char **));
    for (int i = 0; i < nbstruct; i++) {
        pattern[i] = (char **)malloc((nbline_par_struct + 1) * sizeof(char*));  // +1 pour le caractère nul '\0'
        for (int j=0; j<tailleline; j++){
            pattern[i][j]=(char*)malloc(sizeof(char)*tailleline);
        }
    }

    int p=0; 
    //printf("\nstructure n°%d :\n",p+1);
    int k=1;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        
        if (strcmp(ligne, "\n")==0){

            // Utilisation de sprintf pour convertir l'entier en chaîne
            sprintf(pattern[p][0], "%d", k); // pattern[p][0] donne le nombre de lignes +1 dans la structure
            // cela décale de 1 l'indice des autres 
            //printf("nombre de lignes dans la structure : %d\n",k-1);
            ++p;
            k=1;
            //printf("\nstructure n°%d :\n",p+1);
        }

        if (strcmp(ligne, "\n")){
            strcpy(pattern[p][k],ligne);
            //printf("%s",pattern[p][k]);
            ++k;
        }
    }
    sprintf(pattern[p][0], "%d", k);
    //printf("\nnombre de lignes dans la structure : %d\n",k-1);

    // pour afficher les structures 
    for (int i=0; i<nbstruct; i++){
        printf("\n");
        printf("\nstructure n°%d :\n",i+1);
        printf("%s\n", pattern[p][0]);
        printf("nombre de lignes : %d\n",atoi(pattern[i][0])-1);
        printf("nombre de colonnes : %d\n",strlen(pattern[i][1])-1);
        for (int j=1; j<atoi(pattern[i][0]); j++){
            printf("%s", pattern[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
    int sum=0;
    int q=0;
    for (int i=0; i<nbstruct; i++){

        int nline_struct=atoi(pattern[i][0])-1;
        int ncol_struct=strlen(pattern[i][1])-1;

        char** grid=(char**)malloc(sizeof(char*)*nline_struct);
        for (int j=0; j<nline_struct; j++){
            grid[j]=(char*)malloc(sizeof(char)*tailleline);
        }
        for (int j=0; j<nline_struct; j++){
            for (int t=0; t<ncol_struct; t++){
                grid[j][t]=pattern[i][j+1][t];
            }
        }

        
        q=summary(grid, nline_struct, ncol_struct);
        sum=sum+q;


        free(grid);
    }

    printf("\nle résultat est : %d\n", sum);


    fclose(file);
    for (int i = 0; i < nbstruct; i++) {
        for (int j=0; j<tailleline; j++){
            free(pattern[i][j]);
        }
    }
    for (int i = 0; i < nbstruct; i++) {
            free(pattern[i]);
    }
    free(pattern);
    return(0);
}

// on trouve bien 405 dans le fichier test.txt