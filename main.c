// ECE 209 - Fall 2023 - Program 1
// This is an implementation of the Trouble board game.
// Allows for up to four players.

#include <stdio.h>
#include <assert.h>
#include <limits.h>
// add other #include statements as needed

// symbols for the various colors for players
// using global constants instead of #define
const int Red = 0;
const int Green = 1;
const int Yellow = 2;
const int Blue = 3;
const char * const gPlayerNames[] = {"Red", "Green", "Yellow", "Blue"};

// NOTE: We are using global variable to store the state of the game.
// This is appropriate because there is only one game, and different functions
// need access to all of the board information.

// A player's pieces are represented by an integer. There are four pieces per player (0-3),
// and a piece is represented by an integer p = 4 * color + piece.
// So, red's pieces will be 0, 1, 2, 3, green's pieces will be 4, 5, 6, 7, etc.
int gPieces[16];   // the current position of each piece

// board information
int gHome[4];        // number of players in the home area for each player
int gRing[28];       // which piece is in each spot on the ring
int gFinish[4][4];   // which pieces are in each player's finish line

// Each board position is represented by an integer.
// Position -1 is the home area for any player.
//
// Positions 0 through 27 are positions along the ring.
// The ring is divided into four regions, one for each color. Each region has seven positions.
// Red is region 0, Green is region 1, Yellow is region 2, Blue is region 3.
// A "region" begins with the position that transitions from one color to the next.
//
// Each player's starting point (where it enters the ring)
// is position 5 within that player's region.
//
// The finish line positions are represented by 100 + player number * 10 + position.
// Example: Position 132 is the third position in the finish line for player 3 (Blue).

// LOCATION OF EACH PIECE
// The location (position) of each piece is stored in the gPieces array.
// The Ring array contains the identifier of the piece that occupies each ring position,
// or -1 if the position is empty.
// The Finish array contains the identifier of the piece that occupies each finish line position,
// or -1 if the position is empty.

// STRING REPRESENTATION OF A BOARD POSITION
//
// NOTE: Since we are communicating with humans who are not used to counting from zero,
// our printed position values will start counting from 1.
//
// Print "H" for any player's home area.
//
// When a ring position is printed, we specify the color of the region and the
// position within that region. (See the picture in the spec.)
// For a given position on the ring, the color of a region is given by position / 7, and the
// number within the region is given by (index % 7)+1.
// Example: Position 0 is written as "R1" (meaning "red region, position 1")
// because 0 / 7 == 0 (Red) and (0 % 7)+1 == 1.
// Example: Position 19 is written as "Y6" (meaning "yellow region, position 6")
// because 19 / 7 == 2 (Yellow) and (19 % 7)+1 == 6.
//
// The finish line positions are written as "xF1" through "xF4", where x is the color
// (e.g., "RF2" for red's finish line position 2).

// MANDATORY FUNCTIONS ================================================================
// functions that you must implement -- you can add others if you want

void initializeBoard();    // set up the board for a new game (see the spec)

void printStatus();        // print the current position of each player's pieces (see the spec)

int playerTurn(int player);   // do everything necessary for this player's turn
// return 1 if player wins, otherwise return 0;

int printMoves(int player, int roll);   // print the legal moves for the player
// return the number of legal moves

int movePiece(int player, int from, int spaces);    // move a piece forward
// player is requesting a move from a specified place on the board
// return 1 if the move is legal, 0 otherwise

int checkWin(int player);   // check if the player has won the game

// OPTIONAL FUNCTIONS =================================================================
// Please declare all functions that you have created for this program. Do not define
// them here. Put the code **after** the main() function, in the designated area.



// HELPER FUNCTIONS ===================================================================
// These functions are defined for you. (See code below main.)
// DO NOT delete these declarations or alter that code in any way.
void seedRandom(int);   // seed the random number generator
int pop_o_matic();  // "pop" to roll the dice, returns a number between 1 and 6

// MAIN FUNCTION ======================================================================

#ifndef TEST_MAIN   // DO NOT REMOVE this line -- it is needed for grading purposes.
int main() {
   int i,j;  // generic loop counters
   // initialize the board
   initializeBoard();
   // checking the board
   for (i=0; i<4; i++) assert(gHome[i] == 4);
   for (i=0; i<28; i++) assert(gRing[i] == -1);
   for (i=0; i<4; i++) {
      for (j=0; j<4; j++) assert(gFinish[i][j] == -1);
   }
   for (i=0; i<16; i++) assert(gPieces[i] == -1);

   // ask user for a random seed, and for the number of players
   int seed = 0;
   int numPlayers = 0;
   printf("Enter a random seed: ");
   fflush(stdout);
   scanf("%d", &seed);
   if (seed == -1) return 0;   // for testing only -- don't enter -1 in the real game
   seedRandom(seed);
   while (!numPlayers) {
      printf("Enter the number of players (2, 3, 4): ");
      fflush(stdout);
      scanf("%d", &numPlayers);
      if (numPlayers < 2 || numPlayers > 4) numPlayers = 0;
   }

   // play the game
   int player = Red;     // we'll start with Red
   int gameOver = 0;

   while (!gameOver) {
      // perform this player's turn
      gameOver = playerTurn(player);
      if (numPlayers == 2) player = (player + 2) % 4;
      else player = (player + 1) % numPlayers;
   }
   return 0;
}
#endif  // TEST_MAIN: DO NOT REMOVE this line -- it is needed for grading purposes.

// FUNCTION DEFINITIONS ===============================================================
// Please put your function definitions here. Do not put a main() function here.

















//-----------------------------------------------------------------------------------------
// Pseudorandom number generator -- DO NOT edit or remove this variable or these functions

// Using 16-bit linear feedback shift register (LFSR) for pseudorandom number generator
int lfsr = 0;  // global variable -- state of random number generator

// lfsrNext -- function to advance Xorshift LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
int lfsrNext(void) {
   lfsr ^= lfsr >> 7;
   lfsr ^= lfsr << 9;
   lfsr ^= lfsr >> 13;
   return (int) (lfsr & INT_MAX);  // return a positive int
}

// set starting value for LFSR-based random number generator
void seedRandom(int seed) {
   if (seed) lfsr = seed;
}
// return a random integer between 0 and limit-1
int getRandom(int limit) {
   return lfsrNext() % limit;
}

int pop_o_matic() {
   return getRandom(6) + 1;
}
//-----------------------------------------------------------------------------------------

