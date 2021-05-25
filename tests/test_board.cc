//
// Created by neha konjeti on 4/16/21.
//
#include <catch2/catch.hpp>

#include "board.h"
using glm::dvec2;
using pool::Ball;
using pool::Board;
using pool::Player;

/**
 * Testing Strategy:
 * Check ball velocity decreasing
 * Check ball going into holes:
 * top left, top right, bottom left, bottom right, top middle, bottom middle
 * edge case for each hole (not exact hole position but still inside)
 * Test cue ball going into hole: cue ball placed back into board with no
 * velocity
 * Check stick visibility during shot
 * Repositioning cue ball when it is hit into the hole
 * Making sure cue ball isn't repositioned to occupied position by other ball
 * Test stick pull affecting velocity boost of shot
 * Test aim line length changing when stick is pulled back
 * Test board is reset properly when game is over : stick angle and height set
 * back to initial values, all balls are brought back, player state is reset to
 * playing, player's score is set to 0, ball numbers vector for player is empty
 * Test player can win after hitting balls of all same type, then hitting the
 * eight ball into hole Test player can lose after hitting eight ball first, or
 * after hitting the wrong type of ball
 */

TEST_CASE("ball velocity decreasing") {
  Board board = Board(1000);
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {left + 60, top + 60}, {1.5, 0});
  board.SetPoolBalls({cue_ball});
  board.AdvanceOneFrame();
  dvec2 new_velocity = {1.47899, 0};
  REQUIRE(board.GetPoolBalls()[0].GetVelocity().x == Approx(new_velocity.x));
  REQUIRE(board.GetPoolBalls()[0].GetVelocity().y == Approx(new_velocity.y));
}

TEST_CASE("test ball going in top left hole") {
  Board board = Board(1000);
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  Ball ball = Ball(5, pool::Ball::striped, {left, top}, {-2.3, 0.0});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    // have to subtract diameter because ball position is not at center
    // but at top left corner due to image
    dvec2 inside_hole = {
        left + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        top + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {-2.3, 0.0});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test ball going in top right hole") {
  Board board = Board(1000);
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  Ball ball = Ball(5, pool::Ball::striped, {right, top}, {2.3, -.1});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    dvec2 inside_hole = {
        right - board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        top + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {2.3, -.1});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test ball going in bottom right hole") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double right = board.GetRightXBoundary();
  Ball ball = Ball(5, pool::Ball::striped, {right, bottom}, {-2.3, 1.1});
  board.SetPoolBalls({ball});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    dvec2 inside_hole = {
        right - board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        bottom - board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {-2.3, 1.1});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test ball going in bottom left hole") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  Ball ball = Ball(5, pool::Ball::striped, {left, bottom}, {-2.4, .4});
  board.SetPoolBalls({ball});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    dvec2 inside_hole = {
        left + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        bottom - board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {-2.4, .4});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test ball going in bottom middle hole") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  double right = board.GetRightXBoundary();
  double middle = (right + left) / 2;
  Ball ball = Ball(5, pool::Ball::striped, {middle, bottom}, {0.0, .4});
  board.SetPoolBalls({ball});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    dvec2 inside_hole = {
        middle + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        bottom - board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {0.0, .4});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test ball going in top middle hole") {
  Board board = Board(1000);
  double top = board.GetTopYBoundary();
  double left = board.GetLeftXBoundary();
  double right = board.GetRightXBoundary();
  double middle = (right + left) / 2;
  Ball ball = Ball(5, pool::Ball::striped, {middle, top}, {0.0, -.4});
  board.SetPoolBalls({ball});
  SECTION("exact position as hole") {
    board.SetPoolBalls({ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
  SECTION("not exact position but inside hole") {
    dvec2 inside_hole = {
        middle + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2,
        top + board.GetHoleRadius() / 2 - ball.GetDiameter() / 2};
    Ball new_ball = Ball(5, pool::Ball::striped, inside_hole, {0.0, .4});
    board.SetPoolBalls({new_ball});
    REQUIRE(board.GetPoolBalls().size() == 1);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPoolBalls().size() == 0);
  }
}

TEST_CASE("test cue ball going into hole") {
  Board board = Board(1000);
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {right, top}, {-.1, -.1});
  board.SetPoolBalls({cue_ball});
  board.AdvanceOneFrame();
  SECTION("check cue ball wasn't removed") {
    REQUIRE(board.GetPoolBalls().size() == 1);
  }
  SECTION("check cue ball was placed back into middle of board") {
    dvec2 center_pos = {(right + left) / 2, (bottom + top) / 2};
    REQUIRE(board.GetPoolBalls()[0].GetPosition() == center_pos);
  }
  SECTION("check cue ball has no velocity") {
    dvec2 no_velocity = {0.0, 0.0};
    REQUIRE(board.GetPoolBalls()[0].GetVelocity() == no_velocity);
  }
}

TEST_CASE("stick visibility before and during shot") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  SECTION("before shot visibility is on") {
    Ball ball =
        Ball(5, pool::Ball::striped, {left + 50.0, bottom - 50.0}, {0.0, 0.0});
    Ball cue_ball =
        Ball(0, pool::Ball::cue, {right - 50, top + 50}, {0.0, 0.0});
    board.SetPoolBalls({ball, cue_ball});
    REQUIRE(board.GetStickVisibility());
  }
  SECTION("during shot visibility is off") {
    Ball ball =
        Ball(5, pool::Ball::striped, {left + 50.0, bottom - 50.0}, {-2.4, .4});
    Ball cue_ball =
        Ball(0, pool::Ball::cue, {right - 50, top + 50}, {0.0, 0.0});
    board.SetPoolBalls({ball, cue_ball});
    board.HitCueBall();
    board.AdvanceOneFrame();
    REQUIRE(board.GetStickVisibility() == false);
  }
}

TEST_CASE("moving cue ball to center with another ball at center position") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  double diameter = Ball::GetDiameter();
  dvec2 center_pos = {(right + left) / 2, (bottom + top) / 2};
  Ball cue_ball = Ball(0, pool::Ball::cue, {right, top}, {-.1, -.1});
  Ball center_ball = Ball(1, pool::Ball::striped, center_pos, {0, 0});
  board.SetPoolBalls({cue_ball, center_ball});
  board.AdvanceOneFrame();
  dvec2 shifted_to_right = {center_pos.x + diameter * 2, center_pos.y};
  REQUIRE(board.GetPoolBalls()[0].GetPosition() == shifted_to_right);
  REQUIRE(board.GetPoolBalls()[1].GetPosition() == center_pos);
  dvec2 no_velocity = {0, 0};
  REQUIRE(board.GetPoolBalls()[0].GetVelocity() == no_velocity);
  REQUIRE(board.GetPoolBalls()[1].GetVelocity() == no_velocity);
}

TEST_CASE("shifts cue ball twice when ball occupies center") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  dvec2 center_pos = {(right + left) / 2, (bottom + top) / 2};
  Ball cue_ball = Ball(0, pool::Ball::cue, {right, top}, {-.1, -.1});
  double diameter = cue_ball.GetDiameter();
  dvec2 right_center = {center_pos.x + diameter, center_pos.y};
  Ball center_ball = Ball(1, pool::Ball::striped, center_pos, {0, 0});
  Ball next_to_center_ball = Ball(2, pool::Ball::striped, right_center, {0, 0});
  board.SetPoolBalls({cue_ball, center_ball, next_to_center_ball});
  board.AdvanceOneFrame();
  dvec2 shifted_to_right = {center_pos.x + diameter * 3, center_pos.y};
  dvec2 no_velocity = {0, 0};
  SECTION("check cue ball isn't occupying other ball positions") {
    REQUIRE(board.GetPoolBalls()[0].GetPosition() == shifted_to_right);
    REQUIRE(board.GetPoolBalls()[1].GetPosition() == center_pos);
    REQUIRE(board.GetPoolBalls()[2].GetPosition() == right_center);
  }
  SECTION("check velocities are zero") {
    REQUIRE(board.GetPoolBalls()[0].GetVelocity() == no_velocity);
    REQUIRE(board.GetPoolBalls()[1].GetVelocity() == no_velocity);
    REQUIRE(board.GetPoolBalls()[2].GetVelocity() == no_velocity);
  }
}

TEST_CASE("check stick moving to affect power of shot") {
  Board board = Board(1000);
  double top = board.GetTopYBoundary();
  double right = board.GetRightXBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {right - 50, top + 50}, {0.0, 0.0});
  board.SetPoolBalls({cue_ball});
  SECTION("no pull on ball is initial velocity boost") {
    REQUIRE(board.GetPoolBalls()[0].GetVelocityBoost() == 3);
  }
  // velocity boost is updated to account for pull to increase power of shot
  SECTION("stick pulled back once") {
    board.PullStickBackForShot();
    // velocity boost = 3 + (6 * 5/20)
    REQUIRE(board.GetPoolBalls()[0].GetVelocityBoost() == 4.5);
  }
  SECTION("stick pulled back till max") {
    for (size_t i = 0; i < 4; i++) {
      board.PullStickBackForShot();
    }
    // velocity boost = 3 + 6
    REQUIRE(board.GetPoolBalls()[0].GetVelocityBoost() == 9);
  }
  SECTION("stick pushed forward after reaching max") {
    for (size_t i = 0; i < 6; i++) {
      board.PullStickBackForShot();
    }
    // velocity boost = 3 + (6 * 15/20)
    REQUIRE(board.GetPoolBalls()[0].GetVelocityBoost() == 7.5);
  }
}

TEST_CASE("line length for aiming changes") {
  Board board = Board(1000);
  board.CreatePoolBalls();
  REQUIRE(board.GetAimLineLength() == 100);
  board.PullStickBackForShot();
  REQUIRE(board.GetAimLineLength() == 200);
  SECTION("pulling twice should change length") {
    board.PullStickBackForShot();
    REQUIRE(board.GetAimLineLength() == 300);
  }
}

TEST_CASE("test board reset to restart game") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  board.CreatePoolBalls();
  // change stick pull back distance
  board.PullStickBackForShot();
  // change stick angle
  board.UpdateStickRight();
  // change position of solid ball into hole
  std::vector<Ball> balls = board.GetPoolBalls();
  balls[1].SetPosition({left, bottom});
  board.SetPoolBalls(balls);
  // solid ball goes into hole
  board.AdvanceOneFrame();
  SECTION("player ball type is solid") {
    REQUIRE(board.GetPlayer().GetBallTypeToScore() == Ball::solid);
  }
  SECTION("board has one less ball") {
    REQUIRE(board.GetPoolBalls().size() == 15);
  }
  board.ResetBoard();
  // called in setup of pool app
  board.CreatePoolBalls();
  SECTION("board has all balls in board") {
    REQUIRE(board.GetPoolBalls().size() == 16);
  }
  SECTION("stick angle is reset") {
    REQUIRE(board.GetStick().GetAngle() == M_PI / 2);
  }
  SECTION("stick height is reset") {
    REQUIRE(board.GetStick().GetStickHeight() == 400);
  }
  SECTION("player state is set to playing") {
    REQUIRE(board.GetPlayerState() == pool::Player::playing);
  }
  SECTION("player ball type is reset to cue") {
    REQUIRE(board.GetPlayer().GetBallTypeToScore() == pool::Ball::cue);
  }
  SECTION("player score is 0") {
    REQUIRE(board.GetPlayer().GetPlayerScore() == 0);
  }
  SECTION("player ball numbers vector is empty") {
    REQUIRE(board.GetPlayer().GetBallNumbers().size() == 0);
  }
}

TEST_CASE(
    "player wins game after getting 7 balls of same type and eight ball") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  board.CreatePoolBalls();
  std::vector<Ball> balls = board.GetPoolBalls();
  for (size_t i = 1; i <= 7; i++) {
    balls[i].SetPosition({left, bottom});
  }
  // update balls with solid balls except eight ball going into hole
  board.SetPoolBalls(balls);
  board.AdvanceOneFrame();
  balls = board.GetPoolBalls();
  // find eight ball and put into hole position
  for (size_t i = 0; i < balls.size(); i++) {
    if (balls[i].GetBallType() == Ball::eight) {
      balls[i].SetPosition({left, bottom});
      break;
    }
  }
  board.SetPoolBalls(balls);
  board.AdvanceOneFrame();
  SECTION("player type matches type to win") {
    REQUIRE(board.GetPlayer().GetBallTypeToScore() == Ball::solid);
  }
  SECTION("player score is 8") {
    REQUIRE(board.GetPlayer().GetPlayerScore() == 8);
  }
  SECTION("player has ball numbers for all same type ball") {
    std::vector<size_t> ball_numbers = {1, 2, 3, 4, 5, 6, 7, 8};
    REQUIRE(board.GetPlayer().GetBallNumbers() == ball_numbers);
  }
  SECTION("game has correct game state") {
    REQUIRE(board.GetPlayerState() == Player::won);
  }
}

TEST_CASE("player loses game") {
  Board board = Board(1000);
  double bottom = board.GetBottomYBoundary();
  double left = board.GetLeftXBoundary();
  board.CreatePoolBalls();
  std::vector<Ball> balls = board.GetPoolBalls();
  SECTION("eight ball hit into hole first") {
    // find eight ball and put into hole position
    balls[8].SetPosition({left, bottom});
    board.SetPoolBalls(balls);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPlayerState() == Player::lost);
  }
  SECTION("player loses by hitting wrong type of ball") {
    // hits solid ball into hole
    balls[1].SetPosition({left, bottom});
    board.SetPoolBalls(balls);
    board.AdvanceOneFrame();
    // find striped ball to hit
    balls = board.GetPoolBalls();
    // find striped ball to hit it
    for (size_t i = 0; i < balls.size(); i++) {
      if (balls[i].GetBallType() == Ball::striped) {
        balls[i].SetPosition({left, bottom});
        break;
      }
    }
    board.SetPoolBalls(balls);
    board.AdvanceOneFrame();
    REQUIRE(board.GetPlayerState() == Player::lost);
  }
}
