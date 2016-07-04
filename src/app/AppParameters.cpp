//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

DebugParams::DebugParams()
: Params() {
  add(_showLog
      .setKey("showLog")
      .setName("Show Log")
      .setValueAndDefault(false));
  add(_showBounds
      .setKey("showBounds")
      .setName("Show Bounds")
      .setValueAndDefault(false));
  add(_showStatus
      .setKey("showStatus")
      .setName("Show Status")
      .setValueAndDefault(true));
}

CameraParams::CameraParams()
: ::Params() {
  add(_spinEnabled
      .setKey("spinEnabled")
      .setName("Spin Enabled")
      .setValueAndDefault(false));
  add(_spinRate
      .setKey("spinRate")
      .setName("Spin Rate")
      .setValueAndDefault(ofVec3f(2, 4, 5))
      .setRange(ofVec3f(-10), ofVec3f(10)));
}

MemoryAppParameters::MemoryAppParameters()
: ::Params() {
  add(clock
      .setKey("clock")
      .setName("Clock"));
  add(observers
      .setKey("observers")
      .setName("Observers"));
  add(occurrences
      .setKey("occurrences")
      .setName("Occurrences"));
  add(animations
      .setKey("animations")
      .setName("Animations"));
  add(colors
      .setKey("colors")
      .setName("Colors"));
  add(bounds
      .setKey("bounds")
      .setName("Bounds"));
  add(debug
      .setKey("debug")
      .setName("Debug"));
  add(camera
      .setKey("camera")
      .setName("Camera"));
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(*this);
}
