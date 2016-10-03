//
//  EntityController.h
//  memory
//
//  Created by tekt on 8/5/16.
//
//

#ifndef EntityController_h
#define EntityController_h

#include <memory>
#include "../core/Common.h"
#include "../core/ObjectManager.h"
#include "../core/Params.h"
#include "../core/SimulationEvents.h"

class Context;
class SimulationEvents;

template<typename E>
class EntityController
: public NonCopyable {
public:
  using EntityPtr = std::shared_ptr<E>;
  using EntityEvent = SimulationEvent<E>;
  using EntityEventArgs = SimulationEventArgs<E>;

  EntityController(Context& context,
                   SimulationEvents& events,
                   ObjectManager<E>& entities)
  : _context(context)
  , _events(events)
  , _entities(entities) { }

  virtual void setup() {}

  ObjectManager<E>& entities() { return _entities; }
  const ObjectManager<E>& entities() const { return _entities; }

  void killEntities(int count) {
    int i = 0;
    for (auto& entity : _entities) {
      if (i >= count) {
        return;
      }
      entity->kill();
      i++;
    }
  }

  virtual void update() {
    _entities.processAndCullObjects([&](EntityPtr& entity) {
      entity->update(_context.state);
      if (!entity->alive()) {
        _events.died<E>(*entity);
      }
    });
  }

protected:
  Context& _context;
  SimulationEvents& _events;
  ObjectManager<E>& _entities;
};

#endif /* EntityController_h */
