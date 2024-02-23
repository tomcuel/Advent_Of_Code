#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// Fonction pour afficher un tableau de caractères
void affichage1(char* T, int n) {
    for (int i=0;i<n;++i) {
        if (i>0) {
            printf(",");
        }
        printf("%c",T[i]);
    }
    printf("\n");
}

// Fonction pour afficher un tableau d'entiers
void affichage2(int* T, int n) {
    for (int i=0;i<n;++i) {
        if (i>0) {
            printf(",");
        }
        printf("%d",T[i]);
    }
    printf("\n");
}

// Structure pour stocker les nombres de couleurs
struct game {
    int rouge;
    int bleu;
    int vert;
};


// Fonction pour trouver les occurrences d'une sous-chaîne dans une chaîne principale
int* trouver_occurences(char *chainePrincipale,char *chaineRecherche, int *nombreOccurences) {
    int longueurPrincipale = strlen(chainePrincipale);
    int longueurRecherche = strlen(chaineRecherche);

    // Tableau pour stocker les positions des occurrences
    int *positions = NULL;
    *nombreOccurences = 0;

    // Parcourir la chaîne principale
    for (int i = 0; i <= longueurPrincipale - longueurRecherche; ++i) {
        // Vérifier si la sous-chaîne correspond
        if (strncmp(chainePrincipale + i, chaineRecherche, longueurRecherche) == 0) {
            // Ajouter la position de l'occurrence au tableau
            (*nombreOccurences)++;
            positions = realloc(positions, (*nombreOccurences) * sizeof(int));
            if (positions == NULL) {
                fprintf(stderr, "Erreur d'allocation mémoire.\n");
                exit(EXIT_FAILURE);
            }
            positions[(*nombreOccurences) - 1] = i;
        }
    }
    return (positions);
}


// Fonction pour trouver la puissance d'un jeu 
int nmpow(char* ligne ){

    // on trouve les positions de red, blue, green dans la ligne 
    int nombreOccurencesrouge;
    int *positionsrouge = trouver_occurences(ligne, "red", &nombreOccurencesrouge);
    
    int nombreOccurencesbleu;
    int *positionsbleu = trouver_occurences(ligne, "blue", &nombreOccurencesbleu);

    int nombreOccurencesvert;
    int *positionsvert = trouver_occurences(ligne, "green", &nombreOccurencesvert);

    struct game lignenb={0,0,0};
    int p=0;

    // On regarde le chiffre deux caractères avant :  
    // pour les rouges 
    for (int i=0; i<nombreOccurencesrouge;++i){
        if (ligne[positionsrouge[i]-2] >= 48 && ligne[positionsrouge[i]-2] <= 57 && ligne[positionsrouge[i]-3] == ' '){
            p=ligne[positionsrouge[i]-2]-48;
            if (p >= lignenb.rouge){
                lignenb.rouge =p;
            }
        }

        if (ligne[positionsrouge[i]-2] >= 48 && ligne[positionsrouge[i]-2] <= 57 && ligne[positionsrouge[i]-3] >= 48 && ligne[positionsrouge[i]-3] <= 57 && ligne[positionsrouge[i]-4] == ' '){
            p=ligne[positionsrouge[i]-2]-48+10*(ligne[positionsrouge[i]-3]-48);
            if (p >= lignenb.rouge){
                lignenb.rouge =p;
            }
        }
    }

    // pour les bleus 
    for (int i=0; i<nombreOccurencesbleu;++i){
        if (ligne[positionsbleu[i]-2] >= 48 && ligne[positionsbleu[i]-2] <= 57 && ligne[positionsbleu[i]-3] == ' '){
            p=ligne[positionsbleu[i]-2]-48;
            if (p >= lignenb.bleu){
                lignenb.bleu =p;
            }
        }

        if (ligne[positionsbleu[i]-2] >= 48 && ligne[positionsbleu[i]-2] <= 57 && ligne[positionsbleu[i]-3] >= 48 && ligne[positionsbleu[i]-3] <= 57 && ligne[positionsbleu[i]-4] == ' '){
            p=ligne[positionsbleu[i]-2]-48+10*(ligne[positionsbleu[i]-3]-48);
            if (p >= lignenb.bleu){
                lignenb.bleu =p;
            }
        }
    }

    // pour les verts 
    for (int i=0; i<nombreOccurencesvert;++i){
        if (ligne[positionsvert[i]-2] >= 48 && ligne[positionsvert[i]-2] <= 57 && ligne[positionsvert[i]-3] == ' '){
            p=ligne[positionsvert[i]-2]-48;
            if (p >= lignenb.vert){
                lignenb.vert =p;
            }
        }

        if (ligne[positionsvert[i]-2] >= 48 && ligne[positionsvert[i]-2] <= 57 && ligne[positionsvert[i]-3] >= 48 && ligne[positionsvert[i]-3] <= 57 && ligne[positionsvert[i]-4] == ' '){
            p=ligne[positionsvert[i]-2]-48+10*(ligne[positionsvert[i]-3]-48);
            if (p >= lignenb.vert){
                lignenb.vert =p;
            }
        }
    }

    free(positionsrouge);
    free(positionsbleu);
    free(positionsvert);
    
    int k=(lignenb.rouge)*(lignenb.bleu)*(lignenb.vert);
    return(k);
}



int main() {
    // Déclarer une variable pour stocker le fichier
    FILE *fichier;

    // Ouvrir le fichier en mode lecture
    fichier = fopen("input2.txt", "r");

    // Vérifier si le fichier a été ouvert avec succès
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1; // Quitter le programme avec une erreur
    }
    struct game initial;
    initial.rouge=12;
    initial.bleu=14;
    initial.vert=13;

    // Déclarer un tableau pour stocker la ligne lue
    char ligne[256]; // Vous pouvez ajuster la taille en fonction de vos besoins
    int s=0;
    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        s=s+nmpow(ligne);
    }
    printf("%d",s);
    // Fermer le fichier
    fclose(fichier);

    return 0; // Terminer le programme avec succès
}

// doit retourner 48 pour le test pour game1 et 2286 pour test.txt 