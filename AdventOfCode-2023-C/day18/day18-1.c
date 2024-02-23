#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define LEN_VERT 1000
// récupérer l'input comme un bulldog 
// pour calculer l'aire, on va utiliser les théorèmes de Pick et de Shoelace, où l'on calcule des aires trapèzes qui vont jusqu'en bas et on trouve alors l'aire sans avoir à remplir l'input 

enum direction {
    UP,
    DOWN,
    RIGHT, 
    LEFT
};


// trouver la direction selon la lettre de l'input 
int trouver_direction(char dir){

    switch (dir){
        case 'U' :
            return(UP);
            break;

        case 'D' :
            return(DOWN);
            break;

        case 'L' :
            return(LEFT);
            break;

        case 'R' :
            return(RIGHT);
            break;
        default : 
            return(-1);
            break;
    }
}


int main (){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input18.txt", "r");
    char ligne[30];

    // Récupérer les dimensions de l'entrée 
    int nline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
    }
    //printf("\nil y a %d lignes à traiter\n\n", nline);

    char*** input=(char***)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        input[i]=(char**)malloc(sizeof(char)*2);
        for (int j=0; j<2; j++){
            input[i][j]=(char*)malloc(sizeof(char)*5);
        }
    }

    rewind(file);

    //récupérer les données de l'input 
    int k=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        char* str=strdup(ligne);
        char* token;
        token=strtok(str, " ");
        strcpy(input[k][0],token); // switch case pour associer à la direction 
        token=strtok(NULL, " ");
        strcpy(input[k][1],token);
        ++k;
        free(str);
    }
    fclose(file);
    // afficher le tableau de l'input 
    
    for (int i=0; i<nline; i++){
        printf("direction '%s' pour %s cases\n", input[i][0], input[i][1]);
    }
    printf("\n");
    

    // j'aurais pu utiliser des tableaux de strucutre quand même 
    // création du tableau des directions(UP fera aller une ligne au dessus, sur la même colonne d'où le (-1,0), et ainsi de suite)
    int** tabdirection=(int**)malloc(sizeof(int*)*4);
    for (int i=0; i<4; i++){
        tabdirection[i]=(int*)malloc(sizeof(int)*3);
    }
    // UP
    tabdirection[0][0]=UP;
    tabdirection[0][1]=-1;
    tabdirection[0][2]=0;
    // DOWN
    tabdirection[1][0]=DOWN;
    tabdirection[1][1]=1;
    tabdirection[1][2]=0;
    // RIGHT
    tabdirection[2][0]=RIGHT;
    tabdirection[2][1]=0;
    tabdirection[2][2]=1;
    // LEFT
    tabdirection[3][0]=LEFT;
    tabdirection[3][1]=0;
    tabdirection[3][2]=-1;

    // donne les coordonnées des points où y'a des changements de directions 
    int len_points=0;
    int** points=(int**)malloc(sizeof(int*)*LEN_VERT);
    for (int i=0; i<LEN_VERT; i++){
        points[i]=(int*)malloc(sizeof(int)*2);
    }
    // on met le premier point dedans; (ordonnée, abscisse)
    int x=0; 
    int y=0; 
    points[len_points][0]=y;
    points[len_points][1]=x;
    len_points=len_points+1;


    int nb_point_de_liaison=0;

    for (int i=0; i<nline; i++){
        char var=input[i][0][0];
        int direction=trouver_direction(var);
        int distance=atoi(input[i][1]);
        //printf("distance en int : %d\n",distance);

        int dir[2];
        dir[0]=tabdirection[direction][1];
        dir[1]=tabdirection[direction][2];
        // trouver la distance et la direction 

        // prendre les coordonnées du point précédent 
        //printf("on est à la ligne : %d\n", len_points);
        y=points[len_points-1][0];
        //printf("%d,",y);
        x=points[len_points-1][1];
        //printf("%d\n",x);
        nb_point_de_liaison+=distance;
        //printf("%d\n", nb_point_de_liaison);

        // append le nouveau point
        points[len_points][0]=y+dir[0]*distance;
        points[len_points][1]=x+dir[1]*distance;
        len_points=len_points+1;
    }
    // affichage des points où on change de direction 
    for (int i=0; i<len_points; i++){
        printf("(%d,%d)\n", points[i][0],  points[i][1]);
    }

    //printf("nombre de points de changements de direction : %d\n",len_points); 
    //printf("nombre de points sans changer de direction : %d\n",nb_point_de_liaison);

    // Shoelace formula 
    int aire=0;
    int xi=0;
    int p=0;
    int y_im1=0;
    int y_ip1=0;
    for (int i=0; i<len_points; i++){
        p=(i+1)%(len_points); 
        //printf("%d\n",p);
        xi=points[i][0];
        //printf("%d\n",xi);
        if (i-1<0){
            y_im1=points[len_points-1][1];
        }
        else {
            y_im1=points[i-1][1];
        }
        //printf("point en ym : %d\n",y_im1);
        y_ip1=points[p][1];
        //printf("point en yp : %d\n",y_ip1);
        aire+=xi*(y_ip1-y_im1);

    }
    // théorème de Pick
    aire=abs(aire)/2;
    aire=aire-(nb_point_de_liaison/2)+nb_point_de_liaison+1;
    

    printf("\nle résultat est : %d\n\n", aire);


    for (int i=0; i<nline; i++){
        for (int j=0; j<2; j++){
            free(input[i][j]);
        }
        free(input[i]);
    }
    free(input);

    for (int i=0; i<LEN_VERT; i++){
        free(points[i]);
    }
    free(points);

    for (int i=0; i<4; i++){
        free(tabdirection[i]);
    }
    free(tabdirection);
    return(0);
}

// on trouve bien 62 pour test.txt