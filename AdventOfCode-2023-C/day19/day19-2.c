// structure des inputs que l'on va traiter 
#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// Utilitaires pour récupérer les données : 
// Fonction pour copier une chaîne sans le dernier caractère
char* copyStringWithoutLastCharacter(const char* original) {
    // Vérification de la longueur de la chaîne
    size_t length = strlen(original);
    if (length == 0) {
        // La chaîne est vide
        return NULL;
    }
    // Allocation de mémoire pour la nouvelle chaîne
    char* result = (char*)malloc(length);
    if (result == NULL) {
        // Échec de l'allocation de mémoire
        return NULL;
    }
    // Copie des caractères de la chaîne d'origine à l'exception du dernier caractère
    strncpy(result, original, length - 1);
    // Ajout du caractère nul de fin de chaîne
    result[length - 1] = '\0';
    return result;
}
// Fonction pour copier une chaîne sans le premier et le dernier caractère
char* copyStringWithoutEdges(const char* original) {
    // Vérification de la longueur de la chaîne
    size_t length = strlen(original);
    if (length <= 2) {
        // La chaîne est trop courte pour avoir un caractère à enlever des bords
        return NULL;
    }
    // Allocation de mémoire pour la nouvelle chaîne
    char* result = (char*)malloc(length - 1);
    if (result == NULL) {
        // Échec de l'allocation de mémoire
        return NULL;
    }
    // Copie des caractères appropriés dans la nouvelle chaîne
    strncpy(result, original + 1, length - 2);
    // Ajout du caractère nul de fin de chaîne
    result[length - 2] = '\0';
    return result;
}
// fonction pour garder que les chiffres dans une chaîne de caractères
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

// structure qui va créer le graphique
struct filter{
	// en prenant l'exemple de text, ligne 1 : px{a<2006:qkq,m>2090:A,rfg}

	char *node_name; // px 

	char first_type[2]; // a<
	int first_cond; // 2006
	struct filter *first_pointer; // qkq

	char second_type[2]; // m>
	int second_cond; // 2090
	struct filter *second_pointer; // A

	char third_type[2]; //nn
	int third_cond; //0
	struct filter *third_pointer; // rfg

	char fourth_type[2]; //nn
	int fourth_cond; //0
	struct filter *fourth_pointer; // 
};

// structure des inputs que l'on va traiter : 
// on mettra les minimus à 1 et les maximums à 4000
struct gear_ranges{
	long int x_max;
	long int x_min;
	long int m_max;
	long int m_min;
	long int a_max;
	long int a_min;
	long int s_max;
	long int s_min;
	long int prod;
};

//stores pointers to avoid creating the same structure twice
struct temp_store{
	char *filter_string;
	struct filter *filter_pointer;
};

// renvoi l'index de la chaîne de caractères (servant de nom de pointeurs) qui commence par 'name' dans 'filter_string_array'
int find_index(char *name, struct temp_store **filter_string_array, int filter_num){
    int name_index;
	for (name_index = 0; name_index < filter_num; ++name_index){
		for(int i = 0; (*(filter_string_array + name_index))->filter_string[i] == name[i]; ++i){
			if(name[i + 1] == '\0' && (*(filter_string_array + name_index))->filter_string[i + 1] == '{'){
				return name_index;
			}
		}
	}
	return name_index;
}

//retourne un pointeur vers filter avec son nom 
//if no filter is found also creates it and calls it's self on each condition pointer
struct filter *add_filter(char *name, struct filter* accept, struct filter* reject, struct temp_store **filter_string_array, int filter_num) {
	if(!strcmp(name, "A"))
		return accept;
	if(!strcmp(name, "R"))
		return reject;

	int ind_name = find_index(name, filter_string_array, filter_num);
	char *filter_line = NULL;
	char *pointer_loc = NULL;
	struct filter *fp = NULL;
	char temp_num_store[10];
	int arg_num = 0; // numéro de la condition à traiter
	int find_col;
	char *temp_int;
	char *temp_fit;

	if((*(filter_string_array + ind_name))->filter_pointer != NULL){
		return (*(filter_string_array + ind_name))->filter_pointer;
	}

	fp = (struct filter *) malloc(sizeof(struct filter));
	(*(filter_string_array + ind_name))->filter_pointer = fp;

	temp_int = strdup((*(filter_string_array + ind_name))->filter_string);

	filter_line = strtok_r(temp_int, " \n{},", &pointer_loc);

	fp->node_name = strdup(filter_line);

	while((filter_line = strtok_r(NULL, " \n{},", &pointer_loc)) != NULL){
		if(filter_line[1] != '<' && filter_line[1] != '>'){ // cas où il n'y a pas de conditions : ex "pv"
			switch(arg_num){ 
				case 0:
					temp_fit = strdup(filter_line);
					fp->first_pointer = add_filter(temp_fit, accept, reject, filter_string_array, filter_num);
					free(temp_fit);
					fp->first_type[0] = 'n';
					fp->first_type[1] = 'n';
					fp->first_cond = 0;
					break;
				case 1:
					temp_fit = strdup(filter_line);
					fp->second_pointer = add_filter(temp_fit, accept, reject, filter_string_array, filter_num);
					free(temp_fit);
					fp->second_type[0] = 'n';
					fp->second_type[1] = 'n';
					fp->second_cond = 0;
					break;
				case 2:
					temp_fit = strdup(filter_line);
					fp->third_pointer = add_filter(temp_fit, accept, reject, filter_string_array, filter_num);
					free(temp_fit);
					fp->third_type[0] = 'n';
					fp->third_type[1] = 'n';
					fp->third_cond = 0;
					break;
				case 3:
					temp_fit = strdup(filter_line);
					fp->fourth_pointer = add_filter(temp_fit, accept, reject, filter_string_array, filter_num);
					free(temp_fit);
					fp->fourth_type[0] = 'n';
					fp->fourth_type[1] = 'n';
					fp->fourth_cond = 0;
					break;
			}
		}
		else{ // si il y a une condition
			switch(arg_num){
				case 0:
					fp->first_type[0] = filter_line[0];
					fp->first_type[1] = filter_line[1];
					break;
				case 1:
					fp->second_type[0] = filter_line[0];
					fp->second_type[1] = filter_line[1];
					break;
				case 2:
					fp->third_type[0] = filter_line[0];
					fp->third_type[1] = filter_line[1];
					break;
				case 3:
					fp->fourth_type[0] = filter_line[0];
					fp->fourth_type[1] = filter_line[1];
					break;
			}

			for(int i = 2; filter_line[i] != ':'; ++i){
				temp_num_store[i - 2] = filter_line[i];
				temp_num_store[i - 1] = '\0';
			}

			switch(arg_num){
				case 0:
					fp->first_cond = atoi(temp_num_store);
					break;
				case 1:
					fp->second_cond = atoi(temp_num_store);
					break;
				case 2:
					fp->third_cond = atoi(temp_num_store);
					break;
				case 3:
					fp->fourth_cond = atoi(temp_num_store);
					break;
			}
			for(find_col = 0; filter_line[find_col] != ':'; ++find_col);
			switch(arg_num){
				case 0:
					fp->first_pointer = add_filter(filter_line + find_col + 1, accept, reject, filter_string_array, filter_num);
					break;
				case 1:
					fp->second_pointer = add_filter(filter_line + find_col + 1, accept, reject, filter_string_array, filter_num);
					break;
				case 2:
					fp->third_pointer = add_filter(filter_line + find_col + 1, accept, reject, filter_string_array, filter_num);
					break;
				case 3:
					fp->fourth_pointer = add_filter(filter_line + find_col + 1, accept, reject, filter_string_array, filter_num);
					break;
			}
		}
		++arg_num;
	}

	free(temp_int);

	return fp;
}

// copy la gear2 dans la gear1, en passage par arguments
void copy_ranges(struct gear_ranges *gear_range_1, struct gear_ranges *gear_range_2){
	gear_range_1->a_max = gear_range_2->a_max;
	gear_range_1->a_min = gear_range_2->a_min;

	gear_range_1->m_max = gear_range_2->m_max;
	gear_range_1->m_min = gear_range_2->m_min;

	gear_range_1->s_max = gear_range_2->s_max;
	gear_range_1->s_min = gear_range_2->s_min;

	gear_range_1->x_max = gear_range_2->x_max;
	gear_range_1->x_min = gear_range_2->x_min;

	gear_range_1->prod = gear_range_2->prod;
}

// donne le produit de tous les ranges des gears 
void get_gear_prod(struct gear_ranges *gp){
	long int x_range;
	long int m_range;
	long int a_range;
	long int s_range;

	x_range = gp->x_max - gp->x_min + 1;
	m_range = gp->m_max - gp->m_min + 1;
	a_range = gp->a_max - gp->a_min + 1;
	s_range = gp->s_max - gp->s_min + 1;

	if(x_range < 0 || m_range < 0 || a_range < 0 || s_range < 0)
		return;

	gp->prod = x_range * m_range * a_range * s_range;
}

//modifies gp pour être les rangs qui vont jusqu'au compte des arguments allant de 1 à 4 (de 0 à 3 pour la fonction)
//ex. pour arg_count 1 in{x>20:a,r} devrait modifer x_min = 21 et arg_count 2 devrait faire x_max = 20 : on modifie gp en même temps par le passage par adresse
//returns le nombre d'arguments si il y a eu une comparaison dans le processus
int get_range(struct filter *fp, struct gear_ranges *gp, int arg_count)
{
	int cond;
	char *type;
	if(fp == NULL)
		return 0;

	for(int i = 0; i < arg_count; ++i){
		switch(i){
			case 0:
				cond = fp->first_cond;
				type = fp->first_type;
				break;
			case 1:
				cond = fp->second_cond;
				type = fp->second_type;
				break;
			case 2:
				cond = fp->third_cond;
				type = fp->third_type;
				break;
			case 3:
				cond = fp->fourth_cond;
				type = fp->fourth_type;
				break;
		}
		switch(type[0]){
			case 'n': // c'est le cas que l'on a rempli pour dire qu'il y avair rien
				return i + 1;
			case 'x':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->x_min = cond + 1;
					}
					else{
						gp->x_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->x_max = cond - 1;
					}
					else{
						gp->x_min = cond;
					}
				}
				break;
			case 'm':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->m_min = cond + 1;
					}
					else{
						gp->m_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->m_max = cond - 1;
					}
					else{
						gp->m_min = cond;
					}
				}
				break;
			case 'a':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->a_min = cond + 1;
					}
					else{
						gp->a_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->a_max = cond - 1;
					}
					else{
						gp->a_min = cond;
					}
				}
				break;
			case 's':
				if(type[1] == '>'){
					if(i + 1 == arg_count){
						gp->s_min = cond + 1;
					}
					else{
						gp->s_max = cond;
					}
				}
				else{
					if(i + 1 == arg_count){
						gp->s_max = cond - 1;
					}
					else{
						gp->s_min = cond;
					}
				}
				break;
		}
	}

	return arg_count;
}

//prends le nombre de gears qui finissent en A de input_gear_range et les mets dans accepted_gear_arr
void get_acc_gear_ranges(struct filter *input_filter, struct gear_ranges input_gear_range, struct filter *accept, struct filter *reject, struct gear_ranges* accepted_gear_arr, int *accepted_gear_num){

	struct gear_ranges output_1;
	struct gear_ranges output_2;
	struct gear_ranges output_3;
	struct gear_ranges output_4;

	if(input_filter == reject)
		return;

	if(input_filter == accept){
		copy_ranges(&accepted_gear_arr[*accepted_gear_num], &input_gear_range);
		++*accepted_gear_num;
		return;
	}

	copy_ranges(&output_1, &input_gear_range);
	copy_ranges(&output_2, &input_gear_range);
	copy_ranges(&output_3, &input_gear_range);
	copy_ranges(&output_4, &input_gear_range);

	if(get_range(input_filter, &output_1, 1) == 1){
		get_acc_gear_ranges(input_filter->first_pointer, output_1, accept, reject, accepted_gear_arr, accepted_gear_num);
	}

	if(get_range(input_filter, &output_2, 2) == 2){
		get_acc_gear_ranges(input_filter->second_pointer, output_2, accept, reject, accepted_gear_arr, accepted_gear_num);
	}

	if(get_range(input_filter, &output_3, 3) == 3){
		get_acc_gear_ranges(input_filter->third_pointer, output_3, accept, reject, accepted_gear_arr, accepted_gear_num);
	}

	if(get_range(input_filter, &output_4, 4) == 4){
		get_acc_gear_ranges(input_filter->fourth_pointer, output_4, accept, reject, accepted_gear_arr, accepted_gear_num);
	}
}

//affiche le nombre spécifié de tabulations 
void tab_print(int tab_num)
{
	for(int i = 0; i < tab_num; ++i)
		printf("\t");
}

//affiche l'arbre des noeuds selon le nombre de tabulations et tous les noeuds liés à celui là  
void print_tree(struct filter *input, int tab_num, struct filter* accept, struct filter* reject){

	if(input == accept){
		//tab_print(tab_num);
		printf("A\n");
		return;
	}
	if(input == reject){
		//tab_print(tab_num);
		printf("R\n");
		return;
	}
	if(input == NULL){
		return;
	}

	printf("%s{", input->node_name);

	if(input->first_pointer != NULL){
		if(input->first_type[0] == 'n'){
			printf("%s", input->first_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->first_type[0], input->first_type[1], input->first_cond);
			printf("%s", input->first_pointer->node_name);
		}
	}
	if(input->second_pointer != NULL){
		printf(",");
		if(input->second_type[0] == 'n'){
			printf("%s", input->second_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->second_type[0], input->second_type[1], input->second_cond);
			printf("%s", input->second_pointer->node_name);
		}
	}
	if(input->third_pointer != NULL){
		printf(",");
		if(input->third_type[0] == 'n'){
			printf("%s", input->third_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->third_type[0], input->third_type[1], input->third_cond);
			printf("%s", input->third_pointer->node_name);
		}
	}
	if(input->fourth_pointer != NULL){
		printf(",");
		if(input->fourth_type[0] == 'n'){
			printf("%s", input->fourth_pointer->node_name);
		}
		else{
			printf("%c%c%d:", input->fourth_type[0], input->fourth_type[1], input->fourth_cond);
			printf("%s", input->fourth_pointer->node_name);
		}
	}

	printf("}\n");

	if(input->first_pointer != NULL){
		tab_print(tab_num);
		printf("1 ");
		print_tree(input->first_pointer, tab_num + 1, accept, reject);
	}
	if(input->second_pointer != NULL){
		tab_print(tab_num);
		printf("2 ");
		print_tree(input->second_pointer, tab_num + 1, accept, reject);
	}
	if(input->third_pointer != NULL){
		tab_print(tab_num);
		printf("3 ");
		print_tree(input->third_pointer, tab_num + 1, accept, reject);
	}
	if(input->fourth_pointer != NULL){
		tab_print(tab_num);
		printf("4 ");
		print_tree(input->fourth_pointer, tab_num + 1, accept, reject);
	}
}


int main(void) {
	FILE* file;
    file=fopen("input19.txt", "r");

	 char ligne[50];
    // Récupérer les dimensions de l'entrée 
    int nline=0;
    int indice_ligne_vide=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        if (strcmp(ligne, "\n") == 0){
            indice_ligne_vide=nline;
        }
        ++nline;
    }
    printf("\nil y a %d lignes à traiter\n", nline);
    printf("la ligne vide est à la position : %d\n\n", indice_ligne_vide);
    // créer de quoi stocker les lignes
    char** lines=(char**)malloc(sizeof(char*)*nline-1);
    for (int i=0; i<nline; i++){
        lines[i]=(char*)malloc(sizeof(char)*50);
    }
    // récupérer les lignes 
    rewind(file);
    int i=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        strcpy(lines[i], ligne);
        ++i;
    }
    fclose(file);
    printf("\n\naffichage de l'input : \n\n");
    for (int i=0; i<nline; i++){
        printf("%s\n",lines[i]);
    }
	printf("\n\n");

  
    struct filter *in_filter;
    struct temp_store **filter_string_array;
    int nb_workflows=indice_ligne_vide;

    struct gear_ranges *accepted_gear_arr=(struct gear_ranges *)malloc(sizeof(struct gear_ranges)*1000);
    int accepted_gear_num = 0;
	long int total_gear_ranges = 0;

    struct gear_ranges st_range;
	st_range.x_min = 1;
	st_range.x_max = 4000;
	st_range.m_min = 1;
	st_range.m_max = 4000;
	st_range.a_min = 1;
	st_range.a_max = 4000;
	st_range.s_min = 1;
	st_range.s_max = 4000;

	struct filter *accept;
    struct filter *reject;
	accept = (struct filter *) malloc(sizeof(struct filter));
	reject = (struct filter *) malloc(sizeof(struct filter));
	accept->first_type[0] = 'a';
	reject->first_type[0] = 'r';
	accept->node_name = strdup("A");
	reject->node_name = strdup("R");

	// stocker les filters avant de les mettre dans l'arbre 
	filter_string_array = (struct temp_store **) malloc(sizeof(struct temp_store) * 1000);
	
	// obtenir tous les filters 
	for (int filter_num=0; filter_num<nb_workflows; filter_num++){
		*(filter_string_array + filter_num) = (struct temp_store *) malloc(sizeof(struct temp_store));
		(*(filter_string_array + filter_num))->filter_string = strdup(lines[filter_num]);
		(*(filter_string_array + filter_num))->filter_pointer = NULL;
	}

	printf("\nlignes de filtres stockées dans les arrays : \n\n");
	for(int i = 0; i < nb_workflows; ++i){
		printf("%s", (*(filter_string_array + i))->filter_string);
	}

	in_filter = add_filter("in", accept, reject, filter_string_array, nb_workflows);
	printf("\n\n\ntree des possibilités:\n\n");
	print_tree(in_filter, 0, accept, reject);

	printf("\n\n\n");
	printf("valeurs de gears stockées dans les arrays\n\n");
	printf("\n");

    get_acc_gear_ranges(in_filter, st_range, accept, reject, accepted_gear_arr, &accepted_gear_num);

	for(int i = 0; i < accepted_gear_num; ++i){
		printf("gears acceptées : x %ld %ld m %ld %ld a %ld %ld s %ld %ld\n", accepted_gear_arr[i].x_min, accepted_gear_arr[i].x_max,
		accepted_gear_arr[i].m_min, accepted_gear_arr[i].m_max, accepted_gear_arr[i].a_min, accepted_gear_arr[i].a_max,
		accepted_gear_arr[i].s_min, accepted_gear_arr[i].s_max);
		get_gear_prod(&accepted_gear_arr[i]);
	}

	for(int i = 0; i < accepted_gear_num; ++i)
		total_gear_ranges += accepted_gear_arr[i].prod;
	
    
    //jsp pq mais ça prends pas en compte la denrière à chaque fois 
	// ici ce nombre marchera que pour mon input :
	

	printf("\nLe résultat est : %ld\n\n", total_gear_ranges);


	for(int i = 0; i < nb_workflows; ++i){
		free(*(filter_string_array + i));
	}
	free(filter_string_array);
	free(accepted_gear_arr);
    return(0);
} 

// cela donne bien 167409079868000 pour test.txt