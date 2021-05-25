//
// Created by neha konjeti on 4/16/21.
//
#include <catch2/catch.hpp>

#include "board.h"
using glm::dvec2;
using pool::Ball;
using pool::Board;

/**
 * Testing strategy:
 * Board collisions:
 * right, left, top, and bottom boundaries
 * Ball on Ball collision - check correct velocities
 * Cue ball going in angle of stick hitting it
 */

TEST_CASE("cue ball colliding with right wall") {
  Board board = Board(1000);
  double right = board.GetRightXBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double bottom = board.GetBottomYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {right, bottom - 50}, {3.3, 2.2});
  SECTION("collision occurred") {
    REQUIRE(cue_ball.HandleBoardCollision(right, left, top, bottom) == true);
  }
  SECTION("update velocity") {
    cue_ball.HandleBoardCollision(right, left, top, bottom);
    dvec2 new_velocity = {-3.3, 2.2};
    REQUIRE(cue_ball.GetVelocity() == new_velocity);
  }
  SECTION("past right wall") {
    Ball cue_ball_past =
        Ball(0, pool::Ball::cue, {right + 10, bottom - 50}, {3.3, 2.2});
    REQUIRE(cue_ball_past.HandleBoardCollision(right, left, top, bottom) ==
            true);
  }
}

TEST_CASE("cue ball colliding with left wall") {
  Board board = Board(1000);
  double right = board.GetRightXBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double bottom = board.GetBottomYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {left, bottom - 50}, {-3.3, 2.2});
  SECTION("collision occurred") {
    REQUIRE(cue_ball.HandleBoardCollision(right, left, top, bottom) == true);
  }
  SECTION("update velocity") {
    cue_ball.HandleBoardCollision(right, left, top, bottom);
    dvec2 new_velocity = {3.3, 2.2};
    REQUIRE(cue_ball.GetVelocity() == new_velocity);
  }
  SECTION("past left wall") {
    Ball cue_ball_past =
        Ball(0, pool::Ball::cue, {left - 10, bottom - 50}, {3.3, 2.2});
    REQUIRE(cue_ball_past.HandleBoardCollision(right, left, top, bottom) ==
            true);
  }
}

TEST_CASE("cue ball colliding with top wall") {
  Board board = Board(1000);
  double right = board.GetRightXBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double bottom = board.GetBottomYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {left + 60, top}, {-3.3, 2.2});
  SECTION("collision occurred") {
    REQUIRE(cue_ball.HandleBoardCollision(right, left, top, bottom) == true);
  }
  SECTION("update velocity") {
    cue_ball.HandleBoardCollision(right, left, top, bottom);
    dvec2 new_velocity = {-3.3, -2.2};
    REQUIRE(cue_ball.GetVelocity() == new_velocity);
  }
  SECTION("past top wall") {
    Ball cue_ball_past =
        Ball(0, pool::Ball::cue, {left + 40, top - 50}, {3.3, 2.2});
    REQUIRE(cue_ball_past.HandleBoardCollision(right, left, top, bottom) ==
            true);
  }
}

TEST_CASE("cue ball colliding with bottom wall") {
  Board board = Board(1000);
  double right = board.GetRightXBoundary();
  double left = board.GetLeftXBoundary();
  double top = board.GetTopYBoundary();
  double bottom = board.GetBottomYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {left + 60, bottom}, {-3.3, -2.2});
  SECTION("collision occurred") {
    REQUIRE(cue_ball.HandleBoardCollision(right, left, top, bottom) == true);
  }
  SECTION("update velocity") {
    cue_ball.HandleBoardCollision(right, left, top, bottom);
    dvec2 new_velocity = {-3.3, 2.2};
    REQUIRE(cue_ball.GetVelocity() == new_velocity);
  }
  SECTION("past bottom wall") {
    Ball cue_ball_past =
        Ball(0, pool::Ball::cue, {left + 40, bottom + 50}, {3.3, 2.2});
    REQUIRE(cue_ball_past.HandleBoardCollision(right, left, top, bottom) ==
            true);
  }
}

TEST_CASE("collision between two balls") {
  Board board = Board(1000);
  double left = board.GetLeftXBoundary();
  double bottom = board.GetBottomYBoundary();
  double radius = Ball::GetDiameter() / 2;
  Ball cue_ball = Ball(0, pool::Ball::cue,
                       {left + 50 - radius, bottom - 50 - radius}, {3.3, 2.2});
  Ball ball = Ball(1, pool::Ball::solid,
                   {left + 60 - radius, bottom - 50 - radius}, {-1.3, 2});
  SECTION("collision occurred") {
    REQUIRE(Ball::HandlePoolBallsColliding(cue_ball, ball) == true);
  }
  SECTION("accurate velocities") {
    Ball::HandlePoolBallsColliding(cue_ball, ball);
    dvec2 cue_ball_velocity = {-1.3, 2.2};
    dvec2 ball_velocity = {3.3, 2.0};
    REQUIRE(cue_ball.GetVelocity()[0] == Approx(cue_ball_velocity[0]));
    REQUIRE(cue_ball.GetVelocity()[1] == Approx(cue_ball_velocity[1]));
    REQUIRE(ball.GetVelocity()[0] == Approx(ball_velocity[0]));
    REQUIRE(ball.GetVelocity()[1] == Approx(ball_velocity[1]));
  }
}

TEST_CASE("ball moves in angle direction of stick hit") {
  Board board = Board(1000);
  double left = board.GetLeftXBoundary();
  double bottom = board.GetBottomYBoundary();
  Ball cue_ball = Ball(0, pool::Ball::cue, {left + 50, bottom - 50}, {0, 0});
  cue_ball.SetVelocityBoost(6.0);
  cue_ball.StickHit(.2);
  // -cos(angle) * 6, -sin(angle) * 6
  dvec2 new_velocity = {-5.8804, -1.19202};
  REQUIRE(cue_ball.GetVelocity().x == Approx(new_velocity.x));
  REQUIRE(cue_ball.GetVelocity().y == Approx(new_velocity.y));
}
