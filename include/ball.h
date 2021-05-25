//
// Created by neha konjeti on 4/16/21.
//
#pragma once
#include "cinder/gl/gl.h"
namespace pool {
using glm::dvec2;

/**
 * Class to update velocities and positions of pool balls during game.
 */
class Ball {
 public:
  /**
   * Enum for pool ball types.
   * Used for calculating points and determining if player hit correct balls
   * to win.
   */
  enum Type { striped, solid, cue, eight };

  /**
   * Constructor for creating pool ball.
   * @param number of pool ball 1 - 15.
   * @param type of pool ball (striped, solid, cue, eight).
   * @param position of pool ball on board.
   * @param velocity of pool ball.
   */
  Ball(size_t number, Type type, const dvec2& position, const dvec2& velocity);

  /**
   * Empty constructor.
   */
  Ball();

  /**
   * Method to draw pool ball.
   */
  void Display(ci::gl::Texture2dRef image) const;

  /**
   * Method to update velocity of pool ball after collision with board.
   * @param right_boundary
   * @param left_boundary
   * @param top_boundary
   * @param bottom_boundary
   * @return if board collided with board (if contact with any of the
   * boundaries)
   */
  bool HandleBoardCollision(double right_boundary, double left_boundary,
                            double top_boundary, double bottom_boundary);

  /**
   * Method to update velocities of pool balls if collision between them
   * occurred.
   * @param ball_one ball in collision.
   * @param ball_two other ball in collision.
   * @return bool if collision occurred between balls.
   */
  static bool HandlePoolBallsColliding(Ball& ball_one, Ball& ball_two);

  /**
   * Getter for ball diameter (used for positioning stick)
   * @return diameter of ball.
   */
  static double GetDiameter();

  /**
   * Get initial velocity boost to add to velocity boost of ball
   * when player pulls stick for more power.
   * @return double representing velocity boost
   */
  static double GetInitialVelocityBoost();

  /**
   * Getter for position of pool ball.
   * @return current position of pool ball.
   */
  dvec2 GetPosition() const;

  /**
   * Getter for velocity of pool ball.
   * @return current velocity of pool ball.
   */
  dvec2 GetVelocity() const;

  /**
   * Trigger velocity increase in cue ball from stick hitting the cue ball.
   */
  void StickHit(double angle);

  /**
   * Method to update position of ball by adding velocity to its position.
   */
  void Move();

  /**
   * Method to decrease velocity to simulate slowing down of the ball.
   */
  void DecreaseVelocity();

  /**
   * Used to change position of ball when ball goes in hole.
   */
  void SetPosition(dvec2 position);

  /**
   * Get ball type for when it is hit into hole.
   * @return enum Ball type
   */
  Ball::Type GetBallType() const;

  void SetVelocity(dvec2 velocity);
  /**
   * Get number of ball used to determine which image to use.
   * @return size_t number of ball
   */
  size_t GetBallNumber() const;

  /**
   * Set velocity boost to determine power the cue ball is hit with.
   * @param velocity increase for cue ball when hit with cue stick.
   */
  void SetVelocityBoost(double velocity_boost);

  /**
   * Get current velocity boost of ball calculated from pull distance power.
   * @return velocity boost used when cue ball is hit.
   */
  double GetVelocityBoost() const;

  // physics constants used to calculate velocity to slow balls
  // down after collisions
  constexpr static double const kFrictionConstant = .15;
  constexpr static double const kSecondsPerFrame = .01428;
  constexpr static double const kGravityConstant = 9.81;

  // diameter of pool ball
  constexpr static const double kDiameter = 25;
  constexpr static const double kInitialVelocityBoost = 3.0;

 private:
  // number on pool ball: 1-7 are solid, 9-15 are striped, 0 is the cue ball, 8 is
  // the eight ball
  size_t number_;
  // current position of ball
  dvec2 position_;
  // current velocity of ball
  dvec2 velocity_;
  // ball type: cue, eight, striped, solid
  Type type_;
  // depending on power of hit velocity boost changes
  double velocity_boost_;
};
}  // namespace pool
