//
// Created by neha konjeti on 5/4/21.
//

#pragma once
#include "ball.h"
#include "cinder/gl/gl.h"
namespace pool {
using pool::Ball;
using std::vector;
/**
 * Class to keep track of player stats during game.
 */
class Player {
 public:
  /**
   * Constructor to initialize player of game, such as default type of ball they
   * can hit, and state of game to indicate player is playing.
   */
  Player();

  /**
   * Enum to keep track of player game state.
   * won : if they got all the balls needed to win.
   * lost : hit eight ball or wrong type of ball into hole.
   * playing: in progress of playing game.
   */
  enum GameState { won, lost, playing };

  /**
   * Adds one to total count of balls scored,
   * when player scores 7 balls of same type and eight ball
   * they win.
   */
  void AddBallScore();

  /**
   * Displays balls scored by player above game board
   * so they remember which ones they need to win.
   * @param images to draw each ball player scored.
   */
  void DisplayBalls(vector<ci::gl::Texture2dRef> images) const;

  /**
   * Adds the number of the ball that the player scored to vector
   * which keeps track of these numbers to display above game board.
   * @param ball_number represents number of the ball scored
   * and used for the index of the images vector.
   */
  void AddBallNumberScored(size_t ball_number);

  /**
   * Get vector of all the ball numbers that the player hit into hole,
   * should be empty at start of game.
   * @return vector of ball numbers the player scored.
   */
  vector<size_t> GetBallNumbers() const;

  /**
   * The ball type that is determined when the player first scores
   * stripes or solid, player needs to score all balls of same type to win.
   * @return
   */
  Ball::Type GetBallTypeToScore() const;

  /**
   * Sets the ball type the player can hit in the game, determined by the first
   * ball the player scores.
   * @param type : striped or solid.
   */
  void SetBallTypeToScore(Ball::Type type);

  /**
   * Get the number of balls the player has scored, to know if the player has
   * won after hitting the eight ball, or lost if player hasn't gotten all balls
   * of same type before hitting eight ball.
   * @return
   */
  size_t GetPlayerScore() const;

  /**
   * Set the game state of the player, used to change game state of player after
   * winning or losing.
   * @param state representing if player is playing, or has won or lost.
   */
  void SetGameState(GameState state);

  /**
   * Get the current game state of the game to determine what is displayed in
   * the game.
   * @return GameState if player is playing or has lost or won.
   */
  GameState GetGameState() const;

  /**
   * Resets player information such as type of ball they can score, number of
   * balls they scored, and the ball numbers they hit into the hole for new
   * game.
   */
  void ResetPlayer();

 private:
  // how many balls the player had scored of the same type
  // player needs to score all the ball of the same type to win
  size_t num_balls_scored_ = 0;
  // the type of the ball the player can hit, determined by the first shot
  // if player hits wrong type of ball into hole, they lose the game
  Ball::Type type_;
  // keeps track of the ball numbers so the balls the player scored can be
  // displayed and the images vector uses the ball numbers as indices.
  vector<size_t> ball_numbers_scored_ = {};
  // stores current state of game : playing, lost, won
  // so player can play again after losing/ winning
  // and have access to controls when in playing state
  GameState state_;
  // set space between balls the player scored in display above pool board
  double const kSpaceBetweenBalls = 100;
};

}  // namespace pool