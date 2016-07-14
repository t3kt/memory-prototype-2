//
//  Spawner.h
//  memory
//
//  Created by tekt on 7/14/16.
//
//

#ifndef Spawner_h
#define Spawner_h

#include <memory>
#include "Params.h"
#include "State.h"

using SpawnerParams = ParamsWithEnabled;

class IntervalSpawnerParams : public SpawnerParams {
public:
  IntervalSpawnerParams() {
    add(_interval
        .setKey("interval")
        .setName("Interval")
        .setValueAndDefault(4)
        .setRange(0, 60));
  }

  void setIntervalValueAndDefault(float interval) {
    _interval.setValueAndDefault(interval);
  }

  float interval() const { return _interval.get(); }
private:
  TParam<float> _interval;
};

template<typename E>
class IntervalSpawner {
public:
  using Params = IntervalSpawnerParams;

  IntervalSpawner(const Params& params)
  : _params(params) {}

  void update(const State& state) {
    if (_nextTime < 0 || state.time >= _nextTime) {
      spawnEntities(state);
      _nextTime = state.time + _params.interval();
    }
  }
protected:
  virtual void spawnEntities(const State& state) = 0;

  const Params& _params;
  float _nextTime;
};

#endif /* Spawner_h */