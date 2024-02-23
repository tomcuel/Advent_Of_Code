#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *str_digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", NULL};

// fonction qui renvoie le chiffre correspondant à une chaîne de caractères
// renvoie -1 si la chaîne n'est pas un chiffre
int get_digit(char *str){
	int i;

	// si la chaîne est un chiffre
	if (*str >= '0' && *str <= '9') {
		return *str - '0';
	}

	// sinon, on cherche dans le tableau str_digits
	for (i = 0; str_digits[i]; i++) {
		if (!strncmp(str, str_digits[i], strlen(str_digits[i]))) {
			return i + 1;
		}
	}

	return -1;
}

// fonction qui renvoie le total d'une ligne
int totalligne(char* ligne){
	int first=0; 
	int second=0;
	char *ptr;
	ptr = ligne;
	// on cherche le premier chiffre
	while ((first = get_digit(ptr)) == -1) {
		ptr++;
	}

	// on se place à la fin de la ligne
	ptr = ligne + strlen(ligne) - 1;
	// on cherche le deuxième chiffre
	while ((second = get_digit(ptr)) == -1) {
		ptr--;
	}

	return((first * 10) + second);
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

	char ligne[200]={0};
	int sum=0;
	
	while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
		sum+=totalligne(ligne);
	}
	printf("\nLe résultat est : %d\n\n", sum);

	fclose(fichier);
	return 0;
}

// doit donner 281 avec text2.txt : il me donne 271 