//
//  Clock.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Clock_h
#define Clock_h

#include <ofParameter.h>
#include <MSATimer.h>
#include "State.h"
#include "Params.h"
#include "Status.h"

class Clock
: public StatusInfoProvider {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

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

  const StatusInfo& getStatusInfo() const { return _status; }
private:
  void start();
  void stop();
  void onPausedChanged(bool& paused);

  class _Timer : public msa::Timer {
    friend class Clock;
  };

  State& _state;
  Params& _params;
  _Timer _timer;
  StatusInfo _status;
  std::size_t STATUS_STATE;
  std::size_t STATUS_TIME;
};

#endif /* Clock_h */
