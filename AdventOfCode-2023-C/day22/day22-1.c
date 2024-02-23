#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define TAILLE_MAP 500

// structure d'une brique 
struct brick {
	int st_row;
	int st_col;
	int st_hight;
	int end_row;
	int end_col;
	int end_hight;
};

//pour bouger les briques 
struct cord {
	int row;
	int col;
	int hight;
};

//change le début et la fin de la brique 
void swap_ends(struct brick *brick_ptr){
	int temp_hight;
	int temp_col;
	int temp_row;

	temp_hight = brick_ptr->end_hight;
	temp_col = brick_ptr->end_col;
	temp_row = brick_ptr->end_row;

	brick_ptr->end_hight = brick_ptr->st_hight;
	brick_ptr->end_col = brick_ptr->st_col;
	brick_ptr->end_row = brick_ptr->st_row;

	brick_ptr->st_hight = temp_hight;
	brick_ptr->end_col = temp_col;
	brick_ptr->st_row = temp_row;
}

//mets tous les brick_space pointeurs à NULL
void reset_space(struct brick**** brick_space, struct brick ground){
	for(int i = 0; i < TAILLE_MAP; i++) {
        for(int j = 0; j < TAILLE_MAP; j++){
			for(int k = 0; k < TAILLE_MAP; k++){
				brick_space[i][j][k] = NULL;
            }
        }
    }

	for(int j = 0; j < TAILLE_MAP; j++){
		for(int k = 0; k < TAILLE_MAP; k++){
			brick_space[0][j][k] = &ground;
        }
    }
}

//affiche brick_space en tranche jusqu'à hight row col
void print_space(int hight, int row, int col, struct brick**** brick_space){
	for(int i = 0; i <= hight; ++i){
		printf("layer %d:\n", i);
		for(int j = 0; j <= row; ++j){
			for(int k = 0; k <= col; ++k){
				if(brick_space[i][j][k] == NULL)
					printf("0");
				else
					printf("1");

			}
			printf("\n");
		}
		printf("\n");
	}
}

//compare la hauteur de départ de deux briques, sert dans qsort dans le main 
int hight_cmp(const void *a, const void *b){
	return (((struct brick *) a)->st_hight - ((struct brick *) b)->st_hight);
}

//prends la liste de coordonnées de brick_ptr et la mets dans cord_array
//retourne le nombre de coordonées obtenues 
int get_cords(struct brick *brick_ptr, struct cord cord_arr[]){
	int lowest_hight;
	int highest_hight;
	int lowest_row;
	int highest_row;
	int lowest_col;
	int highest_col;

	if(brick_ptr->end_hight > brick_ptr->st_hight){
		lowest_hight = brick_ptr->st_hight;
		highest_hight = brick_ptr->end_hight;
	}
	else{
		lowest_hight = brick_ptr->end_hight;
		highest_hight = brick_ptr->st_hight;
	}

	if(brick_ptr->end_col > brick_ptr->st_col){
		lowest_col = brick_ptr->st_col;
		highest_col = brick_ptr->end_col;
	}
	else{
		lowest_col = brick_ptr->end_col;
		highest_col = brick_ptr->st_col;
	}

	if(brick_ptr->end_row > brick_ptr->st_row){
		lowest_row = brick_ptr->st_row;
		highest_row = brick_ptr->end_row;
	}
	else{
		lowest_row = brick_ptr->end_row;
		highest_row = brick_ptr->st_row;
	}
	int cord_num = 0;
	for(int i = lowest_hight; i <= highest_hight; ++i)
		for(int j = lowest_col; j <= highest_col; ++j)
			for(int k = lowest_row; k <=highest_row; ++k){
				cord_arr[cord_num].hight = i;
				cord_arr[cord_num].col = j;
				cord_arr[cord_num].row = k;
				++cord_num;
			}
	return cord_num;
}

//enlève brick de brick_ptr dans brick_space
void remove_brick(struct brick *brick_ptr, struct brick**** brick_space){

	struct cord cord_array[1000];
	int cord_num = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		brick_space[cord_array[i].hight][cord_array[i].row][cord_array[i].col] = NULL;
	}
}

//palce brick_ptr dans brick_space
void place_brick(struct brick *brick_ptr, struct brick**** brick_space){

	struct cord cord_array[1000];
	int cord_num = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		brick_space[cord_array[i].hight][cord_array[i].row][cord_array[i].col] = brick_ptr;
	}
}

//retourne le point le plus haut en dessous brick_ptr + 1
int get_hight(struct brick *brick_ptr, struct brick**** brick_space) {

	struct cord cord_array[1000];
	int cord_num = 1;
	int highest_point = 1;
	int temp_highest_point = 0;

	cord_num = get_cords(brick_ptr, cord_array);

	for(int i = 0; i < cord_num; ++i){
		for(temp_highest_point = cord_array[i].hight; brick_space[temp_highest_point][cord_array[i].row][cord_array[i].col] == brick_ptr ||
		brick_space[temp_highest_point][cord_array[i].row][cord_array[i].col] == NULL; --temp_highest_point);

		if(temp_highest_point + 1 > highest_point)
			highest_point = temp_highest_point + 1;
	}

	return highest_point;
}

//bouge brick_ptr à l'endroit le plus bas où il peut être et change cela dans brick_space
void brick_fall(struct brick *brick_ptr, struct brick**** brick_space){

	int new_hight = 0;

	new_hight = get_hight(brick_ptr, brick_space);

	remove_brick(brick_ptr, brick_space);

	if(brick_ptr->end_hight > brick_ptr->st_hight){
		brick_ptr->end_hight = new_hight + abs(brick_ptr->end_hight - brick_ptr->st_hight);
		brick_ptr->st_hight = new_hight;
	}
	else{
		brick_ptr->st_hight = new_hight + abs(brick_ptr->st_hight - brick_ptr->end_hight);
		brick_ptr->end_hight = new_hight;
	}

	place_brick(brick_ptr, brick_space);
}

//regarde si une brique peut être enlevée
int can_remove(struct brick *brick_ptr, struct brick**** brick_space){

	struct cord cord_array[1000];
	int cord_num = 0;
	int old_hight;
	int new_hight;
	struct brick *above_ptr;

	cord_num = get_cords(brick_ptr, cord_array);

	remove_brick(brick_ptr, brick_space);

	for(int i = 0; i < cord_num; ++i){
		above_ptr = brick_space[cord_array[i].hight + 1][cord_array[i].row][cord_array[i].col];

		if(above_ptr != NULL){
			old_hight = above_ptr->st_hight;
			new_hight = get_hight(above_ptr, brick_space);

			if(old_hight != new_hight){
				place_brick(brick_ptr, brick_space);
				return 0;
			}
		}
	}

	place_brick(brick_ptr, brick_space);

	return 1;
}


int main (){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input22.txt", "r");
    char ligne[30];

    // Récupérer les dimensions de l'entrée 
    int nline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
    }
    printf("\nil y a %d lignes à traiter\n\n", nline);


    // défini le sol 
    struct brick ground;
    // pile de sable en 3D
    struct brick**** brick_space=(struct brick****)malloc(sizeof(struct brick***)*TAILLE_MAP);
    for(int i = 0; i < TAILLE_MAP; i++) {
        brick_space[i]=(struct brick***)malloc(sizeof(struct brick**)*TAILLE_MAP);
        for(int j = 0; j < TAILLE_MAP; j++){
            brick_space[i][j]=(struct brick**)malloc(sizeof(struct brick*)*TAILLE_MAP);
			for(int k = 0; k < TAILLE_MAP; k++){
				brick_space[i][j][k]=(struct brick*)malloc(sizeof(struct brick)*TAILLE_MAP);
            }
        }
    }
    reset_space(brick_space, ground);


    //stocker les différentes briques de l'input
    struct brick* brick_array=(struct brick*)malloc(sizeof( struct brick)*nline);
    char* token;
    int brick_num=0;
    // récupérer les données du problème 
    rewind(file);
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        //starting column
		token = strtok(ligne, " ,~\n");
		brick_array[brick_num].st_col = atoi(token);

		//starting row
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].st_row = atoi(token);

		//starting hight
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].st_hight = atoi(token);

		//ending column
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_col = atoi(token);

		//ending row
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_row = atoi(token);

		//ending hight
		token = strtok(NULL, " ,~\n");
		brick_array[brick_num].end_hight = atoi(token);

		if(brick_array[brick_num].st_hight > brick_array[brick_num].end_hight){
			swap_ends(&brick_array[brick_num]);
		}

		++brick_num;
    }
    
    int total_brick_bougees=0;
    //trie par hauteur pour que l'élément le plus bas tombe en premier
	qsort(brick_array, brick_num, sizeof(struct brick), hight_cmp);

	for(int i = 0; i < brick_num; ++i){
		place_brick(&brick_array[i], brick_space);
	}

	for(int i = 0; i < brick_num; ++i){
		brick_fall(&brick_array[i], brick_space);
	}

	print_space(10, 10, 10, brick_space);
	printf("%d\n", brick_num);

	for(int i = 0; i < brick_num; ++i){
		total_brick_bougees += can_remove(&brick_array[i], brick_space);
	}
	print_space(10, 10, 10, brick_space);


	printf("\nle résultat est : %d\n\n", total_brick_bougees);


    // on libère la mémoire utilisée 
    for(int i = 0; i < TAILLE_MAP; i++) {
        for(int j = 0; j < TAILLE_MAP; j++){
			for(int k = 0; k < TAILLE_MAP; k++){
				 free(brick_space[i][j][k]);
            }
			free(brick_space[i][j]);
        }
        free(brick_space[i]);
    }
    free(brick_space);

    free(brick_array);
    return(0);
}

// on trouve bien 5 pour test.txt