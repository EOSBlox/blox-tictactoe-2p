#include "TicTacToe.h"

namespace tictactoe {

static uint8_t coord(const uint8_t row, const uint8_t col)
{
  eosio_assert(row >= 0 && row < 3, "Row value must be in [0,2]!");
  eosio_assert(col >= 0 && col < 3, "Column value must be in [0,2]!");
  return (row * 3) + col;
}

TicTacToe::TicTacToe(account_name self) : contract(self)
{
}

void TicTacToe::newgame(const account_name player1, const account_name player2)
{
  require_auth(player1);
  require_auth(player2);
  eosio_assert(player1 != player2, "Players cannot be the same!");

  games games_(_self, _self);
  eosio_assert(games_.find(player1) == games_.end(), "Player 1 already has a game!");
  eosio_assert(games_.find(player2) == games_.end(), "Player 2 already has a game!");
  const auto index = games_.get_index<N(player2)>();
  eosio_assert(index.find(player2) == index.end(), "Player 2 already has a game as player 2!");
  eosio_assert(index.find(player1) == index.end(), "Player 1 already has a game as player 2!");

  games_.emplace(player1, [&](auto &game) {
    game.player1 = player1;
    game.player2 = player2;
    game.winner = 0;
    game.turn = player1;
    game.moves = 0;
    game.board = std::vector<char>(3 * 3, ' ');
    game.setState(State::PlayerTurn);
  });
}

void TicTacToe::endgame(const account_name player1, const account_name player2)
{
  require_auth(player1);
  require_auth(player2);

  games games_(_self, _self);
  auto it = games_.find(player1);
  eosio_assert(it != games_.end(), "No game for account!");
  games_.erase(it);
  eosio_assert(it != games_.end(), "Could not remove game!");
}

void TicTacToe::play(const account_name player1, const account_name player2, uint8_t row,
                     uint8_t col)
{
  require_auth(player1);
  require_auth(player2);

  games games_(_self, _self);
  auto it = games_.find(player1);
  eosio_assert(it != games_.end(), "No game for account!");
  eosio_assert(static_cast<State>(it->state) == State::PlayerTurn, "Game is finished!");
  eosio_assert(it->player2 == player2, "Player 2 is not part of the game!");

  const auto c = coord(row, col);
  eosio_assert(it->board[c] == ' ', "Position is occupied!");

  games_.modify(it, it->turn, [&](auto &game) { game.play(c); });
}

uint64_t TicTacToe::game::primary_key() const
{
  return player1;
}

uint64_t TicTacToe::game::by_player2() const
{
  return player2;
}

void TicTacToe::game::play(const uint8_t c)
{
  board[c] = (turn == player1 ? 'x' : 'o');
  turn = (turn == player1 ? player2 : player1);
  moves++;
  updateState();
}

void TicTacToe::game::updateState()
{
  const auto checkSym = [this](const char sym) {
    if (sym == ' ') {
      return false;
    }
    setState(State::Won);
    winner = (sym == 'x' ? player1 : player2);
    turn = 0;
    return true;
  };

  for (uint8_t row = 0; row < 3; row++) {
    if (checkSym(rowWinner(row))) return;
  }

  for (uint8_t col = 0; col < 3; col++) {
    if (checkSym(colWinner(col))) return;
  }

  if (checkSym(diagWinner())) return;

  if (moves == 9) {
    setState(State::Draw);
    turn = 0;
  }
}

void TicTacToe::game::setState(const State state)
{
  this->state = static_cast<uint8_t>(state);
}

bool TicTacToe::game::isFree(const uint8_t row, const uint8_t col) const
{
  return board[coord(row, col)] == ' ';
}

bool TicTacToe::game::isCross(const uint8_t row, const uint8_t col) const
{
  return board[coord(row, col)] == 'x';
}

bool TicTacToe::game::isNought(const uint8_t row, const uint8_t col) const
{
  return board[coord(row, col)] == 'o';
}

bool TicTacToe::game::isSame(const uint8_t row1, const uint8_t col1, const uint8_t row2,
                             const uint8_t col2) const
{
  return ((isCross(row1, col1) && isCross(row2, col2)) ||
          (isNought(row1, col1) && isNought(row2, col2)));
}

char TicTacToe::game::rowWinner(const uint8_t row) const
{
  const auto v0 = board[coord(row, 0)], v1 = board[coord(row, 1)], v2 = board[coord(row, 2)];
  return (v0 == v1 && v1 == v2 ? v0 : ' ');
}

char TicTacToe::game::colWinner(const uint8_t col) const
{
  const auto v0 = board[coord(0, col)], v1 = board[coord(1, col)], v2 = board[coord(2, col)];
  return (v0 == v1 && v1 == v2 ? v0 : ' ');
}

char TicTacToe::game::diagWinner() const
{
  const auto nw = board[coord(0, 0)], c = board[coord(1, 1)], se = board[coord(2, 2)],
             sw = board[coord(2, 0)], ne = board[coord(0, 2)];
  if (nw == c && c == se) {
    return nw;
  }
  return (sw == c && c == ne ? sw : ' ');
}

} // tictactoe
