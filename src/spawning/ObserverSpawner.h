//
//  ObserverSpawner.h
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#ifndef ObserverSpawner_h
#define ObserverSpawner_h

#include "Params.h"
#include "Spawner.h"
#include "ValueSupplier.h"

class Bounds;
class Context;
class ObserversController;

class ObserverSpawnerParamsMixin {
public:
  void mixInto(Params& params) {
    params.add(lifetime
               .setKey("lifetime")
               .setName("Lifetime Range")
               .setParamValuesAndDefaults(1, 4)
               .setParamRanges(0, 240));
    params.add(initialVelocity
               .setKey("initialVelocity")
               .setName("Initial Velocity")
               .setParamValuesAndDefaults(0, 4)
               .setParamRanges(0, 20));
  }

  RandomValueSupplier<float> lifetime;
  SimpleRandomVectorSupplier initialVelocity;
};

class RateObserverSpawnerParams
: public RateSpawnerParams
, public ObserverSpawnerParamsMixin {
public:
  RateObserverSpawnerParams() {
    ObserverSpawnerParamsMixin::mixInto(*this);
  }
};

class RateObserverSpawner
: public RateSpawner<RateObserverSpawnerParams> {
public:
  RateObserverSpawner(const Params& params,
                      const Bounds& bounds,
                      ObserversController& controller)
  : RateSpawner(params)
  , _bounds(bounds)
  , _controller(controller) { }

protected:
  void spawnEntities(Context& context, int count) override;

  const Bounds& _bounds;
  ObserversController& _controller;
};

class IntervalObserverSpawnerParams
: public IntervalSpawnerParams
, public ObserverSpawnerParamsMixin {
public:
  IntervalObserverSpawnerParams() {
    ObserverSpawnerParamsMixin::mixInto(*this);
  }
};

class IntervalObserverSpawner
: public IntervalSpawner<IntervalObserverSpawnerParams> {
public:
  IntervalObserverSpawner(const Params& params,
                          const Bounds& bounds,
                          ObserversController& controller)
  : IntervalSpawner(params)
  , _bounds(bounds)
  , _controller(controller) { }
protected:
  void spawnEntities(Context& context) override;

  const Bounds& _bounds;
  ObserversController& _controller;
};

#endif /* ObserverSpawner_h */