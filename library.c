#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "library.h"
#include <string.h>
void shuffle( int * board_array, int size, int( * random )(void))
{
    for( int i = size - 1; i > 0; i-- )
    {
        int j = random() % (i + 1);
        int temp = board_array[i];
        board_array[i] = board_array[j];
        board_array[j] = temp;
    }
}


struct board *createBoard( int board_width, int board_height, int number_of_positions )
{
    int board_tiles = board_height*board_width;
    struct board *return_board = malloc(sizeof(struct board));
    return_board->columns = board_width;
    return_board->rows = board_height;
    return_board->tiles = malloc(board_tiles*sizeof(int));
    return_board->penguins = malloc(board_tiles*sizeof(char));
    srand(time(0));

    for( int i = 0; i < board_tiles; i++ )
    {
        return_board->penguins[i] = ' ';
        if ( i <  number_of_positions )
        {
            return_board->tiles[i] = 1;
        }
        else
        {
            return_board->tiles[i] = rand() % 4;
        }
    }
    shuffle( return_board->tiles, board_tiles, rand );

    return return_board;
}

void printBoard( struct board *gameBoard )
{
    for( int i = 0; i < gameBoard->rows; i++ )
    {
        for( int j = 0; j < gameBoard->columns; j++){
            int tileIndex = i * gameBoard->columns + j;
            printf("%c ", gameBoard->penguins[tileIndex] != ' ' ? gameBoard->penguins[tileIndex] : gameBoard->tiles[tileIndex] + '0');
        }
        printf("\n");
    }
}

void placePenguin(struct board *gameBoard, int playerIndex, int row, int column) {
    gameBoard->penguins[row * gameBoard->columns + column] = 'A' + playerIndex;
}

void placePenguins(struct board *gameBoard, Player players[], int num_players, int num_pengwings) {
    int peng_row, peng_col;

    for (int x = 0; x < num_pengwings; x++) {
        for (int i = 0; i < num_players; i++) {
            int peng_count = 0;
            for (int k = 0; k < gameBoard->rows*gameBoard->columns; k++){
                if(gameBoard->penguins[k] == 'A'+ i){
                    peng_count++;
                }

            }
            if (peng_count >= num_pengwings){
                continue;
            }
            int validPlacement = 0;

            bool tileOccupied = false;

            while (!validPlacement) {
                printf("Player %s, enter the coordinates you want to place penguin %d at: \n", players[i].player_name, peng_count+1);
                printf("row: ");
                scanf("%d", &peng_row);
                printf("column: ");
                scanf("%d", &peng_col);

                int tileIndexPlacementPhase = (peng_row - 1) * gameBoard->columns + (peng_col - 1);

                if (peng_row < 1 || peng_row > gameBoard->rows || peng_col < 1 || peng_col > gameBoard->columns) {
                    printf("Invalid row or column. Please try again.\n");
                } else if (gameBoard->tiles[tileIndexPlacementPhase] == 0) {
                    printf("You can't place penguins on a water spot. Please try again.\n");
                } else if (gameBoard->tiles[tileIndexPlacementPhase] != 1) {
                    printf("You can't collect more than one fishy on your first move! \n");
                } else if (tileOccupied || gameBoard->penguins[tileIndexPlacementPhase] != ' ') {
                    printf("You can't place your penguin there, it's already occupied. Please try again.\n");
                } else {
                    validPlacement = 1;
                    players[i].fish += gameBoard->tiles[tileIndexPlacementPhase];
                    placePenguin(gameBoard, i, peng_row - 1, peng_col - 1);
                    players[i].Penguin[x].peng_row = peng_row - 1;
                    players[i].Penguin[x].peng_col = peng_col - 1;
                    gameBoard->tiles[tileIndexPlacementPhase] = 0;
                    printBoard(gameBoard);
                    printf("Penguin placed!\n");
                    saveGameProgress(gameBoard, players, num_players, num_pengwings, "save.txt");
                }
            }
        }
    }
    printf("All penguins have been placed!\n");
}

bool CanPenguinMoveThere(struct board* b, int peng_row, int peng_col, int choice_row, int choice_col)
{
    int dr = choice_row - peng_row;
    int dc = choice_col - peng_col;
    if (dr < 0) dr = -1;
    else if (dr > 0) dr = 1;
    if (dc < 0) dc = -1;
    else if (dc > 0) dc = 1;
    int c_row = peng_row + dr;
    int c_col = peng_col + dc;
    while (c_row!=choice_row||c_col!=choice_col)
    {
        if (b->tiles[c_row * b->columns + c_col] == 0) {
            return false;
        }
        c_row += dr;
        c_col += dc;
    }
    return true;
}

bool canPenguinMove(struct board *gameBoard, Player players, int peng_id){
     int peng_row = players.Penguin[peng_id].peng_row;
        int peng_col = players.Penguin[peng_id].peng_col;

        if (peng_col > 0) {
            switch (gameBoard->tiles[peng_row * gameBoard->columns + (peng_col - 1)]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_col < gameBoard->columns - 1) {
            switch (gameBoard->tiles[peng_row * gameBoard->columns + (peng_col + 1)]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_row > 0) {
            switch (gameBoard->tiles[(peng_row - 1) * gameBoard->columns + peng_col]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_row < gameBoard->rows - 1) {
            switch (gameBoard->tiles[(peng_row + 1) * gameBoard->columns + peng_col]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }
        return false;
}

bool canPlayerMove(struct board *gameBoard, Player players, int num_pengwings) {
    for (int x = 0; x < num_pengwings; x++) {
        int peng_row = players.Penguin[x].peng_row;
        int peng_col = players.Penguin[x].peng_col;

        if (peng_col > 0) {
            switch (gameBoard->tiles[peng_row * gameBoard->columns + (peng_col - 1)]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_col < gameBoard->columns - 1) {
            switch (gameBoard->tiles[peng_row * gameBoard->columns + (peng_col + 1)]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_row > 0) {
            switch (gameBoard->tiles[(peng_row - 1) * gameBoard->columns + peng_col]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }

        if (peng_row < gameBoard->rows - 1) {
            switch (gameBoard->tiles[(peng_row + 1) * gameBoard->columns + peng_col]) {
                case 1:
                    return true;
                case 2:
                    return true;
                case 3:
                    return true;
            }
        }
    }

    return false;
}


bool canAnyoneMove(struct board *gameBoard, Player players[], int num_players, int num_pengwings){
    for( int i = 0; i < num_players; i++ ){
       if ( canPlayerMove(gameBoard, players[i], num_pengwings) == true) return true;
    
    }
    
    return false;
}



//Allows the player to move their penguin.
//Currently we are working on resolving a bug that doesn't properly loop to the next penguins, and instead moves peng 1 over and over.

void movementPhase(struct board *gameBoard, Player players[], int num_players, int num_pengwings) {
    int peng_row, peng_col, peng_id;
    int player_counter = num_players;

    while (canAnyoneMove(gameBoard, players, num_players, num_pengwings) == true){
        for (int i = 0; i < num_players; i++) {
            int validPenguin = 0;
            bool tileOccupied = false;

            if( canPlayerMove(gameBoard, players[i], num_pengwings) == false)
                printf("\nSkipping player %s...\n\n", players[i].player_name);
            else{
                int tileIndexMovementPhase;
                while (!validPenguin) {
                    printf("Player %s, enter the coordinates of the penguin you want to move: ", players[i].player_name);
                    scanf("%d %d", &peng_row, &peng_col);
                    getchar();
                    
                    tileIndexMovementPhase = (peng_row - 1) * gameBoard->columns + (peng_col - 1);
                    int tempTile;
                    for(int j = 0; j < num_pengwings; j++){
                        if((peng_row - 1 == players[i].Penguin[j].peng_row) && (peng_col - 1 == players[i].Penguin[j].peng_col )){
                            peng_id = j;
                        }
                    }
                    if (gameBoard->penguins[tileIndexMovementPhase] != 'A' + i) {
                        printf("Invalid row or column. Please try again.\n");
                    } 
                    else if(canPenguinMove(gameBoard, players[i], peng_id) == false){
                        printf("YES, GREAT IDEA, move the one penguin who is stuck on an ice floe. Real smart.\n\n");
                    }
                    else {
                        validPenguin = 1;
                    }
                }
                
                bool validMove = false;
                
                int row_choice, col_choice;
                
                while(!validMove){
                    printf("Enter the coordinates where you want the penguin to go (in a straight line): \n");
                    printf("row: ");
                    while(scanf("%d", &row_choice)==0){
                        getchar();
                        printf("Invalid input\n");
                    }
                    printf("column: ");
                    while(scanf("%d", &col_choice)==0){
                        getchar();
                        printf("Invalid input\n");
                    }

                    int tempTile = (row_choice - 1) * gameBoard->columns + (col_choice - 1);
                    if (row_choice < 1 || row_choice > gameBoard->rows || col_choice < 1 || col_choice > gameBoard->columns){
                        printf("Invalid coordinates\n\n");
                        validMove = false;
                    }
                    else if(gameBoard->tiles[tempTile] == 0){
                        printf("Penguins can drown, didn't you know?\n\n");
                        validMove = false;
                    }
                    
                    else if(gameBoard->penguins[tempTile] != ' '){
                        printf("You want a brawl? Don't interupt other penguins!\n\n");
                    }
                    
                    else if(row_choice != peng_row && col_choice != peng_col){
                        printf("You can only move in a straight line.\n\n");
                        validMove = false;
                    }
                    else if(CanPenguinMoveThere(gameBoard, peng_row-1, peng_col-1, row_choice-1, col_choice-1)==false){
                        printf("Your Penguin cannot move there.\n\n");
                    }
                    else{
                        validMove = true;
                    }
                }
                
                gameBoard->penguins[tileIndexMovementPhase] = ' ';
                peng_row = row_choice;
                peng_col = col_choice;
                placePenguin(gameBoard, i, peng_row - 1, peng_col - 1);
                players[i].Penguin[peng_id].peng_row = peng_row - 1;
                players[i].Penguin[peng_id].peng_col = peng_col - 1;
                players[i].fish += gameBoard->tiles[tileIndexMovementPhase];
                gameBoard->tiles[tileIndexMovementPhase] = 0;
                printBoard(gameBoard);
                saveGameProgress(gameBoard, players, num_players, num_pengwings, "save.txt");

                printf("Nice move! Time for the next player...\n\n");
            }
        }
            
    }
    printf("Nobody can move!\n");
}

void saveGameProgress(struct board *gameBoard, Player players[], int num_players, int num_pengwings, const char *filename) {
    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Save board dimensions
    fprintf(file, "%d %d, %d\n", gameBoard->columns, gameBoard->rows, num_pengwings);

    // Save board state
    for (int i = 0; i < gameBoard->rows; i++) {
        for (int j = 0; j < gameBoard->columns; j++) {
            int tileIndex = i * gameBoard->columns + j;
            fprintf(file, "%01d", gameBoard->tiles[tileIndex]);
            fprintf(file, "%01d ", gameBoard->penguins[tileIndex] != ' ' ? gameBoard->penguins[tileIndex] - 'A' + 1 : 0);
            
        }
        fprintf(file, "\n");
    }
 // Save player information
    for (int i = 0; i < num_players; i++) {
        fprintf(file, "%s %d %d", players[i].player_name, players[i].fish, players[i].num_pengwings);
        for (int x = 0; x < players[i].num_pengwings; x++) {
            fprintf(file, " %d %d", players[i].Penguin[x].peng_row, players[i].Penguin[x].peng_col);
            }
            fprintf(file, "\n");
        }

    fclose(file);

}

void loadGameProgress(struct board *gameBoard, Player players[], int *num_players, int *num_pengwings, const char *filename) {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for reading\n");
        return;
    }
    // Load board dimensions
    fscanf(file, "%d %d, %d", &(gameBoard->columns), &(gameBoard->rows), num_pengwings);
    gameBoard->tiles = malloc(gameBoard->columns * gameBoard->rows * sizeof(int));
    gameBoard->penguins = malloc(gameBoard->columns * gameBoard->rows * sizeof(char));
    
    // Load board state
    for (int i = 0; i < gameBoard->rows; i++) {
        for (int j = 0; j < gameBoard->columns; j++) {
            int tileIndex = i * gameBoard->columns + j;
            
            fscanf(file, "%01d%01d", &(gameBoard->tiles[tileIndex]), &(gameBoard->penguins[tileIndex]));
            if(gameBoard->penguins[tileIndex] == 0)
            {
                gameBoard->penguins[tileIndex] = ' ';
            }
            else
            {
                gameBoard->penguins[tileIndex] += 'A' - 1;
            }
        }
    }

    // Load player information
    int i = 0;
    while (fscanf(file, "%s %d %d", players[i].player_name, &(players[i].fish), &(players[i].num_pengwings)) != EOF) {
        for (int x = 0; x < players[i].num_pengwings; x++) {
            fscanf(file, " %d %d", &(players[i].Penguin[x].peng_row), &(players[i].Penguin[x].peng_col));
        }
        i++;
    }
    *num_players = i;
    *num_pengwings = players[0].num_pengwings; // assuming all players have the same number of pengwings

    fclose(file);
}

void gettingData(int *board_width, int *board_height, int *num_players, int *num_pengwings, bool *valid_input) {
    char str_of_bw[20], str_of_bh[20], str_of_num_pl[20], str_of_num_pen[20];

    while (!(*valid_input)) {
        printf("Enter the width of the board: \n");
        scanf("%s", str_of_bw);
        printf("Enter the height of the board: \n");
        scanf("%s", str_of_bh);
        printf("Enter the number of players: \n");
        scanf("%s", str_of_num_pl);
        printf("Enter the number of penguins per player: \n");
        scanf("%s", str_of_num_pen);

        if (!(sscanf(str_of_bw, "%d", board_width) == 1 &&
              sscanf(str_of_bh, "%d", board_height) == 1 &&
              sscanf(str_of_num_pl, "%d", num_players) == 1 &&
              sscanf(str_of_num_pen, "%d", num_pengwings) == 1)) {
            printf("Do not use characters or special signs! Use only integer numbers.\n");
        } else if (*board_width <= 0 || *board_height <= 0 || *num_players <= 0 || *num_pengwings <= 0) {
            printf("Why are you being so negative?\n");
        } else if (*board_width * (*board_height) < (*num_pengwings) * (*num_players)) {
            printf("The board is too small!\n");
        } 
        else if(*board_height > 20 || *board_width > 20){
            printf("The values of the height and the width can reach up to 20.\nPlease try again.\n");
        }
        else if(*num_players> 9){
            printf("There can be up to four players.\n Please try again.\n");
        }
        else if(*num_pengwings>4){
            printf("That is way too many penguins, there can be max. 8 for each player.\n Please try again.\n");
        }
        else {
            *valid_input = true;
        }
       
    }
}

Player tallyWinner(Player players[], int num_players) {
    int maxFish = -1;
    Player maxFishPlayer;

    for(int i = 0; i < num_players; i++) {
        if(players[i].fish > maxFish) {
            maxFish = players[i].fish;
            maxFishPlayer = players[i];
        }
    }

    return maxFishPlayer;
}
