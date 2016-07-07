//
//  Clock.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Clock_h
#define Clock_h

#include <memory>
#include <ofParameter.h>
#include "State.h"
#include "Params.h"

class Clock {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(_paused
          .setKey("paused")
          .setName("Paused")
          .setValueAndDefault(false));
      add(_rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(1)
          .setRange(0, 4));
    }

    bool paused() const { return _paused.get(); }
    float rate() const { return _rate.get(); }

    void setPaused(bool paused) { _paused.set(paused); }

  private:
    TParam<bool> _paused;
    TParam<float> _rate;
  };

  Clock(Params& params, State& state);

  void setup();

  void toggleState();

  void update();

  bool isRunning() const { return _isRunning; }

  float time() const { return _state.time; }
private:
  void start();
  void stop();
  void onPausedChanged(bool& paused);

  void DUMP_STATE(std::string message) const;

  State& _state;
  Params& _params;
  float _lastTime;
  bool _isRunning;
};

#endif /* Clock_h */
