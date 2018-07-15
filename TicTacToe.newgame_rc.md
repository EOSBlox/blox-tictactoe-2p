# CONTRACT FOR tictactoe::newgame

## ACTION NAME: newgame

### Parameters
Input parameters:

* `player1` (person initiating game as player 1)
* `player2` (person initiating game as player 2)

### Intent
INTENT. The intent of the `{{newgame}}` action is to create a new game of Tic Tac Toe in which `{{player1}}` plays against `{{player2}}`.

I understand that `{{player1}}` pays for the RAM used to store the game state, and there can only be one active game per `{{player1}}`. Therefore, `{{player2}}` can initiate another game as `{{player1}}` but there can only be one at any point in time for each account as a "first player".

### Term
TERM. This Contract expires after the game is finished, either by winning, losing, draw, or early termination via the `{{endgame}}` function.
