#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define NB_MAX_NOEUDS 100000
#define MAX_DIST 26501365

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
void add_node(long int row, long int col, long int calling_dist_from_point_S, int local_max, int chemin_ligne_max, int chemin_colonne_max, int* que_enter, struct noeud* que, struct chemin** local_path_arr){
	if(row < 0 || row >= chemin_ligne_max || col < 0 || col >= chemin_colonne_max)
		return;
	if(calling_dist_from_point_S == local_max)
		return;
	if(local_path_arr[row][col].est_libre == 0)
		return;

	local_path_arr[row][col].est_libre = 0;
	local_path_arr[row][col].distance_de_S = calling_dist_from_point_S + 1;
	que[*que_enter].ligne = row;
	que[*que_enter].colonne = col;
	(*que_enter) += 1;
}

// copie path_arr vers local_path_arr
void copy_graph(int chemin_ligne_max, int chemin_colonne_max, struct chemin** local_path_arr, struct chemin** path_arr){
	for(int i = 0; i < chemin_ligne_max; ++i){
		for(int j = 0; j < chemin_colonne_max; ++j){
			local_path_arr[i][j].distance_de_S = -1;
			local_path_arr[i][j].est_libre = path_arr[i][j].est_libre;
		}
	}
}

//faire un parcours en largueur du point S (st_row, st_col) à la fin du graphe
// attention au passage par adresse pour que_enter et next_in_que
// fait une fonction de ce qu'on faisait pour la p1, qui n'avait besoin d'être fait qu'une seule fois
long int bfs(long int st_row, long int st_col, long int local_max, long int*** cashe, int chemin_ligne_max, int chemin_colonne_max, struct chemin** local_path_arr, struct chemin** path_arr, struct noeud* que){
	if(local_max <= 0)
		return 0;
	if(local_max > 499){
	//	printf("%ld\n", local_max % 2 == 1);
		if(local_max % 2l == 1){

			local_max = 499;
		}
		else{
			local_max = 498;
		}
	}
	if(cashe[st_row][st_col][local_max] != -1){
		return cashe[st_row][st_col][local_max];
	}

	copy_graph(chemin_ligne_max, chemin_colonne_max, local_path_arr, path_arr);

	long int num_of_paths = 0;

	que[0].ligne = st_row;
	que[0].colonne = st_col;
	int que_enter = 1;
	int next_in_que = 0;
	local_path_arr[st_row][st_col].est_libre = 0;
	local_path_arr[st_row][st_col].distance_de_S = 0;

	while(que_enter != next_in_que){
        // on regarde en haut 
		add_node(que[next_in_que].ligne + 1, que[next_in_que].colonne, local_path_arr[que[next_in_que].ligne][que[next_in_que].colonne].distance_de_S, local_max, chemin_ligne_max, chemin_colonne_max, &que_enter, que, local_path_arr);
        
        // on regarde en bas 
		add_node(que[next_in_que].ligne - 1, que[next_in_que].colonne, local_path_arr[que[next_in_que].ligne][que[next_in_que].colonne].distance_de_S, local_max, chemin_ligne_max, chemin_colonne_max, &que_enter, que, local_path_arr);

        // on regarde à droite  
		add_node(que[next_in_que].ligne, que[next_in_que].colonne + 1, local_path_arr[que[next_in_que].ligne][que[next_in_que].colonne].distance_de_S, local_max, chemin_ligne_max, chemin_colonne_max, &que_enter, que, local_path_arr);

        // on regarde à gauche
		add_node(que[next_in_que].ligne, que[next_in_que].colonne - 1, local_path_arr[que[next_in_que].ligne][que[next_in_que].colonne].distance_de_S, local_max, chemin_ligne_max, chemin_colonne_max, &que_enter, que, local_path_arr);

		++next_in_que;
	}

	for(int i = 0; i < chemin_ligne_max; ++i){
		for(int j = 0; j < chemin_colonne_max; ++j){
			if(local_path_arr[i][j].distance_de_S != -1 && local_path_arr[i][j].distance_de_S % 2 == local_max % 2)
				++num_of_paths;
		}
	}

	cashe[st_row][st_col][local_max] = num_of_paths;
	return num_of_paths;
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
    /*
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
    */

    
    long int total_same_internal_plots = 0;
    long int total_diff_internal_plots = 0;

    long int*** cashe=(long int***)malloc(sizeof(long int**)*nline);
    for (int i=0; i<nline; i++){
        cashe[i]=(long int **)malloc(sizeof(long int*)*nline);
        for (int j=0; j<nline; j++){
            cashe[i][j]=(long int *)malloc(sizeof(long int)*500);
        }
    }
    for(int i = 0; i < nline; i++)
		for(int j = 0; j < nline; j++)
			for(int k = 0; k < 500; k++)
				cashe[i][j][k] = -1;


    struct chemin** local_path_arr=(struct chemin**)malloc(sizeof(struct chemin*)*nline);
    for (int i=0; i<nline; i++){
        local_path_arr[i]=(struct chemin*)malloc(sizeof(struct chemin)*tailleline);
    }
    struct noeud* que=(struct noeud*)malloc(sizeof(struct noeud)*NB_MAX_NOEUDS);


    int same_path_count = 0;
    same_path_count = bfs(point_S.ligne, point_S.colonne, MAX_DIST, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
    int diff_path_count = 0;
	diff_path_count = bfs(point_S.ligne, point_S.colonne, MAX_DIST - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);


	long int internal_plot_length = 0;
	internal_plot_length = (MAX_DIST - 2 * (chemin_ligne_max - (point_S.ligne + 1))) / chemin_ligne_max;
	printf("same_path : %d\ndiff_path : %d\ninternal_plot %ld\n", same_path_count, diff_path_count, internal_plot_length);


    long int num_of_paths = 0;
    long int upper_edge_off = 0;
	long int lower_edge_off = 0;
	long int edge_size[4] = {0, 0, 0, 0};

    //gets the internal path count
	for(long int i = 1; i <= internal_plot_length; ++i){
		if(i % 2 == 1){
			num_of_paths += diff_path_count + (diff_path_count * (i - 1));
		}
		else{
			num_of_paths += same_path_count + (same_path_count * (i - 1));
		}
	}
	num_of_paths *= 4;
	num_of_paths += same_path_count;

	//gets corners
	for(long int i = MAX_DIST - (chemin_ligne_max * internal_plot_length) - (chemin_ligne_max / 2) - 1; i > 0; i -= chemin_ligne_max){
		printf("%ld\n", i);
		num_of_paths += bfs(point_S.ligne, 0, i, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(point_S.colonne, chemin_colonne_max - 1, i, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(0, point_S.colonne, i, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(chemin_ligne_max - 1, point_S.colonne, i, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		upper_edge_off = i;
		lower_edge_off = i + chemin_ligne_max;
		if(i > chemin_ligne_max){
			num_of_paths += bfs(0, 0, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
			num_of_paths += bfs(chemin_ligne_max - 1, 0, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
			num_of_paths += bfs(0, chemin_colonne_max - 1, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
			num_of_paths += bfs(chemin_ligne_max - 1, chemin_colonne_max - 1, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		}
		num_of_paths += bfs(0, 0, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(chemin_ligne_max - 1, 0, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(0, chemin_colonne_max - 1, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
		num_of_paths += bfs(chemin_ligne_max - 1, chemin_colonne_max - 1, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);
	}

	edge_size[0] = bfs(0, 0, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que) + bfs(0, 0, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);

	edge_size[1] = bfs(chemin_ligne_max - 1, 0, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que) +bfs(chemin_ligne_max - 1, 0, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);

	edge_size[2] = bfs(0, chemin_colonne_max - 1, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que) +bfs(0, chemin_colonne_max - 1, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);

	edge_size[3] = bfs(chemin_ligne_max - 1, chemin_colonne_max - 1, lower_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que) +bfs(chemin_ligne_max - 1, chemin_colonne_max - 1, upper_edge_off - (chemin_ligne_max / 2) - 1, cashe, chemin_ligne_max, chemin_colonne_max, local_path_arr, chemin_array, que);

	for(int i = 0; i < internal_plot_length; ++i){
		num_of_paths += edge_size[0];
		num_of_paths += edge_size[1];
		num_of_paths += edge_size[2];
		num_of_paths += edge_size[3];
	}


	

	printf("\nle résultat est : %ld\n\n", num_of_paths);


    for (int i=0; i<nline; i++){
        free(input[i]);
    }
    free(input);

    for (int i=0; i<nline; i++){
        free(chemin_array[i]);
    }
    free(chemin_array);

     for (int i=0; i<nline; i++){
        free(local_path_arr[i]);
    }
    free(local_path_arr);

    for (int i=0; i<nline; i++){
        for (int j=0; j<nline; j++){
            free(cashe[i][j]);
        }
        free(cashe[i]);
    }
    free(cashe);

    free(que);
 
    return(0);
}

// il manque juste 100 à 600090522932019 pour que ça marche