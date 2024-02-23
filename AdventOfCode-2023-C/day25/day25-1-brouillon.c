#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

// encore du parcours en largeur avec des noeuds 

struct node{
	int has_been;
	char *name;
	struct node *connections[2000];
	int con_id[2000];
	int weight[2000];
	char *con_name[2000];
	int con_count;
	int real_con_count;
};

struct path{
	struct node *node_1;
	struct node *node_2;
	int num;
};

//trouver un noeud dans node_arr par son nom 
struct node *find_node(char *name, struct node* node_arr, int node_num){
	for(int i = 0; i < node_num; ++i)
		if(!strcmp(node_arr[i].name, name))
			return &node_arr[i];

	//if a node is found only in connections it is added here
	node_arr[node_num].name = strdup(name);
	node_arr[node_num].con_count = 0;
	node_arr[node_num].has_been = 0;
	node_arr[node_num].real_con_count = 0;

	++node_num;

	return &node_arr[node_num - 1];
}

// ajoute les connections d'émission et de réception du noeud dans le path_arr
void add_con(struct node *np, int con_num, struct node* node_arr, int node_num, struct path* path_arr){
	struct node *cnp;

	for(int i = 0; i < np->con_count; ++i){
		cnp = find_node(np->con_name[i], node_arr, node_num);
		if(cnp != NULL){
			np->connections[np->real_con_count] = cnp;
			np->con_id[np->real_con_count] = con_num;
			np->real_con_count += 1;

			cnp->connections[cnp->real_con_count] = np;
			cnp->con_id[cnp->real_con_count] = con_num;
			cnp->real_con_count += 1;

			path_arr[con_num].node_1 = np;
			path_arr[con_num].node_2 = cnp;
			path_arr[con_num].num = 0;

			++con_num;
		}
	}
}

//trouve la plus petite distance entre a er b et ajoute son chemin dans path_arr
void bfs(struct node *a, struct node *b, struct node* node_arr, int node_num, struct path* path_arr){
	typedef struct{
		struct node *call_node;
		struct node *curr_node;
		int trav_id;
		int call_id;
	}open_node;

	open_node open_list[200000];

	int open_que = 1;
	int current_place = 0;
	struct node *curr_node = a;
	int back_track;

	open_list[0].curr_node = a;
	open_list[0].call_node = NULL;
	open_list[0].trav_id = -1;
	open_list[0].call_id = -1;

	for(int i = 0; i < node_num; ++i)
		node_arr[i].has_been = 0;

	while((curr_node = open_list[current_place].curr_node) != b){
		for(int i = 0; i < curr_node->real_con_count; ++i){
			if(curr_node->connections[i]->has_been == 0){
				open_list[open_que].call_node = curr_node;
				open_list[open_que].curr_node = curr_node->connections[i];
				open_list[open_que].trav_id = curr_node->con_id[i];
				open_list[open_que].call_id = current_place;
				curr_node->connections[i]->has_been = 1;
				++open_que;
			}
		}
		++current_place;

		if(current_place == open_que)
			return;
	}

	for(back_track = current_place; open_list[back_track].call_node != NULL; back_track = open_list[back_track].call_id)
		path_arr[open_list[back_track].trav_id].num += 1;

}

// trouve le nombre de noeuds pouvant être tiré de a 
int count_bfs(struct node *a, struct node* node_arr, int node_num, struct path* path_arr){
	typedef struct{
		struct node *call_node;
		struct node *curr_node;
		int trav_id;
		int call_id;
	}open_node;

	open_node open_list[200000];

	int open_que = 1;
	int current_place = 0;
	struct node *curr_node = a;

	open_list[0].curr_node = a;
	open_list[0].call_node = NULL;
	open_list[0].trav_id = -1;
	open_list[0].call_id = -1;

	for(int i = 0; i < node_num; ++i){
		node_arr[i].has_been = 0;
	}

	a->has_been = 1;

	while(current_place != open_que){
		curr_node = open_list[current_place].curr_node;

		for(int i = 0; i < curr_node->real_con_count; ++i){
			if(curr_node->connections[i]->has_been == 0 && path_arr[curr_node->con_id[i]].num != -1){
				open_list[open_que].call_node = curr_node;
				open_list[open_que].curr_node = curr_node->connections[i];
				open_list[open_que].trav_id = curr_node->con_id[i];
				open_list[open_que].call_id = current_place;
				curr_node->connections[i]->has_been = 1;
				++open_que;
				printf("%d %s %s\n", path_arr[curr_node->con_id[i]].num, curr_node->name,
				curr_node->connections[i]->name);
			}
		}
		++current_place;
	}

	return current_place;
}


int main(){

    //récupérer les données de l'input 
    FILE* file;
    file=fopen("input25.txt", "r");
    char ligne[100];
	char *token;;
	int biggest_ind[3] = {0, 0, 0};
	int store;
	int store_b;

    struct node* node_arr=(struct node*)malloc(sizeof(struct node)*2000);
    int node_num = 0;
    int con_num = 0;

    struct path* path_arr=(struct path*)malloc(sizeof(struct path)*2000);
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
		token = strtok(ligne, ": \n");
		node_arr[node_num].name = strdup(token);
		node_arr[node_num].con_count = 0;
		node_arr[node_num].has_been = 0;

		while((token = strtok(NULL, ": \n")) != NULL){
			node_arr[node_num].con_name[node_arr[node_num].con_count] = strdup(token);
			node_arr[node_num].con_count += 1;
			node_arr[node_num].real_con_count = 0;
		}

		++node_num;
	}
    fclose(file);

    //connecter tous les inputs
	for(int i = 0; i < node_num; ++i){
		add_con(&node_arr[i], con_num, node_arr, node_num, path_arr);
	}

	for(int i = 0; i < node_num; ++i){
		printf("%s : ", node_arr[i].name);
		for(int j = 0; j < node_arr[i].real_con_count; ++j){
			if(node_arr[i].connections[j] != NULL)
				printf("%s ", node_arr[i].connections[j]->name);
		}
		printf("\n");
	}

	//donne la plus courte distance entre chaque noeud
	for(int i = 0; i < node_num; ++i){
		for(int j = i + 1; j < node_num; ++j)
			bfs(&node_arr[i], &node_arr[j], node_arr, node_num, path_arr);
	}

	//donne les 3 trajets les plus parcourus
	for(int i = 0; i < con_num; ++i){
		printf("%s %s %d\n", path_arr[i].node_1->name, path_arr[i].node_2->name, path_arr[i].num);

		if(path_arr[i].num > path_arr[biggest_ind[0]].num){
			biggest_ind[2] = biggest_ind[1];
			biggest_ind[1] = biggest_ind[0];
			biggest_ind[0] = i;
		}
		else if(path_arr[i].num > path_arr[biggest_ind[1]].num){
			biggest_ind[2] = biggest_ind[1];
			biggest_ind[1] = i;
		}
		else if(path_arr[i].num > path_arr[biggest_ind[2]].num){
			biggest_ind[2] = i;
		}
	}

    //enlève les noeuds et donne la taille de chaque graphe disjoint
	printf("%d %d %d\n", path_arr[biggest_ind[0]].num, path_arr[biggest_ind[1]].num, path_arr[biggest_ind[2]].num);
	path_arr[biggest_ind[0]].num = -1;
	path_arr[biggest_ind[1]].num = -1;
	path_arr[biggest_ind[2]].num = -1;

	store = count_bfs(&node_arr[0], node_arr, node_num, path_arr);
	store_b = node_num - store;

	printf("%d %d %d\nfinal value: %d\n", store, store_b, node_num, store * store_b);


    free(node_arr);
    free(path_arr);

    return(0);
}

// marche pas …