#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// fonction pour séparer les chaïnes par des espaces et en ressortir un tableau de chiffres
int* splitBySpaces(const char *input, int k, int p) {
    int* T=malloc(sizeof(int)*p);
    char *token;
    char *str = strdup(input);
    token = strtok(str, " ");
    int i=0;
    while (token != NULL) {
        T[i]=atoi(token);
        token = strtok(NULL," ");
        if (token != NULL && strlen(token) != k) {
            T[i+1]=atoi(token);
            token = strtok(NULL, " "); 
            ++i;
        }
        ++i;
    }
    free(str);  
    return(T);
}

// fonction pour vérifier si un élément est présent dans un tableau
int estpresent(int t, int* T, int n){
    for (int i=0; i<n; ++i){
        if (t==T[i]){
            return(1);
        }
    }
    return(0);
}


// chaque carte gagnante devient copié pour la suite 
int main() {
    int nline=199;
    int nb=100;
   
    // création d'un tableau de copies
    // ligne pour la carte gagnante, colonne pour les cartes copiées 
    int copie[nline][nb];
    for (int i=0;i<nline;++i){
        for (int j=0;j<nb;++j){
            copie[i][j]=0;
        }
    }

    FILE *fichier;
    fichier = fopen("input4.txt", "r");
    char ligne[256];
    int nbligne=0;
    // extraction des données comme dans le premier exercice
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        size_t debut1=10;
        size_t fin1=39;
        size_t lendest1=fin1-debut1+1;
        char* destination1=(char*)malloc(lendest1+1);
        memcpy(destination1, ligne+debut1,lendest1);
        destination1[lendest1]='\0';
        int p1=10; // nombre chiffre avant le | 
        int* T1=splitBySpaces(destination1,2,p1);

        size_t debut2=42;
        size_t fin2=115;
        size_t lendest2=fin2-debut2+1;
        char* destination2=(char*)malloc(lendest2+1);
        memcpy(destination2, ligne+debut2,lendest2);
        destination2[lendest2]='\0';
        int p2=25; // nombre chiffre après le |
        int* T2=splitBySpaces(destination2,2,p2);

        // nouvelle méthode pour compter les points
        int score=0;
        for (int i=0; i<p2;++i){
            if (estpresent(T2[i], T1, p1)){
                score=score+1;
            }
        }

        // copie de la carte gagnante dans un tableau
        for (int j=nbligne+1;j<nbligne+score+1;++j){
            copie[nbligne][j-nbligne-1]=j;
        }
    

        free(T1);
        free(destination1);
        free(T2);
        free(destination2);
        ++nbligne;
    }

    fclose(fichier);

    // calcul du score de chaque carte
    int score[nline];
    for (int i=0; i<nline;++i){
        score[i]=1;
    }

    // on commence par la fin pour que les cartes gagnantes soient bien comptées
    // on ajoute le score de la carte gagnante à partir du tableau des copies 
    for (int i = nline - 1; i >= 0; --i) {
        for (int j = 0; j < nline && copie[i][j] != 0; ++j) {
            int copyIndex = copie[i][j];
            score[i] += score[copyIndex];
        }
    }


    int s =0; 
    for (int i=0; i<nline;++i){
        s=s+score[i];
    }
    printf("\nle résultat est : %d\n\n",s);
    return(0);
}

// résultat : 5921508