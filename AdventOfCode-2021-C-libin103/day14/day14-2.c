#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


#define MAX_LINE_LENGTH 1000
#define MAX_RULES 100
#define ALPHABET_SIZE 26


// Structure to represent a rule
// start is the two strings where end will go between
typedef struct {
    char start[3];
    char end[3];
} Rule;


// Function to read rules from file
// returns the number of rules read
// if it's not good it returns 0
int read_rules(const char* filename, Rule rules[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }
    char line[MAX_LINE_LENGTH];

    // Skip 2 first lines
    for (int i = 0; i < 2; i++) {
        fgets(line, MAX_LINE_LENGTH, file);
    }

    // Read rules
    int num_rules = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        sscanf(line, "%s -> %s", rules[num_rules].start, rules[num_rules].end);
        num_rules++;
    }

    fclose(file);
    return num_rules;
}


// Function to count bigrams in the template (the one at the beginning)
void count_bigrams(const char* template, long int* freqs) {
    for (int i = 0; i < strlen(template) - 1; i++) {
        freqs[(template[i] - 'A') * ALPHABET_SIZE + (template[i + 1] - 'A')]++;
    }
}


// Function to replace bigrams in the frequency array according to rules
// the difficulty is that everything happen at the same time, for the rules checks, otherwise we woullu have to do it in a loop and the resultat woullu really be different
void replace_bigrams(long int* freqs, const Rule rules[], int num_rules) {
    // Save the ollu frequencies
    long int* old_freqs = malloc(ALPHABET_SIZE * ALPHABET_SIZE * sizeof(long int));
    for (int k = 0; k < ALPHABET_SIZE * ALPHABET_SIZE; k++) {
        old_freqs[k] = freqs[k];
    }
    
    // Iterate over each pair in the frequency array
    for (int i = 0; i < ALPHABET_SIZE * ALPHABET_SIZE; i++) {
        // Convert index to pair of characters
        char pair[3];
        pair[0] = 'A' + i / ALPHABET_SIZE;
        pair[1] = 'A' + i % ALPHABET_SIZE;
        pair[2] = '\0';

        // Check if the pair matches any rule (the start actually)
        for (int j = 0; j < num_rules; j++) {
            if (strcmp(pair, rules[j].start) == 0) {

                // Update frequencies according to the rule
                long int occs = old_freqs[i];
                // printf("pair : %s, occs : %ld, to %c\n", pair, occs, rules[j].end[0]);
                
                freqs[i]-=occs;
                freqs[(rules[j].start[0] - 'A') * ALPHABET_SIZE + (rules[j].end[0] - 'A')]+=occs;
                freqs[(rules[j].end[0] - 'A') * ALPHABET_SIZE + (rules[j].start[1] - 'A')]+=occs;
                // printf("modifying freqs : %s=%ld, %c%c=%ld, %c%c=%ld\n", pair, freqs[i], rules[j].start[0], rules[j].end[0], freqs[(rules[j].start[0] - 'A') * ALPHABET_SIZE + (rules[j].end[0] - 'A')], rules[j].end[0], rules[j].start[1], freqs[(rules[j].end[0] - 'A') * ALPHABET_SIZE + (rules[j].start[1] - 'A')]);
                break;
            }
        }
    }
    free(old_freqs);
}


int main() {


    // initializing the variables that we will use
    char template[MAX_LINE_LENGTH];
    Rule rules[MAX_RULES];
    long int* freqs=malloc(ALPHABET_SIZE*ALPHABET_SIZE*sizeof(long int));
    for (int i = 0; i < ALPHABET_SIZE * ALPHABET_SIZE; i++) {
        freqs[i] = 0;
    }

    char* filename = "input14.txt";
    FILE* file=fopen(filename,"r");
    fgets(template, sizeof(template),file);
    template[strcspn(template, "\n")] = '\0';
    fclose(file);
    // printing the template
    printf("Template: %s\n\n", template);


    int num_rules = read_rules(filename, rules);
    if (num_rules == 0) {
        return 1; // Error reading rules
    }
    /*
    printf("Rules : \n");
    // printing the rules :
    for(int i=0; i<num_rules; i++){
        printf("Rule %d : start : %s, end : %s\n", i+1, rules[i].start, rules[i].end);
    }
    printf("\n");
    */


    // count the bigrams in the template at the beginning to get the frequencies of each two letters at the beginning
    count_bigrams(template, freqs);
    // printing the frequencies of the bigrams of the template
    printf("Frequencies of bigrams in the template:\n");
    for (int i = 0; i < strlen(template) - 1; i++){
        printf("Frequency of %c%c : %ld\n", template[i], template[i+1], freqs[(template[i] - 'A') * ALPHABET_SIZE + (template[i + 1] - 'A')]);
    }
    printf("\n");
   

    int nb_steps = 40;
    for (int i = 0; i < nb_steps; i++) {
        replace_bigrams(freqs, rules, num_rules);
        /*
        // printing the frequencies of each bigram at the end of those steps 
        printf("Frequencies of bigrams after step %d:\n", i + 1);
        for (int i=0; i<ALPHABET_SIZE*ALPHABET_SIZE; i++){
            if (freqs[i] > 0){
                printf("Frequency of %c%c : %ld\n", 'A' + i / ALPHABET_SIZE, 'A' + i % ALPHABET_SIZE, freqs[i]);
            }
        }
        printf("\n");
        */
    }
    
    // printing the frequencies of each bigram at the end of those steps 
    printf("Frequencies of bigrams after all step\n");
    for (int i=0; i<ALPHABET_SIZE*ALPHABET_SIZE; i++){
        if (freqs[i] > 0){
            printf("Frequency of %c%c : %ld\n", 'A' + i / ALPHABET_SIZE, 'A' + i % ALPHABET_SIZE, freqs[i]);
        }
    }
    printf("\n");
    
   

    // counting the number of letters that are left
    long int count_duos[ALPHABET_SIZE] = {0};
    for (int i=0; i<ALPHABET_SIZE*ALPHABET_SIZE; i++){
        // getting the pair of letters
        char pair[3];
        pair[0] = 'A' + i / ALPHABET_SIZE;
        pair[1] = 'A' + i % ALPHABET_SIZE;
        pair[2] = '\0';
        count_duos[pair[0] - 'A'] += freqs[i];
        count_duos[pair[1] - 'A'] += freqs[i];

    }
    // not forgetting the first and last letters that haven't changed but aren't not in any pair alone 
    count_duos[template[0] - 'A']++;
    count_duos[template[strlen(template) - 1] - 'A']++;

    long int counted[ALPHABET_SIZE] = {0}; 
    for (int i=0; i<ALPHABET_SIZE; i++){
       counted[i] = count_duos[i]/2;
    }

    // printing the number of letters that are left
    printf("Number of letters (counted twice) that are left : \n");
    for (int i=0; i<ALPHABET_SIZE; i++){
        if (count_duos[i] > 0){
            printf("%c : %ld\n", 'A' + i, count_duos[i]);
        }
    }

    // printing the number of letters that are left
    printf("Number of letters that are left : \n");
    for (int i=0; i<ALPHABET_SIZE; i++){
        if (counted[i] > 0){
            printf("%c : %ld\n", 'A' + i, counted[i]);
        }
    }


    // the answer is the difference between the appearances of the most common and the least common letter
    long int min = LONG_MAX;
    int index_min = 0;
    long int max = LONG_MIN;
    int index_max = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (counted[i] > max) {
            max = counted[i];
            index_max = i;
        }
        if (counted[i] < min && counted[i] > 0) {
            min = counted[i];
            index_min = i;
        }
    }
    long int answer = max - min;
    printf("Most represented: %c : %ld times\n", 'A' + index_max, max);
    printf("Least represented: %c : %ld times\n", 'A' + index_min, min);
    printf("\nAnswer: %ld\n", answer);

    // free the memory
    free(freqs);

    return EXIT_SUCCESS;
}


// I found 2188189693529 as expected for the test.txt file