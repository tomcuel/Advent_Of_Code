#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// j'aurais pu faire beaucoup plus rapide dans l'extraction de données vu le fichier mais j'ai préféré faire une fonction qui marche pour tous les cas

// Fonction pour afficher un tableau
void affichage(int* T, int n) {
    for (int i=0;i<n;i++) {
        if (i>0) {
            printf(",");
        }
        printf("%u",T[i]);
    }
    printf("\n");
}

// Fonction pour calculer le nombre de possibilités
int ways(int t, int d){
    int nbpossible=0;
    for (int i=0; i<=t;i++){
        if ((t-i)*i > d){
            nbpossible+=1;
        }
    }
    return(nbpossible);
}

// Fonction principale
int main(){
    FILE* fichier;
    int n=5;
    int Tab[2][n];
    int nbligne=0;
    fichier = fopen("input6.txt", "r");
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int T[n];
        char* token; 
        char *str = strdup(ligne); 
        token = strtok(str, " ");
        int i=0;
        while (token != NULL) {
            T[i]=atoi(token);
            printf("%s ", token);

            // Appel suivant pour obtenir le prochain jeton
            token = strtok(NULL," ");

            // Si le jeton actuel n'est pas NULL et le nombre d'espaces entre les mots est égal à 1,
            // mettre le nombre seul 
            if (token != NULL && strlen(token) != 1) {
                T[i+1]=atoi(token);
                printf("%s ",token);
                token = strtok(NULL, " ");  // Avancer au prochain mot après l'espace
                ++i;
            }
            ++i;
           
        }
        for (int k=0;k<n;k++){
            Tab[nbligne][k]=T[k];
        }
        printf("\n");
        ++nbligne;
        free(str);

    }
    affichage(Tab[0],n); // T[0][Ø] n'est pas intéressant dans ce cas 
    affichage(Tab[1],n); // T[1][Ø] n'est pas intéressant dans ce cas 
    printf("%d\n",nbligne);

    fclose(fichier);
    int p=1;

    for (int i=1;i<n;i++){ //on va parcourir les temps 
        int nbpossibility=ways(Tab[0][i],Tab[1][i]);
        p=p*nbpossibility;
    }

    printf("\nle résultat est %d\n",p);
    return(0);
}

// on trouve bien 288 comme résultat pour le test.txt en changeant n=5 en n=4