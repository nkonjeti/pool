//
// Created by neha konjeti on 4/16/21.
//
#include "pool_app.h"
namespace pool {

PoolApp::PoolApp() : board_(kWindowSize) {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void PoolApp::setup() {
  for (size_t i = 0; i < kBallImagePaths.size(); i++) {
    ci::gl::Texture2dRef texture =
        ci::gl::Texture2d::create(cinder::loadImage(kBallImagePaths[i]));
    images_.push_back(texture);
  }
  board_.CreatePoolBalls();
}

void PoolApp::draw() {
  ci::Color background_color("white");
  ci::gl::clear(background_color);
  board_.Display(images_);
  // message displayed over board
  if (board_.GetPlayerState() == Player::lost) {
    board_.DisplayLosingMessage();
  } else if (board_.GetPlayerState() == Player::won) {
    board_.DisplayWinningMessage();
  }
}

void PoolApp::mouseDrag(ci::app::MouseEvent event) {
  if (board_.GetPlayerState() == Player::playing) {
    if (board_.IsCueInHole()) {
      board_.SetCueBallPosition(event.getPos());
    }
  }
}

void PoolApp::mouseUp(ci::app::MouseEvent event) {
  if (board_.GetPlayerState() == Player::playing) {
    if (board_.IsCueInHole()) {
      board_.RepositionCueBall({static_cast<double>(event.getPos().x),
                                static_cast<double>(event.getPos().y)});
    }
  }
}

void PoolApp::keyDown(ci::app::KeyEvent event) {
  if (board_.GetPlayerState() == Player::playing) {
    if (event.getCode() == ci::app::KeyEvent::KEY_RIGHT) {
      board_.UpdateStickRight();
    } else if (event.getCode() == ci::app::KeyEvent::KEY_LEFT) {
      board_.UpdateStickLeft();
    } else if (event.getCode() == ci::app::KeyEvent::KEY_UP) {
      board_.HitCueBall();
    } else if (event.getCode() == ci::app::KeyEvent::KEY_DOWN) {
      board_.PullStickBackForShot();
    }
  } else {  // to restart game when player loses or wins
    if (event.getCode() == ci::app::KeyEvent::KEY_SPACE) {
      // reset board
      board_.ResetBoard();
      setup();
    }
  }
}

void PoolApp::update() {
  if (board_.GetPlayerState() == Player::playing) {
    board_.AdvanceOneFrame();
  }
}

}  // namespace pool
