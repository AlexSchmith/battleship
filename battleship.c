/*
*   @Author: Alexander Schmith
*   @Title: battleship.c
*   @Description: This code is a single player version of battleship (since having two players on the 
*   same computer would ruin the aspect of not knowing the opponents board). Using a generated map from .txt file, 
*   the player will keep guessing until either his shot limit runs out, or he sinks all of the ships
*
*
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void read_map(FILE *filename_read, FILE *filename_error, int shot_limit, int *battleships,  int row, int col, char map[row][col]); //returns the number of battleships
void print_map(int row, int col, char map[row][col]);
int play_game(int shot_limit, int *battleships, int row, int col, char map[row][col]);
int shoot(int row, int col, int x, int y, char map[row][col]);

/* 
    first commmand line argument is the limit of shots that you can take 
    second command line argument is the name of the file that stores the opponents map
*/
int main(int argc, char *argv[]){

    /*creates two different file readers on for an error checker and one to read the map*/
    FILE *fin_read = fopen(argv[2], "r"); 
    FILE *fin_error = fopen(argv[2], "r");
    /* amount of shots the player takes before the game is over */
    int shots = atoi(argv[1]);
    /* number of battleships in the map */
    int battleships = 0;
    /* row and column for the map is read in through the file */
    int row;
    int col;
    

    fscanf(fin_read, "%d %d", &row, &col);

    char map[row][col];

    read_map(fin_read, fin_error, shots, &battleships, row, col, map);

    if(play_game(shots, &battleships, row, col, map)){
        printf("Captain, we have sunk all the battleships. You win!\n");
    }
    else{
        printf("Captain, we ran out of ammo, you lose!\n", battleships);
        /* printf("There were %d battleships remaining\n", battleships); */
    }

    return 0;
}




/*
*   function to read in the map from a file and put it into a 2d array
*   this function also checks the map for errors and checks how many targets and if the 
*   shot limit is greater than or equal to the amount of battleships
*   
*/
void read_map(FILE *filename_read, FILE *filename_error, int shot_limit, int *battleships, int row, int col, char map[row][col]){


    /* error checking */
    int row_error;
    int row_count = 0;
    int col_error;
    char buffer[100];

    fscanf(filename_error, "%d %d", &row_error, &col_error);

    /* loop that gets the true line of the map and makes sure that it has the right amount of space in the column area */
    for(int i = 0; i < row_error; i++){
        fscanf(filename_error, "%s", buffer);
        

        if(strlen(buffer) != col_error){
            printf("There is a problem with the columns in this map\n");
            exit(-1);
            
        }
        
        
        if(strlen(buffer) != 0){
            row_count++;
        }
    }
    

    if(row_count < row_error){
        printf("There is a problem with the columns in this row\n");
        exit(-1);
        
    }



    /* checks if the row has continued and if it hasn't even if the file has ended, the map is generated normally */
    buffer[0] = '\0';

    fgets(buffer, col_error, filename_error);
    
    if(strlen(buffer) != 0){
        printf("There are too many rows\n");
        exit(-1);
        
    }





    /* read map to file */
    for(int i = 0; i < row; i++){
        
        fscanf(filename_read, "%s", &map[i]);
        for(int k = 0; k < col; k++){
            if(map[i][k] == 'B'){
                (*battleships)++;
            }
            else{
                map[i][k] == '~';  /*If a part of the map isn't a battleship, it sets it to ~ as the default*/
            }
        }
        
        fscanf(filename_read, "\n");
    }
    
    

    
    /* checks if the shot limit is reasonable such as when you can't win the game or you can't lose the game */
    if( (*battleships) > shot_limit){
        printf("The shot limit is too low\n");
        exit(-1);
    }
    else if( row * col <= shot_limit){
        printf("The shot limit is too high\n");
        exit(-1);
    }

    
}




void print_map(int row, int col, char map[row][col]){
    

    /*for (int i = 0; i < col; i++){
        printf("%d ", i);
    }
    printf("\n");
    */
    for(int i = 0; i < row; i++){
        //printf("%d ", i);
        for(int k = 0; k < col; k++){
            printf("%c ", map[i][k]);
            
        }
        printf("\n");
    }
    printf("\n\n");
}




int play_game(int shot_limit, int *battleships, int row, int col, char map[row][col]){
    char unrevealed_map[row][col];
    /* coordinates for the player to choose where to shoot */
    int x, y;

    /* Displays the table for the row*/
    for (int i = 0; i < row; i++){
        for (int k = 0; k < col; k++){
            unrevealed_map[i][k] = '~';
        }
    }

    

    while(shot_limit > 0){
        print_map(row, col, unrevealed_map);

        if ((*battleships) <= 0){
            break;
        }

        printf("%d shots remaining\n", shot_limit);
        printf("Captain please enter the coordinates: ");
        scanf("%d %d", &x, &y);

        if((unrevealed_map[x][y] == 'H')||(unrevealed_map[x][y] == 'M')){
            continue;
        }

        if((x >= row || y >= col)||(x < 0 || y < 0)){
            printf("Captain out of range.\n");
            continue;
        }

        if(shoot(row, col,  x, y, map)){
            unrevealed_map[x][y] = 'H';
            (*battleships)--;
        }
        else{
            unrevealed_map[x][y] = 'M';
        }


        shot_limit--;
    
    }
    
    if((*battleships) <= 0){
        return 1;
    }

    return 0;

}




int shoot(int row, int col, int x, int y, char map[row][col]){

    if(map[x][y] == 'B'){
        printf("HIT!\n");
        return 1;
    }
    else{
        printf("MISS!\n");
    }
    
    return 0;

}
