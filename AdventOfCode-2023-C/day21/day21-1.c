#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define NB_MAX_NOEUDS 10000
#define MAX_DIST 64

struct noeud {
    int ligne;
    int colonne;
};

struct chemin {
    int est_libre; // vaut 0 si non, 1 si oui 
    int distance_de_S;
};

// trouver le point S
struct noeud trouver_le_point_S(char **input, int nline, int tailleline){
    struct noeud point_S;
    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            if (input[i][j]=='S'){
                point_S.ligne=i;
                point_S.colonne=j;
                break;
            }
        }
    }
    return(point_S);
}

//si le noeud est libre et à une distance inférieur à MAX_DIST du point S, on ajoute ce noeud à l'input en le marquant comme occupée et en notant sa distance à S
// bien penser au passage par adresse de entrée_de_queue
void add_node(int row, int col, int calling_dist_from_start, struct chemin** chemin_array, int chemin_ligne_max, int chemin_colonne_max, struct noeud* queue, int* entrée_de_queue){
	if (row >= 0 && row<chemin_ligne_max && col >= 0 && col<chemin_colonne_max){
        if(calling_dist_from_start == MAX_DIST)
		    return;
	    if(chemin_array[row][col].est_libre == 0)
		    return;

	    chemin_array[row][col].est_libre = 0;
	    chemin_array[row][col].distance_de_S = calling_dist_from_start + 1;
	    queue[*entrée_de_queue].ligne = row;
	    queue[*entrée_de_queue].colonne = col;
	    ++(*entrée_de_queue);
    }
    else {
        return;
    }
}


int main (){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input21.txt", "r");
    char ligne[140];

    // Récupérer les dimensions de l'entrée 
    int nline=0;
    int tailleline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
        tailleline=strlen(ligne);
    }
    printf("\nil y a %d lignes à traiter\n", nline);
    printf("les lignes sont de longueur %d\n\n", tailleline);

    //récupérer les données de l'input (même si elles seront inutiles comme ça, j'aurais aussi pu faire une fonction auxiliaire mais c'était plus facile comme ça)
    rewind(file);
    char** input=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        input[i]=(char*)malloc(sizeof(char)*tailleline);
    }
    int k=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        strcpy(input[k], ligne);
        ++k;
    }
    // affichage de l'input 
    printf("tableau de l'input\n");
    for (int i=0; i<nline; i++){
        printf("%s",input[i]);
    }
    printf("\n\n");


    // correspond à notre point de départ 
    struct noeud point_S=trouver_le_point_S(input, nline, tailleline);
    printf("point_S aux coordonnées : (%d,%d)\n\n",point_S.ligne, point_S.colonne);


    rewind(file);
    struct chemin** chemin_array=(struct chemin**)malloc(sizeof(struct chemin*)*nline);
    for (int i=0; i<nline; i++){
        chemin_array[i]=(struct chemin*)malloc(sizeof(struct chemin)*tailleline);
    }
    int chemin_ligne_max=0;
    int chemin_colonne_max=0;
    // comme récupérer l'input n'était pas très intéressant au final : 
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for(int i = 0; ligne[i] !=  '\n'; ++i){
			switch(ligne[i]){
				case '.':
					chemin_array[chemin_ligne_max][i].est_libre = 1;
					chemin_array[chemin_ligne_max][i].distance_de_S = -1;
					break;
				case '#':
					chemin_array[chemin_ligne_max][i].est_libre = 0;
					chemin_array[chemin_ligne_max][i].distance_de_S = -1;
					break;
				case 'S':
					chemin_array[chemin_ligne_max][i].est_libre = 0;
					chemin_array[chemin_ligne_max][i].distance_de_S = 0;
					break;
			}
			chemin_colonne_max = i + 1;
		}
		++chemin_ligne_max;
    }
    fclose(file);
    printf("map pour dire si les points sont libres :\n");
    for (int i=0; i<chemin_ligne_max; i++){
        for (int j=0; j<chemin_colonne_max; j++){
            printf("%d,", chemin_array[i][j].est_libre);
        }
        printf("\n");
    }
    printf("\nmap pour montrer la distance à S, pas encore complétées pour les noeuds :\n");
    for (int i=0; i<chemin_ligne_max; i++){
        for (int j=0; j<chemin_colonne_max; j++){
            printf("%d,", chemin_array[i][j].distance_de_S);
        }
        printf("\n");
    }


    struct noeud* queue=(struct noeud*)malloc(sizeof(struct noeud)*NB_MAX_NOEUDS);
    queue[0].ligne=point_S.ligne;
    queue[0].colonne=point_S.colonne;
    int entrée_de_queue=1;
    int suivant_en_queue=0;
    while(entrée_de_queue != suivant_en_queue){
        // on regarde en haut 
		add_node(queue[suivant_en_queue].ligne + 1, queue[suivant_en_queue].colonne, chemin_array[queue[suivant_en_queue].ligne][queue[suivant_en_queue].colonne].distance_de_S, chemin_array, chemin_ligne_max, chemin_colonne_max, queue, &entrée_de_queue);
        
        // on regarde en bas 
		add_node(queue[suivant_en_queue].ligne - 1, queue[suivant_en_queue].colonne, chemin_array[queue[suivant_en_queue].ligne][queue[suivant_en_queue].colonne].distance_de_S, chemin_array, chemin_ligne_max, chemin_colonne_max, queue, &entrée_de_queue);
        
        // on regarde à droite  
		add_node(queue[suivant_en_queue].ligne, queue[suivant_en_queue].colonne + 1, chemin_array[queue[suivant_en_queue].ligne][queue[suivant_en_queue].colonne].distance_de_S, chemin_array, chemin_ligne_max, chemin_colonne_max, queue, &entrée_de_queue);
        
        // on regarde à gauche
		add_node(queue[suivant_en_queue].ligne, queue[suivant_en_queue].colonne - 1, chemin_array[queue[suivant_en_queue].ligne][queue[suivant_en_queue].colonne].distance_de_S, chemin_array, chemin_ligne_max, chemin_colonne_max, queue, &entrée_de_queue);
		++suivant_en_queue;
	}
    

    int nombre_de_chemins=0;
    for(int i = 0; i < chemin_ligne_max; ++i){
		for(int j = 0; j < chemin_colonne_max; ++j){
			if(chemin_array[i][j].distance_de_S != -1 && chemin_array[i][j].distance_de_S % 2 == MAX_DIST % 2)
				++nombre_de_chemins;
		}
	}

	printf("\nle résultat est : %d\n\n", nombre_de_chemins);


    for (int i=0; i<nline; i++){
        free(input[i]);
    }
    free(input);

    for (int i=0; i<nline; i++){
        free(chemin_array[i]);
    }
    free(chemin_array);

    free(queue);
 
    return(0);
}