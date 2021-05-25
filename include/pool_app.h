//
// Created by neha konjeti on 4/16/21.
//

#ifndef FINAL_PROJECT_NKONJETI_POOL_APP_H
#define FINAL_PROJECT_NKONJETI_POOL_APP_H

#endif  // FINAL_PROJECT_NKONJETI_POOL_APP_H
#include "board.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
namespace pool {
using pool::Board;
/**
 * An app for playing pool.
 */
class PoolApp : public ci::app::App {
 public:
  PoolApp();

  /**
   * Balls are created in this method.
   */
  void setup() override;

  /**
   * Balls, board, stick, holes are drawn.
   */
  void draw() override;

  /**
   * Ball positions and stick angle are updated.
   */
  void update() override;

  /**
   * Method used to drag cue ball by changing its position
   * after getting hit into hole.
   */
  void mouseDrag(ci::app::MouseEvent event) override;

  /**
   * When mouse is up from dragging to set new cue ball position.
   * @param event (get current mouse position)
   */
  void mouseUp(ci::app::MouseEvent event) override;

  /**
   * Method used to determine stick action
   * RIGHT ARROW -> rotate to the right
   * LEFT ARROW -> rotate to the left
   * UP ARROW -> shoot cue ball
   * DOWN ARROW -> to pull cue stick back for more power
   * SPACE -> restart game when game ends
   * @param event to determine stick action.
   */
  void keyDown(ci::app::KeyEvent event) override;

  const int kWindowSize = 1000;

 private:
  Board board_;
  // image paths for loading images
  vector<string> kBallImagePaths = {
      "cue_ball.png", "1.png", "2.png", "3.png", "4.png", "5.png",
      "6.png", "7.png",  "8.png", "9.png", "10.png", "11.png",
      "12.png", "13.png", "14.png", "15.png"};
  // contains images of all the pool balls
  vector<ci::gl::Texture2dRef> images_;
};
}  // namespace pool
