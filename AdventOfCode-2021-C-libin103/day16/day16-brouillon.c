#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  

#define NUMBER_OF_BITS 4   

// function to convert 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F into base 2 array where array[0] is the most significant bit
int* convert_base_16_into_base_2(char c){
    int* converted = (int*)malloc(NUMBER_OF_BITS*sizeof(int));
    switch (c){
        case '0':
            converted[0] = 0;
            converted[1] = 0;
            converted[2] = 0;
            converted[3] = 0;
            break;
        case '1':
            converted[0] = 0;
            converted[1] = 0;
            converted[2] = 0;
            converted[3] = 1;
            break;
        case '2':
            converted[0] = 0;
            converted[1] = 0;
            converted[2] = 1;
            converted[3] = 0;
            break;
        case '3':
            converted[0] = 0;
            converted[1] = 0;
            converted[2] = 1;
            converted[3] = 1;
            break;
        case '4':
            converted[0] = 0;
            converted[1] = 1;
            converted[2] = 0;
            converted[3] = 0;
            break;
        case '5':
            converted[0] = 0;
            converted[1] = 1;
            converted[2] = 0;
            converted[3] = 1;
            break;
        case '6':
            converted[0] = 0;
            converted[1] = 1;
            converted[2] = 1;
            converted[3] = 0;
            break;
        case '7':
            converted[0] = 0;
            converted[1] = 1;
            converted[2] = 1;
            converted[3] = 1;
            break;
        case '8':
            converted[0] = 1;
            converted[1] = 0;
            converted[2] = 0;
            converted[3] = 0;
            break;
        case '9':
            converted[0] = 1;
            converted[1] = 0;
            converted[2] = 0;
            converted[3] = 1;
            break;
        case 'A':
            converted[0] = 1;
            converted[1] = 0;
            converted[2] = 1;
            converted[3] = 0;
            break;
        case 'B':
            converted[0] = 1;
            converted[1] = 0; 
            converted[2] = 1;
            converted[3] = 1;
            break;
        case 'C':
            converted[0] = 1;
            converted[1] = 1;
            converted[2] = 0;
            converted[3] = 0;
            break;
        case 'D':
            converted[0] = 1;
            converted[1] = 1;
            converted[2] = 0;
            converted[3] = 1;
            break;
        case 'E':
            converted[0] = 1;
            converted[1] = 1;
            converted[2] = 1;
            converted[3] = 0;
            break;
        case 'F':
            converted[0] = 1;
            converted[1] = 1;
            converted[2] = 1;
            converted[3] = 1;
            break;
    }
    return converted;
}


// function to convert a binary array into a decimal number
int convert_base_2_into_base_10(int* binary, int start, int end){
    int decimal = 0;
    for (int i = start; i < end; i++){
        // using horner formula for polynomial evaluation
        decimal = decimal*2 + binary[i];
    }
    return decimal;
}


// function to convert a decimal number into a binary array
int* convert_base_10_into_base_2(int decimal){
    int* binary = (int*)malloc(NUMBER_OF_BITS*sizeof(int));
    for (int i = NUMBER_OF_BITS - 1; i >= 0; i--){
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }
    return binary;
}



int main (){
    /*
    // just to test that the function is working as expected
    int* converted = convert_base_16_into_base_2('A');
    for (int i = 0; i < NUMBER_OF_BITS; i++){
        printf("%d", converted[i]);
    }
    free(converted);
    */

    FILE* file = fopen("test_type4.txt", "r");
    // getting 1500 since the line is 1370 characters long for my input
    char line[1500];
    fgets(line, sizeof(line), file);
    int length = strlen(line);
    // printf("\nlength of the command line : %d\n\n", length);


    // we're now converting the line into a binary array
    // each character is written in base 16, we need to convert it into base 2
    int lenght_binary = length*NUMBER_OF_BITS;
    int* binary = (int*)malloc(lenght_binary*sizeof(int));
    for (int i = 0; i < length; i++){
        int* converted = convert_base_16_into_base_2(line[i]);
        for (int j = 0; j < NUMBER_OF_BITS; j++){
            binary[i*NUMBER_OF_BITS + j] = converted[j];
        }
        free(converted);
    }
    /*
    // printing the binary array to check if it's working as expected
    printf("binary array : ");
    for (int i = 0; i < lenght_binary; i++){
        printf("%d", binary[i]);
    }
    printf("\n\n");
    */
    // converting the binary array into a decimal number to see if it matches the expected output
    int decimal_test = convert_base_2_into_base_10(binary, 0, lenght_binary);
    printf("decimal : %d\n", decimal_test);

    // 3 bits for the packet version 
    int packet_version = convert_base_2_into_base_10(binary, 0, 3);
    printf("packet_version : %d\n", packet_version);

    // 3 bits for the packet type ID
    int packet_type_id = convert_base_2_into_base_10(binary, 3, 6);
    printf("packet_type_id : %d\n", packet_type_id);

    // after that, we group the bits by 5 and check if the first bit is a 1 or a 0 (end of the number)
    // we then convert the 4 bits remaining into a decimal number
    printf("\n");

    int starting_bit = 6;
    int length_of_group = 5;
    int number_of_groups = (lenght_binary - starting_bit)/length_of_group;

    // to store the remaining bits after processing as the puzzle asks
    int* binary_remaining = malloc(NUMBER_OF_BITS*number_of_groups*sizeof(int));
    int index = 0; // at the end, it represents the number of bits we have in the binary_remaining array

    // printf("number_of_groups : %d\n", number_of_groups);
    for (int i = 0; i < number_of_groups; i++){
        
        int start = starting_bit + i*length_of_group;
        // printf("start : %d\n", start);

        if (binary[start] == 1){
            
            // getting the 4 bits behind the first bit
            int* binary_number = (int*)malloc(NUMBER_OF_BITS*sizeof(int));
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                binary_number[j] = binary[start + j + 1];
            }

            // printing the binary number we got 
            printf("binary %d : ",i);
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                printf("%d", binary_number[j]);
            }
            printf("\n");

            // putting the binary number into the binary_remaining array at the end 
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                binary_remaining[index] = binary_number[j];
                index++;
            }

            // free the memory
            free(binary_number);
        }

        else if (binary[start] == 0){

            // getting the 4 bits behind the first bit
            int* binary_number = (int*)malloc(NUMBER_OF_BITS*sizeof(int));
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                binary_number[j] = binary[start + j + 1];
            }

            // printing the binary number we got 
            printf("binary %d : ",i);
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                printf("%d", binary_number[j]);
            }
            printf("\n");

            // putting the binary number into the binary_remaining array at the end 
            for (int j = 0; j < NUMBER_OF_BITS; j++){
                binary_remaining[index] = binary_number[j];
                index++;
            }

            // free the memory
            free(binary_number);

            // breaking the loop since we're done when a 0 is encountered
            printf("end of the number\n");
            break;

        }
        
    }
    
    // printing the binary_remaining array to see it 
    printf("binary_remaining : ");
    for (int i = 0; i < index; i++){
        printf("%d", binary_remaining[i]);
    }
    printf("\n");

    // converting the binary_remaining array into a decimal number to see if it matches the expected output
    int decimal = convert_base_2_into_base_10(binary_remaining, 0, index);
    printf("decimal of binary_remaining : %d\n", decimal);







    // free the memory
    free(binary);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}