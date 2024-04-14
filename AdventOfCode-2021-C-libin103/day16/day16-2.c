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
#define lenght 1370
int binary_array[lenght*NUMBER_OF_BITS];

// function that calculate the min of 2 numbers
int min(int a, int b){
    if (a<b){
        return a;
    }
    return b;
}
// function that calculate the max of 2 numbers
int max(int a, int b){
    if (a>b){
        return a;
    }
    return b;
}


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


// function that returns the number corresponding to the type_ID and the value array given 
// for example, if the type_ID is 0, it returns the sum of the values in the values array
int get_the_final_value_for_operator_packet(int type_ID, int* values, int lenght_values){
    int result=0;
    // making the switch depending on the different type_ID
    
    // sum all the values 
    if (type_ID == 0){
        int sum=0;
        for (int i=0; i<lenght_values; i++){
            sum+=values[i];
        }
        result=sum;
    } 
        
    // returns the products of the values 
    else if (type_ID == 1){
        int product=1;
        for (int i=0; i<lenght_values; i++){
            product*=values[i];
        }
        result=product;
    }

    // return the min of the values
    else if (type_ID == 2){
        int min_value=values[0];
        for (int i=1; i< lenght_values; i++){
            min_value=min(min_value,values[i]);
        }
        result = min_value;
    }

    // return the max of the values
    else if (type_ID == 3){
        int max_value=values[0];
        for (int i=1; i< lenght_values; i++){
            max_value=max(max_value,values[i]);
        }
        result = max_value;
    }

    // no need to treat type_ID=4 since it does different things and we don't need this function for it 

    // compare the two differents values of the values array, but there is the need for it to be only of lenght 2
    else if (type_ID == 5){
        int greater=0;
        if (lenght_values ==2){
            if (values[0] > values[1]){
                greater =1;
            }
        }
        result = greater;
    }

    // compare the two differents values of the values array, but there is the need for it to be only of lenght 2
    else if (type_ID == 6){
        int less=0;
        if (lenght_values ==2){
            if (values[0] < values[1]){
                less =1;
            }
        }
        result = less;
    }
    
    // compare the two differents values of the values array, but there is the need for it to be only of lenght 2
    else if (type_ID == 7){
        int equal=0;
        if (lenght_values ==2){
            if (values[0] == values[1]){
                equal =1;
            }
        }
        if (lenght_values == 3){
            if (values[0] + values[1] == values[2]){
                equal =1;
            }
        }
        result = equal;
    }

    return result;
}



// function that parse the binary array input and return the answer of the puzzle 
// i represents the starting index of the parsing 
// j represents the end of the parsing
// we will not parse the data each time and gets some parts, we will update some parameters (i and j) to know where we're in the data
long int* parsing (int starting_index, int* ending_index, int real_lenght_binary_array){
    
    if (starting_index == *ending_index){
        return NULL;
    }

    // if there isn't enough space to parse the data
    if (starting_index > real_lenght_binary_array-6){
        return NULL;
    }

     // 3 bits for the packet version 
    int packet_version = convert_base_2_into_base_10(binary_array, starting_index, starting_index+3);
    printf("packet_version : %d\n", packet_version);

    // 3 bits for the packet type ID
    int packet_type_id = convert_base_2_into_base_10(binary_array, starting_index+3, starting_index+6);
    printf("packet_type_id : %d\n", packet_type_id);

    // if this is a literal packet
    if (packet_type_id==4){
        starting_index+=6;
        int length_of_group = 5;
        int number_of_groups = (real_lenght_binary_array - starting_index)/length_of_group;

        int index_remaining = 0;
        int* binary_treated = malloc(NUMBER_OF_BITS*number_of_groups*sizeof(int));

        // printf("number_of_groups : %d\n", number_of_groups);
        // when we have a zero at the beginning of the group, we're at the end of the packet, and then we break the loop
        bool is_ending = false;
        while (is_ending == false){
            // if we're at the last packet
            if (binary_array[starting_index]==0){
                is_ending=true;
            }
            // getting the 4 bits of the group after the first bit and adding them to the binary_treated array
            for (int j=0; j<NUMBER_OF_BITS; j++){
                binary_treated[index_remaining] = binary_array[starting_index+j+1];
                index_remaining++;
            }
            starting_index+=5; // implementing the index_array, to know until where we've explored the binary_array
        }

        // printing the binary_remaining array
        printf("binary_treated : ");
        for (int i = 0; i < index_remaining; i++){
            printf("%d", binary_treated[i]);
        }
        printf("\n");

        // value to see things
        int value = convert_base_2_into_base_10(binary_treated, 0, index_remaining);
        printf("value : %d\n", value);

        free(binary_treated);

        // result is the value and the starting_index in which we stopped to get this value / result 
        long int* result = (long int*)malloc(2*sizeof(long int));
        result[0] = value;
        result[1] = starting_index;
        return result;

    }    

    // otherwise it's an operator packet 
    int* sub_packets = (int*)malloc(real_lenght_binary_array*sizeof(int));
    int index_sub_packets = 0;
    int next_starting_index = -1;

    // getting what type of action we need to do with the packet depending on this value
    int len_type_ID = binary_array[starting_index+6];

    // if the packet is of len_type 0 : the 15 next bits represent the number of bits that are inside the next packet
    // if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
    if (len_type_ID == 0){ 
        int number_of_bits = convert_base_2_into_base_10(binary_array, 7, 22);
        printf("number_of_bits : %d\n", number_of_bits);
        int ending_index_new = starting_index+22+number_of_bits;
        int index=starting_index+22;
        int previous_index = 0;
        // index is -1 if we go out of the packet function, if i==j or if we don't have enough space to parse the data
        while (index != -1){
            previous_index = index;
            long int* result = parsing(index, &ending_index_new, real_lenght_binary_array);
            if (result != NULL){
                sub_packets[index_sub_packets] = result[0];
                index_sub_packets++;
                index = result[1];
            }
            else {
                index = -1;
            }
            free(result);
        }
        next_starting_index = previous_index;
    }

    // if the packet is of len_type 1 : the 11 next bits represent the number of bits that are inside the next packet
    // if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
    else if (len_type_ID == 1){
        // getting the number of packets that are inside the packet, that we will make the parsing on
        int number_of_packets = convert_base_2_into_base_10(binary_array, 7, 18);
        printf("number_of_packets : %d\n", number_of_packets);
        int index=starting_index+18;
        int j=-1;
        while (number_of_packets > 0){
            long int* result = parsing(index, &j, real_lenght_binary_array);
            if (result != NULL){
                sub_packets[index_sub_packets] = result[0];
                index_sub_packets++;
                index = result[1];
            }
            else {
                index = -1;
            }
            number_of_packets--;
            free(result);
        }
        next_starting_index = index;
    }


    printf("sub_packets : ");
    for (int i = 0; i < index_sub_packets; i++){
        printf("%d ", sub_packets[i]);
    }
    printf("tested\n");


    long int* result = (long int*)malloc(2*sizeof(long int));
    result[0] = get_the_final_value_for_operator_packet(packet_type_id, sub_packets, index_sub_packets);
    printf("result : %ld\n", result[0]);
    if (packet_type_id==7 && result[0]%2==0){
       result[0]=1;
    }
    result[1] = next_starting_index;

    return result;

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

    FILE* file = fopen("test12.txt", "r");
    // getting 1500 since the line is 1370 characters long for my input
    char line[1500];
    fgets(line, sizeof(line), file);
    int lenght_array = strlen(line);

    // we're now converting the line into a binary array
    // each character is written in base 16, we need to convert it into base 2
    int lenght_binary = lenght_array*NUMBER_OF_BITS;
    for (int i = 0; i < lenght_array; i++){
        int* converted = convert_base_16_into_base_2(line[i]);
        for (int j = 0; j < NUMBER_OF_BITS; j++){
            binary_array[i*NUMBER_OF_BITS + j] = converted[j];
        }
        free(converted);
    }
    /*
    // printing the binary array to check if it's working as expected
    printf("binary array : ");
    for (int i = 0; i < lenght_binary; i++){
        printf("%d", binary_array[i]);
    }
    printf("\n\n");
    */
    
    // parsing the binary array to get the answer of the puzzle
    int ending_index=-1;
    long int* answer = parsing(0, &ending_index, lenght_binary);
    printf("\nThe answer of the puzzle is : %ld\n\n", answer[0]);

    // free the memory
    free(answer);

    // close the file
    fclose(file);

    return EXIT_SUCCESS;
}

// It first does work on test_type4.txt to get the right value for the answer of the puzzle (packet_type_id=4 works fine)
// I found 3 for test5.txt, which is the right answer for the puzzle
// I found 54 for test6.txt, which is the right answer for the puzzle
// I found 7 for test7.txt, which is the right answer for the puzzle
// I found 9 for test8.txt, which is the right answer for the puzzle
// I found 1 for test9.txt, which is the right answer for the puzzle
// I found 0 for test10.txt, which is the right answer for the puzzle
// I found 0 for test11.txt, which is the right answer for the puzzle
// I found 1 for test12.txt, which is not the right answer for the puzzle : seems not to work for me, since it needed to make chnages in the code to make it work, not with much sense 
// the python code that is equivalent works fine for me so this strange, I might have miss so
// I don't know why i've a seg fault for the input16.txt, so it doesn't work on it, or an infinite loop, I don't know why