/* we create a struct that gives us to access certain elements specific to certain players, 
such as the number of the penguins they have left and their positions, and their current points(fish)
*/
#include <stdbool.h>

#define MAX_NAME_SIZE 50
#define MAX_PENGUINS 10

typedef struct {
    char player_name[MAX_NAME_SIZE];
    int playerNumber;
    int num_pengwings;
    int fish;
    int playerID;
    struct Penguin {
        int peng_row;
        int peng_col;
    } Penguin [MAX_PENGUINS]; 
} Player;

/* we define a structure which we can later get columns, rows and the amount of tiles from. We also declare the char penguins, later used
for positioning the penguins */
struct board 
{
    int columns, rows;
    int *tiles;
    char *penguins;
};

/* it shuffles the elements of an array (board_array) of which we declare its size (the amount of tiles on the board), then we use a parameter
 which we later assign rand to */
void shuffle( int * board_array, int size, int( * random )(void));

/* we use the structure declared previously to create a board with a certain width, height and the last parameter checks how many positions there HAS to be
so the ones with number 1 has to be equal to the total amount of penguins */
struct board *createBoard( int board_width, int board_height, int number_of_positions );

// we print the board that we declared earlier
void printBoard( struct board *gameBoard );

//using the board structure of the gameBoard (previously declared, check up), counting the players to check whose turn it is
void placePenguin(struct board *gameBoard, int playerIndex, int row, int column);

// This function allows each player to place each penguin they currently still have. 

void placePenguins(struct board *gameBoard, Player players[], int num_players, int num_pengwings);
bool canPenguinMove(struct board *gameBoard, Player players, int peng_id);


//Allows the player to move their penguin. 
//Currently we are working on resolving a bug that doesn't properly loop to the next penguins, and instead moves peng 1 over and over.
void movementPhase(struct board *gameBoard, Player players[], int num_players, int num_pengwings);
#ifndef LIBRARY_H
#define LIBRARY_H

void saveGameProgress(struct board *gameBoard, Player players[], int num_players, int num_pengwings, const char *filename);

void loadGameProgress(struct board *gameBoard, Player players[], int *num_players, int *num_pengwings, const char *filename);


// Function declarations for saving and loading game progress
//void saveGameProgress(struct board *gameBoard, Player players[], int num_players, int num_pengwings, const char *filename);
//void loadGameProgress(struct board *gameBoard, Player players[], int *num_players, int *num_pengwings, const char *filename);

void gettingData(int *board_width, int *board_height, int *num_players, int *num_pengwings, bool *valid_input);
Player tallyWinner(Player players[], int num_players);

#endif
