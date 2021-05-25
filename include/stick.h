//
// Created by neha konjeti on 4/16/21.
//
#pragma once
#include "ball.h"
#include "cinder/gl/gl.h"
namespace pool {
using glm::dvec2;
/**
 * Class to change cue stick position and rotation.
 */
class Stick {
 public:
  /**
   * Empty constructor.
   */
  Stick();

  /**
   * Display stick on board next to cue ball.
   */
  void Display(Ball& ball) const;

  /**
   * Rotates stick clockwise when right arrow is clicked.
   */
  void RotateStickClockwise();

  /**
   * Rotates stick counter clockwise when left arrow is clicked.
   */
  void RotateStickCounterClockwise();

  /**
   * Get angle for hitting cue ball.
   * @return double angle (in radians) to hit cue ball.
   */
  double GetAngle() const;

  /**
   * Reset angle of stick and pulling ability to true for when game is reset.
   */
  void ResetStick();

  /**
   * Updates pull back from stick by adding to it, this will increase the
   * power of the shot.
   */
  void PullBackStick();

  /**
   * Get distance of how much the ball was pulled from stick by player.
   * @return double of how much the stick was pulled using down arrow
   * to determine velocity boost of shot.
   */
  double GetPullBackDistance() const;

  /**
   * Set pull distance for how much the stick was moved back.
   * @param distance the stick was pulled back.
   */
  void SetPullBackDistance(double distance);

  /**
   * Get max distance to calculate percentage that the stick was pulled by
   * player to calculate the velocity boost of the shot.
   * @return max distance the stick can be pulled back by player.
   */
  double GetMaxPullBackDistance() const;

  /**
   * Getter for stick height to check height is adapting to pull and push.
   * @return double representing height of stick.
   */
  double GetStickHeight() const;

 private:
  // width of cue stick
  double width_;
  // height of cue stick
  double height_;
  // in week 3 pull from ball will change when player drags stick away from
  // cue ball
  double const kInitialSpaceFromCueBall = 20;
  // initial height of cue stick
  double const kInitialHeight = 400;
  // the pull distance from ball which affects the velocity boost
  double pull_from_ball_ = 0;
  // current angle the stick is at
  double angle_;
  // how much angle is decreased or increased when stick is rotated
  double const kRotateAngle = .05;
  // how much the stick is pulled back when the player pulls on stick
  // with down arrow
  double const kPullBack = 5;
  // max distance the stick can be pulled back
  double const kMaxPull = 20;
  // if player can pull back
  // if set to false player can move stick closer to cue ball
  bool is_pull_back;
};
}  // namespace pool
