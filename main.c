#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "library.h"
#include <string.h>

int main() {
    Player *players;  // Declare as a pointer

    int board_width, board_height, num_players, num_pengwings;
    char str_of_bw[20], str_of_bh[20], str_of_num_pl[20], str_of_num_pen[20];
    bool valid_input = false;
    bool ask_for_input = false;
    // Check if "save.txt" exists and has content
    FILE *saveFile = fopen("save.txt", "r");
    struct board *gameBoard;

    if (saveFile != NULL) {
        fseek(saveFile, 0, SEEK_END);
        long fileSize = ftell(saveFile);
        fclose(saveFile);

        if (fileSize > 0) {
            // File has content, ask the user if they want to load the saved game
            char choice;
            printf("Save file found. Do you want to load the saved game? (y/n): ");
            scanf(" %c", &choice);

            if (choice == 'y' || choice == 'Y') {
                // Load the game and exit

                printf("Loading saved game...\n");

                gameBoard = malloc(sizeof(struct board));

                // Allocate memory for players after loadGame
                players = malloc(num_players * sizeof(Player));
                loadGameProgress(gameBoard, players, &num_players, &num_pengwings, "save.txt");
                printBoard(gameBoard);
            } else {
                ask_for_input = true;
            }
        }
    }
    else {
        ask_for_input = true;
    }

    if(ask_for_input) {
        while (!valid_input) {
            printf("Enter the width of the board: \n");
            scanf("%s", str_of_bw);
            printf("Enter the height of the board: \n");
            scanf("%s", str_of_bh);
            printf("Enter the number of players: \n");
            scanf("%s", str_of_num_pl);
            printf("Enter the number of penguins per player: \n");
            scanf("%s", str_of_num_pen);

            sscanf(str_of_bw, "%d", &board_width);
            sscanf(str_of_bh, "%d", &board_height);
            sscanf(str_of_num_pl, "%d", &num_players);
            sscanf(str_of_num_pen, "%d", &num_pengwings);

            players = malloc(num_players * sizeof(Player));  // Allocate memory for players here

            for (int i = 0; i < num_players; i++) {
                printf("Enter a name for player %d: ", i + 1);
                scanf("%s", players[i].player_name);
            }

            if (!(board_width > 0 && board_height > 0 && num_players > 0 && num_pengwings > 0 &&
                board_width * board_height >= num_pengwings * num_players)) {
                printf("Invalid input. Please try again.\n");
            } else {
                valid_input = true;

            }

        }
        
        gameBoard = createBoard(board_width, board_height, num_players * num_pengwings);
        printBoard(gameBoard);
        for (int i = 0; i < num_players; i++) {
            players[i].playerNumber = i + 1;
            players[i].num_pengwings = num_pengwings;
            players[i].fish = 0;
            players[i].playerID = 0;
        }
    }
    

    saveGameProgress(gameBoard, players, num_players, num_pengwings, "save.txt");
    placePenguins(gameBoard, players, num_players, num_pengwings);

    movementPhase(gameBoard, players, num_players, num_pengwings);
    saveGameProgress(gameBoard, players, num_players, num_pengwings, "save.txt");

    Player winner = tallyWinner(players, num_players);
    printf("The player %s with number %d wins with %d fish!\n", winner.player_name, winner.playerNumber, winner.fish);

    free(gameBoard->tiles);
    free(gameBoard->penguins);
    free(gameBoard);
    free(players);  // Don't forget to free players

    return 0;
}
