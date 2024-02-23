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
int* summary(char** grid, int nblignes, int nbcolonnes, int* T){ // T de longueur 2
    int* summary=malloc(sizeof(int)*2);

    int horiz=-1;
    for (int i=0; i<nblignes-1; i++){
        if (is_horiz(grid, nblignes, nbcolonnes, i)==1 && i != T[0]){
            horiz=i;
            break;
        }
    }

    int verti=-1;
    for (int j=0; j<nbcolonnes-1; j++){
        if (is_verti(grid, nblignes, nbcolonnes, j)==1 && j != T[1]){
            verti=j;
            break;
        }
    }

    summary[0]=horiz;
    summary[1]=verti;
    return (summary);
}


// on va tester les différentes possibilités jusqu'à que il y ait qqch qui change, sachant que y'a qu'un caractère qui peut faire changer qqch 
int summary_2(char** grid, int nblignes, int nbcolonnes, int tailleline){
    int contrib=0;
    int* T=malloc(sizeof(int)*2);
    T[0]=-1;
    T[1]=-1;
    int* sum_old=summary(grid, nblignes, nbcolonnes, T);
    free(T);
    for (int i=0;i<nblignes; i++){
        for (int j=0;j<nbcolonnes; j++){

            char** grid2=(char**)malloc(sizeof(char*)*nblignes);
            for (int q=0; q<nblignes; q++){
                grid2[q]=(char*)malloc(sizeof(char)*tailleline);
                for (int h=0; h<nbcolonnes; h++){
                    grid2[q][h]=grid[q][h];
                }
            }

            if (grid[i][j]=='.'){
                grid2[i][j]='#';
            }
            if (grid[i][j]=='#'){
                grid2[i][j]='.';
            }

            int* sum_new=summary(grid2, nblignes,nbcolonnes, sum_old);
            free(grid2);
            // on s'occupe du cas seulement où le résultat a changé 
            if ( ((sum_new[0] != sum_old[0]) || (sum_new[1] != sum_old[1]))  && ((sum_new[0] != -1) || (sum_new[1] != -1)) ){
                if (sum_new[0] != -1){
                    contrib=(sum_new[0]+1)*100;
                }
                else {
                    if (sum_new[1] != 1){
                        contrib =sum_new[1] + 1;
                    }
                }
            }
            free(sum_new);
        }
    }
    free(sum_old);
    return(contrib);
}



int main(){

    FILE* file;
    file=fopen("test.txt", "r");

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

        
        q=summary_2(grid, nline_struct, ncol_struct, tailleline);
        sum=sum+q;
        printf("%d\n", q);


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

// on trouve bien 400 avec test.txt
// il m'en manque 10 avec input13.txt