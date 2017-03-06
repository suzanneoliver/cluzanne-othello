#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    void set_Board(Board *board);
    Move *bestMinimaxMove(Side ourSide, Side oppside);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    
private:
	int playtime;
	int corners[8];
	int good_corners[8];
	int inner_board[16];
	int edges[32];
	int nextMoves[16];
	int nextMoves1[16];
	int nexteight[8];
	
	Board *playerBoard;
	Side ourside;
	Side opponentside;
};

























#endif
