#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// affichage du tableau
void affichage(int* T, int n) {
  printf("pour le tableau, ");
  for (int i=0;i<n;++i) {
    if (i>0) {
      printf(",");
    }
    printf("%d",T[i]);
  }
  printf("\n");
}


// fonction pour séparer les chaïnes par des espaces et en ressortir un tableau de chiffres
int* splitBySpaces(const char *input, int k, int p) {
    printf("la partie sans espace :");
    int* T=malloc(sizeof(int)*p);
    
    char *token;
    char *str = strdup(input);  // Dupliquer la chaîne pour ne pas la modifier

    // La première utilisation de strtok avec la chaîne complète
    token = strtok(str, " ");

    int i=0;
    while (token != NULL) {
        T[i]=atoi(token);
        printf("%s,", token);

        // Appel suivant pour obtenir le prochain jeton
        token = strtok(NULL," ");

        // Si le jeton actuel n'est pas NULL et le nombre d'espaces entre les mots est égal à k,
        // mettre le nombre seul 
        if (token != NULL && strlen(token) != k) {
            T[i+1]=atoi(token);
            printf("%s,",token);
            token = strtok(NULL, " ");  // Avancer au prochain mot après l'espace
            ++i;
        }
        ++i;
    }

    printf("\n");
    free(str);  // Libérer la mémoire allouée par strdup
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

// fonction pour compter les points pour chaque ligne, à base de puissance de 2
// j'aurais pu optimiser en utilisant un tableau de puissance de 2 plutôt que de calculer à chaque fois avec la fonction pow
int point(int* T1, int p1, int* T2, int p2){
    int score=0;
    for (int i=0; i<p2;++i){
        if (estpresent(T2[i], T1, p1)){
            score=score+1;
        }
    }

    int points=0;
    if (score > 0){
        points=pow(2,score-1);
    }
    return(points);
}


int main() {
    FILE *fichier;
    // servait à compter les lignes et leur longueurs
    /*
    fichier = fopen("input4.txt", "r");
    char line[256];
    int nline=0;
    int lenline=0;
    while (fgets(line, sizeof(line), fichier) != NULL) {
        nline=nline+1;
        lenline=strlen(line);
    }
    
    fclose(fichier);
    */

    // servait à afficher le tableau 
    /*
    fichier = fopen("input4.txt", "r");
    char tableau[nline][lenline];
    int nombreDeLignes = 0;
    while (fgets(tableau[nombreDeLignes], lenline, fichier) != NULL) {
        int longueur = strlen(tableau[nombreDeLignes]);
        if (longueur > 0 && tableau[nombreDeLignes][longueur - 1] == '\n') {
            tableau[nombreDeLignes][longueur - 1] = '\0';
        }
        nombreDeLignes++;
        if (nombreDeLignes == lenline) {
            break;
        }
    }
    fclose(fichier);
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("%s\n", tableau[i]);
    }
    */

    printf("\n");
    fichier = fopen("input4.txt", "r");
    char ligne[256]; 
    int s=0;
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        size_t debut1=10;
        size_t fin1=39;
        size_t lendest1=fin1-debut1+1;
        char* destination1=(char*)malloc(lendest1+1);
        memcpy(destination1, ligne+debut1,lendest1);
        destination1[lendest1]='\0';
        printf("la partie avec espace : %s\n",destination1);
        int p1=10; // nombre chiffre avant le | 
        int* T1=splitBySpaces(destination1,2,p1);

        size_t debut2=42;
        size_t fin2=115;
        size_t lendest2=fin2-debut2+1;
        char* destination2=(char*)malloc(lendest2+1);
        memcpy(destination2, ligne+debut2,lendest2);
        destination2[lendest2]='\0';
        printf("la partie avec espace : %s\n",destination2);
        int p2=25; // nombre chiffre après le |
        int* T2=splitBySpaces(destination2,2,p2);

        s+=point(T1,p1,T2,p2);

        free(T1);
        free(destination1);
        free(T2);
        free(destination2);
        
    }
    fclose(fichier);
    printf("\n");
    printf("le résultat est : %d\n",s);

    return(0);
}

// fonction faite à la main en repérant les indices des espaces et du | 
// j'aurais pu le faire en utilisant strtok de manière plus élégante ou avec des expressions régulières
// c'est pour cela que ça ne marche pas pour les autres inputs
// cela aurait du donné 13 pour test.txt