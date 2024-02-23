#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// on donne des valeurs de nombres (0,1,2,3,4) pour les directions et le split 
enum direction{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPLIT
};


// structure d'un emplacement
struct beam{
	int ligne;
	int colonne;
	int direction;
};


// trouver la direction selon ce qui se passe, attention à ne pas confondre direction et placement par rapport au miroir
int trouver_direction(int direction_actuelle, int ligne, int colonne, char** tableau_miroir, int nline, int tailleline){
	
    switch(tableau_miroir[ligne][colonne]){
		
        case '.':
			return direction_actuelle;
		case '-':
			if(direction_actuelle == LEFT || direction_actuelle == RIGHT)
				return direction_actuelle;
			return SPLIT;
		case '|':
			if(direction_actuelle == UP || direction_actuelle == DOWN)
				return direction_actuelle;
			return SPLIT;
		case '\\': // '\\' pour représenter le caractère '\'
			switch(direction_actuelle){
				case UP:
					return LEFT;
				case DOWN:
					return RIGHT;
				case LEFT:
					return UP;
				case RIGHT:
					return DOWN;
			}
        case '/':
			switch(direction_actuelle){
				case UP:
					return RIGHT;
				case DOWN:
					return LEFT;
				case LEFT:
					return DOWN;
				case RIGHT:
					return UP;
			}

	}
	return -1;
}


// trouve le premier caractère (symbole) différent de '.' dans la direction indiquée par la beam 
// renvoie la ligne ou la colonne selon la condition de direction, car on connaît alors l'autre donnée
// renvoie -1 aux bordures 
int trouver_symbole(int direction, int ligne, int colonne, char** tableau_miroir, int nline, int tailleline){
    
    switch(direction){
		
        case UP: // on cherche le premier symbole au dessus 
			for(int i = ligne - 1; i >= 0; --i)
				if(tableau_miroir[i][colonne] != '.')
					return i;
			return -1;
		case DOWN: // on cherche le premier symbole en dessous
			for(int i = ligne + 1; i < nline; ++i)
				if(tableau_miroir[i][colonne] != '.')
					return i;
			return -1;
		case LEFT: // on cherche le premier symbole à gauche
			for(int i = colonne - 1; i >= 0; --i)
				if(tableau_miroir[ligne][i] != '.')
					return i;
			return -1;
		case RIGHT: // on cherche le premier symbole à droite
			for(int i = colonne + 1; i < tailleline; ++i)
				if(tableau_miroir[ligne][i] != '.')
					return i;
			return -1;
        
	}
	return -1;
}


//marque de (l1, c1) à (l2, c2) comme tiles avec de l'énergie 
// utilisée pour le faire entre deux différents symboles en traçant une ligne droite entre leurs deux coordonnées
void marquer_light(int l1, int c1, int l2, int c2, char** power_tiles, int nline, int tailleline){
	if (c1 == c2){
		for(int i = 0; i <= abs(l1 - l2); ++i){
			if(l1 > l2)
				power_tiles[i + l2][c1] = '#';
			else
				power_tiles[i + l1][c1] = '#';
		}
	}
	else {
		for(int i = 0; i <= abs(c1 - c2); ++i){
			if(c1 > c2)
				power_tiles[l1][i + c2] = '#';
			else
				power_tiles[l1][i + c1] = '#';
		}
	}
}


//calls marquer_light and handels splitting of beams and changing of directions
void beam_logic(struct beam input, int*** memo, char** tableau_miroir, char** power_tiles, int nline, int tailleline){
	
    // condition d'arret, de faire tourner récusrivement, lorsque l'on a visiter tout ce qui était possible de visiter
    if(memo[input.direction][input.ligne][input.colonne] == 1)
		return;
	memo[input.direction][input.ligne][input.colonne] = 1; // on marque qu'on est passé par là

	int new_cord = -1;
	int new_dir = -1;
	struct beam temp_a; // utilisé de partout 
	struct beam temp_b; // utilisé pour le split 

	new_cord = trouver_symbole(input.direction, input.ligne, input.colonne, tableau_miroir, nline, tailleline);

	while(1){
        // si le beam arrive sur une bordure de l'input
		if(new_cord == -1){
			switch(input.direction){
				case UP:
					marquer_light(input.ligne, input.colonne, 0, input.colonne, power_tiles, nline, tailleline);
					break;
				case DOWN:
					marquer_light(input.ligne, input.colonne, nline - 1, input.colonne, power_tiles, nline, tailleline);
					break;
				case RIGHT:
					marquer_light(input.ligne, input.colonne, input.ligne, tailleline - 1, power_tiles, nline, tailleline);
					break;
				case LEFT:
					marquer_light(input.ligne, input.colonne, input.ligne, 0, power_tiles, nline, tailleline);
					break;
			}
			return;
		}
        // on arrête si on croise un symbole, sinon on trouve l'autre symbole le plus proche 
        // un changement de direction signifie, un passage par un symbole 
		if (input.direction == UP || input.direction == DOWN){
			new_dir = trouver_direction(input.direction, new_cord, input.colonne, tableau_miroir, nline, tailleline);
			if(new_dir != input.direction)
				break;
			else
				new_cord = trouver_symbole(input.direction, new_cord, input.colonne, tableau_miroir, nline, tailleline);
		}
		else {
			new_dir = trouver_direction(input.direction, input.ligne, new_cord, tableau_miroir, nline, tailleline);
			if(new_dir != input.direction)
				break;
			else
				new_cord = trouver_symbole(input.direction, input.ligne, new_cord, tableau_miroir, nline, tailleline);
		}
	}

	switch(input.direction){ // on marque les lumières, en faisant des lignes droites cohérentes à la direction suivant 
		case UP:
			marquer_light(input.ligne, input.colonne, new_cord, input.colonne, power_tiles, nline, tailleline);
			break;
		case DOWN:
			marquer_light(input.ligne, input.colonne, new_cord, input.colonne, power_tiles, nline, tailleline);
			break;
		case RIGHT:
			marquer_light(input.ligne, input.colonne, input.ligne, new_cord, power_tiles, nline, tailleline);
			break;
		case LEFT:
			marquer_light(input.ligne, input.colonne, input.ligne, new_cord, power_tiles, nline, tailleline);
			break;
	}

	switch(new_dir){ // selon les cas, on fait le processus de récursivité
		case UP:
			temp_a.direction = UP;
			temp_a.ligne = input.ligne;
			temp_a.colonne = new_cord;
			beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
			break;
		case DOWN:
			temp_a.direction = DOWN;
			temp_a.ligne = input.ligne;
			temp_a.colonne = new_cord;
			beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
			break;
		case RIGHT:
			temp_a.direction = RIGHT;
			temp_a.ligne = new_cord;
			temp_a.colonne = input.colonne;
			beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
			break;
		case LEFT:
			temp_a.direction = LEFT;
			temp_a.ligne = new_cord;
			temp_a.colonne = input.colonne;
			beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
			break;
		case SPLIT: // plus compliqué, plus long mais revient au même, besoin d'un deuxième chemin temp_b
			if(input.direction == UP || input.direction == DOWN){
				temp_a.direction = RIGHT;
				temp_a.ligne = new_cord;
				temp_a.colonne = input.colonne;
				temp_b.direction = LEFT;
				temp_b.ligne = new_cord;
				temp_b.colonne = input.colonne;
				beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
				beam_logic(temp_b, memo, tableau_miroir, power_tiles, nline, tailleline);
			}
			else{
				temp_a.direction = UP;
				temp_a.ligne = input.ligne;
				temp_a.colonne = new_cord;
				temp_b.direction = DOWN;
				temp_b.ligne = input.ligne;
				temp_b.colonne = new_cord;
				beam_logic(temp_a, memo, tableau_miroir, power_tiles, nline, tailleline);
				beam_logic(temp_b, memo, tableau_miroir, power_tiles, nline, tailleline);
			}
			break;
	}
}

// compte le nombre de tiles avec de l'énergie
long int compter_power(char** power_tiles, int nline, int tailleline){
	long int nb_tiles_power=0;
	for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            if (power_tiles[i][j]=='#'){
                ++nb_tiles_power;
            }
        }
    }
	return(nb_tiles_power);
}

// remet à -1 le tableau mémo, pour dire qu'on est pas déjà passé par là
void reset_memo(int*** memo, int nline, int tailleline){
	for (int i=0; i<4; i++){
        for (int j=0; j<nline; j++){
            for (int k=0; k<tailleline; k++){
                memo[i][j][k]=-1;
            }
        }
    }
}

// remet à 0 le tableau des tiles avec de l'énergie
void reset_power(char** power_tiles, int nline, int tailleline){
	for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            power_tiles[i][j]='.';
        }
    }
}



int main(){

    FILE* file;
    file=fopen("input16.txt", "r");
    char ligne[200];


    // récupérer les dimensions de l'entrée 
    int nline=0;
    int tailleline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
        tailleline=strlen(ligne);
    }
    printf("\nil y a %d lignes à traiter, de longueur %d\n\n", nline, tailleline);


    // tableau mémo pour savoir si on a déjà regarder ce point de l'input et selon une telle direction
    // pour savoir si le cas à déjà été traité avant pour éviter de le refaire 
    // memo(direction)(ligne)(colonne)
    int*** memo=(int***)malloc(sizeof(int**)*4);
    for (int i=0; i<4; i++){
        memo[i]=(int**)malloc(sizeof(int*)*nline);
        for (int j=0; j<nline; j++){
            memo[i][j]=(int*)malloc(sizeof(int)*tailleline);
            for (int k=0; k<tailleline; k++){
                memo[i][j][k]=-1;
            }
        }
    }

    // tableau input des miroirs 
    printf("Tableau de l'input avec les miroirs :\n");
    char** tableau_miroir=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        tableau_miroir[i]=(char*)malloc(sizeof(char)*tailleline);
    }
    rewind(file);
    int p=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        strcpy(tableau_miroir[p],ligne);
        printf("%s", tableau_miroir[p]);
        ++p;
    }
    printf("\n\n");
    fclose(file);


    // tableau des tiles où la lumière est passée 
    char** power_tiles=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        power_tiles[i]=(char*)malloc(sizeof(char)*tailleline);
        for (int j=0; j<tailleline; j++){
            power_tiles[i][j]='.';
        }
    }


    // commencer par la case en haut à gauche 
    struct beam start_beam;
    start_beam.ligne=0;
    start_beam.colonne=0;
    start_beam.direction=RIGHT;

    // compter le max de # dans la map résultat selon l'entrée que l'on choisit
    long int nb_tiles_power=0;
    long int max_light=0;
	// départ du coté gauche, en direction de la droite  
	for (int i = 0; i < nline; i++){
		reset_memo(memo, nline, tailleline);
		reset_power(power_tiles, nline, tailleline);

		start_beam.direction = RIGHT;
		start_beam.ligne = i;
		start_beam.colonne = 0;

		start_beam.direction = trouver_direction(start_beam.direction, start_beam.ligne, start_beam.colonne, tableau_miroir, nline, tailleline);

		beam_logic(start_beam, memo, tableau_miroir, power_tiles, nline, tailleline);

		nb_tiles_power = compter_power(power_tiles, nline, tailleline);

		if (nb_tiles_power > max_light)
			max_light = nb_tiles_power;
	}


	// départ du coté droit, en direction de la gauche  
	for (int i = 0; i < nline; i++){
		reset_memo(memo, nline, tailleline);
		reset_power(power_tiles, nline, tailleline);

		start_beam.direction = LEFT;
		start_beam.ligne = i;
		start_beam.colonne = tailleline - 1;

		start_beam.direction = trouver_direction(start_beam.direction, start_beam.ligne, start_beam.colonne, tableau_miroir, nline, tailleline);

		beam_logic(start_beam, memo, tableau_miroir, power_tiles, nline, tailleline);

		nb_tiles_power = compter_power(power_tiles, nline, tailleline);

		if (nb_tiles_power > max_light)
			max_light = nb_tiles_power;
	}


	// départ du bas, en direction du haut  
	for (int i = 0; i < tailleline; i++){
		reset_memo(memo, nline, tailleline);
		reset_power(power_tiles, nline, tailleline);

		start_beam.direction = UP;
		start_beam.ligne = nline - 1;
		start_beam.colonne = i;

		start_beam.direction = trouver_direction(start_beam.direction, start_beam.ligne, start_beam.colonne, tableau_miroir, nline, tailleline);

		beam_logic(start_beam, memo, tableau_miroir, power_tiles, nline, tailleline);

		nb_tiles_power = compter_power(power_tiles, nline, tailleline);

		if (nb_tiles_power > max_light)
			max_light = nb_tiles_power;
	}


	// départ du haut, en direction du bas  
	for (int i = 0; i < tailleline; i++){
		reset_memo(memo, nline, tailleline);
		reset_power(power_tiles, nline, tailleline);

		start_beam.direction = DOWN;
		start_beam.ligne = 0;
		start_beam.colonne = i;

		start_beam.direction = trouver_direction(start_beam.direction, start_beam.ligne, start_beam.colonne, tableau_miroir, nline, tailleline);

		beam_logic(start_beam, memo, tableau_miroir, power_tiles, nline, tailleline);

		nb_tiles_power = compter_power(power_tiles, nline, tailleline);

		if (nb_tiles_power > max_light)
			max_light = nb_tiles_power;
	}
	

	// Afficher le résultat
	printf("\nLe résultat est : %ld\n\n", max_light);


	// libérer la mémoire
	for (int i=0; i<nline; i++){
		free(tableau_miroir[i]);
		free(power_tiles[i]);
	}
	free(tableau_miroir);
	free(power_tiles);

	for (int i=0; i<4; i++){
		for (int j=0; j<nline; j++){
			free(memo[i][j]);
		}
		free(memo[i]);
	}
	free(memo);

    return(0);
}

// trop long à exécuter, je n'ai pas réussi à le faire fonctionner