#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// Extraire les chiffres d’une chaîne de caractères : utilitaire
char* extraireChiffres(const char* chaine) {
    char* chiffres = (char*)malloc(strlen(chaine) + 1);
   
    if (chiffres == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    for (int i = 0; chaine[i] != '\0'; i++) {
        if (isdigit((unsigned char)chaine[i])) {
            chiffres[j] = chaine[i];
            ++j;
        }
    }
    chiffres[j] = '\0';
    return chiffres;
}


// fonction de Hash
int Hash(char* s){
    int current_hash=0;
    int i=0;
    while (s[i] != '\0'){
        current_hash=((current_hash + s[i])*17) % 256;
        ++i;
    }
    return(current_hash);
}


// petite boite dans les boîtes 
struct slot {
    char* label;
    int focal_lenght; // =-1 si c'est vide 
};

#define NB_slot 1000
// une boîte parmi les 256
struct int_boxe{
    struct slot slot_array[NB_slot];
    int position;
};


// trouve l'index de label (chaîne de caractères) dans la boîte n°num_box de boxes 
// renvoi -1 si il n'y ait pas 
int trouve_label(int box_num, char *label, struct int_boxe *boxes, int nboxes){
	for(int i = 0; i < boxes[box_num].position; ++i){
		if(boxes[box_num].slot_array[i].focal_lenght != -1 && !strcmp(label, boxes[box_num].slot_array[i].label)){
			return i;
		}
	}
	return -1;
}


// ajouter le caractère label (et sa focale) dans la boîte n°num_box de boxes 
// remplacer si cela existe déjà 
void ajoute_label(int box_num, char *label, int lens_size, struct int_boxe *boxes, int nboxes){

	int slot_index = trouve_label(box_num, label, boxes, nboxes);

	if (slot_index != -1) {
        boxes[box_num].slot_array[slot_index].focal_lenght = lens_size;
    }
	else { // on remplit la case suivante 
		boxes[box_num].slot_array[boxes[box_num].position].focal_lenght = lens_size;
		boxes[box_num].slot_array[boxes[box_num].position].label = strdup(label);
		boxes[box_num].position += 1; //il y a une position de plus de prise 
	}
}


// ne fait rien si il n'y pas la boîte à enlever 
// enlève les autres boîtes sinon
void enleve_label(int box_num, char *label, struct int_boxe *boxes, int nboxes){

	int slot_index = trouve_label(box_num, label, boxes, nboxes);

	if(slot_index != -1)
		boxes[box_num].slot_array[slot_index].focal_lenght = -1;
}


#define NB_SAUT 213
// vaut 213 pour le nombre de sauts de ligne à considérer sur input15.txt
// vaut 0 pour le nombre de sauts de ligne à considérer sur text.txt

int main(){
    
    FILE* file;
    file=fopen("input15.txt", "r");

    int nstring=0;
    char ligne[200];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        int len=strlen(ligne);
        for (int i=0; i<len; i++){
            if (ligne[i]==','){
                ++nstring;
            }
        }
    }

    nstring=nstring+NB_SAUT;

    char** Tab=(char**)malloc(sizeof(char*)*nstring);

    rewind(file);
    char* delim=",";
    int k=0;

    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        char* token; 
        char *str = strdup(ligne); 
        token = strtok(str, delim);
        Tab[k]=malloc(sizeof(char)*strlen(token));
        strcpy(Tab[k],token);
        printf("%d : %s\n", k, Tab[k]);
        ++k;
        while (token != NULL && strcmp(token, "\n") != 0){
            token=strtok(NULL, delim);
            Tab[k]=malloc(sizeof(char)*strlen(token));
            strcpy(Tab[k],token);
            printf("%d : %s\n", k, Tab[k]);
            ++k;
        }

        free(str);
    }

    for (int i=0; i<nstring; i++){
        printf("Tab[%d] : %s\n", i, Tab[i]);
    }

    printf("\nIl y a %d chaînes de caractères à traiter\n\n", nstring);


    // boxes contient les 256 boîtes 
    // dans chaque boîte, il y a des boites possédant des labels et des longueurs de focales 
    // on met les focales à -1 et les positions à 0
    int nboxes=256;
    struct int_boxe *boxes=(struct int_boxe *)malloc(sizeof(struct int_boxe *)*nboxes);
    for (int i=0; i<nboxes; i++){
        for (int j=0; j<NB_slot; j++){
            boxes[i].slot_array[j].focal_lenght=-1;
        }
        boxes[i].position=0;
    }

    for (int i=0; i<nstring; i++){
        if (strcmp(Tab[i],"\n") != 0){

            if (strchr(Tab[i],'-') != NULL){

                char* label;
                char* copie= strdup(Tab[i]); 
                label=strtok(copie,"-");
                printf("'%s' ",label);

                int boxe_num=Hash(label);
                printf("à la lentille %d\n",boxe_num);

                enleve_label(boxe_num, label, boxes, nboxes);

                free(copie);
            }


            if (strchr(Tab[i],'=') != NULL){

                char* label;
                char* copie= strdup(Tab[i]); 
                label=strtok(copie,"=");
                int focal_lenght=atoi(extraireChiffres(Tab[i]));
                printf("'%s' (de focal %d) ",label, focal_lenght);

                int boxe_num=Hash(label);
                printf("à la lentille %d\n",boxe_num);

                ajoute_label(boxe_num, label, focal_lenght, boxes, nboxes);

                free(copie);
            }

        }
    }

    // boxes[1].slot_array[4].focal_lenght=5 : pour un test 

    long int sum=0;
    long int sum_inter=0;

    for (int i = 0; i < nboxes; i++){
		int emplacement_vide = 0;
		for(int k = 0; k < boxes[i].position; k++){
			if (boxes[i].slot_array[k].focal_lenght != -1){
				sum_inter = (i + 1) * (k + 1 - emplacement_vide) * boxes[i].slot_array[k].focal_lenght;
				sum += sum_inter;
			}
			else {
				++emplacement_vide;
            }
		}
	}

    printf("\nle résultat est : %ld\n\n", sum);


    free(boxes);


    for (int i=0; i<nstring; i++){
        free(Tab[i]);
    }
    free(Tab);

    fclose(file);
    return(0);
}

// en modifiant la ligne 100, on peut changer le fichier à lire et : 
// on trouve bien 145 pour le fichier test.txt