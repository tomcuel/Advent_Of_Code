#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// function to determine if there is a winning player
int winning_player(int player_1_score, int player_2_score, int points_to_win){
    if (player_1_score>=points_to_win){
        return 1;
    }
    if (player_2_score>=points_to_win){
        return 2;
    }
    return 0;
}


int main (){

    FILE* file=fopen("input21.txt", "r");
    char line[100];

    fgets(line, sizeof(line),file);
    int len=strlen(line);
    int start_pos_player_1=line[len-2]-'0';
    fgets(line, sizeof(line),file);
    int start_pos_player_2=line[len-2]-'0';

    // printf("start_pos_player_1: %d\n", start_pos_player_1);
    // printf("start_pos_player_2: %d\n", start_pos_player_2);

    // points a player need to win
    int points_to_win=1000;

    // each player has a total score of points and a placement between 1 and 10
    int player_1_score=0;
    int player_2_score=0;

    // number of rolls per round
    int number_of_rolls=3;

    // number of rounds made so far
    int rounds=0;
    while (winning_player(player_1_score, player_2_score, points_to_win)==0){
        
        // if rounds%2==0 player 1 plays
        if (rounds%2==0){
            int roll1=rounds*number_of_rolls+1;
            int roll2=rounds*number_of_rolls+2;
            int roll3=rounds*number_of_rolls+3;
            start_pos_player_1=(roll1+roll2+roll3+start_pos_player_1)%10;
            if (start_pos_player_1==0){
                start_pos_player_1=10;
            }
            player_1_score+=start_pos_player_1;
            printf("player1_start_pos : %d and player_1_score: %d\n", start_pos_player_1, player_1_score);

        }

        // if rounds%2==1 player 2 plays
        if (rounds%2==1){
            int roll1=rounds*number_of_rolls+1;
            int roll2=rounds*number_of_rolls+2;
            int roll3=rounds*number_of_rolls+3;
            start_pos_player_2=(roll1+roll2+roll3+start_pos_player_2)%10;
            if (start_pos_player_2==0){
                start_pos_player_2=10;
            }
            player_2_score+=start_pos_player_2;
            printf("player2_start_pos : %d and player_2_score: %d\n", start_pos_player_2, player_2_score);

        }
        rounds++;
    }

    // getting the answer
    int answer=0; 
    int winner=winning_player(player_1_score, player_2_score, points_to_win);
    if (winner==1){
        answer=rounds*number_of_rolls*player_2_score;
    }
    if (winner==2){
        answer=rounds*number_of_rolls*player_1_score;
    }

    printf("\nThe answer is: %d\n\n", answer);

    // free the memory


    // close the file 
    fclose(file);

    return EXIT_SUCCESS;
}

// I found 745 * 993 = 739785 as expected for the test.txt file