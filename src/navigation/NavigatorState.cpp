//
//  NavigatorState.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "AppParameters.h"
#include "NavigatorState.h"

void PointNavState::outputFields(std::ostream& os) const {
  os << "point: " << _point;
}

void PointNavState::fillInfo(Info &info,
                             const std::string &prefix) const {
  info.add(prefix + "point:", _point);
}

NavigatorStatePtr PointNavState::nextState(Context &context) {
  auto point = context.appParams.core.bounds.randomPoint();
  return std::make_shared<PointNavState>(point);
}

void ObserverNavState::outputFields(std::ostream& os) const {
  os << "entity: " << *_entity;
}

void ObserverNavState::fillInfo(Info &info,
                                const std::string &prefix) const {
  info.add(prefix + "observer:", _entity->id());
}

void OccurrenceNavState::outputFields(std::ostream &os) const {
  os << "entity: " << *_entity;
}

void OccurrenceNavState::fillInfo(Info &info,
                                  const std::string &prefix) const {
  info.add(prefix + "occurrence:", _entity->id());
}

NavigatorStatePtr ObserverNavState::nextState(Context& context) {
  auto other = getRandomEntity(_entity->getConnectedOccurrences());
  if (!other) {
    return NavigatorStatePtr();
  }
  return std::make_shared<OccurrenceNavState>(other);
}

NavigatorStatePtr OccurrenceNavState::nextState(Context& context) {
  auto other = getRandomEntity(_entity->getConnectedObservers());
  if (!other) {
    return NavigatorStatePtr();
  }
  return std::make_shared<ObserverNavState>(other);
}
