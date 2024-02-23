#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// fonction pour extraire les chiffres d'une chaîne de caractères
char* extraireChiffres(const char* chaine) {
    char* chiffres = (char*)malloc(strlen(chaine) + 1);
   
    if (chiffres == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    for (int i = 0; chaine[i] != '\0'; i++) {
        if (isdigit((unsigned char)chaine[i])) {
            chiffres[j] = chaine[i];
            ++j;
        }
    }
    chiffres[j] = '\0';
    return chiffres;
}

// Fonction pour calculer le nombre de possibilités
uint64_t ways(uint64_t t, uint64_t d){
    uint64_t nbpossible=0;
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
    uint64_t Tab[2];
    int nbligne=0;
    fichier = fopen("input6.txt", "r");
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char* nombres=extraireChiffres(ligne);
        Tab[nbligne]=strtoull(nombres, NULL, 10); // strtoull pour les grands nombres
        printf("%llu\n",Tab[nbligne]);
        ++nbligne;
    }
   
    printf("%d\n",nbligne);

    fclose(fichier);

    int nbpossibility=ways(Tab[0],Tab[1]);

    printf("\nle résultat est %llu\n",nbpossibility);
    
    return(0);
}

// on trouve bien 71503 pour le test.txt