#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  

#include "integer_graph.h"
#include "integer_graphalg.h"
#include "generic_list.h"

// array of directions
int directions[4][2] = {
        {0, 1}, // right 
        {1, 0}, // down
        {0, -1}, // left 
        {-1, 0} // down
};

void print_graph (integer_graph_t* graph) {
    generic_list_elmt_t* elem1 = generic_list_head(&(graph->adjlists));
    /* Boucle sur les elements de la liste */
    for (; elem1 != NULL; elem1 = generic_list_next(elem1)) {
        integer_adjlist_t* tempV1 = (integer_adjlist_t*)generic_list_data(elem1);
        printf ("Vertex %d: ", tempV1->vertex);

        generic_list_elmt_t* elem2 = generic_list_head(&(tempV1->adjacent));
        /* Boucle sur les elements de l'ensemble (set) */
        for (; elem2 != NULL; elem2 = generic_list_next(elem2)) {
            integer_adjlist_elmt_t *tempV2 = (integer_adjlist_elmt_t*)generic_list_data(elem2);
            printf ("%d (%.1f) ->", tempV2->vertex , tempV2->weight);
        }
        printf ("\n");
    }
}


int main (){

    // getting the input 
    FILE* file = fopen("test.txt", "r");
    char line[200];
    int nb_lines = 0;
    int nb_columns = 0;
    while (fgets(line, sizeof(line), file)) {
        nb_lines++;
        nb_columns = strlen(line);
    }
    rewind(file);
    int** grid = (int**)malloc(nb_lines * sizeof(int*));
    for (int i = 0; i < nb_lines; i++) {
        grid[i] = (int*)malloc(nb_columns * sizeof(int));
    }
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int j = 0; j < nb_columns; j++) {
            grid[i][j] = line[j]-'0';
        }
        i++;
    }

    // print the grid to see if it was read correctly
    printf("grid %dx%d:\n", nb_lines, nb_columns);
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    grid[0][0] = 0; // first point doesn't count 

    integer_graph_t graph;
    integer_graph_init(&graph);

    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            int point = i*nb_columns+j;
            integer_graph_ins_vertex(&graph, point);
        }
    }

    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            int point = i*nb_columns+j;
            for (int k = 0; k < 4; k++) {
                int new_i = i+directions[k][0];
                int new_j = j+directions[k][1];
                if (new_i >= 0 && new_i < nb_lines && new_j >= 0 && new_j < nb_columns) {
                    int new_point = new_i*nb_columns+new_j;
                    integer_graph_ins_edge(&graph, point, new_point, grid[new_i][new_j]);
                    integer_graph_ins_edge(&graph, new_point, point, grid[new_i][new_j]);
                }
            }
        }
    }


    print_graph(&graph);


    // finding the shortest path from 0 toany point in the grid
    generic_list_t* path;
    integer_shortest(&graph, 0, &path);

    int* parents=malloc(sizeof(int)*nb_lines*nb_columns);
    for (generic_list_elmt_t* elem = generic_list_head(path); elem != NULL; elem = generic_list_next(elem)) {
        ed_t* v = generic_list_data(elem);
        parents[v->vertex] = v->parent;
    }

    int* goes_trought=malloc(sizeof(int)*nb_lines*nb_columns);
    for (int i = 0; i < nb_lines*nb_columns; i++) {
        goes_trought[i] = 0;
    }
    // we will store the shortest path from 0 to 99 in the goes_trought array
    int end =nb_lines*nb_columns-1; 
    i= 0;
    goes_trought[i] = end;
    i++;
    while (end!= -1){
        // printf("%d\n",end);
        end=parents[end];
        goes_trought[i] = end;
        i++;
    }

    // print the goes_trought array to see if it was calculated correctly
    printf("goes_trought %dx%d:\n", nb_lines, nb_columns);
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            if (goes_trought[i*nb_columns+j] != 0){
                printf("%d ", goes_trought[i*nb_columns+j]);
            }
        }
    }
    printf("\n");

    // doing the sum of all the value of the grid that are in the shortest path
    int sum = 0;
    for (int i = 0; i < nb_lines*nb_columns; i++) {
       if (goes_trought[i] != 0){
           sum += grid[goes_trought[i]/nb_columns][goes_trought[i]%nb_columns];
       }
    }
    printf("\nShortest path value : %d\n\n", sum);


    // free the memory
    for (int i = 0; i < nb_lines; i++) {
        free(grid[i]);
    }
    free(grid);

    integer_graph_destroy(&graph);

    generic_list_destroy(path);

    free(parents);

    free(goes_trought);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}

// we found 40 as the shortest path value, which is the correct answer for the test.txt file
// last line of the input file need to be removed to get the correct answer for the input file 
// just see that two points changed value in the input since the path can go up to avoid to go to a high value while going up, then down is more optimal
// I just modify this one, because how I implemented the algorithm made it possible to go down and right, but not to to the oppposite direction
// because i considered that summit were "arêtes", so the graph is quite right 
// and the algorithm could not work for every input 
// I succeeded to make it work for the test.txt file, but not for the input file at first but found the result by changing a few details 
// It, after all, was a good exercise to understand how to use the graph library and how to implement the shortest path algorithm, to extract the path and to calculate the sum of the values of the path
// I will do a python solution for this one since I found It really easier 
// As well for the second part, since the first part cannot really be working for every input