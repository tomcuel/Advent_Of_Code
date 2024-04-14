#include <stdio.h> // pour le printf
#include <stdlib.h> // pour le scanf
#include <math.h> // pour les fonctions mathématiques 
#include <string.h> // pour les chaînes de caractères, strlen par exemple
#include <stdbool.h> // pour les booléens 
#include <time.h> // pour les randint 
#include <limits.h> // pour les limites
#include <ctype.h> // pour isalpha 
#include <unistd.h> // pour getpid  


// function that returns the minimum of two numbers
int min(int a, int b){
    if (a<b){
        return a;
    }
    else{
        return b;
    }
}
// function that returns the maximum of two numbers
int max(int a, int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}

// function that make one step with the velocity
// it modifies it to keep considering the gravity and the drag affecting vy and vx
int** make_one_step(int x, int y, int vx, int vy){
    int** result = (int**)malloc(2*sizeof(int*));
    result[0] = (int*)malloc(2*sizeof(int));
    result[1] = (int*)malloc(2*sizeof(int));
    
    // creating the new position
    result[0][0] = x+vx;
    result[0][1] = y+vy;

    // creating the new velocity
    result[1][1] = vy-1;
    // taking care of the differents conditions concerning the drag
    if (vx>0){
        result[1][0] = vx-1;
    }
    else if (vx<0){
        result[1][0] = vx+1;
    }
    else{
        result[1][0] = 0;
    }

    return result;
}


// function that returns wether the position is in the target area
bool is_in_target(int x, int y, int* target_x_range, int* target_y_range){
    if (x>=target_x_range[0] && x<=target_x_range[1] && y>=target_y_range[0] && y<=target_y_range[1]){
        return true;
    }
    else{
        return false;
    }
}


// funciton to tell if we've already passed the target area
bool has_past_target(int x, int y, int vx, int vy, int* target_x_range, int* target_y_range){
   // if we're on the right side of the target area and the velocity is positive (going right)
   if (vx>0 && x > target_x_range[1]){
       return true;
   }
    // if we're on the left side of the target area and the velocity is negative (going left)
   else if (vx<0 && x < target_x_range[0]){
       return true;
   }
   // if we're below the target area and the velocity is positive (going down)
   else if (vy<0 && y < target_y_range[0]){
       return true;
   }
   else{
       return false;
   }
}


int* does_hit_target(int x_velocity, int y_velocity, int* target_x_range, int* target_y_range){
    int* result=(int*)malloc(2*sizeof(int));

    int pos_x = 0;
    int pos_y = 0;
    int max_y = 0;
    // while we haven't hit the target area and we haven't past it
    while (has_past_target(pos_x,pos_y,x_velocity,y_velocity,target_x_range,target_y_range)==false){
        max_y=max(max_y,pos_y);
        if (is_in_target(pos_x,pos_y,target_x_range,target_y_range)==true){
            // we've hit the target area, same as a true
            result[0]=1;
            // we return the maximum y we've got, even if in this part of the code, we don't need it
            result[1]=max_y;
            return result;
           
        }
        // updating the position and the velocity according to the rules
        int** new_pos = make_one_step(pos_x,pos_y,x_velocity,y_velocity);
        pos_x = new_pos[0][0];
        pos_y = new_pos[0][1];
        x_velocity = new_pos[1][0];
        y_velocity = new_pos[1][1];
        free(new_pos[0]);
        free(new_pos[1]);
        free(new_pos);
    }

    // we've past the target area, same as a false
    result[0]=0;
    result[1]=0;
    return result;
}


int main (){

    // getting the input not by opening the file this time 
    /*
    // for the test.txt file
    int x1=20;
    int x2=30;
    int y1=-10;
    int y2=-5;
    */
    
    // for the input17.txt file
    int x1=139;
    int x2=187;
    int y1=-148;
    int y2=-89;
    

    int* target_x_range = (int*)malloc(2*sizeof(int));
    target_x_range[0] = x1;
    target_x_range[1] = x2;
    int* target_y_range = (int*)malloc(2*sizeof(int));
    target_y_range[0] = y1;
    target_y_range[1] = y2;


    // beginning by the maximum y velocity possible in order to reach the target area at some point
    int max_velocity = abs(target_y_range[0]);
    int max_y = 0;

    // counting the number of velocities that work
    int number_of_velocity_that_works = 0;

    int y_velocity = max_velocity;
    while (y_velocity>=target_y_range[0]){
        // going trough all the x velocities to see if one is making the y velocity reach the target area
        for (int x_velocity = -100; x_velocity<=201; x_velocity++){
            // has_hit[0] says if we've hit the target area, has_hit[1] get us the maximum y we've got
            int* has_hit=does_hit_target(x_velocity,y_velocity,target_x_range,target_y_range);
            if (has_hit[0]==1){
                max_y = has_hit[1];
                number_of_velocity_that_works++;
            }
            free(has_hit);
        }

        

        // we need after this part of the loop to look at the y_velocity-1 to see if we can reach the target area with it
        y_velocity--;
    }

    // printing the number of velocities that work
    printf("\nThe number of velocities that work is %d\n\n",number_of_velocity_that_works);


    // free the memory
    free(target_x_range);
    free(target_y_range);

    return EXIT_SUCCESS;
}

// we found 112 velocities that works for the test.txt file as expected in the example