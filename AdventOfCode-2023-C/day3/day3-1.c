#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


int is_symbol(char c) {
    return (c == '*' || c == '=' || c == '-' || c == '+'  || c == '#'  || c == '%'  || c == '/'  || c == '@'  || c == '&'  || c == '$' );
}

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 140
#define MAX_LINE_LENGTH 142

int n=MAX_LINES;
int m=MAX_LINE_LENGTH;

int main() {
    FILE *file = fopen("input3.txt", "r");

    char tableau[MAX_LINES][MAX_LINE_LENGTH];
    int nombreDeLignes = 0;

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

    int ans = 0;
    for (int i = 0; i < n; i++) {
        int start = 0;
        int j = 0;

        while (j < m) {
            start = j;
            char num[20]; 
            num[0] = '\0';
            // on regarde jusqu'où va le nombre
            while (j < m && isdigit(tableau[i][j])) {
                strncat(num, &tableau[i][j], 1);
                j=j+1;
            }

            // cas de fin de ligne
            if (num[0] == '\0') {
                j=j+1;
                continue;
            }

            // garder en mémoire le nombre
            int numInt = atoi(num);

            // on regarde à gauche et à droite
            if (is_symbol(tableau[i][start - 1]) || is_symbol(tableau[i][j])) {
                ans += numInt;
                continue;
            }

            // on regarde si on a un symbole au dessus ou en dessous
            for (int k = start - 1; k <= j; k++) {
                if (is_symbol(tableau[i-1][k]) || is_symbol(tableau[i+1][k])){
                    ans += numInt;
                    break;
                }
            }
        }
    }

    printf("\n\nLe résultat est %d\n\n", ans);

    return 0;
}

// doit retourner 4361 pour le fichier test.txt en changeant n=MAX_LINES et m=MAX_LINE_LENGTH en n=10 et m=12