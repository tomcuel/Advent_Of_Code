#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 


// function that returns the number of times a character appears in a string
int number_of_times(char* ch, char x){
    int n=0;
    int i=0;
    while (ch[n] != 0){
        if (ch[n]==x){
            i=i+1;
        }
        n=n+1;
    }
    return(i);
}


// function that splits a string into an array of strings using a delimiter string (here i will use ',')
// I'm using malloc allocation to allocate memory for the array of strings
// so there is a need to free the memory after using the array of strings
char** split_line(char* line, int split_line, char* delimiter){
    char** result = malloc(split_line * sizeof(char*));
    for (int i=0; i<split_line; i++){
        result[i] = malloc(5 * sizeof(char));
    }
    char* token = strtok(line, delimiter);
    int i = 0;
    while (token != NULL){
        strcpy(result[i],token);
        token = strtok(NULL, delimiter);
        i++;
    }
    return result;

}


// copy the board to a new board, to avoid modifying the original board
int*** copy_board(int*** board, int nb_lines, int len_lines){
    int*** new_board=malloc(nb_lines*sizeof(int**));
    for (int i=0; i<nb_lines; i++){
        new_board[i]=(int**)malloc(len_lines*sizeof(int*));
        for (int j=0; j<len_lines; j++){
            new_board[i][j]=(int*)malloc(len_lines*sizeof(int));
            for (int k=0; k<len_lines; k++){
                new_board[i][j][k]=board[i][j][k];
            }
        }
    }
    return new_board;
}


// fill the table with -1 if the number is in the chosen numbers which is the line of command 
void fill_table(int** table, int number, int size_of_table){
    for (int i=0; i<size_of_table; i++){
        for (int j=0; j<size_of_table; j++){
            if (table[i][j] == number){
                table[i][j] = -1;
                break;
            }
        }
    }
}


// check if the table is winning
bool is_winning(int** table, int size_of_table){
    // check the rows by looking if there is 5 -1 in a row
    for (int i=0; i<size_of_table; i++){
        int count = 0;
        for (int j=0; j<size_of_table; j++){
            if (table[i][j] == -1){
                count++;
            }
        }
        if (count == size_of_table){
            return true;
        }
    }

    // check the columns by looking if there is 5 -1 in a column
    for (int i=0; i<size_of_table; i++){
        int count = 0;
        for (int j=0; j<size_of_table; j++){
            if (table[j][i] == -1){
                count++;
            }
        }
        if (count == size_of_table){
            return true;
        }
    }

    return false;
}


int main (){
    
    FILE *file = fopen("input4.txt", "r");
    char line[1000];
    // getting the first line that represents the chosen numbers of the bingo game
    fgets(line, sizeof(line), file);

    // getting the number of numbers in the bingo game
    int len_numbers = number_of_times(line, ',') + 1;
    int* numbers = malloc(len_numbers * sizeof(int));
    char** numbers_str = split_line(line, len_numbers, ",");
    // converting the numbers from string to int
    // printf("The numbers are:\n");    
    for (int i=0; i<len_numbers; i++){
        numbers[i] = atoi(numbers_str[i]);
        // printf("%d, ", numbers[i]);
    } 
    // printf("\n");

    // passing the empty line
    fgets(line, sizeof(line), file);

    // getting the number of tables
    int number_of_tables = 1;
    while (fgets(line, sizeof(line), file) != NULL){
       if (strcmp(line, "\n") == 0){
           number_of_tables++;
       }
    }
    // printf("The number of tables is: %d\n", number_of_tables);

    // reopeing the file to read the tables
    rewind(file);
    // passing the command line and the first line
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    // creating the tables
    int*** tables = malloc(number_of_tables * sizeof(int**));
    int size_of_table = 5;
    for (int i=0; i<number_of_tables; i++){
        tables[i] = malloc(size_of_table * sizeof(int*));
        for (int j=0; j<size_of_table; j++){
            tables[i][j] = malloc(size_of_table * sizeof(int));
        }
    }
    // parsing the input file to get the tables
    // if we encouter an empty line, we increment the index_table, return pos to 0, to be able to fill the start of the next table and continue
    int index_table = 0;
    int pos =0;
    while (fgets(line, sizeof(line), file) != NULL){
        if (strcmp(line, "\n") == 0){
            index_table++;
            // printf("The index table is: %d\n", index_table);
            pos = 0;
            continue;
        }


        int numbers_table[5];
        // Using sscanf to extract integers
        int num_read = sscanf(line, "%d %d %d %d %d", &numbers_table[0], &numbers_table[1], &numbers_table[2], &numbers_table[3], &numbers_table[4]);
        if (num_read != 5){
            // printf("Error in reading the numbers\n");
            return 1;
        }

        for (int i=0; i<size_of_table; i++){
            tables[index_table][i][pos] = numbers_table[i];
        }
        pos++;
    }
    

    // putting -1 in the table if the number is in the chosen numbers
    // we are fulling the boards
    int k=0;
    // we are going to copy the tables to avoid modifying the original tables
    // we're are going to take the number of winning tables
    int number_of_winning_tables=0;
    // we are going to keep track of the winning tables, to see if we have already counted them in the number_of_winning_tables when we pull a new number
    bool* winning_tables=malloc(number_of_tables * sizeof(bool));
    for (int i=0; i<number_of_tables; i++){
        winning_tables[i]=false;
    }
    int*** copy_tables=copy_board(tables, number_of_tables, size_of_table);
    while (k<len_numbers){
        
        int number_pulled=numbers[k];
        for (int i=0; i< number_of_tables; i++){
            fill_table(copy_tables[i], number_pulled, size_of_table);
        }
        for (int i=0; i<number_of_tables; i++){
            if (winning_tables[i]==true){
                continue;
            }
            if (is_winning(copy_tables[i], size_of_table)==true){
                number_of_winning_tables++;
                winning_tables[i]=true;
            }
        }
        ++k;
    } 

    /*
    // Looking at the tables to see the one that won
    for (int i=0; i<number_of_tables; i++){
        printf("Table %d\n", i+1);
        for (int j=0; j<size_of_table; j++){
            for (int k=0; k<size_of_table; k++){
                printf("%d ", copy_tables[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    */

    // free the memory of things I copied for this loop 
    for (int i=0; i<number_of_tables; i++){
        for (int j=0; j<size_of_table; j++){
            free(copy_tables[i][j]);
        }
        free(copy_tables[i]);
    }
    free(copy_tables);


    printf("The number of winning tables is: %d\n", number_of_winning_tables);


    // putting -1 in the table if the number is in the chosen numbers
    // we are fulling the boards
    // reseting the winning tables situation to false to be able to find the last winning table
    for (int i=0; i<number_of_tables; i++){
        winning_tables[i]=false;
    }
    int index_of_last_winning_table = -1;
    int number_called_when_winning = 0;
    k=0;
    // test is used to break the loop when we find the last winning table
    int test=0;
    while (k<len_numbers && test != number_of_winning_tables){
        // taking the next number that will be called
        int number_pulled=numbers[k];
        // filling the tables with -1 if the number is in the chosen numbers
        for (int i=0; i< number_of_tables; i++){
            fill_table(tables[i], number_pulled, size_of_table);
        }
        // checking if the table is winning
        for (int i=0; i<number_of_tables; i++){
            if (winning_tables[i]==true){
                continue;
            }
            if (is_winning(tables[i], size_of_table)==true){
                // if the table is winning, we store the index of the winning table and the number that was called when the table won
                winning_tables[i]=true;
                index_of_last_winning_table = i;
                number_called_when_winning = number_pulled;
                test++;
            }
        }
        ++k;
    }   

    
    /*
    // printing the tables to see them 
    for (int i=0; i<number_of_tables; i++){
        printf("Table %d\n", i+1);
        for (int j=0; j<size_of_table; j++){
            for (int k=0; k<size_of_table; k++){
                printf("%d ", tables[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    */
   

    /*
    if (index_of_winning_table == -1){
        printf("No winner\n");
    }
    else{
        printf("Table %d wins\n", index_of_winning_table+1);
    }
    */


    // calculating the answer by adding every number that hasn't been marked with -1 in the winning table
    int answer = 0;
    for (int i=0; i<size_of_table; i++){
        for (int j=0; j<size_of_table; j++){
            if (tables[index_of_last_winning_table][i][j] != -1){
                answer+=tables[index_of_last_winning_table][i][j];
            }
        }
    }
    // formula to get the result
    answer*=number_called_when_winning;


    // printing the results 
    printf("\nThe answer is: %d\n\n", answer);



     // free the memory and close the file
    for (int i=0; i<number_of_tables; i++){
        for (int j=0; j<size_of_table; j++){
            free(tables[i][j]);
        }
        free(tables[i]);
    }
    free(tables);

    for (int i=0; i<len_numbers; i++){
        free(numbers_str[i]);
    }
    free(numbers_str);

    free(numbers);

    free(winning_tables);

    fclose(file);
    return 0;
}



// there is the need to not remove the last line of the input for the result to be correct
// or to add a new line at the end of the input file if there's no new line at the end of the file
// I found 1924 with the test.txt file as expected