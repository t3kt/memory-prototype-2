//
//  ObserversController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "../core/AnimationObject.h"
#include "../core/ObserversController.h"
#include "../core/OccurrenceEntity.h"
#include "../core/SimulationEvents.h"

ObserversController::ObserversController(const Params& params,
                                         const Bounds& bounds,
                                         Context& context,
                                         SimulationEvents& events)
: EntityController(context,
                   events,
                   context.observers)
, _params(params)
, _bounds(bounds) { }

void ObserversController::setup() {
  _rateSpawner = std::make_shared<RateObserverSpawner>(_params.rateSpawner,
                                                       _bounds,
                                                       *this);

  registerAsActionHandler();
}

bool ObserversController::performAction(AppAction action) {
  switch (action) {
    case AppAction::SPAWN_FEW_OBSERVERS:
      spawnObservers(5);
      break;
    case AppAction::SPAWN_MANY_OBSERVERS:
      spawnObservers(100);
      break;
    case AppAction::SPAWN_TONS_OF_OBSERVERS:
      spawnObservers(4000);
      break;
    case AppAction::KILL_FEW_OBSERVERS:
      killObservers(5);
      break;
    case AppAction::KILL_MANY_OBSERVERS:
      killObservers(100);
      break;
    default:
      return false;
  }
  return true;
}

void ObserversController::update() {
  _entities.performAction([&](std::shared_ptr<ObserverEntity>& observer) {
    observer->update(_context.state);
  });

  _entities.cullDeadObjects([&](std::shared_ptr<ObserverEntity> observer) {
    observer->detachConnections();
    ObserverEventArgs e(SimulationEventType::OBSERVER_DIED,
                        *observer);
    _events.observerDied.notifyListeners(e);
  });

  _rateSpawner->update(_context);
  _context.state.observerCount = _entities.size();
}

void ObserversController::draw() {
}

bool ObserversController::registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  bool connected = false;
  
  _entities.performAction([&] (std::shared_ptr<ObserverEntity>& observer) {
    float dist = occurrence->position().distance(observer->position());
    if (dist <= occurrence->originalRadius()) {
      occurrence->addObserver(observer);
      observer->addOccurrence(occurrence);
      connected = true;
    }
    
  });
  return connected;
}

void ObserversController::spawnObservers(int count) {
  if (_rateSpawner->spawnNow(_context, count)) {
    return;
  }
}

bool ObserversController::tryAddEntity(std::shared_ptr<ObserverEntity> entity) {
  _entities.add(entity);
  ObserverEventArgs e(SimulationEventType::OBSERVER_SPAWNED,
                      *entity);
  _events.observerSpawned.notifyListeners(e);
  return true;
}

void ObserversController::killObservers(int count) {
  int i = 0;
  for (auto& observer : _entities) {
    if (i >= count) {
      return;
    }
    observer->kill();
    i++;
  }
}
