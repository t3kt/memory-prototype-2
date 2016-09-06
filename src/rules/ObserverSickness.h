//
//  ObserverSickness.h
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#ifndef ObserverSickness_h
#define ObserverSickness_h

#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/Scheduler.h"

class ObserverSicknessParams
: public ParamsWithEnabled {
public:
  ObserverSicknessParams() {
    add(scheduler
        .setKey("scheduler")
        .setName("Scheduler"));
    add(lifetimeMult
        .setKey("lifetimeMult")
        .setName("Life Mult.")
        .setRange(0, 1)
        .setValueAndDefault(0.5));
  }

  IntervalSchedulerParams scheduler;
  TParam<float> lifetimeMult;
};

class ObserverSickness {
public:
  using Params = ObserverSicknessParams;

  ObserverSickness(Context& context, const Params& params)
  : _context(context)
  , _params(params)
  , _scheduler(context, params.scheduler) { }

  void update();

private:
  Context& _context;
  const Params& _params;
  IntervalScheduler _scheduler;
};

#endif /* ObserverSickness_h */
