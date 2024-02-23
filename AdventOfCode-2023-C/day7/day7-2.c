#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

#define NCARDS 13
#define HAND_SIZE 6


typedef enum {
    HAND_NOTHING,
    HAND_ONE,
    HAND_TWO,
    HAND_THREE,
    HAND_FH,
    HAND_FOUR,
    HAND_FIVE
} HAND_TYPE;


struct hand {
    HAND_TYPE type;
    char cards[HAND_SIZE];
    int bid;
};


int card_to_index(char c){
    switch (c) {
    case 'J':
        return 0;
    case '2':
        return 1;
    case '3':
        return 2;
    case '4':
        return 3;
    case '5':
        return 4;
    case '6':
        return 5;
    case '7':
        return 6;
    case '8':
        return 7;
    case '9':
        return 8;
    case 'T':
        return 9;
    case 'Q':
        return 10;
    case 'K':
        return 11;
    case 'A':
        return 12;
    default:
        return 13;
    }
}


// l'addition de la carte joker va juste changer le type de la main, le reste du programme reste inchangé
void get_type(struct hand *hand) {
    int i;
    int card_type_count[NCARDS] = {0};
    int ntwopairs = 0, nthreepairs = 0;
    int njokers = 0;

    for (i = 0; hand->cards[i]; i++) {
        card_type_count[card_to_index(hand->cards[i])]++;
        if (hand->cards[i] == 'J') {
        	njokers++;
        }
    }

    for (i = 0; i < NCARDS; i++) {
        if (card_type_count[i] == 5) {
            hand->type = HAND_FIVE;
            return;
        } 
        else if (card_type_count[i] == 4) {
            hand->type = HAND_FOUR;
            if ((njokers == 1) || (njokers == 4)) {
            	hand->type = HAND_FIVE;
            }
            return;
        }
    }

    for (i = 0; i < NCARDS; i++) {
        if (card_type_count[i] == 3) {
            nthreepairs++;
        } 
        else if (card_type_count[i] == 2) {
            ntwopairs++;
        }
    }

    if ((nthreepairs == 1) && (ntwopairs == 1)) {
        if (njokers == 0) {
        	hand->type = HAND_FH;
        } 
        else {
        	hand->type = HAND_FIVE;
        }
    } 
    else if (nthreepairs == 1) {
    	if (njokers ==0) {
    		hand->type = HAND_THREE;
    	} else {
        	hand->type = HAND_FOUR;
    	}
    } 
    else if (ntwopairs == 2) {
    	if (njokers == 1) {
    		hand->type = HAND_FH;
    	} 
        else if (njokers == 2) {
        	hand->type = HAND_FOUR;
    	} 
        else {
    		hand->type = HAND_TWO;
    	}
    } 
    else if (ntwopairs == 1) {
    	if ((njokers == 1) || (njokers == 2)) {
    		hand->type = HAND_THREE;
    	} 
        else {
        	hand->type = HAND_ONE;
    	}
    } 
    else {
    	if (njokers !=0) {
    		hand->type = HAND_ONE;
    	} 
        else {
        	hand->type = HAND_NOTHING;
    	}
    }
}


// renvoie 1 si arg2 est plus fort 
int is_hand_stronger(const void *arg1, const void *arg2){
    int i, val1, val2;
    const struct hand *h1 = arg1, *h2 = arg2;

    if (h2->type > h1->type) {
        return -1;/* plus fort  */
    } 
    else if (h2->type == h1->type) {
        for (i = 0; i < HAND_SIZE; i++) {
            val1 = card_to_index(h1->cards[i]);
            val2 = card_to_index(h2->cards[i]);
            if (val2 > val1) {
                return -1;
            } 
            else if (val1 > val2) {
                return 1;
            }
        }
    } 
    else {
        return 1;/* plus faible */
    }
    return 0;
}


int main(){
    FILE *fichier = fopen("input7.txt", "r");
    char line[64] = {0};
    struct hand *hands = NULL;
    int nblignes=0;

    while (fgets(line, 64, fichier)) {
        hands = realloc(hands, (nblignes + 1) * sizeof *hands);
        sscanf(line, "%s %d", hands[nblignes].cards, &hands[nblignes].bid);
        nblignes++;
    }

    /* get type */
    for (int i = 0; i < nblignes; i++) {
        get_type(&hands[i]);
    }

    /* sort them */
    qsort(hands, nblignes, sizeof *hands, is_hand_stronger);

    int sum=0;
    for (int i = 0; i < nblignes; i++) {
        //printf("%s %d\n", hands[i].cards, hands[i].bid);
        sum += (i + 1) * hands[i].bid;
    }
    printf("%d\n", sum);

    fclose(fichier);
    free(hands);
    return 0;
}

// On trouve bien 5905 pour le test.txt