//
//  NavigatorEntity.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef NavigatorEntity_h
#define NavigatorEntity_h

#include <memory>
#include "../core/ParticleObject.h"

class Context;
class NavigatorState;
using NavigatorStatePtr = std::shared_ptr<NavigatorState>;

class NavigatorEntity
: public ParticleObject {
public:
  static std::shared_ptr<NavigatorEntity> createEmpty() {
    // unsupported
    return std::shared_ptr<NavigatorEntity>();
  }

  NavigatorEntity(NavigatorStatePtr prevState);

  const ofVec3f& prevPosition() const;
  const ofVec3f& nextPosition() const;

  NavigatorState* prevState() { return _prevState.get(); }
  NavigatorState* nextState() { return _nextState.get(); }

  const ofVec3f& targetPoint() const;

  EntityType entityType() const override { return EntityType::NAVIGATOR; }

  void updateNextState(Context& context);
  void reachNextState(Context& context);

  bool stateAlive() const;

  std::string typeName() const override { return "NavigatorEntity"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  NavigatorStatePtr _prevState;
  NavigatorStatePtr _nextState;
};

#endif /* NavigatorEntity_h */
