#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 

int charge_au_nord(char** grid, int nline, int tailleline){

    int sum=0;
    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            if (grid[i][j]=='O'){
                sum=sum+nline-i;
            }
        }
    }
    return(sum);
}

char** tilt_au_nord(char** grid, int nline, int tailleline){
    
    char** grid2=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid2[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            grid2[i][j]=grid[i][j];
        }
    }
    
    for (int j=0; j<tailleline; j++){
        int i=0; 
        while (i<nline){
            while (i<nline && grid[i][j]=='#'){
                ++i;
            }

            // on est maintenant à un 'O' ou un '.'
            int nb=0; // nombre de 0 jusqu'à la prochaine pierre fixe '#'
            int p=i; // on commence à partir de cette ligne, donne notre point de départ pour ensuite faire les calculs
            while (i<nline && grid[i][j]!='#'){
                if (grid[i][j]=='O'){
                    ++nb;
                }
                ++i;
            }

            // on et des 'O' là où les zéros se sont superposés jusqu'au nord de la pièce 
            for (int k=p; k<p+nb;k++){
                grid2[k][j]='0';
            }
            for (int k=p+nb; k<i;k++){
                grid2[k][j]='.';
            }
        }
    }
    return(grid2);
}


//heureusement que l'input donne une matrice carée, 90 degré dans le sens anti-horaire / trigo
char** rotation_90_grille(char** grid, int nline, int tailleline){

    char** grid2=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid2[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            grid2[i][j]=grid[i][j];
        }
    }

    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            grid2[i][j]=grid[j][tailleline-1-i];
        }
    }

    return(grid2);
}


char** rotation(char** grid, int nline, int tailleline, int k){ //k est le nombre de rotations
    
    char** grid2=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid2[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    for (int i=0; i<(k%4); i++){
        grid2=rotation_90_grille(grid2,nline,tailleline);
    }

    return(grid2);
}


// fait la rotation, en demandant le nombre de fois que l'on veut tourner, cela fera tourner au max d'un cycle, si k>4
char** do_cycle(char** grid, int nline, int tailleline){
    
    char** grid2=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid2[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    for (int i=0; i<4; i++){
        grid2=rotation(grid2,nline,tailleline, 4-(i%4));
        grid2=tilt_au_nord(grid2,nline,tailleline);
        grid2=rotation(grid2,nline,tailleline, i%4);
    }

    return(grid2);
}


char* get_hash(char** grid, int rows, int cols) {
    // Allouer de la mémoire pour la chaîne résultante
    int total_length = (rows * (cols + 1)) + 1; // +1 pour le caractère de fin de chaîne '\0'
    char* hash = (char*)malloc(total_length * sizeof(char));
    
    // Construire la chaîne résultante
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            hash[index++] = grid[i][j];
        }
        hash[index++] = '\n'; // Ajouter un saut de ligne à la fin de chaque ligne
    }
    hash[index] = '\0'; // Ajouter le caractère de fin de chaîne

    return hash;
}


int main(){

    FILE* file;
    file=fopen("input14.txt", "r");

    int nline=0;
    int tailleline=0; 
    char ligne[120];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        tailleline=strlen(ligne);
        ++nline;
    }

    rewind(file);

    char** grid=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    int i=0;
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        for (int j=0; j<tailleline; j++){
            grid[i][j]=ligne[j];
        }
        ++i;
    }

    // affichage de la grille 
    /*
    printf("\n");
    for (int i=0; i<nline; i++){
        printf("%s\n",grid[i]);
    }
    printf("\n");
    */

    char** grid2=(char**)malloc(sizeof(char*)*nline);
    for (int i=0; i<nline; i++){
        grid2[i]=(char*)malloc(sizeof(char)*tailleline);
    }

    for (int i=0; i<nline; i++){
        for (int j=0; j<tailleline; j++){
            grid2[i][j]=grid[i][j];
        }
    }

   for (int cycle=0; cycle<1000000; cycle++){
        char** grid2=do_cycle(grid2, nline, tailleline);
        // regarder si grid2=grid pour trouver un cycle 
        // et ensuite calculer pour les nombres qu'il reste sur les 1 000 000 000
   }


    for (int i=0; i<nline; i++){
        free(grid[i]);
    }
    free(grid);
    fclose(file);
    return(0);
}

//  non fini pour trouver tous les cycles, 
//il faudrait regarder si une situtation s'est déjà présentée à nous pour en déduire si on peut abréger les cycles à refaire ensuite 
// on cherche donc le nombre de tours à partir duquel où on tourne en boucle 