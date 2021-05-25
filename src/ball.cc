//
// Created by neha konjeti on 4/16/21.
//
#include "ball.h"
namespace pool {
Ball::Ball() {
}

Ball::Ball(size_t number, const Type type, const dvec2& position,
           const dvec2& velocity) {
  number_ = number;
  type_ = type;
  position_ = position;
  velocity_ = velocity;
  velocity_boost_ = kInitialVelocityBoost;
}

void Ball::Display(ci::gl::Texture2dRef image) const {
  ci::gl::color(ci::ColorA("white", 1));
  ci::gl::draw(image, ci::Rectf(position_, {position_.x + kDiameter,
                                            position_.y + kDiameter}));
}

double Ball::GetDiameter() {
  return kDiameter;
}

dvec2 Ball::GetPosition() const {
  return position_;
}

dvec2 Ball::GetVelocity() const {
  return velocity_;
}

bool Ball::HandleBoardCollision(double right_boundary, double left_boundary,
                                double top_boundary, double bottom_boundary) {
  // board collisions
  if (position_.x <= left_boundary ||
      position_.x + kDiameter >= right_boundary) {
    velocity_.x *= -1;
    return true;
  } else if (position_.y + kDiameter >= bottom_boundary ||
             position_.y <= top_boundary) {
    velocity_.y *= -1;
    return true;
  }
  return false;
}

bool Ball::HandlePoolBallsColliding(Ball& ball_one, Ball& ball_two) {
  double radius = GetDiameter() / 2;
  dvec2 velocity_one = ball_one.velocity_;
  dvec2 velocity_two = ball_two.velocity_;
  // get center positions of balls
  // to better judge if they are colliding
  dvec2 position_one = {ball_one.position_.x + radius,
                        ball_one.position_.y + radius};
  dvec2 position_two = {ball_two.position_.x + radius,
                        ball_two.position_.y + radius};
  dvec2 velocity_difference = velocity_one - velocity_two;
  dvec2 position_difference = position_one - position_two;
  double between_dist = glm::distance(position_one, position_two);
  double dot_prod = glm::dot(velocity_difference, position_difference);
  bool is_collision = between_dist <= 2 * radius;
  // if particles are not moving towards each other
  if (dot_prod >= 0) {
    return false;
  }
  if (!is_collision) {
    return false;
  }

  // calculate new velocities
  double squared_difference_length = pow(glm::length(position_difference), 2);
  double dot_over_length = dot_prod / squared_difference_length;
  // change velocity for first ball
  dvec2 updated_velocity_one =
      velocity_one - (dot_over_length * position_difference);
  // change velocity for other ball
  dvec2 velocity_difference_two = velocity_two - velocity_one;
  dvec2 position_difference_two = position_two - position_one;
  double dot_prod_two =
      glm::dot(velocity_difference_two, position_difference_two);
  double squared_difference_length_two =
      pow(glm::length(position_difference_two), 2);
  double dot_over_length_two = dot_prod_two / squared_difference_length_two;
  dvec2 updated_velocity_two =
      velocity_two - (dot_over_length_two * position_difference_two);
  // update velocities
  ball_one.velocity_ = updated_velocity_one;
  ball_two.velocity_ = updated_velocity_two;
  return true;
}

void Ball::StickHit(double angle) {
  // velocity boost depends on angle ball is hit at by stick
  dvec2 velocity_boost = {-velocity_boost_ * cos(angle),
                          -velocity_boost_ * sin(angle)};
  velocity_ += velocity_boost;
}

void Ball::DecreaseVelocity() {
  // reduces velocity for ball to stop rolling
  // Vf = Vi - (friction constant) * g * t
  double const kReduceVelocity =
      kGravityConstant * kFrictionConstant * kSecondsPerFrame;
  // positive x and y velocities
  if (velocity_.y > kReduceVelocity) {
    velocity_.y -= kReduceVelocity;
  } else if (velocity_.y < kReduceVelocity && velocity_.y > 0) {
    velocity_.y = 0;  // prevents lingering velocity
  }
  if (velocity_.x > kReduceVelocity) {
    velocity_.x -= kReduceVelocity;
  } else if (velocity_.x < kReduceVelocity && velocity_.x > 0) {
    velocity_.x = 0;
  }
  // negative x and y velocities
  if (velocity_.y < -kReduceVelocity) {
    velocity_.y += kReduceVelocity;
  } else if (velocity_.y > -kReduceVelocity && velocity_.y < 0) {
    velocity_.y = 0;
  }
  if (velocity_.x < -kReduceVelocity) {
    velocity_.x += kReduceVelocity;
  } else if (velocity_.x > -kReduceVelocity && velocity_.x < 0) {
    velocity_.x = 0;
  }
}

void Ball::SetVelocityBoost(double velocity_boost) {
  velocity_boost_ = velocity_boost;
}

double Ball::GetInitialVelocityBoost() {
  return kInitialVelocityBoost;
}

double Ball::GetVelocityBoost() const {
  return velocity_boost_;
}

void Ball::SetPosition(dvec2 position) {
  position_ = position;
}

void Ball::SetVelocity(dvec2 velocity) {
  velocity_ = velocity;
}

size_t Ball::GetBallNumber() const {
  return number_;
}

void Ball::Move() {
  position_ += velocity_;
}

Ball::Type Ball::GetBallType() const {
  return type_;
}
}  // namespace pool
