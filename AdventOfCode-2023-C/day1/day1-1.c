#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


void affichage(char* T, int n) {
    for (int i=0;i<n;++i) {
        if (i>0) {
            printf(",");
        }
        printf("%c",T[i]);
    }
    printf("\n");
}

//ne sélectionne que le premier et le dernier chiffre de la ligne et en fait un nombre à 2 chiffres 
int totalligne(char* ligne){
    int s=0;
    int n=strlen(ligne);

    int first =0;
    int last =0; 
    for (int i=0;i<n;++i){
        if (ligne[i] > 48 && ligne[i] <= 57){
            if (first == 0){
                first=ligne[i]-48;
            }
            last=ligne[i]-48;
        }     
    }
    
    affichage(ligne,n);
    printf("%d first, %d last\n", first, last);
    s=s+10*first+last;
    return(s);
}




int main() {
    // Déclarer une variable pour stocker le fichier
    FILE *fichier;

    // Ouvrir le fichier en mode lecture
    fichier = fopen("input1.txt", "r");

    // Vérifier si le fichier a été ouvert avec succès
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1; // Quitter le programme avec une erreur
    }

    // Déclarer un tableau pour stocker la ligne lue
    char ligne[256]; // Vous pouvez ajuster la taille en fonction de vos besoins
    int s=0;
    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Traitement de la ligne lue
        s=s+totalligne(ligne);
    }
    printf("\nLe résultat est : %d\n\n",s);
    // Fermer le fichier
    fclose(fichier);

    return 0; // Terminer le programme avec succès
}

// doit donner 142 avec text1.txt