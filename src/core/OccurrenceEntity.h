//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <ofTypes.h>
#include <map>
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "Params.h"
#include <iostream>
#include "ValueSupplier.h"
#include "Bounds.h"
#include "State.h"
#include "ObjectManager.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params() {
      add(radius
          .setKey("radius")
          .setName("Radius Range")
          .setParamValuesAndDefaults(0.4, 1.3)
          .setParamRanges(0, 4));
    }
    
    RandomValueSupplier<float> radius;
  };
  static shared_ptr<OccurrenceEntity> spawn(const Params& params, const Bounds& bounds, const State& state);
  
  OccurrenceEntity(ofVec3f pos, float radius, const Params& params, const State& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(shared_ptr<ObserverEntity> observer);
  
  void removeObserver(ObjectId id);
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation(const State& state) const;
  
  void update(const State& state) override;
  
  void handleDeath() override;
  
  float originalRadius() const { return _originalRadius; }

  float actualRadius() const { return _actualRadius; }

  const EntityMap<ObserverEntity>& connectedObservers() const {
    return _connectedObservers;
  }

  EntityMap<ObserverEntity>& connectedObservers() {
    return _connectedObservers;
  }

protected:
  std::string typeName() const override { return "OccurrenceEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  void recalculateRadius();
  
  const float _originalRadius;
  float _actualRadius;
  float _startTime;
  EntityMap<ObserverEntity> _connectedObservers;
};

#endif /* OccurrenceEntity_h */
