#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

enum direction {
    NORTH, 
    SOUTH, 
    EAST, 
    WEST, 
}; 

struct coordonnées {
    int ligne;
    int colonne;
};

struct chemin {
    int est_visité; // 1 si oui, 0 sinon 
    char type; // # si on est sur une forêt, . pour un chemin, > pour une pente ascendante, qui demande une pente descente descendante au point d'après
    // on ajoutera 2 au chemin, si l'on croise une montée, car on devra en redescendre, attention à la direction (switch case)
};


// fonction qui renvoie 1 si la direction est valide en partant de la case (ligne, colonne), 0 sinon 
int is_valid(struct chemin** chemin_array, int nline, int ligne, int colonne, int direction){
    
    if (chemin_array[ligne][colonne].est_visité == 1){
        return(0);
    }
    
    switch(chemin_array[ligne][colonne].type){

        case '.' :
            return(1); 

        // pas de problème de bord normalement, pas besoin de faire de condition car on part d'un point valide et on regarde autour 
        // et comme les bordures du problème sont des '#', on a pas de problèmes 

        case '>' : 
            if (direction == EAST){
                return(1);
            }
            return(0);

        case '<' : 
             if (direction == WEST){
                return(1);
            }
            return(0);

        case '^' :
             if (direction == NORTH){
                return(1);
            }
            return(0);

        case 'v' : 
             if (direction == SOUTH){
                return(1);
            }
            return(0);

    }
    return(-1); // dans le cas où l'on tombe sur '#'
}

// fonction qui donne le résultat
// on commence en row=0+1, col=1, direction=SOUTH (on aurait pu prendre n'importe laquelle), visited=1
// on marque le point actuelle comme regarder
// on compare le nombre de visités à longest_path
// on regarde si les 4 directions sont valides et on récursit quand c'est le cas
// on re marque comme non visité quand on quitte le point si on sort d'une branche récursive 
void find_longest_path(int row, int col, int direction, int visited, struct chemin** chemin_array, int nline, int* longest_path, struct coordonnées arrivée){
    
    chemin_array[row][col].est_visité = 1;
    if(row == arrivée.ligne && col == arrivée.colonne){
		if(visited > (*longest_path)){
            *longest_path = visited;
        }
        printf("longueur du chemin intermédiaire : %d\n",visited);
		chemin_array[row][col].est_visité = 0;
		return;
	}

    // on récursive au nord 
    if (is_valid(chemin_array, nline, row-1, col, NORTH) == 1){
        find_longest_path(row-1, col, NORTH, visited+1, chemin_array, nline, longest_path, arrivée);
    }

     // on récursive au sud 
    if (is_valid(chemin_array, nline, row+1, col, SOUTH) == 1){
        find_longest_path(row+1, col, SOUTH, visited+1, chemin_array, nline, longest_path, arrivée);
    }

     // on récursive à l'est 
    if (is_valid(chemin_array, nline, row, col+1, EAST) == 1){
        find_longest_path(row, col+1, EAST, visited+1, chemin_array, nline, longest_path, arrivée);
    }

    // on récursive à l'ouest 
    if (is_valid(chemin_array, nline, row, col-1, WEST) == 1){
        find_longest_path(row, col-1, WEST, visited+1, chemin_array, nline, longest_path, arrivée);
    }

	chemin_array[row][col].est_visité = 0;
}



int main (){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input23.txt", "r");
    char ligne[200];

    // Récupérer les dimensions de l'entrée 
    int nline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
    }
    printf("\nil y a %d lignes à traiter\n\n", nline);

    // parcours test d'un labyrinthe 
    // algortihme bfs de parcours de graphe en largeur
    // on commence à l'input en position (0,1)
    // on chercher à finir en position (nline-1, nline-2)

    // pour stocker l'input avec des données différentes de l'input
    struct chemin** chemin_array=(struct chemin**)malloc(sizeof(struct chemin*)*nline);
    for (int i=0; i<nline; i++){
        chemin_array[i]=(struct chemin*)malloc(sizeof(struct chemin)*nline);
    }

    // on complète chemin_array 
    rewind(file);
    int k=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for (int j=0; j<nline; j++){
            if (ligne[j] == '#'){
                chemin_array[k][j].est_visité=1;
                chemin_array[k][j].type='#';
            }
            else {
                chemin_array[k][j].est_visité=0;
                chemin_array[k][j].type=ligne[j];
            }
        }
        ++k;
    }

    // affichage des données récoltées : si les chemins sont visités  
    printf("\nmap donnant si les chemins sont visités :\n");
    for (int i=0; i<nline; i++){
        for (int j=0; j<nline; j++){
            printf("%d",chemin_array[i][j].est_visité);
        }
        printf("\n");
    }
    // affichage des données récoltées : du type des chemins 
    printf("\nmap donnant si les chemins sont visités :\n");
    for (int i=0; i<nline; i++){
        for (int j=0; j<nline; j++){
            printf("%c",chemin_array[i][j].type);
        }
        printf("\n");
    }

    // départ au point (0,1)
    struct coordonnées départ;
    départ.ligne=0;
    départ.colonne=1;
    // on dit qu'on est passé par l'entrée
    chemin_array[départ.ligne][départ.colonne].est_visité=1;
    printf("\ndépart en (%d,%d) : '%c'\n",départ.ligne,départ.colonne,chemin_array[départ.ligne][départ.colonne].type);
    // arrivée au point (nline-1, nline-2)
    struct coordonnées arrivée;
    arrivée.ligne=nline-1;
    arrivée.colonne=nline-2;
    printf("arrivée en (%d,%d) : '%c'\n\n",arrivée.ligne,arrivée.colonne,chemin_array[arrivée.ligne][arrivée.colonne].type);

    int longest_path=0;
    find_longest_path(départ.ligne+1, départ.colonne, SOUTH, 1, chemin_array, nline, &longest_path, arrivée);
    
    printf("\nle résultat est : %d\n\n", longest_path);

    for (int i=0; i<nline; i++){
        free(chemin_array[i]);
    }
    free(chemin_array);
    return(0);
}

// on trouve bien 94 pour test.txt