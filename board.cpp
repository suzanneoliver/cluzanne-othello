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

// Returns a list of all the possible moves a side can make.
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


// Returns the number of pieces that would change color for a given move
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
// calculates value of move based on number of pieces changing color
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
// calculates best move, using number of possible pieces flipped 
// and location of move on board
{
	vector<Move*> pot_moves = movesWeCanMake(side);
	if (pot_moves.size() > 0)
	{
	    int best_score = score(pot_moves[0]);
	    Move* best_move = pot_moves[0];
	    int cur_score;
	    for (unsigned int i = 1; i < pot_moves.size(); i ++)
	    {
	        cur_score = score(pot_moves[i]) + changed(pot_moves[i], side);
	        
	        if (cur_score > best_score)
	        {
	            best_move = pot_moves[i];
	            best_score = cur_score;
	        }   
	    }  
	    return best_move;
    }
    std::cerr << "No valid moves" << std::endl;
    return nullptr;
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
		
int Board::score(Move* move)
// calculates value of move based on its location
{
    // sort the squares based on their location on the board.
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};
	int edges[] = {0, 2, 0, 3, 0, 4, 0, 5, 7, 2, 7, 3, 7, 4, 7, 5, 2, 0, 3, 0, 4, 0, 5, 0, 2, 7, 3, 7, 4, 7, 5, 7};
	int inner_board[] = {3,3, 3,4, 4,3, 4,4, 2,2, 2,5, 5,2, 5,5};
	int mid_board[] = {2,3, 2,4, 3,2, 4,2, 5,3, 5,4, 4,5, 3,5};
	int inner_edges[] = {1,2, 1,3, 1,4, 1,5, 2,1, 3,1, 4,1, 5,1, 6,2, 6,3, 6,4, 6,5, 2,6, 3,6, 4,6, 5,6};
	int bad_moves[] = {0, 1, 1, 0, 6, 0, 1, 7, 0, 6, 7, 1, 6, 7, 7, 6};
    int very_bad[] = {1,1, 1,6, 6,1, 6,6};

	
	int x = move->getX();
	int y = move->getY();
	
	// assign scores based on favourability of locations
	for (int a = 0; a < 8; a += 2)
	{
		if (x == corners[a] && y == corners[a+1])
		{
			return 4;					
		}
	}
	for (int a = 0; a < 32; a += 2)
	{
		if (x == edges[a] && y == edges[a+1])
		{
			return 2;					
		}
	}
	for (int a = 0; a < 16; a += 2)
	{
		if (x == inner_board[a] && y == inner_board[a+1])
		{
			return 1;					
		}
	}
	for (int a = 0; a < 16; a += 2)
	{
		if (x == mid_board[a] && y == mid_board[a+1])
		{
			return 0;					
		}
	}
	for (int a = 0; a < 32; a += 2)
	{
		if (x == inner_edges[a] && y == inner_edges[a+1])
		{
			return -1;					
		}
	}
	for (int a = 0; a < 16; a += 2)
	{
		if (x == bad_moves[a] && y == bad_moves[a+1])
		{
			return -2;					
		}
	}
	for (int a = 0; a < 8; a += 2)
	{
		if (x == very_bad[a] && y == very_bad[a+1])
		{
			return -3;					
		}
	}
	std::cerr << "Not a valid move" << x << y << std::endl;
	return -99; // if the move was not in one of the groups, it is not valid.
}
	


