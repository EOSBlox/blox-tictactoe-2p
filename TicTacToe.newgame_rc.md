# CONTRACT FOR tictactoe::newgame

## ACTION NAME: newgame

### Parameters
Input parameters:

* `player1` (person initiating game as player 1)
* `player2` (person initiating game as player 2)

### Intent
INTENT. The intent of the `{{newgame}}` action is to create a new game of Tic Tac Toe in which `{{player1}}` plays against `{{player2}}`. The two players cannot be the same player!

I understand that `{{player1}}` pays for the RAM used to store the game state. Additionally, both `{{player1}}` and `{{player2}}` can only be part of _one_ game at any one time, as either first or second player.

### Term
TERM. This Contract expires after the game is finished, either by winning, losing, draw, or early termination via the `{{endgame}}` function.
