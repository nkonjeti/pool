//
// Created by neha konjeti on 5/4/21.
//
#include "player.h"
namespace pool {
Player::Player() {
  // set default to cue till actual type is found
  type_ = Ball::Type::cue;
  state_ = playing;
}

void Player::DisplayBalls(vector<ci::gl::Texture2dRef> images) const {
  dvec2 position = {kSpaceBetweenBalls, kSpaceBetweenBalls};
  for (size_t i = 0; i < ball_numbers_scored_.size(); i++) {
    ci::gl::color(ci::ColorA("white", 1));
    ci::gl::draw(images[ball_numbers_scored_[i]],
                 ci::Rectf(position, {position.x + 2 * Ball::GetDiameter(),
                                      position.y + 2 * Ball::GetDiameter()}));
    position.x += kSpaceBetweenBalls;
  }
}

void Player::AddBallScore() {
  num_balls_scored_ += 1;
}

void Player::AddBallNumberScored(size_t ball_number) {
  ball_numbers_scored_.push_back(ball_number);
}

vector<size_t> Player::GetBallNumbers() const {
  return ball_numbers_scored_;
}

Ball::Type Player::GetBallTypeToScore() const {
  return type_;
}

size_t Player::GetPlayerScore() const {
  return num_balls_scored_;
}

void Player::SetBallTypeToScore(Ball::Type type) {
  type_ = type;
}

void Player::SetGameState(GameState state) {
  state_ = state;
}

Player::GameState Player::GetGameState() const {
  return state_;
}

void Player::ResetPlayer() {
  type_ = Ball::cue;
  state_ = playing;
  ball_numbers_scored_ = {};
  num_balls_scored_ = 0;
}
}  // namespace pool
