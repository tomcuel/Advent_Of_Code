#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

char directions[512];
int current = 0;

// définir la structure de noeud
struct noeud {
    char name[4];
    struct noeud *left;
    struct noeud *right;
};


// faire le noeud associé au nom entré, sera utilisé lors de la deuxième lecture de fichier  
struct noeud * recupnoeudparlenom(struct noeud *noeuds, int nbnoeuds, char *name){
    for (int i=0; i<nbnoeuds;i++){
        if (!strcmp(noeuds[i].name, name)){
            return(&noeuds[i]);
        }
    }
    return (NULL);
}


// trouver la direction dans laquelle on va en avancant dans la liste des directions 
char direction(){
    char d;
    d = directions[current];
    // traiter le cas où l'on doit recommencer à regarder la commande 
    if ((d != 'L') && (d != 'R')) {
        current = 1;
        return directions[0];
    }
    current++;
    return d;
}


int main(){
    FILE *file = fopen("input8.txt", "r");
    struct noeud *noeuds = NULL;
    int nbnoeuds=0;
    char line[512] = {0}; 
    char left[4] = {0}; 
    char right[4] = {0};
    char *ptr;
   
    fgets(directions, sizeof(line), file);
    fgets(line, sizeof(line), file);

    // stocker chaque ligne dans une structure : la première chaîne de caractère va dans le nom de la structure
    while (fgets(line, sizeof(line), file)) {
        noeuds = realloc(noeuds, (nbnoeuds + 1) * sizeof *noeuds);
        sscanf(line, "%s", noeuds[nbnoeuds].name);
        nbnoeuds++;
    }

    rewind(file); // se remettre au début 
    fgets(directions, sizeof(line), file);
    fgets(line, sizeof(line), file);

    int i=0;
    // on regarde les infos après le nom pour savoir vers quel noeud on va rediriger 
    while (fgets(line, sizeof(line), file)) {
        ptr = line + 6;
        sscanf(ptr, "(%3[^,], %3[^)])", left, right);
        noeuds[i].left = recupnoeudparlenom(noeuds, nbnoeuds, left);
        noeuds[i].right = recupnoeudparlenom(noeuds, nbnoeuds, right);
        i++;
    }

    int sum=0; // compter le nombre de tours 
    struct noeud *iter;
    char dir;
    // On initialise 
    iter = recupnoeudparlenom(noeuds, nbnoeuds, "AAA");
    // tant qu'on est pas arrivé à la fin de l'arbre
    while (strcmp(iter->name, "ZZZ")) {
        dir = direction();

        if (dir == 'L') {
            iter = iter->left;
        } 
        else { // dir == 'R'
            iter = iter->right;
        }
        sum++;
    }

    printf("\nLe résultat est : %d\n\n",sum);

    return(0);
}

// on trouve bien 2 pour le test1.txt