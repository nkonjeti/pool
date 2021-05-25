//
// Created by neha konjeti on 4/16/21.
//
#include "board.h"
namespace pool {
Board::Board(double window_size) : cue_stick_(), player_() {
  outer_rect_top_pos_ = {window_size * .05, window_size * .20};
  outer_rect_bottom_pos_ = {window_size * .95, window_size * .8};
  board_outline_width_ = window_size * .05;
  hole_radius_ = window_size * .025;
  inner_rect_bottom_pos_ = {outer_rect_bottom_pos_.x - board_outline_width_,
                            outer_rect_bottom_pos_.y - board_outline_width_};
  inner_rect_top_pos_ = {outer_rect_top_pos_.x + board_outline_width_,
                         outer_rect_top_pos_.y + board_outline_width_};
  // initialize hole positions based on board rectangle positions
  dvec2 hole_one_position = {inner_rect_top_pos_.x, inner_rect_bottom_pos_.y};
  dvec2 hole_two_position = inner_rect_top_pos_;
  dvec2 hole_three_position = {inner_rect_bottom_pos_.x, inner_rect_top_pos_.y};
  dvec2 hole_four_position = inner_rect_bottom_pos_;
  dvec2 hole_five_position = {
      (inner_rect_bottom_pos_.x + inner_rect_top_pos_.x) / 2,
      inner_rect_top_pos_.y};
  dvec2 hole_six_position = {hole_five_position.x, inner_rect_bottom_pos_.y};
  hole_positions_ = {hole_one_position,   hole_two_position,
                     hole_three_position, hole_four_position,
                     hole_five_position,  hole_six_position};
  min_line_length_ = window_size * .1;
  aim_line_length_ = min_line_length_;
  extend_line_length_ = window_size * .02;
}

void Board::MakeCueBall() {
  dvec2 starting_pos = {(inner_rect_top_pos_.x + inner_rect_bottom_pos_.x) / 4,
                        (inner_rect_top_pos_.y + inner_rect_bottom_pos_.y) / 2};
  Ball cue_ball = Ball(0, Ball::cue, starting_pos, {0.0, 0.0});
  balls_.push_back(cue_ball);
}

void Board::MakeSolidPoolBalls() {
  for (size_t ball_number = 1; ball_number <= kNumberOfBallsPerType;
       ball_number++) {
    Ball solid_ball = Ball(ball_number, Ball::solid, {0, 0}, {0, 0});
    balls_.push_back(solid_ball);
  }
}

void Board::MakeEightBall() {
  Ball eight_ball = Ball(kEightBallNumber, Ball::eight, {0, 0}, {0, 0});
  balls_.push_back(eight_ball);
}

void Board::MakeStripedPoolBalls() {
  for (size_t ball_number = kEightBallNumber + 1; ball_number <= kNumberOfBalls;
       ball_number++) {
    Ball striped_ball = Ball(ball_number, Ball::striped, {0, 0}, {0, 0});
    balls_.push_back(striped_ball);
  }
}

void Board::MakeTriangle() {
  // starting pos is center of board
  dvec2 starting_pos = {(inner_rect_top_pos_.x + inner_rect_bottom_pos_.x) / 2,
                        (inner_rect_top_pos_.y + inner_rect_bottom_pos_.y) / 2};
  // number of columns in triangle
  size_t const kNumCols = 5;
  // found order of balls online
  vector<size_t> kBallNumberOrder = {1, 11, 2,  6, 8,  12, 13, 7,
                                     9, 3,  10, 4, 14, 15, 5};
  // this increments by one for each column
  size_t num_balls_in_col = 1;
  size_t index = 0;
  for (size_t c = 0; c < kNumCols; c++) {
    for (size_t num = 0; num < num_balls_in_col; num++) {
      balls_[kBallNumberOrder[index]].SetPosition(starting_pos);
      index += 1;
      // check if there is next ball in column to add to y
      if (num != num_balls_in_col - 1) {
        starting_pos.y += Ball::GetDiameter();
      }
    }
    // subtract the y that was added to the position for previous column
    starting_pos.y -= ((double)(num_balls_in_col - 1) * Ball::GetDiameter());
    // add diameter to starting position x for next column first ball position
    starting_pos.x += Ball::GetDiameter();
    // add radius for new starting position of next column
    starting_pos.y -= Ball::GetDiameter() / 2;
    num_balls_in_col += 1;
  }
}

void Board::DrawHoles() const {
  ci::gl::color(ci::Color("black"));
  for (size_t i = 0; i < hole_positions_.size(); i++) {
    ci::gl::drawSolidCircle(hole_positions_[i], hole_radius_);
  }
}

void Board::CreatePoolBalls() {
  // creates all the ball objects
  MakeCueBall();
  MakeSolidPoolBalls();
  MakeEightBall();
  MakeStripedPoolBalls();
  // puts the balls in triangle shape for staring position
  MakeTriangle();
}

void Board::Display(const vector<ci::gl::Texture2dRef> &images) {
  // draws board outline
  ci::gl::color(ci::Color(kPoolBoardOutlineColor));
  ci::gl::drawSolidRect(ci::Rectf(outer_rect_bottom_pos_, outer_rect_top_pos_));
  // draws inside of board
  ci::gl::color(ci::Color(kPoolBoardColor));
  ci::gl::drawSolidRect(ci::Rectf(inner_rect_bottom_pos_, inner_rect_top_pos_));
  // holes are drawn before balls so ball would appear above hole
  DrawHoles();
  // displays the balls the player hit into holes above the pool board
  if (player_.GetGameState() == Player::playing) {
    for (size_t i = 0; i < balls_.size(); i++) {
      size_t ball_number = balls_[i].GetBallNumber();
      balls_[i].Display(images[ball_number]);
    }
    if (stick_visible_) {
      cue_stick_.Display(balls_[0]);
      DrawLine();
    }
    player_.DisplayBalls(images);
  }
}

void Board::DisplayWinningMessage() const {
  dvec2 text_center_pos = {
      (inner_rect_bottom_pos_.x + inner_rect_top_pos_.x) / 2,
      (inner_rect_bottom_pos_.y + inner_rect_top_pos_.y) / 2};
  ci::gl::drawStringCentered("You Won! Press SPACE to play again!",
                             text_center_pos, "black", ci::Font("Arial", 30));
}

void Board::DisplayLosingMessage() const {
  dvec2 text_center_pos = {
      (inner_rect_bottom_pos_.x + inner_rect_top_pos_.x) / 2,
      (inner_rect_bottom_pos_.y + inner_rect_top_pos_.y) / 2};
  ci::gl::drawStringCentered("You Lost! Press SPACE to play again",
                             text_center_pos, "black", ci::Font("Arial", 30));
}

void Board::DrawLine() const {
  double rad_angle = cue_stick_.GetAngle() + kInitialStickAngle;
  double radius = Ball::GetDiameter() / 2;
  // line starts at cue ball center
  dvec2 cue_ball_center_pos = {balls_[0].GetPosition().x + radius,
                               balls_[0].GetPosition().y + radius};
  // calculate ending position of cue ball using the angle of stick
  dvec2 triangle_legs = {-aim_line_length_ * cos(rad_angle),
                         -aim_line_length_ * sin(rad_angle)};
  dvec2 line_end_pos = cue_ball_center_pos + triangle_legs;
  ci::gl::color(ci::Color("white"));
  ci::gl::drawLine(cue_ball_center_pos, line_end_pos);
}

void Board::HitCueBall() {
  // stick has to be there for ball to be hit
  // prevents cue ball being hit during shot
  if (stick_visible_) {
    double angle = cue_stick_.GetAngle();
    // have to add initial angle of stick
    angle += kInitialStickAngle;
    balls_[0].StickHit(angle);
    stick_visible_ = false;
  }
}

bool Board::CheckIfInHole(const Ball &ball) {
  dvec2 pos = ball.GetPosition();
  // calculate the distance between the center of the ball and the
  // hole center to see if it is less than the hole radius
  dvec2 center_pos = {pos.x + ball.GetDiameter() / 2,
                      pos.y + ball.GetDiameter() / 2};
  for (dvec2 const &hole_position : hole_positions_) {
    dvec2 difference_in_center_pos = {abs(center_pos.x - hole_position.x),
                                      abs(center_pos.y - hole_position.y)};
    double distance = sqrt(pow(difference_in_center_pos.x, 2) +
                           pow(difference_in_center_pos.y, 2));
    if (distance < hole_radius_) {
      return true;
    }
  }
  return false;
}

bool Board::IsCueInHole() {
  return cue_in_hole_;
}

bool Board::CheckOverlap(dvec2 center_pos) {
  double diameter = Ball::GetDiameter();
  // start at 1 because index 0 is the cue ball
  for (size_t i = 1; i < balls_.size(); i++) {
    dvec2 ball_center_pos = {balls_[i].GetPosition().x + diameter / 2,
                             balls_[i].GetPosition().y + diameter / 2};
    dvec2 difference_in_center_pos = {abs(center_pos.x - ball_center_pos.x),
                                      abs(center_pos.y - ball_center_pos.y)};
    double distance = sqrt(pow(difference_in_center_pos.x, 2) +
                           pow(difference_in_center_pos.y, 2));
    if (distance <= diameter) {
      return true;
    }
  }
  return false;
}

void Board::RepositionCueBall(const dvec2 &pos) {
  double diameter = Ball::GetDiameter();
  dvec2 center_pos = {pos.x + diameter / 2, pos.y + diameter / 2};
  // keeps changes x or y of cue ball till there is no overlap in balls
  while (CheckOverlap((center_pos))) {
    if (center_pos.x < inner_rect_bottom_pos_.x - diameter) {
      center_pos.x += diameter;
    } else {
      if (center_pos.y < inner_rect_bottom_pos_.y - diameter) {
        center_pos.y += diameter;
      }
    }
  }
  balls_[0].SetPosition(
      {center_pos.x - diameter / 2, center_pos.y - diameter / 2});
  balls_[0].SetVelocity({0, 0});
  cue_in_hole_ = false;
}

void Board::AdvanceOneFrame() {
  size_t num_balls_moving = 0;
  for (size_t i = 0; i < balls_.size(); i++) {
    if (CheckIfInHole(balls_[i])) {
      if (balls_[i].GetBallType() == Ball::cue) {
        dvec2 center = {(inner_rect_top_pos_.x + inner_rect_bottom_pos_.x) / 2,
                        (inner_rect_top_pos_.y + inner_rect_bottom_pos_.y) / 2};
        RepositionCueBall({center.x, center.y});
        cue_in_hole_ = true;
      } else {
        if (player_.GetBallTypeToScore() == Ball::Type::cue &&
            balls_[i].GetBallType() != Ball::eight) {
          player_.SetBallTypeToScore(balls_[i].GetBallType());
        }
        if (player_.GetBallTypeToScore() == balls_[i].GetBallType()) {
          player_.AddBallNumberScored(balls_[i].GetBallNumber());
          player_.AddBallScore();
          // set to temporary position not on screen
          // to remove later from vector
          // removing now will mess up indices of vector in looping through it
          balls_[i].SetPosition(kOutsideOfView);
        } else if (balls_[i].GetBallType() == Ball::eight) {
          if (player_.GetPlayerScore() == kNumberOfBallsPerType) {
            player_.AddBallNumberScored(balls_[i].GetBallNumber());
            player_.AddBallScore();
            player_.SetGameState(Player::won);
          } else {
            player_.SetGameState(Player::lost);
          }
        } else if (player_.GetBallTypeToScore() != balls_[i].GetBallType()) {
          player_.SetGameState(Player::lost);
        }
      }
    } else {
      balls_[i].HandleBoardCollision(
          inner_rect_bottom_pos_.x, inner_rect_top_pos_.x,
          inner_rect_top_pos_.y, inner_rect_bottom_pos_.y);
      balls_[i].DecreaseVelocity();
      for (size_t j = i; j < balls_.size(); j++) {
        Ball::HandlePoolBallsColliding(balls_[i], balls_[j]);
      }
      balls_[i].Move();
    }
    dvec2 no_velocity = {0.0, 0.0};
    if (balls_[i].GetVelocity() != no_velocity) {
      num_balls_moving += 1;
    }
  }
  // stick is made visible if all balls (including cue ball) are not
  // not moving
  if (num_balls_moving == 0) {
    stick_visible_ = true;
  }
  for (size_t i = 0; i < balls_.size(); i++) {
    if (balls_[i].GetPosition() == kOutsideOfView) {
      balls_.erase(balls_.begin() + i);
    }
  }
}

void Board::ResetBoard() {
  balls_.clear();
  cue_stick_.ResetStick();
  stick_visible_ = true;
  cue_in_hole_ = false;
  player_.ResetPlayer();
}

void Board::UpdateStickRight() {
  cue_stick_.RotateStickCounterClockwise();
}

void Board::UpdateStickLeft() {
  cue_stick_.RotateStickClockwise();
}

void Board::PullStickBackForShot() {
  cue_stick_.PullBackStick();
  double pull_dist = cue_stick_.GetPullBackDistance();
  double max_dist = cue_stick_.GetMaxPullBackDistance();
  double pull_over_max = pull_dist / max_dist;
  double initial_velocity_boost = Ball::GetInitialVelocityBoost();
  balls_[0].SetVelocityBoost(initial_velocity_boost +
                             (kVelocityPower * pull_over_max));
  aim_line_length_ = (cue_stick_.GetPullBackDistance() * extend_line_length_) +
                     min_line_length_;
}

Player::GameState Board::GetPlayerState() const {
  return player_.GetGameState();
}

double Board::GetAimLineLength() const {
  return aim_line_length_;
}

double Board::GetRightXBoundary() const {
  return inner_rect_bottom_pos_.x;
}

double Board::GetLeftXBoundary() const {
  return inner_rect_top_pos_.x;
}

double Board::GetTopYBoundary() const {
  return inner_rect_top_pos_.y;
}

double Board::GetBottomYBoundary() const {
  return inner_rect_bottom_pos_.y;
}

void Board::SetPoolBalls(const vector<Ball> &balls) {
  balls_ = balls;
}

vector<Ball> Board::GetPoolBalls() {
  return balls_;
}

double Board::GetHoleRadius() {
  return hole_radius_;
}

bool Board::GetStickVisibility() {
  return stick_visible_;
}

void Board::SetCueBallPosition(const dvec2 &position) {
  balls_[0].SetPosition(position);
}

Player Board::GetPlayer() const {
  return player_;
}
Stick Board::GetStick() const {
  return cue_stick_;
}
}  // namespace pool
