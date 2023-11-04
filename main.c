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
void RingIntToBoard(int NumberFromRing);
void seedRandom(int);   // seed the random number generator
int pop_o_matic();  // "pop" to roll the dice, returns a number between 1 and 6

void initializeBoard(){
    int i;
    for (i = 0; i < 16; i++) {
        gPieces[i] = -1;
    }
    for (i = 0; i < 28; i++) {
        gRing[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        gHome[i] = 4;
    }
    for (i = 0; i < 4; i++) {
        int j;
        for (j = 0; j < 4; j++) {
            gFinish[i][j] = -1;
        }
    }
}
   // set up the board for a new game (see the spec)

void printStatus(){
   int piece = 0;
   for(int loop = 0; loop < 4; loop++){
      printf("%7s: ", gPlayerNames[loop]);
      if(loop == 0){
      for(piece; piece < 16; piece++){
      if(piece < 4){
         if(gPieces[piece] == -1){
            printf("H");
            if(piece != 3){
               printf(",");
            }
         }
         if(gPieces[piece] != -1 && gPieces[piece] < 100){
            RingIntToBoard(gPieces[piece]);
         }
         if(gFinish[0][piece] != -1){
            printf("RF%d", piece + 1);
         }
      }
      
   }
   }
   if(loop == 1){
      for(piece; piece < 16; piece++){
      if(piece < 8 && piece > 3){
         if(gPieces[piece] == -1){
            printf("H");
            if(piece != 7){
               printf(",");
            }
         }
         if(gPieces[piece] != -1 && gPieces[piece] < 100){
            RingIntToBoard(gPieces[piece]);
         }
         if(gFinish[1][piece - 4] != -1){
            printf("RF%d", piece - 3);
         }
      }
      
   }
   }
   if(loop == 2){
      for(piece; piece < 16; piece++){
      if(piece < 12 && piece > 7){
         if(gPieces[piece] == -1){
            printf("H");
            if(piece != 11){
               printf(",");
            }
         }
         if(gPieces[piece] != -1 && gPieces[piece] < 100){
            RingIntToBoard(gPieces[piece]);
         }
         if(gFinish[2][piece - 8] != -1){
            printf("RF%d", piece - 7);
         }

      }
   
      
   }
   }
   if(loop == 3){
      for(piece; piece < 16; piece++){
      if(piece < 16 && piece > 11){
         if(gPieces[piece] == -1){
            printf("H");
            if(piece != 15){
               printf(",");
            }
         }
         if(gPieces[piece] != -1 && gPieces[piece] < 100){
            RingIntToBoard(gPieces[piece]);
         }
         if(gFinish[3][piece - 12] != -1){
            printf("RF%d", piece - 11);
         }
      }
      
   }
   }

   if(loop < 4){
         printf("\n");
      }
      piece -=16;
      piece += 4;
   }

   }
   
    // print the current position of each player's pieces (see the spec)

int playerTurn(int player){
   int y;
   y = pop_o_matic();
   printStatus();
   printMoves(player, y);
   return 1;
}   // do everything necessary for this player's turn
// return 1 if player wins, otherwise return 0;

int printMoves(int player, int roll){

   if(player == 0){
      int w;
      int counterred = 0;
   for(int i = 0; i < 4; i++){
      if(gPieces[i] != -1 && gPieces[i] < 100 && (gRing[gPieces[i] + roll]) / 4 != 0){
         RingIntToBoard(gPieces[i]);
         printf(" to ");
         RingIntToBoard(gPieces[i] + roll);
         counterred++;
      }
      if(gPieces[i] == -1 && roll == 6 && gRing[4] != 0){
         printf("H");
         printf(" to ");
         RingIntToBoard(4);
         counterred++;
         break;
      }
      if(gPieces[i] > 100){
         
      }
      if(gPieces[i] + roll > 27){
         w = gPieces[i] + roll - 28;
      }
         if(w > 4 && w < 9 && gPieces[i] < 100){
            if(w == 5 && gFinish[0][0] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("RF1");
            counterred++;
            }
            else if(w == 6 && gFinish[0][1] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("RF2");
            counterred++;
            }
            else if(w == 7 && gFinish[0][2] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("RF3");
            counterred++;
            }
            else if(w == 8 && gFinish[0][3] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("RF4");
            counterred++;
            }
         }
         if(w < 5 && gPieces[i] + roll < 28 && (gRing[gPieces[i] + roll]) / 4 != 0){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            RingIntToBoard(gPieces[i] + roll);
            counterred++;
         }
         
      
      }
      if(counterred == 0){
         printf("You have no legal moves.");
      }
      else return counterred;
   }
   if(player == 1){
      int startmark = 0;
      int w;
      int countergreen = 0;
   for(int i = 4; i < 8; i++){
      if(gPieces[i] != -1 && gPieces[i] < 100 && (gRing[gPieces[i] + roll]) / 4 != 1){
         RingIntToBoard(gPieces[i]);
         printf(" to ");
         RingIntToBoard(gPieces[i] + roll);
         if(gPieces[i] > 18){
            startmark++;
         }
         countergreen++;
      }
      if(gPieces[i] == -1 && roll == 6 && gRing[11] != 1){
         printf("H");
         printf(" to ");
         RingIntToBoard(11);
         countergreen++;
         break;
      }
      if(gPieces[i] > 100){
         
      }
      w = gPieces[i] + roll;
      if(w > 10 && startmark != 0){
         if(w > 10 && w < 15 && gPieces[i] < 100){
            if(w == 11 && gFinish[1][0] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("GF1");
            countergreen++;
            }
            else if(w == 12 && gFinish[1][1] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("GF2");
            countergreen++;
            }
            else if(w == 13 && gFinish[1][2] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("GF3");
            countergreen++;
            }
            else if(w == 14 && gFinish[1][3] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("GF4");
            countergreen++;
            }
         }
         if(w < 5 && gPieces[i] + roll < 28 && (gRing[gPieces[i] + roll]) / 4 != 1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            RingIntToBoard(gPieces[i] + roll);
            countergreen++;
         }
         
      }
      }
      if(countergreen == 0){
         printf("You have no legal moves.");
      }
      else return countergreen;
   }
   if(player == 2){
      int startmark = 0;
      int w;
      int counteryellow = 0;
   for(int i = 8; i < 12; i++){
      if(gPieces[i] != -1 && gPieces[i] < 100 && (gRing[gPieces[i] + roll]) / 4 != 2){
         RingIntToBoard(gPieces[i]);
         printf(" to ");
         RingIntToBoard(gPieces[i] + roll);
         if(gPieces[i] > 23){
            startmark++;
         }
         counteryellow++;
      }
      if(gPieces[i] == -1 && roll == 6 && gRing[18] != 2){
         printf("H");
         printf(" to ");
         RingIntToBoard(18);
         counteryellow++;
         break;
      }
      if(gPieces[i] > 100){
         
      }
      w = gPieces[i] + roll;
      if(w > 17 && startmark != 0){
         if(w > 17 && w < 22 && gPieces[i] < 100){
            if(w == 18 && gFinish[2][0] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("YF1");
            counteryellow++;
            }
            else if(w == 19 && gFinish[2][1] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("YF2");
            counteryellow++;
            }
            else if(w == 20 && gFinish[2][2] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("YF3");
            counteryellow++;
            }
            else if(w == 21 && gFinish[2][3] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("YF4");
            counteryellow++;
            }
         }
         if(w < 12 && gPieces[i] + roll < 28 && (gRing[gPieces[i] + roll]) / 4 != 2){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            RingIntToBoard(gPieces[i] + roll);
            counteryellow++;
         }
         
      }
      }
      if(counteryellow == 0){
         printf("You have no legal moves.");
      }
      else return counteryellow;
   }
   if(player == 3){
      int startmark = 0;
      int w;
      int counterblue = 0;
   for(int i = 12; i < 16; i++){
      if(gPieces[i] != -1 && gPieces[i] < 100 && (gRing[gPieces[i] + roll]) / 4 != 3){
         RingIntToBoard(gPieces[i]);
         printf(" to ");
         RingIntToBoard(gPieces[i] + roll);
         if(gPieces[i] > 28){
            startmark++;
         }
         counterblue++;
      }
      if(gPieces[i] == -1 && roll == 6 && gRing[25] != 3){
         printf("H");
         printf(" to ");
         RingIntToBoard(25);
         counterblue++;
         break;
      }
      if(gPieces[i] > 100){
         
      }
      w = gPieces[i] + roll;
      if(w > 24 && startmark != 0){
         if(w > 24 && w < 29 && gPieces[i] < 100){
            if(w == 25 && gFinish[3][0] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("BF1");
            counterblue++;
            }
            else if(w == 26 && gFinish[3][1] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("BF2");
            counterblue++;
            }
            else if(w == 27 && gFinish[3][2] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("BF3");
            counterblue++;
            }
            else if(w == 28 && gFinish[3][3] == -1){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            printf("BF4");
            counterblue++;
            }
         }
         if(w < 18 && gPieces[i] + roll < 28 && (gRing[gPieces[i] + roll]) / 4 != 3){
            RingIntToBoard(gPieces[i]);
            printf(" to ");
            RingIntToBoard(gPieces[i] + roll);
            counterblue++;
         }
         
      }
      }
      if(counterblue == 0){
         printf("You have no legal moves.");
}
   }
}


  // print the legal moves for the player
// return the number of legal moves

int movePiece(int player, int from, int spaces){

}    // move a piece forward
// player is requesting a move from a specified place on the board
// return 1 if the move is legal, 0 otherwise

int checkWin(int player){
   if(player == 0 && gPieces[0] == 100 && gPieces[1] == 101 && gPieces[2] == 102 && gPieces[3] == 103){
      return 1;
   } else return 0;
   if(player == 1 && gPieces[4] == 110 && gPieces[5] == 111 && gPieces[6] == 112 && gPieces[7] == 113){
      return 1;
   } else return 0;
   if(player == 2 && gPieces[8] == 120 && gPieces[9] == 121 && gPieces[10] == 122 && gPieces[11] == 123){
      return 1;
   } else return 0;
   if(player == 3 && gPieces[12] == 130 && gPieces[13] == 131 && gPieces[14] == 132 && gPieces[15] == 133){
      return 1;
   } else return 0;
}   // check if the player has won the game

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
void RingIntToBoard(int NumberFromRing) {
         if(NumberFromRing > 27){
            NumberFromRing -= 28;
         }
        int colors = NumberFromRing / 7;
        int position = (NumberFromRing % 7) + 1;
        if (colors == 0) {
            printf("R%d", position);
        } else if (colors == 1) {
            printf("G%d", position);
        } else if (colors == 2) {
            printf("Y%d", position);
        } else if (colors == 3) {
            printf("B%d", position);
        }
    }


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

