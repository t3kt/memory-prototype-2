//
//  Clock.cpp
//
//

#include <ofUtils.h>
#include "../core/Clock.h"

Clock::Clock(Clock::Params& params, State& state)
: _params(params)
, _state(state) {
}

void Clock::setup() {
  _lastTime = ofGetElapsedTimef();
  _state.running = true;
  _state.time = 0;
  _state.timeDelta = 0;

  registerAsActionHandler();
}

bool Clock::performAction(AppAction action) {
  switch (action) {
    case AppAction::TOGGLE_CLOCK_STATE:
      toggleState();
      break;
    default:
      return false;
  }
  return true;
}

void Clock::start() {
  _lastTime = ofGetElapsedTimef();
  _state.running = true;
}

void Clock::stop() {
  _state.running = false;
}

void Clock::toggleState() {
  _params.paused.toggle();
}

void Clock::update() {
  if (_params.paused()) {
    if (_state.running) {
      stop();
    }
    _state.timeDelta = 0;
  } else {
    if (!_state.running) {
      start();
    }
    float nowTime = ofGetElapsedTimef();
    float rawDelta = nowTime - _lastTime;
    float delta = rawDelta * _params.rate();
    _state.timeDelta = delta;
    _state.time += delta;
    _lastTime = nowTime;
  }
}
