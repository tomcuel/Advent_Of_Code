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


// définir la structure de fin
struct fin {
    int *steps;
    int nsteps;
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


// Fonction pour trouver le pgcd
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
// Fonction pour trouver le ppcm
unsigned long long lcm(unsigned long long a, unsigned long long b) {
    return (a * b) / gcd(a, b);
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

    // On a recréer la structure en forme de graph, maintenant il faut compter les tours avant d'avoir le résultat 
    // on prend tous les noeuds avec un A en troisème place, on bouge selon les commandes toutes les valeurs jusqu'à finir avec que des Z en troisème position 
    
    // On fait 6 cas, parce que on a vu que le fichier ne possède que 6 fois des A en troisième position du mot
    struct noeud* iter1=NULL; 
    struct noeud* iter2=NULL; 
    struct noeud* iter3=NULL; 
    struct noeud* iter4=NULL; 
    struct noeud* iter5=NULL; 
    struct noeud* iter6=NULL; 
    for (i = 0; i < nbnoeuds; i++) {
        if (noeuds[i].name[2] == 'A'){
            if (!iter1){
                iter1=&noeuds[i];
            }
            else if (!iter2){
                iter2=&noeuds[i];
            }
            else if (!iter3){
                iter3=&noeuds[i];
            }
            else if (!iter4){
                iter4=&noeuds[i];
            }
            else if (!iter5){
                iter5=&noeuds[i];
            }
            else if (!iter6){
                iter6=&noeuds[i];
            }
        }
    }

    int one, two, three, four, five, six;
    int first = 0, second = 0;
    int match = 0;
    i = 0;
    char dir;
    while (1) {
        if (iter1->name[2] == 'Z') {
            if (!first) {
                first = i;
            } 
            else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter1 = iter1->left;
        } 
        else {
            iter1 = iter1->right;
        }
        i++;
        if (match == 8) {
            break;
        }
    }

    printf("\n");
    one = second - first;
    i = 0;
    first = second = 0;
    match = 0;
    current = 0;
    while (1) {
        if (iter2->name[2] == 'Z') {
            if (!first) {
                first = i;
            } else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter2 = iter2->left;
        } else {
            iter2 = iter2->right;
        }
        i++;
        if (match == 10) {
            break;
        }
    }

    printf("\n");
    two = second - first;
    i = 0;
    first = second = 0;
    match = 0;
    current = 0;
    while (1) {
        if (iter3->name[2] == 'Z') {
            if (!first) {
                first = i;
            } else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter3 = iter3->left;
        } else {
            iter3 = iter3->right;
        }
        i++;
        if (match == 10) {
            break;
        }
    }

    printf("\n");
    three = second - first;
    i = 0;
    first = second = 0;
    match = 0;
    current = 0;
    while (1) {
        if (iter4->name[2] == 'Z') {
            if (!first) {
                first = i;
            } else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter4 = iter4->left;
        } else {
            iter4 = iter4->right;
        }
        i++;
        if (match == 10) {
            break;
        }
    }

    printf("\n");
    four = second - first;
    i = 0;
    first = second = 0;
    match = 0;
    current = 0;
    while (1) {
        if (iter5->name[2] == 'Z') {
            if (!first) {
                first = i;
            } else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter5 = iter5->left;
        } else {
            iter5 = iter5->right;
        }
        i++;
        if (match == 10) {
            break;
        }
    }

    printf("\n");
    five = second - first;
    i = 0;
    first = second = 0;
    match = 0;
    current = 0;
    while (1) {
        if (iter6->name[2] == 'Z') {
            if (!first) {
                first = i;
            } else {
                second = i;
                break;
            }
            match++;
        }
        dir = direction();
        if (dir == 'L') {
            iter6 = iter6->left;
        } else {
            iter6 = iter6->right;
        }
        i++;
        if (match == 10) {
            break;
        }
    }
    six = second - first;

    unsigned long long ans;
    ans = lcm(one, two);
    ans = lcm(ans, three);
    ans = lcm(ans, four);
    ans = lcm(ans, five);
    ans = lcm(ans, six);

    printf("%llu\n", ans);



    return(0);
}

// pas adapté pour test2.txt car on prends ici pour 6 noeuds avec un A en troisième position, mais on peut avoir plus de noeuds avec un A en troisième position