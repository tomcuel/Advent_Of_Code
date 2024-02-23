#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// Fonction pour trouver le nombre de chiffres dans une ligne
int nbchiffres(char* ligne){
    int k=0;
    int i=0;
    while (ligne[i] != '\0'){
        if (ligne[i]== ' '){
            ++k;
        }
        ++i;
    }
    return(k+1);
}

// Fonction pour séparer une chaîne de caractères en un tableau d'entiers
int* splitBySpaces(const char *input, int p) {
    int* T=(int *)malloc(sizeof(int)*p);
    char *token;
    char *str = strdup(input);
    token = strtok(str, " ");
    int i=0;
    while (token != NULL) {
        T[i]=atoi(token);
        token = strtok(NULL," ");
        i++;
    }
    free(str);  
    return(T);
}

// Fonction pour trouver la différence entre deux tableaux
int* difference_tableau(int* T, int* p){
    *p=*p-1; // on diminue la taille du tableau de 1
    int* diff=(int*)malloc(sizeof(int)*(*p));
    for (int i=0;i<*p;i++){
        diff[i]=T[i+1]-T[i];
    }
    return(diff);
}

// Fonction pour trouver le chiffre suivant selon le tableau de différences
int chiffresuivant(int* T,int p){
    int res=0;
    int k=p;
    int i=0;

    // Allocation de mémoire pour le tableau de pointeurs vers les lignes
    // construction du tableau de différences
    int **Tab = (int **)malloc((p) * sizeof(int *));
    // Allocation de mémoire pour chaque ligne
    for (int i = 0; i < p; i++) {
        Tab[i] = (int *)malloc((p) * sizeof(int));
    }
    // On met que des zéros dans le tableau
    for (int j=0; j<p;j++){
        for (int q=0; q<p;q++){
            Tab[j][q]=0;
        } 
    }
    // On initialise avec notre ligne de valeurs
    for (int j=0;j<p;j++){
        Tab[0][j]=T[j];
    }

    // on diminue k jusqu'à 0 pour remplir le tableau de différences
    while (k!=0){
        int* diff=difference_tableau(Tab[i],&k); //k=k-1, passage par adresse
        for (int j=0;j<k;j++){
            Tab[i+1][j]=diff[j];
        }
        free(diff);
        ++i;
    }

    // On remplit le tableau de la somme des diagonales
    for (int q=1; q<p-1;q++){
        Tab[p-1-q][q+1]=Tab[p-1-q][q]+Tab[p-q][q];
    }

    res=Tab[0][p-1]+Tab[1][p-1];

    // Libération de la mémoire
    for (int q = 0; q < p; q++) {
        free(Tab[q]);
    }
    free(Tab);

    return(res);
}


int main (){

    FILE* file;
    file = fopen("input9.txt", "r");
    char ligne[256]; 
    int s=0;
    int nline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        int p=nbchiffres(ligne);
        int* T=splitBySpaces(ligne,p);
        s=s+chiffresuivant(T,p);
        free(T);
        nline=nline+1; 
    }

    printf("Le résultat est : %d\n",s);
    return(0);
}

// on trouve bien 114 pour le test.txt