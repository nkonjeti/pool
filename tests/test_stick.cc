//
// Created by neha konjeti on 4/16/21.
//
#include <catch2/catch.hpp>

#include "stick.h"

/*
 * Testing Strategy:
 * Check angle updating when moving clockwise
 * Check angle updating when moving counterclockwise
 * Check stick moving back and forward : pull back distance and height of stick
 * updating
 */

using pool::Stick;
TEST_CASE("stick rotates clockwise") {
  Stick stick = Stick();
  stick.RotateStickClockwise();
  REQUIRE(stick.GetAngle() == M_PI / 2 + .05);
}

TEST_CASE("stick rotates counterclockwise") {
  Stick stick = Stick();
  stick.RotateStickCounterClockwise();
  REQUIRE(stick.GetAngle() == M_PI / 2 - .05);
}

TEST_CASE("check stick is pulled back") {
  Stick stick = Stick();
  stick.PullBackStick();
  REQUIRE(stick.GetPullBackDistance() == 5);
  SECTION("check height of stick adapting to pull") {
    REQUIRE(stick.GetStickHeight() == 405);
  }
}

TEST_CASE("check stick is pulled forward after reaching max distance") {
  Stick stick = Stick();
  double max_dist = stick.GetMaxPullBackDistance();
  stick.SetPullBackDistance(max_dist);
  stick.PullBackStick();
  stick.PullBackStick();
  REQUIRE(stick.GetPullBackDistance() == 15);
  SECTION("check height of stick adapting to push") {
    REQUIRE(stick.GetStickHeight() == 395);
  }
}
