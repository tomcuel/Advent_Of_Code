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

    int sum=0;
    int left=0; 
    int right=0;
    int have_F = 0; // vaudra 1 si on a F, 0 si on l'a pas 
    int have_L = 0; // vaudra 1 si on a L, 0 si on l'a pas 

    /* Voir la loupe comme un polygone que l'on peut croiser avec trois différents symboles : 
    | ou F ----- J ou L ----- 7
    */
    for (int i = 1; i < nbline - 1; i++) {
        for (int j = 1; j < tailleline - 1; j++) {
            // S est dans la boucle, mais on ne sait pas sa fonction
            if (Pas[i][j] || (Maze[i][j] == 'S')) {
                continue;
            }

            // si l'on trace une droite horizontale partant de Maze[i][j] dans les deux sens 
            left =0; //calcul le nombre de crossing à gauche du point 
            right = 0; //calcul le nombre de crossing à droite du point 

            for (int k = 0; k < j; k++) {
                if (!Pas[i][k]) {
                    continue;
                }
                switch (Maze[i][k]) {
                // on passe dans la loupe si on croise des bars verticals : |
                case '|':
                    left++; //On ajoute un crossing 
                    have_F = 0;
                    have_L = 0;
                    break;
                // on passe dans la loupe si on croise : F ----- J
                case 'F':
                    have_F = 1;
                    have_L = 0;
                    break;
                case 'J':
                    if (have_F) {
                        left++; //On ajoute un crossing 
                    }
                    have_F = 0;
                    have_L = 0;
                    break;
                // on passe dans la loupe si on croise : L ----- 7
                case 'L':
                    have_L = 1;
                    have_F = 0;
                    break;
                case '7':
                    if (have_L) {
                        left++; //On ajoute un crossing 
                    }
                    have_L = 0;
                    have_F = 0;
                    break;
                default:
                    break;
                }
            }
            for (int l = j; l < tailleline; l++) {
                if (!Pas[i][l]) {
                    continue;
                }
                switch (Maze[i][l]) {
                case '|':
                    right++;
                    have_F = 0;
                    have_L = 0;
                    break;
                case 'F':
                    have_F = 1;
                    have_L = 0;
                    break;
                case 'J':
                    if (have_F) {
                        right++;
                    }
                    have_F = 0;
                    have_L = 0;
                    break;
                case 'L':
                    have_L = 1;
                    have_F = 0;
                    break;
                case '7':
                    if (have_L) {
                        right++;
                    }
                    have_L = 0;
                    have_F = 0;
                    break;
                default:
                    break;
                }
            }
            //condition pour savoir si le point Maze[i][j] était à l'intérieur : 
            // si l'on croisé un nombre de fois pair la loop, Maze[i][j] était dehors
            // si l'on croisé un nombre de fois impair la loop, Maze[i][j] était à l'intérieur 
            if ((left % 2) && (right % 2)) {  // regarde si le reste est égal à 1, différent de 0
                sum++;
            }
        }
    }

    printf("Le nombre de valeurs à l'intérieur de la loop est de : %d\n\n", sum);



    // Libération de la mémoire
    for (int i = 0; i < nbline; i++) {
        free(Maze[i]);
    }
    free(Maze);

    return(0);
}

// on trouve bien 4 pour le test2.txt