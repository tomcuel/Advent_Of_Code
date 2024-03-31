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
#include "integer_set.h"

#define MAX_STRING_LENGTH 100


void print_graph (integer_graph_t* graph) {
  generic_list_elmt_t* elem1 =
    generic_list_head(&(graph->adjlists));
  /* Boucle sur les elements de la liste */
  for (; elem1 != NULL; elem1 = generic_list_next(elem1)) {
    integer_adjlist_t* tempV1 =
      (integer_adjlist_t*)generic_list_data(elem1);
    printf ("Vertex %d: ", tempV1->vertex);

    generic_list_elmt_t* elem2 =
      generic_list_head(&(tempV1->adjacent));
    /* Boucle sur les elements de l'ensemble (set) */
    for (; elem2 != NULL; elem2 = generic_list_next(elem2)) {
      integer_adjlist_elmt_t *tempV2 =
	(integer_adjlist_elmt_t*)generic_list_data(elem2);
      printf ("%d->", tempV2->vertex);
    }
    printf ("\n");
  }
}


// Function to count the number of unique strings in a file
int count_unique_strings(const char* filename, int nb_lines) {
    char unique_strings[nb_lines][MAX_STRING_LENGTH];
    int unique_count = 0;

    // Open the file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open file %s\n", filename);
        return -1;
    }

    // Read each line of the file
    char line[MAX_STRING_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Tokenize the line to extract strings
        char* token = strtok(line, "-\n");
        while (token != NULL) {
            // Check if the string is already in the unique_strings array
            int found = 0;
            for (int i = 0; i < unique_count; i++) {
                if (strcmp(unique_strings[i], token) == 0) {
                    found = 1;
                    break;
                }
            }
            // If not found, add it to the unique_strings array
            if (!found) {
                strncpy(unique_strings[unique_count], token, MAX_STRING_LENGTH);
                unique_count++;
            }
            // Move to the next token
            token = strtok(NULL, "-\n");
        }
    }

    // Close the file
    fclose(file);

    return unique_count;
}


// function to know if a string is in an array of strings
bool is_in_array(char** array, int size, char* string){
    for (int i = 0; i < size; i++){
        if (strcmp(array[i], string) == 0){
            return true;
        }
    }
    return false;
}


// function that get the posistion of a character in a string
int get_pos(char* string, char c){
    for (int i = 0; i < strlen(string); i++){
        if (string[i] == c){
            return i;
        }
    }
    return -1;
}


// Function to check if a string is lowercase
int is_lowercase(const char* string) {
    // Iterate through each character of the string
    while (*string != '\0') {
        // Check if the character is not a lowercase letter
        if (!islower(*string)) {
            return 0; // The string contains at least one character that is not a lowercase letter
        }
        string++; // Move to the next character
    }
    return 1; // All characters in the string are lowercase letters
}


// Function to get the index of a string in a list of strings
int get_string_index(char** string_list, int list_size, char* target) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(string_list[i], target) == 0) {
            return i; // Return the index if a match is found
        }
    }
    return -1; // Return -1 if the target string is not found in the list
}



// global variable to store the answer
int answer = 0;

// function that does the depth first search on the caves, by starting from the cave "cave"
// we will use it by starting from the "strat" cave
// we want the number of caves that are not visited if we follow the rule : one path for a small cave, as much as we want for a big cave
void dfs(integer_graph_t graph, integer_set_t visited, char** caves, int nb_strings, char* cave){
    
    // if we reached the "end" cave, we stop the search
    // we've found a path, so we increment the answer
    if (strcmp(cave, "end") == 0){
        answer++;
        return;
    }

    // if the cave is a small cave, we can only visit it once, so if it has already been visited, we stop the search
    int index = get_string_index(caves, nb_strings, cave);
    if (is_lowercase(cave) && integer_set_is_member(&visited, index)){
        return;
    }

    // we mark the small cave as visited if it is one
    if (is_lowercase(cave)){
        integer_set_insert(&visited, index);
    }

    // we now need to consider the neighbours of the cave, to do the dfs again recursively
    // we get the neighbours of the cave
    integer_list_t* adjlist = integer_graph_adjlist(&graph, index);
    for (integer_list_elmt_t* elem = integer_list_head(adjlist); elem != NULL; elem = integer_list_next(elem)){
        int index_neighbors=integer_list_data(elem);
        dfs(graph, visited, caves, nb_strings, caves[index_neighbors]);
    }

    // once we've done the dfs on the neighbours, we mark the small cave as not visited
    // why ? because we want to be able to visit it again if we come back to it in another path
    if (is_lowercase(cave)){
        integer_set_remove(&visited, &index);
    }
}


int main (){

    // getting the input 
    const char* filename = "input12.txt";
    FILE* file = fopen(filename, "r");
    char line[10];
    int nb_lines = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        nb_lines++;
    }
    char*** lines = malloc(nb_lines * sizeof(char**));
    for (int i = 0; i < nb_lines; i++){
        lines[i] = malloc(2 * sizeof(char*));
        for (int j = 0; j < 2; j++){
            lines[i][j] = malloc(10 * sizeof(char));
        }
    }

    rewind(file);
    int i = 0;
    // parsing the input to get the two strings that have a link
    while(fgets(line, sizeof(line), file) != NULL){
        int pos = get_pos(line, '-');
        for (int j = 0; j < pos; j++){
            lines[i][0][j] = line[j];
        }
        lines[i][0][pos] = '\0';
        for (int j = pos + 1; j < strlen(line) - 1; j++){
            lines[i][1][j - pos - 1] = line[j];
        }
        lines[i][1][strlen(line) - pos - 1] = '\0';
        i++;
    }

    
    // printing the input
    printf("Input :\n");
    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < 2; j++){
            printf("%s ", lines[i][j]);
        }
        printf("\n");
    }
    

    // counting the number of different strings
    int nb_strings = count_unique_strings(filename, nb_lines);
    printf("Number of different strings in the file: %d\n", nb_strings);
    char** caves = malloc(nb_strings * sizeof(char*));
    for (int i = 0; i < nb_strings; i++){
        caves[i] = malloc(10 * sizeof(char));
    }
    int k=0;
    for (int i = 0; i <= nb_strings; i++){
        if (is_in_array(caves, nb_strings, lines[i][0])==false){
            strcpy(caves[k], lines[i][0]);
            ++k;
        }
        if (is_in_array(caves, nb_strings, lines[i][1])==false){
            strcpy(caves[k], lines[i][1]);
            ++k;
        }
    }

    // printing the caves
    printf("Different Caves :\n");
    for (int i = 0; i < nb_strings; i++){
        printf("cave %d : %s\n", i, caves[i]);
    }
    printf("\n");
    

    integer_graph_t graph;
    integer_graph_init(&graph);
    
    // adding the caves to the graph
    for (int i = 0; i < nb_strings; i++){
       integer_graph_ins_vertex(&graph, i);
    }

    // adding the link between the caves
    // they need to go both ways
    for (int i = 0; i < nb_lines; i++){
        int index1 = get_string_index(caves, nb_strings, lines[i][0]);
        int index2 = get_string_index(caves, nb_strings, lines[i][1]);
        integer_graph_ins_edge(&graph, index1, index2, 0.0);
        integer_graph_ins_edge(&graph, index2, index1, 0.0);
    }

    // to see the number of the caves and it's adjacents
    print_graph(&graph);

    integer_set_t visited;
    integer_set_init(&visited);


    // doing the dfs
    dfs(graph, visited, caves, nb_strings, "start");


    // printing the answer
    printf("\nAnswer : %d\n\n", answer);



    // free the memory
    for (int i = 0; i < nb_lines; i++){
        for (int j = 0; j < 2; j++){
            free(lines[i][j]);
        }
        free(lines[i]);
    }
    free(lines);

    for (int i = 0; i < nb_strings; i++){
        free(caves[i]);
    }
    free(caves);

    integer_graph_destroy(&graph);
    integer_set_destroy(&visited);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// we need to add an empty line at the end of the .txt file to avoid mistakes
// we found as expected for : 
// - little_test.txt : 10
// - test.txt : 19
// - larger_test.txt : 226
// so it worked fine when using the lib 