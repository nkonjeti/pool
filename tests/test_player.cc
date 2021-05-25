//
// Created by neha konjeti on 5/4/21.
//
#include <catch2/catch.hpp>

#include "player.h"
/*
 * Testing strategy:
 * Test player is reset properly: state, ball type, ball numbers, score
 */
using pool::Player;
TEST_CASE("player information is reset") {
  Player player = Player();
  player.SetGameState(pool::Player::won);
  player.SetBallTypeToScore(pool::Ball::solid);
  double const kNumBalls = 7;
  for (size_t num = 1; num <= kNumBalls; num++) {
    player.AddBallNumberScored(num);
    player.AddBallScore();
  }
  player.ResetPlayer();
  SECTION("game state reset") {
    REQUIRE(player.GetGameState() == pool::Player::playing);
  }
  SECTION("ball type reset") {
    REQUIRE(player.GetBallTypeToScore() == pool::Ball::cue);
  }
  SECTION("ball numbers reset") {
    REQUIRE(player.GetBallNumbers().size() == 0);
  }
  SECTION("number of balls scored is 0") {
    REQUIRE(player.GetPlayerScore() == 0);
  }
}
