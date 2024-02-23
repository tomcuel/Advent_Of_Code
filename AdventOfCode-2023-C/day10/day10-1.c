#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// evaluation du Maze, on complète les pas à faire pour chaque position par récursivité
// en prenant comme argument la position du point : (pX,pY)
// ns le nombre de pas déjà fait pour arriver à ce point 
// et la position de la direction où l'on va regarder ce qui se passe : (x,y)
void evaluation(char** Maze, int** Pas, int pX, int pY, int ns, int x, int y){
	if (Maze[x][y] == 'S') {
		return;
	}

	if (!Pas[x][y]) {
		Pas[x][y] = ns + 1;
	} 
    else {
		Pas[x][y] = Pas[x][y] < (ns + 1) ? Pas[x][y] : (ns + 1);
        // Pas[x][y] garde la même valeur si < ns+1, prend la valeur ns+1 sinon
	}

    //disjonction de cas selon le symbole et donc la direction du labyrinthe
	switch (Maze[x][y]) {
	case '|': //On regarde en verticale 
		if (pX == x + 1) {
			evaluation(Maze, Pas, x, y, ns + 1,  x - 1, y);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x + 1, y);
		}
		break;
	case '-': // On regarde en horizontale
		if (pY == y - 1) {
			// left to right
			evaluation(Maze, Pas, x, y, ns + 1, x, y + 1);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x, y - 1);
		}
		break;
	case 'L': // On regarde du nord à l'est 
		if (pX == x - 1) {
			evaluation(Maze, Pas, x, y, ns + 1, x, y + 1);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x - 1, y);
		}
		break;
	case 'J': // On regarde du nord à l'ouest 
		if (pX == x - 1) {
			evaluation(Maze, Pas, x, y, ns + 1, x, y - 1);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x - 1, y);
		}
		break;
	case '7': // On regarde du sud à l'ouest 
		if (pY == y - 1) {
			evaluation(Maze, Pas, x, y, ns + 1, x + 1, y);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x, y - 1);
		}
		break;
	case 'F': // On regarde du sud à l'est
		if (pY == y + 1) {
			evaluation(Maze, Pas, x, y, ns + 1, x + 1,  y);
		} 
        else {
			evaluation(Maze, Pas, x, y, ns + 1, x, y + 1);
		}
		break;
	default:
		break;
	}
}


int main (){

    FILE* file;
    file = fopen("input10.txt", "r");
    char ligne[256]; 
    int nbline=0;
    int tailleline;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        tailleline=strlen(ligne); //on a compter le sautage de ligne à la fin mais on s'en fou, si ça marche pas on décale de -1
        nbline=nbline+1; 
    }

    printf("\n");
    printf("nombre de lignes : %d\n",nbline);
    printf("nombre de colonnes : %d\n",tailleline);
    printf("\n");

    // Faire le labyrinthe
    // Allocation de mémoire pour le tableau de pointeurs vers les lignes
    char **Maze = (char **)malloc((nbline) * sizeof(char *));
    // Allocation de mémoire pour chaque ligne
    for (int i = 0; i < nbline; i++) {
        Maze[i] = (char *)malloc((tailleline) * sizeof(char));
    }

    // Compter le nombre de pas à faire pour aller à chaque point 
    // Allocation de mémoire pour le tableau de pointeurs vers les lignes
    int **Pas = (int **)malloc((nbline) * sizeof(int *));
    // Allocation de mémoire pour chaque ligne
    for (int i = 0; i < nbline; i++) {
        Pas[i] = (int *)malloc((tailleline) * sizeof(int));
    }
    // on l'initialise qu'avec des zéros 
    for (int i = 0; i < nbline; i++) {
        for (int j = 0; j < nbline; j++) {
            Pas[i][j]=0;
        }
    }    

    rewind(file);
    int i=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for (int j = 0; j < tailleline; j++) {
            Maze[i][j]=ligne[j];
        }
        i=i+1;
    }

    // Affichage des valeurs du tableau
    for (int q = 0; q < nbline; q++) {
        for (int j = 0; j < tailleline; j++) {
            printf("%c", Maze[q][j]);
        }
        printf("\n");
    }
    printf("\n");

    // j'ai récupéré le labyrinthe 

    // trouver le S par ses coordonnées 
    int Sx;
    int Sy;
    for (int i = 0; i < nbline; i++) {
		for (int j = 0; j < tailleline; j++) {
			if (Maze[i][j] == 'S') {
				Sx = i;
				Sy = j;
				break;
			}
		}
	}

    printf("Le point %c est à la position (%d,%d)\n\n",Maze[Sx][Sy],Sx,Sy);

    //S peut valeur n'importe quoi, cela dépendra de la constitution à côté 
    //Le Maze est bien fait normalement pour qu'il y ait 2 connections à chaque fois, pour chaque tuyau du chemin, autres que ceux du rond central

    // en dessous du S 
	if ((Maze[Sx + 1][Sy] == '|') || (Maze[Sx + 1][Sy] == 'J') || (Maze[Sx + 1][Sy] == 'L')) {
		evaluation(Maze, Pas, Sx, Sy, 0, Sx + 1, Sy);
	}
	// à droite du S
	if ((Maze[Sx][Sy + 1] == '-') || (Maze[Sx][Sy + 1] == '7') || (Maze[Sx][Sy + 1] == 'J')) {
		evaluation(Maze, Pas, Sx, Sy, 0, Sx, Sy + 1);
	}
	// au dessus du S
	if ((Maze[Sx - 1][Sy] == '|') || (Maze[Sx - 1][Sy] == 'F') || (Maze[Sx - 1][Sy] == '7')) {
		evaluation(Maze, Pas, Sx, Sy, 0, Sx - 1, Sy);
	}
	// à gauche du S
	if ((Maze[Sx][Sy - 1] == '-') || (Maze[Sx][Sy - 1] == 'L') || (Maze[Sx][Sy - 1] == 'F')) {
		evaluation(Maze, Pas, Sx, Sy, 0, Sx, Sy - 1);
	}

    // Affichage des valeurs du tableau
    for (int i = 0; i < nbline; i++) {
        for (int j = 0; j < tailleline; j++) {
            printf("%d,", Pas[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int max=0;
    for (int i = 0; i < nbline; i++) {
        for (int j = 0; j < tailleline; j++) {
            if (Pas[i][j] > max){
                max = Pas[i][j];
            }
        }
    }

    printf("\nLe tuyau le plus éloigné reliant à l'animal à une distance  : %d\n\n",max);

    // Libération de la mémoire
    for (int i = 0; i < nbline; i++) {
        free(Maze[i]);
    }
    free(Maze);

    return(0);
}

// on trouve bien 4 pour le test1.txt