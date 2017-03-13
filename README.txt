Over the past two weeks, Suzanne and Claudia have both contributed to
our AI in the following ways.

Claudia:
- Wrote player.cpp
- Wrote the majority of the minimax algorithm
- Outlined functions to be used in board.cpp / wrote some of the 
 code for it

Suzanne:
-Researched and implemented heuristics used in AI such that it could
beat Constant Time Player
-Debugged minimax algorithm
-Debugged functions in board.cpp / made them more elegant


To improve our AI, we evaluated the possible moves in two different ways. First, we weighted the value of each square on the board, according to the desirability of the square (ie. corners are weighted highly, spaces that give the other player access to corners have negative weight). The second evaluation was to compare how many coins would be flipped for each possible move. These two considerations were given equal weight and the AI choses the best scoring space to play on. This evaluation is enough to beat ConstantTimePlayer, and makes our AI tournament-worthy.
