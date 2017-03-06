#include "player.hpp"

// Consider this Claudias comment.
// commiting some changes

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     
     // Initialize player board.
     playerBoard = new Board();
     playtime = 0;
     
     ourside = side;
     if (ourside == BLACK)
     {
		 opponentside = WHITE;
	 }
	 else
	 {
		 opponentside = BLACK;
	 }
		 
}

/*
 * Destructor for the player.
 */
Player::~Player() 
{
	delete playerBoard;
}

void Player::set_Board(Board *board)
{
	playerBoard = board;
}
/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */


 
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
     if (!playerBoard->isDone())
     {
		 playerBoard->Board::doMove(opponentsMove, opponentside);
		 
		 Move *ourmove;
		 
		 if (!testingMinimax)
		 {
			 ourmove = playerBoard->Board::bestSpace(ourside);
		 }
		 else
		 {
			 ourmove = bestMinimaxMove(ourside, opponentside);
		 }
		 playerBoard->Board::doMove(ourmove, ourside);
		 return ourmove;
	 }
    return nullptr;
}

// Using minimax algorithm
Move *Player::bestMinimaxMove(Side ourSide, Side oppside)
{
	// While we can still make a move
	if (playerBoard->hasMoves(ourSide))
	{
		vector<Move*> movestomake = playerBoard->Board::movesWeCanMake(ourSide);
		Move *bestmove = movestomake[0];
		Move *temp;
		int tscore;
		
		Board *tboard = playerBoard->Board::copy();
		tboard->Board::doMove(bestmove, ourSide);
		temp = tboard->Board::bestSpace(oppside);
		
		tboard->Board::doMove(temp, oppside);
		int mecount = tboard->Board::count(ourSide);
		int oppcount = tboard->Board::count(oppside);
		
		int bestscore = mecount - oppcount;
		
		if (movestomake.size() >= 2)
		{
			for (unsigned int i = 1; i < movestomake.size(); i++)
			{
				tboard = playerBoard->Board::copy();
				tboard->Board::doMove(movestomake[i], ourSide);
				if (tboard->hasMoves(oppside))
				{
					temp = tboard->Board::bestSpace(oppside);
					tboard->Board::doMove(temp, oppside);
					tscore = tboard->Board::score(ourSide) - tboard->Board::score(oppside);
					
					if (tscore > bestscore)
					{
						bestmove = movestomake[i];
					}
				}
			}
		}
		return bestmove;
	}
	return nullptr;
}



















