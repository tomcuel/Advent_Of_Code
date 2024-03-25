#ifndef __RESULTS_H__
#define __RESULTS_H__

#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include "board.h"
#include "numbers.h"

// function that returns the oxygen generating rating
int find_oxygen_generating_rating(int** board_real, int nb_lines, int len);


// function that returns the oxygen generating rating
int find_co2_generating_rating(int** board_real, int nb_lines, int len);


#endif