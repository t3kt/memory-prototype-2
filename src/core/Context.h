//
//  Context.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef Context_h
#define Context_h

#include <functional>
#include "Common.h"
#include "ObjectManager.h"
#include "State.h"

class AnimationObject;
class MemoryAppParameters;
class NavigatorEntity;
class ObserverEntity;
class OccurrenceEntity;

class Context
: public NonCopyable {
public:
  Context(MemoryAppParameters& appPars)
  : appParams(appPars) { }

  float time() const { return state.time; }

  template<typename E>
  ObjectManager<E>& getEntities();

  template<typename E>
  const ObjectManager<E>& getEntities() const;

  template<typename E>
  void performActionOnEntitiesOfType(std::function<void(E*)> action) {
    getEntities<E>().performTypedAction(action);
  }

  template<typename E, typename T>
  void performTypedActionOnEntities(std::function<void(T*)> action) {
    getEntities<E>().performTypedAction(action);
  }

  MemoryAppParameters& appParams;
  State state;
  ObjectManager<AnimationObject> animations;
  ObjectManager<NavigatorEntity> navigators;
  ObjectManager<ObserverEntity> observers;
  ObjectManager<OccurrenceEntity> occurrences;
};

#endif /* Context_h */
