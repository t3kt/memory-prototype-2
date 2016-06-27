//
//  ObserverEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef ObserverEntity_h
#define ObserverEntity_h

#include <ofTypes.h>
#include "Params.h"
#include "Behavior.h"
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include <vector>
#include <iostream>
#include "ValueSupplier.h"
#include "Behavior.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    RandomValueSupplier<float> lifetime;
    RandomValueSupplier<ofVec3f> spawnArea;
    ofParameter<ofFloatColor> color;
    ofParameter<float> drawRadius;
  };
  
  static shared_ptr<ObserverEntity> spawn(const Params& params, const State& state);
  
  ObserverEntity(ofVec3f pos, float life, const Params& params, const State& state);
  virtual ~ObserverEntity() override {}

  void addBehavior(shared_ptr<Behavior<ObserverEntity>> behavior) {
    _behaviors.add(behavior);
  }
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  std::vector<shared_ptr<OccurrenceEntity>>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }
  
  void update(const State& state) override;
  
  void draw(const State& state) override;
  
  void handleDeath() override;
  
  float lifetime() const { return _totalLifetime; };

protected:
  std::string typeName() const override { return "ObserverEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  const Params& _params;
  const float _startTime;
  const float _totalLifetime;
  float _lifeFraction;
  std::vector<shared_ptr<OccurrenceEntity>> _connectedOccurrences;
  BehaviorCollection<ObserverEntity> _behaviors;

  friend class ObserverOccurrenceAttraction;
};

class ObserverOccurrenceAttraction
: public Behavior<ObserverEntity> {
public:
  class Params : public ::Params {
  public:
    Params();

    ValueRange<float> distanceBounds;
    ValueRange<float> forceRange;
  };

  ObserverOccurrenceAttraction(const Params& params) : _params(params) { }

  void update(ObserverEntity& observer, const State& state) override;
private:
  const Params& _params;
};

#endif /* ObserverEntity_h */
