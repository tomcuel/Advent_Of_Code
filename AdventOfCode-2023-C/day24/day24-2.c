#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define SEARCH_MIN -300
#define SEARCH_MAX 300
# define PRECISION 0.0000001

// on ne s'occupe pas de l'axe z dans un premier temps 
struct vect {
    long double start_x;
    long double start_y; 
    long double start_z;

    long double x_vel;
    long double y_vel;
    long double z_vel;
    
};

enum type{
	X_AND_Y,
	X_AND_Z,
	Y_AND_Z
};

//enlève des valeurs à la vitesse pour un tel vecteur 
void vec_mod(struct vect *vp, int x, int y, int z){
	vp->x_vel -= (long double) x;
	vp->y_vel -= (long double) y;
	vp->z_vel -= (long double) z;
}

//copie vp2 dans vp1
void vec_cpy(struct vect *vp1, struct vect *vp2){
	vp1->start_x = vp2->start_x;
	vp1->start_y = vp2->start_y;
	vp1->start_z = vp2->start_z;
	vp1->x_vel = vp2->x_vel;
	vp1->y_vel = vp2->y_vel;
	vp1->z_vel = vp2->z_vel;
}

// est ce que les coordoonées sont les mêmes à un epsilon près
int does_match(long double *cord_1, long double *cord_2)
{
	for(int i = 0; i < 3; ++i)
		if(!(fabs(cord_1[i] - cord_2[i]) < 1)){
			printf("hi  %Lf %Lf\n", cord_1[i], cord_2[i]);
			return 0;
		}
	return 1;
}

// renvoie 1 si c'est bon, 0 sinon 
// et modifie sc_1 en le point d'intersection en abscisse
// sc_2 deveient alors l'ordonnée 
int get_scale(struct vect *vp1, struct vect *vp2, long double *sc_1, long double *sc_2, int mode) {
	long double e1;
	long double e2;
	long double a1;
	long double a2;
	long double d1;
	long double d2;
	long double b1;
	long double b2;
	long double c1;
	long double c2;
	long double f1;
	long double f2;

	switch(mode){
		case X_AND_Y:
			e1 = vp1->y_vel;
			e2 = vp2->y_vel;
			a1 = vp1->start_x;
			a2 = vp2->start_x;
			d1 = vp1->x_vel;
			d2 = vp2->x_vel;
			b1 = vp1->start_y;
			b2 = vp2->start_y;
			c1 = vp1->start_z;
			c2 = vp2->start_z;
			f1 = vp1->z_vel;
			f2 = vp2->z_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));

			break;

		case X_AND_Z:
			e1 = vp1->z_vel;
			e2 = vp2->z_vel;
			a1 = vp1->start_x;
			a2 = vp2->start_x;
			d1 = vp1->x_vel;
			d2 = vp2->x_vel;
			b1 = vp1->start_z;
			b2 = vp2->start_z;
			c1 = vp1->start_y;
			c2 = vp2->start_y;
			f1 = vp1->y_vel;
			f2 = vp2->y_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));
			break;
		case Y_AND_Z:
			e1 = vp1->z_vel;
			e2 = vp2->z_vel;
			a1 = vp1->start_y;
			a2 = vp2->start_y;
			d1 = vp1->y_vel;
			d2 = vp2->y_vel;
			b1 = vp1->start_z;
			b2 = vp2->start_z;
			c1 = vp1->start_x;
			c2 = vp2->start_x;
			f1 = vp1->x_vel;
			f2 = vp2->x_vel;

			*sc_1 = (((e2 * a1) - (e2 * a2) - (d2 * b1) + (d2 * b2)) / ((d2 * e1) - (e2 * d1)));
			break;
	}

	*sc_2 = (a1 + (d1 * (*sc_1)) - a2) / d2;

	if(fabs(c1 - (c2 + (f2 * (*sc_2)) - (f1 * (*sc_1)))) < 1)
		return 1;
	return 0;
}

// renvoie 1 si les vp1 et vp2 possèdent une intersection, ie les coodonnées sont les mêmes, en modiifant les coordonées dans ce cas là
int does_inter(struct vect *vp1, struct vect *vp2, long double *scale_1, long double *scale_2){
	long double pot_scale_1;
	long double pot_scale_2;
	int is_good;

	if(fabs((vp2->x_vel * vp1->y_vel) - (vp2->y_vel * vp1->x_vel)) < PRECISION){
		is_good = get_scale(vp1, vp2, &pot_scale_1, &pot_scale_2, X_AND_Z);
	}
	else
		is_good = get_scale(vp1, vp2, &pot_scale_1, &pot_scale_2, X_AND_Y);

	if(pot_scale_1 < 0 || pot_scale_2 < 0 || is_good != 1)
		return 0;

	*scale_1 = pot_scale_1;
	*scale_2 = pot_scale_2;
	return 1;
}

// avoir la sortie, on modifie vec  
void do_vec(struct vect in_vec, long double sca, long double *cord_arr){
	cord_arr[0] = in_vec.start_x + (in_vec.x_vel * sca);
	cord_arr[1] = in_vec.start_y + (in_vec.y_vel * sca);
	cord_arr[2] = in_vec.start_z + (in_vec.z_vel * sca);
}

int main (){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input24.txt", "r");
    char ligne[100];

    // Récupérer les dimensions de l'entrée 
    int nline=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ++nline;
    }
    printf("\nil y a %d lignes à traiter\n\n", nline);

    // on récupère toutes les données 
    rewind(file);
    struct vect* vects=(struct vect*)malloc(sizeof(struct vect)*nline);
    int k=0;
    char* token;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        token = strtok(ligne, " @,\n");
		vects[k].start_x = atof(token);

		token = strtok(NULL, " @,\n");
		vects[k].start_y = atof(token);

		token = strtok(NULL, " @,\n");
		vects[k].start_z = atof(token);

		token = strtok(NULL, " @,\n");
		vects[k].x_vel = atof(token);

		token = strtok(NULL, " @,\n");
		vects[k].y_vel = atof(token);

        token = strtok(NULL, " @,\n");
		vects[k].z_vel= atof(token);

		printf("position de départ (%Lf,%Lf,%Lf) et vitesse (%Lf,%Lf,%Lf)\n\n", vects[k].start_x, vects[k].start_y,vects[k].start_z,vects[k].x_vel,vects[k].y_vel,vects[k].z_vel);
        ++k;
    }
    fclose(file);

    struct vect mod_1;
	struct vect mod_2;
	long double pot_scale_1;
	long double pot_scale_2;
	long double new_cord[3];
	long double old_cord[3];

   //cherche toute modification de vecteurs entre search_min et search_max
	for(int i = SEARCH_MIN; i < SEARCH_MAX; ++i){
		for(int j = SEARCH_MIN; j < SEARCH_MAX; ++j){
			for(int k = SEARCH_MIN; k < SEARCH_MAX; ++k){
				vec_cpy(&mod_1, &vects[0]);
				vec_cpy(&mod_2, &vects[1]);

				vec_mod(&mod_1, i, j, k);
				vec_mod(&mod_2, i, j, k);

				//si deux intersections sont trouvées, on regarde le reste 
				if(does_inter(&mod_1, &mod_2, &pot_scale_1, &pot_scale_2)){
					do_vec(mod_2, pot_scale_2, old_cord);
					for(int h = 2; h < nline; ++h){
						vec_cpy(&mod_1, &vects[h]);

						vec_mod(&mod_1, i, j, k);

						does_inter(&mod_1, &mod_2, &pot_scale_1, &pot_scale_2);

						do_vec(mod_1, pot_scale_1, new_cord);

						if(h == nline - 1 && does_match(old_cord, new_cord)){
							printf("starting point: %Lf %Lf %Lf\nangle thrown: %d %d %d\nsum: %Lf\n",new_cord[0], new_cord[1], new_cord[2], i, j, k, new_cord[0] + new_cord[1] + new_cord[2]);
							exit(0);
						}
					}
				}
			}
		}
	}

    free(vects);
    return(0);
}

// prendre la partie entière du résultat 