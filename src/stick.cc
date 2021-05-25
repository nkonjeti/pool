//
// Created by neha konjeti on 4/16/21.
//
#include "stick.h"
namespace pool {
Stick::Stick() {
  // set starting angle of stick
  // so stick is horizontal
  angle_ = M_PI / 2;
  is_pull_back = true;
  height_ = kInitialHeight;
  width_ = 10;
}

void Stick::RotateStickClockwise() {
  angle_ += kRotateAngle;
}
void Stick::RotateStickCounterClockwise() {
  angle_ -= kRotateAngle;
}
double Stick::GetAngle() const {
  return angle_;
}

void Stick::PullBackStick() {
  // player can pull back or pull closer to ball when pressing down arrow
  if (is_pull_back) {
    if (pull_from_ball_ == kMaxPull) {
      is_pull_back = false;
    } else {
      pull_from_ball_ += kPullBack;
      height_ += kPullBack;
    }
  } else {
    if (pull_from_ball_ == 0) {
      is_pull_back = true;
    } else {
      pull_from_ball_ -= kPullBack;
      height_ -= kPullBack;
    }
  }
}

double Stick::GetPullBackDistance() const {
  return pull_from_ball_;
}

void Stick::SetPullBackDistance(double distance) {
  pull_from_ball_ = distance;
}

double Stick::GetMaxPullBackDistance() const {
  return kMaxPull;
}

double Stick::GetStickHeight() const {
  return height_;
}
void Stick::Display(Ball& ball) const {
  ci::gl::color(ci::Color("chocolate"));
  ci::gl::pushModelMatrix();
  // grid is rotated by rad
  double x = ball.GetPosition().x + ball.GetDiameter() / 2;
  double y = ball.GetPosition().y + ball.GetDiameter() / 2;
  ci::gl::translate({x, y});
  ci::gl::rotate(angle_);
  // change position to match rotated grid
  ci::gl::drawSolidRect(
      ci::Rectf({-width_, pull_from_ball_ + kInitialSpaceFromCueBall},
                {width_, height_}));
  ci::gl::popModelMatrix();
}

void Stick::ResetStick() {
  angle_ = M_PI / 2;
  is_pull_back = true;
  height_ = kInitialHeight;
}
}  // namespace pool
