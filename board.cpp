#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
    
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

vector<Move*> Board::movesWeCanMake(Side side)
{
	vector<Move*> ans;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Move *move = new Move(i,j);
			if (checkMove(move, side))
			{
				ans.push_back(move);
			}
			else
			{
				delete move;
			}
		}
	}
	return ans;
}

Move *Board::firstMove(Side side)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Move *move = new Move(i, j);
			if (checkMove(move, side))
			{
				return move;
			}
			else
			{
				delete move;
			}
		}
	}
	return nullptr;
}

int Board::changed(Move *m, Side side)
{
	int counter = 0;
	// Nothing changed if player passes or makes invalid move.
	if (m == nullptr || !checkMove(m, side))
	{
		return 0;
	}
	int xcoord = m->getX();
	int ycoord = m->getY();
	
	Side otherside = (side == WHITE) ? BLACK : WHITE;
	for (int xchange = -1; xchange < 2; xchange++)
	{
		for (int ychange = -1; ychange < 2; ychange++)
		{
			if (xchange == 0 && ychange == 0)
			{
				continue;
			}
			int X = xcoord;
			int Y = ycoord;
			do
			{
				X += xchange;
				Y += ychange;
			} while (onBoard(X, Y) && get(otherside, X, Y));
			
			if (onBoard(X, Y) && get(side, X, Y))
			{
				X = xcoord;
				X += xchange;
				Y = ycoord;
				Y += ychange;
				while (onBoard(X, Y) && get(otherside, X, Y))
				{
					X += xchange;
					Y += ychange;
					counter += 1;
				}
			}
		}
	}
	return counter;
}
				
Move *Board::countBestMoves(Side side)
{
	Move *move;	
	vector<Move*> potentialmoves = movesWeCanMake(side);
	Move *bestmove = potentialmoves[0];
	
	if (potentialmoves.size() >= 2)
	{
		for (unsigned int i = 1; i < potentialmoves.size(); i++)
		{
			move = potentialmoves[i];
			if (changed(move, side) > changed(bestmove, side))
			{
				bestmove = move;
			}
		}
	}
	return bestmove;
}

Move *Board::bestSpace(Side side)
{
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};
	int edges[] = {0, 2, 0, 3, 0, 4, 0, 5, 7, 2, 7, 3, 7, 4, 7, 5, 2, 0, 3, 0, 4, 0, 4, 0, 2, 7, 3, 7, 4, 7, 5, 7};
	int inner_board[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};
	int good_corners[] = {2, 2, 2, 4, 5, 2, 5, 4};
	int nextMoves[] = {3, 1, 4, 1, 1, 3, 1, 4, 6, 3, 6, 4, 3, 6, 4, 6};
	int nextMoves1[] = {2, 1, 5, 1, 1, 2, 1, 5, 6, 2, 6, 5, 2, 6, 5, 6};
	int nexteight[] = {0, 1, 1, 0, 6, 0, 1, 7, 6, 0, 7, 1, 6, 7, 7, 6};
	
	if (hasMoves(side))
	{
		Move *movecorner = new Move(corners[0], corners[1]);
		for (int j = 2; j < 8; j+=2)
		{
			Move *move = new Move(corners[j], corners[j+1]);
			if (changed(move, side) > changed(movecorner, side))
			{
				delete movecorner;
				movecorner = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(movecorner, side))
		{
			return movecorner;
		}
		
		Move *moveedge = new Move(edges[0], edges[1]);
		for (int j = 2; j < 32; j += 2)
		{
			Move *move = new Move(corners[j], corners[j+1]);
			if (changed(move, side) > changed(moveedge, side))
			{
				delete moveedge;
				moveedge = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(moveedge, side))
		{
			return moveedge;
		}
		Move *movegcorner = new Move(good_corners[0], good_corners[1]);
		
		for (int j = 2; j < 8; j += 2)
		{
			Move *move = new Move(good_corners[j], good_corners[j+1]);
			if (changed(move, side) > changed(movegcorner, side))
			{
				delete movegcorner;
				movegcorner = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(movegcorner, side))
		{
			return movegcorner;
		}
		
		Move *movein = new Move(inner_board[0], inner_board[1]);
		for (int j = 2; j < 16; j += 2)
		{
			Move *move = new Move(inner_board[j], inner_board[j+1]);
			if (changed(move, side) > changed(movein, side))
			{
				delete movein;
				movein = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(movein, side))
		{
			return movein;
		}
		
		Move *movenext = new Move(nextMoves[0], nextMoves[1]);
		for (int j = 2; j < 16; j += 2)
		{
			Move *move = new Move(nextMoves[j], nextMoves[j+1]);
			if (changed(move, side) > changed(movenext, side))
			{
				delete movenext;
				movenext = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(movenext, side))
		{
			return movenext;
		}
		
		Move *movenext1 = new Move(nextMoves1[0], nextMoves1[1]);
		for (int j = 2; j < 16;  j += 2)
		{
			Move *move = new Move(nextMoves1[j], nextMoves1[j+1]);
			if (changed(move, side) > changed(movenext1, side))
			{
				delete movenext1;
				movenext1 = move;
			}
			else
			{
				delete move;
			}
		}
		
		if (checkMove(movenext1, side))
		{
			return movenext1;
		}
		Move *moveeight = new Move(nexteight[0], nexteight[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(nexteight[i], nexteight[i+1]);
			if (changed(move, side) > changed(moveeight, side))
			{
				delete moveeight;
				moveeight = move;
			}
			else
			{
				delete move;
			}
		}
		if (checkMove(moveeight, side))
		{
			return moveeight;
		}
		
		return firstMove(side);
	}
	else
	{
		return nullptr;
	}
}

bool Board::innerSquare()
{
	bool ocupado = 1;
	int inner_board[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};
	for (int i = 0; i < 16; i += 2)
	{
		if (!occupied(inner_board[i], inner_board[i+1]))
		{
			ocupado = 0;
		}
	}
	return ocupado;
}		
		
int Board::score(Side side)
{
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};
	int edges[] = {0, 2, 0, 3, 0, 4, 0, 5, 7, 2, 7, 3, 7, 4, 7, 5, 2, 0, 3, 0, 4, 0, 4, 0, 2, 7, 3, 7, 4, 7, 5, 7};
	int inner_board[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};
	int good_corners[] = {2, 2, 2, 4, 5, 2, 5, 4};
	int nextMoves[] = {3, 1, 4, 1, 1, 3, 1, 4, 6, 3, 6, 4, 3, 6, 4, 6};
	int nextMoves1[] = {2, 1, 5, 1, 1, 2, 1, 5, 6, 2, 6, 5, 2, 6, 5, 6};
	int nexteight[] = {0, 1, 1, 0, 6, 0, 1, 7, 6, 0, 7, 1, 6, 7, 7, 6};
	int	scor = 0;
	bool gotscore;
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (get(side, i, j))
			{
				gotscore = 0;
				for (int a = 0; a < 8; a += 2)
				{
					if (i == corners[a] && j == corners[a+1])
					{
						scor = scor + 50;
						gotscore = 1;
					}
				}
				if (gotscore == 0)
				{
					for (int b = 0; b < 32; b += 2)
					{
						if (i == edges[b] && j == corners[b+1])
						{
							scor = scor + 10;
							gotscore = 1;
						}
					}
				}
				if (gotscore == 0)
				{
					for (int c = 0; c < 16; c += 2)
					{
						if (i == inner_board[c] && j == inner_board[c+1])
						{
							scor = scor + 3;
							gotscore = 1;
							
						}
					}
				}
				if (gotscore == 0)
				{
					for (int d = 0; d < 8; d += 2)
					{
						if (i == good_corners[d] && j == good_corners[d+1])
						{
							scor = scor + 7;
							gotscore = 1;
						}
					}
				}
				if (gotscore == 0)
				{
					for (int e = 0; e < 16; e += 2)
					{
						if (i == nextMoves[e] && j == nextMoves[e+1])
						{
							scor = scor + 2;
							gotscore = 1;
						}
					}
				}
				if (gotscore == 0)
				{
					for (int f = 0; f < 16; f += 2)
					{
						if (i == nextMoves1[f] && j == nextMoves1[f+1])
						{
							scor = scor + 1;
							gotscore = 1;
						}
					}
				}
				if (gotscore == 0)
				{
					for (int g = 0; g < 16; g += 2)
					{
						if (i == nexteight[g] && j == nexteight[g+1])
						{
							scor = scor - 15;
							gotscore = 1;
						}
					}
				}
				else if (gotscore == 0)
				{
					scor = scor - 999;
				}
			}
		}
	}
	return scor;
}



