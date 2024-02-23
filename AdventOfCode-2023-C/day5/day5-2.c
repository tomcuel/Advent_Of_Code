// résultats de la partie 2 non trouvé 

#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid 
void affichageseeds(uint32_t* T, int n) {
  printf("tableau des seeds ");
  for (int i=0;i<n;++i) {
    if (i>0) {
      printf(",");
    }
    printf("%u",T[i]);
  }
  printf("\n");
}
void affichage1(uint32_t* T, int n, int nbligne) {
  printf("àfairemod de la ligne %d : ",nbligne);
  for (int i=0;i<n;++i) {
    if (i>0) {
      printf(",");
    }
    printf("%u",T[i]);
  }
  printf("\n");
}
void affichage2(uint32_t* T, int n, int nbligne) {
  printf("modifiée de la ligne %d : ",nbligne);
  for (int i=0;i<n;++i) {
    if (i>0) {
      printf(",");
    }
    printf("%u",T[i]);
  }
  printf("\n");
}
void affichage3(uint32_t* T, int n, int nbligne) {
  printf("stockages de la ligne %d : ",nbligne);
  for (int i=0;i<n;++i) {
    if (i>0) {
      printf(",");
    }
    printf("%u",T[i]);
  }
  printf("\n");
}


uint32_t* splitBySpaces(const char *input, int p) {
    uint32_t* T=malloc(sizeof(uint32_t)*p);
    char *token;
    char *str = strdup(input);
    token = strtok(str, " ");
    int i=0;
    while (token != NULL) {
        T[i]=atoi(token);
        token = strtok(NULL," ");
        ++i;
    }
    free(str);  
    return(T);
}


// Structure to represent a mapping entry
typedef struct {
    uint32_t dest_start;
    uint32_t src_start;
    uint32_t length;
} MapEntry;
// Function to perform the mapping
uint32_t map_number(u_int32_t number, MapEntry* map, uint32_t map_size) {
    for (int i = 0; i < map_size; ++i) {
        if (number >= map[i].src_start && number < map[i].src_start + map[i].length) {
            return map[i].dest_start + (number - map[i].src_start);
        }
    }
    return number; // If no mapping is found, return the original number
}


int main() {
  
    int p=20;
    uint32_t Tseeds[p];

    uint32_t stock[174][3]; 
    for (int i=0;i<174;++i){
        for (int k=0;k<3;++k){
        stock[i][k]=0;
        }
    }

    FILE *fichier;
    fichier = fopen("input5.txt", "r");
    char ligne[1000];
    int nbligne=0;
    int n=0;
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        if (nbligne==0){
            size_t debut=7;
            size_t fin=205;
            size_t lendest=fin-debut+1;
            char* destination=(char*)malloc(lendest+1);
            memcpy(destination, ligne+debut,lendest);
            destination[lendest]='\0';
            uint32_t* T1=splitBySpaces(destination,p);
            for (int i=0;i<p;++i){
                Tseeds[i]=T1[i];
            }
            affichageseeds(T1,p);
            printf("\n");
            ++nbligne;
        }
        else{

        if (strcmp(ligne,"\n")==0){
            fgets(ligne, sizeof(ligne), fichier);
            nbligne=nbligne+2;
            ++n;
            continue; 
        }

        uint32_t* T=splitBySpaces(ligne,3);
        for (int k=0;k<3;++k){
            stock[nbligne][k]=T[k];
        }
        free(T);
        ++nbligne;
        }
    }

    fclose(fichier);

    for (int i=0;i<174;++i){
        affichage3(stock[i],3,i+1);
    }
    uint32_t num_seeds=0;
    for (int i=0; i<p;++i){
        if (i%2==1){
            num_seeds=num_seeds+Tseeds[i];
        }
    }

    uint32_t* seeds=malloc(sizeof(uint32_t)*num_seeds);
    for (int i=0; i<(p/2);++i){
        for (int k=0;k<Tseeds[2*i+1];++k){
            seeds[k]=Tseeds[2*i]+k;
        }
    }
        
    MapEntry* seed_to_soil=malloc(sizeof(MapEntry)*18);
    MapEntry* soil_to_fertilizer=malloc(sizeof(MapEntry)*8); 
    MapEntry* fertilizer_to_water=malloc(sizeof(MapEntry)*35); 
    MapEntry* water_to_light=malloc(sizeof(MapEntry)*45);
    MapEntry* light_to_temperature=malloc(sizeof(MapEntry)*14); 
    MapEntry* temperature_to_humidity=malloc(sizeof(MapEntry)*28);
    MapEntry* humidity_to_location=malloc(sizeof(MapEntry)*11);

    for (int i=4;i<22;++i){
        seed_to_soil[i-4].dest_start=stock[i][0];
        seed_to_soil[i-4].src_start=stock[i][1];
        seed_to_soil[i-4].length=stock[i][2];
    }

    for (int i=24;i<32;++i){
        soil_to_fertilizer[i-24].dest_start=stock[i][0];
        soil_to_fertilizer[i-24].src_start=stock[i][1];
        soil_to_fertilizer[i-24].length=stock[i][2];
    }

    for (int i=34;i<69;++i){
        fertilizer_to_water[i-34].dest_start=stock[i][0];
        fertilizer_to_water[i-34].src_start=stock[i][1];
        fertilizer_to_water[i-34].length=stock[i][2];
    }

    for (int i=71;i<116;++i){
        water_to_light[i-71].dest_start=stock[i][0];
        water_to_light[i-71].src_start=stock[i][1];
        water_to_light[i-71].length=stock[i][2];
    }

    for (int i=118;i<132;++i){
        light_to_temperature[i-118].dest_start=stock[i][0];
        light_to_temperature[i-118].src_start=stock[i][1];
        light_to_temperature[i-118].length=stock[i][2];
    }

    for (int i=134;i<162;++i){
        temperature_to_humidity[i-134].dest_start=stock[i][0];
        temperature_to_humidity[i-134].src_start=stock[i][1];
        temperature_to_humidity[i-134].length=stock[i][2];
    }

    for (int i=164;i<175;++i){
        humidity_to_location[i-164].dest_start=stock[i][0];
        humidity_to_location[i-164].src_start=stock[i][1];
        humidity_to_location[i-164].length=stock[i][2];
    }

    uint32_t* location_numbers=malloc(sizeof(uint32_t)*num_seeds);

    // Perform mapping for each seed
    for (int i = 0; i < num_seeds; ++i) {
        // Seed to Soil mapping
        uint32_t soil_number = map_number(seeds[i], seed_to_soil, sizeof(*seed_to_soil) / sizeof(seed_to_soil[0]));
        printf("number numéro %d: %d\n",i,soil_number);
        // Soil to Fertilizer mapping
        uint32_t fertilizer_number = map_number(soil_number, soil_to_fertilizer, sizeof(*soil_to_fertilizer) / sizeof(soil_to_fertilizer[0]));
        // Fertilizer to Water mapping
        uint32_t water_number = map_number(fertilizer_number, fertilizer_to_water, sizeof(*fertilizer_to_water) / sizeof(fertilizer_to_water[0]));
        // Water to Light mapping
        uint32_t light_number = map_number(water_number, water_to_light, sizeof(*water_to_light) / sizeof(water_to_light[0]));
        // Light to Temperature mapping
        uint32_t temperature_number = map_number(light_number, light_to_temperature, sizeof(*light_to_temperature) / sizeof(light_to_temperature[0]));
        // Temperature to Humidity mapping
        uint32_t humidity_number = map_number(temperature_number, temperature_to_humidity, sizeof(*temperature_to_humidity) / sizeof(temperature_to_humidity[0]));
        // Humidity to Location mapping
        location_numbers[i] = map_number(humidity_number, humidity_to_location, sizeof(*humidity_to_location) / sizeof(humidity_to_location[0]));
    }
    // Find the lowest location number
    uint32_t lowest_location = location_numbers[0];
    for (int i = 1; i < num_seeds; ++i) {
        if (location_numbers[i] < lowest_location) {
            lowest_location = location_numbers[i];
        }
    }

    free(seed_to_soil);
    free(soil_to_fertilizer);
    free(fertilizer_to_water);
    free(water_to_light);
    free(light_to_temperature);
    free(temperature_to_humidity);
    free(humidity_to_location);
    free(location_numbers);
    free(seeds);
    printf("\nnombres de lignes : %d\n",nbligne);
    printf("\nnombres de lignes vides : %d\n",n);
    printf("\nle résultat est : %d\n", lowest_location);
    return(0);
}

/*normalemment ça devrait marcher même si ici ce n'est pas le cas, stackoverflow*/
// méthode pas assez optimisée
/* vrai résultat : 50855035*/