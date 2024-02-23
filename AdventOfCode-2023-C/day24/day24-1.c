#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

# define PRECISION 0.0000001

// on ne s'occupe pas de l'axe z dans un premier temps 
struct data {
    long double x_start;
    long double y_start; 

    long double pente; //(dy divisé par dx), coefficent directeur de la droite 
    long double décalage; //(y-pente*x) // ordonnée à l'origine 

    long double vx;
    long double vy;
    
};

//regarder si le point est dans le passé pour les coordonées 
int check_past(long double x, long double y, struct data *eqp){
	if(x > eqp->x_start && eqp->vx < 0)
		return 0;
	if(x < eqp->x_start && eqp->vx > 0)
		return 0;
	if(y > eqp->y_start && eqp->vy < 0)
		return 0;
	if(y < eqp->y_start && eqp->vy > 0)
		return 0;
	return 1;
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
    struct data* datas=(struct data*)malloc(sizeof(struct data)*nline);
    int k=0;
    char* token;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        token = strtok(ligne, " @,\n");
		datas[k].x_start = atof(token);

		token = strtok(NULL, " @,\n");
		datas[k].y_start = atof(token);

		token = strtok(NULL, " @,\n");
		token = strtok(NULL, " @,\n");
		datas[k].vx = atof(token);

		token = strtok(NULL, " @,\n");
		datas[k].vy = atof(token);

        datas[k].pente=datas[k].vy/datas[k].vx;

        datas[k].décalage=datas[k].y_start-datas[k].pente*datas[k].x_start;

		printf("position de départ (%Lf,%Lf) et vitesse (%Lf,%Lf)\n", datas[k].x_start, datas[k].y_start,datas[k].vx,datas[k].vy);
		printf("de pente %Lf et de décalage %Lf\n", datas[k].pente, datas[k].décalage);
        printf("\n");
        ++k;
    }
    fclose(file);

    long double MAX = 400000000000000.0;
    long double MIN = 200000000000000.0;

    long int nombre_intersections=0;
    for (int i=0; i<nline; i++){
        for (int j=i+1; j<nline; j++){
            // droites parrallèles 
            if(fabs(datas[i].pente - datas[j].pente) < 0.0000000001){
				if(fabs(datas[i].décalage - datas[j].décalage) < 0.001){
					++nombre_intersections;
				}
				continue;
			}

            // abscisse point d'intersection 
			long double temp_x = (datas[i].décalage - datas[j].décalage) / (datas[j].pente - datas[i].pente);
            // ordonnée car droite affine
			long double temp_y = (datas[i].pente * temp_x) + datas[i].décalage;

			if(MIN <= temp_x && MAX >= temp_x && MIN <= temp_y && MAX >= temp_y && check_past(temp_x, temp_y, &datas[i]) && check_past(temp_x, temp_y, &datas[j])){
				++nombre_intersections;
            }

        }
    }


    printf("\nle résultat est : %ld\n\n",nombre_intersections);


    free(datas);
    return(0);
}
