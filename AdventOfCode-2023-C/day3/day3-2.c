#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


#define MAX_LINES 140
#define MAX_LINE_LENGTH 142
#define lenappend 1

int n=MAX_LINES;
int m=MAX_LINE_LENGTH;

// fonction qui regarde si le caractère est un symbol
// dans le cas d'une roue *, elle remplie le tableau goods avec les deux nombres entourant le *
// retourne 0 si on est en dehors du tableau
// retourne 1 si on est sur un symbole différent de . et des chiffres (0 dans le cas contraire)
int is_symbol(int i, int j, int num, char tableau[MAX_LINES][MAX_LINE_LENGTH], int goods[MAX_LINES][MAX_LINE_LENGTH][2]) {
    if (!(0 <= i && i < n && 0 <= j && j < m)) {
        return 0;
    }

    if (tableau[i][j] == '*') {
       if (goods[i][j][0] !=0){
            goods[i][j][1]=num;
       }
       if (goods[i][j][0] ==0){
            goods[i][j][0]=num;
       }
    }
    return (tableau[i][j] != '.' && !(tableau[i][j] >= 48 && tableau[i][j] <= 57));
}

int main() {
    FILE *file = fopen("input3.txt", "r");

    int nombreDeLignes = 0;
    char tableau[MAX_LINES][MAX_LINE_LENGTH];

    int goods[MAX_LINES][MAX_LINE_LENGTH][2]; // emplacements des *, avec les deux nombres qui en sont entourés pour la gear en coordonnées i,j
    // initialisation de goods
    for (int i=0;i<n;++i){
        for (int j=0;j<n;++j){
            goods[i][j][0]=0;
            goods[i][j][1]=0;
        }
    }

    // Lecture du fichier ligne par ligne
    while (fgets(tableau[nombreDeLignes], MAX_LINE_LENGTH, file) != NULL) {
        // Supprimer le caractère de nouvelle ligne s'il est présent
        int longueur = strlen(tableau[nombreDeLignes]);
        if (longueur > 0 && tableau[nombreDeLignes][longueur - 1] == '\n') {
            tableau[nombreDeLignes][longueur - 1] = '\0';
        }

        nombreDeLignes++;

        // Arrête la lecture si le nombre maximal de lignes est atteint
        if (nombreDeLignes == MAX_LINES) {
            break;
        }
    }

    // Fermeture du fichier
    fclose(file);

    // Affichage des lignes du tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("Ligne %d : %s\n", i + 1, tableau[i]);
    }

    // initialisation de la réponse
    long int ans = 0;

    // on parcourt le tableau pour trouver les nombres
    for (int i = 0; i < n; i++) {
        int start = 0;
        int j = 0;

        while (j < m) {
            start = j;
            char num[20]; 
            num[0] = '\0';

            // on récupère le nombre sous forme de chaîne de caractères
            while (j < m && isdigit(tableau[i][j])) {
                strncat(num, &tableau[i][j], 1);
                j=j+1;
            }

            // si on est en bout de ligne, on va passer à la suivante, car j sera trop grand 
            if (num[0] == '\0') {
                j=j+1;
                continue;
            }

            // on convertit la chaîne de caractères en entier
            int numInt = atoi(num);

            // On regarde à gauche et à droite du nombre pour voir si on est sur un symbole 
            is_symbol(i, start-1, numInt, tableau, goods) || is_symbol(i, j, numInt,tableau, goods);

            // on regarde en haut et en bas du nombre pour voir si on est sur un symbole
            for (int k = start-1; k <= j; k++) {
                is_symbol(i-1, k, numInt,tableau, goods) || is_symbol(i+1, k, numInt,tableau, goods);
            }

        }
    }

    // on réalise la somme des produits des deux nombres entourant les *
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (tableau[i][j] == '*') {
                ans += goods[i][j][0] * goods[i][j][1];
            }
        }
    }

    printf("\n\nLe résultat est : %ld\n\n", ans);

    return 0;
}

// doit retourner 467835 pour le fichier test.txt en changeant n=MAX_LINES et m=MAX_LINE_LENGTH en n=10 et m=12