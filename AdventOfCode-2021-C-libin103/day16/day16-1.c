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
long long int convert_base_2_into_base_10(int* binary, int start, int end){
    long long int decimal = 0;
    for (int i = start; i < end; i++){
        // using horner formula for polynomial evaluation
        decimal = decimal*2 + binary[i];
    }
    return decimal;
}


// function to convert a decimal number into a binary array
int* convert_base_10_into_base_2(long long int decimal){
    int* binary = (int*)malloc(NUMBER_OF_BITS*sizeof(int));
    for (int i = NUMBER_OF_BITS - 1; i >= 0; i--){
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }
    return binary;
}


// function that parse the binary array input and return the answer of the puzzle 
// count represents the number of packets that are inside the packet we're parsing and that we need to parse
int parsing(int* binary_array, int length, int* count){

    long long int binary_array_number = convert_base_2_into_base_10(binary_array, 0, length);
    // printf("binary_array_number : %lld\n", binary_array_number);

    // printing the binary array
    printf("binary array : ");
    for (int i = 0; i < length; i++){
        printf("%d", binary_array[i]);
    }
    printf("\n");

    // if there is no number in the binary array, nothing is to be done
    if (binary_array_number == 0 || length < 6){
        return 0;
    }

    // if nothing is to be done, we return 
    if (count == 0){
        *count=-1;
        return parsing(binary_array, length, count);
    }

    // 3 bits for the packet version 
    int packet_version = convert_base_2_into_base_10(binary_array, 0, 3);
    printf("packet_version : %d\n", packet_version);

    // 3 bits for the packet type ID
    int packet_type_id = convert_base_2_into_base_10(binary_array, 3, 6);
    printf("packet_type_id : %d\n", packet_type_id);

    int starting_bit = 6;


    // if the packet is of type 4
    if (packet_type_id==4){
        // gets the index of the bit xe're in 
        int index_array = 6;
        int length_of_group = 5;
        int number_of_groups = (length - index_array)/length_of_group;

        int index_remaining = 0;
        int* binary_treated = malloc(NUMBER_OF_BITS*number_of_groups*sizeof(int));

        // printf("number_of_groups : %d\n", number_of_groups);
        // when we have a zero at the beginning of the group, we're at the end of the packet, and then we break the loop
        bool is_ending = false;
        while (is_ending == false){
            // if we're at the last packet
            if (binary_array[index_array]==0){
                is_ending=true;
            }
            // getting the 4 bits of the group after the first bit and adding them to the binary_treated array
            for (int j=0; j<NUMBER_OF_BITS; j++){
                binary_treated[index_remaining] = binary_array[index_array+j+1];
                index_remaining++;
            }
            index_array+=5; // implementing the index_array, to know until where we've explored the binary_array
        }

        // printing the binary_remaining array
        printf("binary_treated : ");
        for (int i = 0; i < index_remaining; i++){
            printf("%d", binary_treated[i]);
        }
        printf("\n");

        // value to see things, not useful here
        int value = convert_base_2_into_base_10(binary_treated, 0, index_remaining);
        printf("value : %d\n", value);

        // what's left in the binary array, that has not been explored
        int* binary_remaining = malloc((length - index_array)*sizeof(int));
        for (int i = 0; i < length - index_array; i++){
            binary_remaining[i] = binary_array[index_array + i];
        }

        // updating the count and returning the value of the packet
        *count -=1;
        return (packet_version + parsing(binary_remaining, length-index_array, count));

    }

    // in every others cases 
    else {
        // getting this parameter
        int len_type_ID = binary_array[6];
        // if the packet is of type 0 : the 15 next bits represent the number of bits that are inside the next packet
        // if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
        if (len_type_ID == 0 && length -7 - 15 > 0){ 
            int number_of_bits = convert_base_2_into_base_10(binary_array, 7, 22);
            printf("number_of_bits : %d\n", number_of_bits);

            // getting the first part with number_of_bits bits
            int* binary_remaining_P1 = malloc(number_of_bits*sizeof(int));
            for (int i = 0; i < number_of_bits; i++){
                binary_remaining_P1[i] = binary_array[22 + i];
            }

            // printing the binary_remaining_P1 array
            printf("binary_remaining_P1 : ");
            for (int i = 0; i < number_of_bits; i++){
                printf("%d", binary_remaining_P1[i]);
            }
            printf("\n");

            // getting the second part with all the remaining bits
            int* binary_remaining_P2 = malloc((length-22-number_of_bits)*sizeof(int));
            for (int i = 0; i < length-22-number_of_bits; i++){
                binary_remaining_P2[i] = binary_array[22 + number_of_bits + i];
            }

            // printing the binary_remaining_P2 array
            printf("binary_remaining_P2 : ");
            for (int i = 0; i < length-22-number_of_bits; i++){
                printf("%d", binary_remaining_P2[i]);
            }
            printf("\n");

            // updating the count and returning the value of the packets
            // we have to packets to parse, so we update the count twice
            // first count is updated it with -1 since we cut one part
            // second count since it's a big one, it will need many parse, so we update it with the number of bits that are inside the packet -1 because we cut one part
            int count1_int=-1;
            int* count1=&count1_int;
            int count2_int=*count-1;
            int* count2=&count2_int;
            return (packet_version + parsing(binary_remaining_P1, number_of_bits, count1) + parsing(binary_remaining_P2, length-number_of_bits-22, count2));
        }

        // if the packet is of type 1 : the 11 next bits represent the number of bits that are inside the next packet
        // if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
        if (len_type_ID == 1 && length -7 - 11 > 0){
            // getting the number of packets that are inside the packet, that we will make the parsing on
            int number_of_packets = convert_base_2_into_base_10(binary_array, 7, 18);
            printf("number_of_packets : %d\n", number_of_packets);
            
            // updating what's left in the binary array to be explored
            int* binary_remaining = malloc((length - 18)*sizeof(int));
            for (int i = 0; i < length - 18; i++){
                binary_remaining[i] = binary_array[18 + i];
            }
            
            // updating the count and returning the value of the packets
            *count += number_of_packets;
            return (packet_version + parsing(binary_remaining, length-18, count));
        }

        // if we have not gone through those conditions, we return the packet version, since we're at the end of the packet parsing
        return packet_version;

    }


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

    FILE* file = fopen("input16.txt", "r");
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

    // getting the answer of the puzzle
    int count = -1;
    int answer = parsing(binary, lenght_binary, &count);
    // printing the answer
    printf("\nAnswer : %d\n\n", answer);



    // free the memory
    free(binary);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}


// It does work as expected : 
// for test_type4.txt, the answer is 6 with a value of 2021 to end the recursion
// for test_type_ID_bit_0.txt, the answer is 9 and every parsing is correct
// for test_type_ID_bit_1.txt, the answer is 14 and every parsing is correct
// for test1.txt, the answer is 16 as expected
// for test2.txt, the answer is 12 as expected
// for test3.txt, the answer is 23 as expected
// for test4.txt, the answer is 31 as expected