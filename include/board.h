//
// Created by neha konjeti on 4/16/21.
//
#pragma once
#include <vector>

#include "ball.h"
#include "cinder/gl/gl.h"
#include "player.h"
#include "stick.h"
namespace pool {
using glm::dvec2;
using pool::Ball;
using pool::Player;
using pool::Stick;
using std::string;
using std::vector;

/**
 * Class to initialize pool balls and stick and direct actions between these.
 */
class Board {
 public:
  /**
   * Initializes vectors for outline and inside of board.
   * @param window_size size_t to scale the board respective to window size.
   */
  Board(double window_size);

  /**
   * Method to create billiard balls in board.
   */
  void CreatePoolBalls();

  /**
   * Method to display billiard board.
   */
  void Display(const vector<ci::gl::Texture2dRef> &images);

  /**
   * Method to rotate stick when right arrow is clicked.
   */
  void UpdateStickRight();

  /**
   * Method to rotate stick when left arrow is clicked.
   */
  void UpdateStickLeft();

  /**
   * Method to trigger updating balls velocity and position from cue stick
   * hitting it.
   */
  void HitCueBall();

  /**
   * Method to update ball positions on billiard board.
   */
  void AdvanceOneFrame();

  /**
   * Get the left x position for ball with left side of board collision.
   * @return double of left x position of pool board.
   */
  double GetLeftXBoundary() const;

  /**
   * Get the right x position for ball with right side of board collision.
   * @return double of right x position of pool board.
   */
  double GetRightXBoundary() const;

  /**
   * Get the top y position for ball with top side of board collision.
   * @return double of top y position of pool board.
   */
  double GetTopYBoundary() const;

  /**
   * Get the bottom y position for ball with bottom side of board collision.
   * @return double of bottom y position of pool board.
   */
  double GetBottomYBoundary() const;

  /**
   * Setter for balls vector used in testing.
   * @param balls represents Ball objects in pool board.
   */
  void SetPoolBalls(const vector<Ball> &balls);

  /**
   * Getter for balls vector used in testing to check balls velocities are
   * updating.
   */
  vector<Ball> GetPoolBalls();

  /**
   * Getter for radius of all the holes.
   * @return double radius of hole.
   */
  double GetHoleRadius();

  /**
   * Getter for stick visibility to test if stick is visible during shot.
   * @return boolean if stick is visible.
   */
  bool GetStickVisibility();

  /**
   * Getter for variable which keeps track if cue ball is currently in any
   * of the holes so cue ball can be dragged to new point.
   * @return boolean if cue ball was hit into hole.
   */
  bool IsCueInHole();

  /**
   * Changes cue ball position after getting hit into hole.
   * Places in position requested and if position requested is occupied,
   * cue ball is shifted accordingly.
   */
  void RepositionCueBall(const dvec2 &pos);

  /**
   * Sets cue ball position for when it's being dragged by player
   * after getting hit into hole.
   * @param position (current position of mouse) when being dragged.
   */
  void SetCueBallPosition(const dvec2 &position);

  /**
   * Calculates percentage of max distance that the player pulled the stick
   * from the cue ball to add to initial velocity boost.
   */
  void PullStickBackForShot();

  /**
   * Get the player's state: win, lose, playing to determine what to display in
   * app.
   * @return current state of player's game (win, lose, playing)
   */
  Player::GameState GetPlayerState() const;

  /**
   * If player won, a winning message is shown on pool board prompting player
   * to play again.
   */
  void DisplayWinningMessage() const;

  /**
   * If player lost, a losing message is shown on pool board prompting player to
   * play again.
   */
  void DisplayLosingMessage() const;

  /**
   * Board pieces are reset like the balls, player, and stick
   * to original positions and initial game setting.
   */
  void ResetBoard();

  /**
   * Get the line length for aim which depends on the pull back distance of
   * stick.
   * @return line length of aim.
   */
  double GetAimLineLength() const;

  /**
   * Get player object with information pertaining to player : ball type, score,
   * number of balls scored by player.
   * @return player that is playing pool game.
   */
  Player GetPlayer() const;

  /**
   * Get Stick object with information pertaining to cue stick such as current
   * angle.
   * @return Stick object used in game.
   */
  Stick GetStick() const;

 private:
  /**
   * Helper method to create Ball objects for solid pool balls.
   */
  void MakeSolidPoolBalls();

  /**
   * Helper method to make cue ball and place in beginning position for game.
   */
  void MakeCueBall();

  /**
   * Helper method to create Ball objects for striped pool balls.
   */
  void MakeStripedPoolBalls();

  /**
   * Helper method to create eight ball Ball object.
   */
  void MakeEightBall();

  /**
   * Helper method to draw the holes on the board.
   */
  void DrawHoles() const;

  /**
   * Checks if ball went into hole from position on board.
   * @param ball used to check if ball position is in hole.
   * @return if ball position was in hole circle.
   */
  bool CheckIfInHole(const Ball &ball);

  /**
   * Change balls positions to make beginning triangle formation.
   */
  void MakeTriangle();

  /**
   * Helper to check distance between cue ball center position and other balls
   * in board center positions for overlap when repositioning cue ball.
   * @param center_pos (mouse position to where cue ball is dragged to)
   * @return if there is overlap between cue ball and other ball on board.
   */
  bool CheckOverlap(dvec2 center_pos);

  /**
   * Draws line for shooting cue ball,
   * helps player with aim and uses stick angle to be drawn.
   */
  void DrawLine() const;

  // vector of all the balls on the board
  // balls_[0] is the cue ball
  vector<Ball> balls_;
  // stores angle of stick and hits cue ball
  Stick cue_stick_;
  // player keeps track of balls scored and game state
  Player player_;
  // number of balls in a pool game
  // excluding the cue ball
  size_t const kNumberOfBalls = 15;
  // number of striped or solid balls
  size_t const kNumberOfBallsPerType = 7;
  // outline of board positions
  dvec2 outer_rect_bottom_pos_;
  dvec2 outer_rect_top_pos_;
  // width between outline of board and playing space
  double board_outline_width_;
  // inside rectangle of board positions
  dvec2 inner_rect_top_pos_;
  dvec2 inner_rect_bottom_pos_;
  size_t const kEightBallNumber = 8;
  // colors to draw board
  ci::Color const kPoolBoardColor = "green";
  ci::Color const kPoolBoardOutlineColor = "sienna";
  double hole_radius_;
  // stores all the hole center positions
  vector<dvec2> hole_positions_;
  // if the stick is visible on board
  // false during shots (when balls on board are moving)
  bool stick_visible_ = true;
  // if cue is in hole to trigger repositioning of cue ball
  bool cue_in_hole_ = false;
  // initial angle taken into account for the ball moving in angle direction
  // of stick
  double const kInitialStickAngle = M_PI / 2;
  // velocity power with pull back distance of stick
  // is used to calculate velocity boost
  double const kVelocityPower = 6.0;
  // starting line length of aim
  double min_line_length_;
  // current line length of aim
  double aim_line_length_;
  // how much line length extends when the pull distance of stick changes
  double extend_line_length_;
  // position not in screen to temporarily assign position to ball
  // that got hit into holes
  // use this position to find those balls and remove from balls_ vector
  dvec2 const kOutsideOfView = {-100.0, -100.0};
};
}  // namespace pool
