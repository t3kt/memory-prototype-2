//
//  PhysicsController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsController_h
#define PhysicsController_h

#include "AnchorPointBehavior.h"
#include "AttractionBehavior.h"
#include "Bounds.h"
#include "DampingBehavior.h"
#include "ForceFieldBehavior.h"
#include "Params.h"
#include "PhysicsWorld.h"
#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ObserversController.h"
#include "OccurrencesController.h"

class DebugParams;

class EntityPhysicsParams
: public Params {
public:
  EntityPhysicsParams() {
    add(_speed
        .setKey("speed")
        .setName("Speed")
        .setValueAndDefault(1)
        .setRange(0, 10));
  }

  float speed() const { return _speed.get(); }

private:
  TParam<float> _speed;
};

class PhysicsController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(observers
          .setKey("observers")
          .setName("Observers"));
      add(occurrences
          .setKey("occurrences")
          .setName("Occurrences"));
      add(rebound
          .setKey("rebound")
          .setName("Rebound"));
      add(observerOccurrenceAttraction
          .setKey("observerOccurrenceAttraction")
          .setName("Observer to Occurrence"));
      add(occurrenceObserverAttraction
          .setKey("occurrenceObserverAttraction")
          .setName("Occurrence to Observer"));
      add(observerSpatialNoiseForce
          .setKey("observerSpatialNoiseForce")
          .setName("Observer Spatial Noise"));
      add(occurrenceSpatialNoiseForce
          .setKey("occurrenceSpatialNoiseForce")
          .setName("Occurrence Spatial Noise"));
      add(observerAnchorPointAttraction
          .setKey("observerAnchorPointAttraction")
          .setName("Observer Anchoring"));
      add(occurrenceAnchorPointAttraction
          .setKey("occurrenceAnchorPointAttraction")
          .setName("Occurrence Anchoring"));
      add(observerDamping
          .setKey("observerDamping")
          .setName("Observer Damping"));
      add(occurrenceDamping
          .setKey("occurrenceDamping")
          .setName("Occurrence Damping"));
      rebound.setEnabledValueAndDefault(true);
      observerSpatialNoiseForce.setEnabledValueAndDefault(false);
      occurrenceObserverAttraction.setEnabledValueAndDefault(false);
      observerAnchorPointAttraction.setEnabledValueAndDefault(false);
      occurrenceAnchorPointAttraction.setEnabledValueAndDefault(true);
      observerDamping.setEnabledValueAndDefault(true);
      occurrenceDamping.setEnabledValueAndDefault(true);
    }

    EntityPhysicsParams observers;
    EntityPhysicsParams occurrences;
    ParamsWithEnabled rebound;
    AbstractAttractionBehavior::Params observerOccurrenceAttraction;
    AbstractAttractionBehavior::Params occurrenceObserverAttraction;
    AbstractNoiseForceFieldBehavior::Params observerSpatialNoiseForce;
    AbstractNoiseForceFieldBehavior::Params occurrenceSpatialNoiseForce;
    AbstractAttractionBehavior::Params observerAnchorPointAttraction;
    AbstractAttractionBehavior::Params occurrenceAnchorPointAttraction;
    AbstractDampingBehavior::Params observerDamping;
    AbstractDampingBehavior::Params occurrenceDamping;
  };

  PhysicsController(const Params& params,
                    const Bounds& bounds,
                    const DebugParams& debugParams,
                    const State& state);

  void stopAllEntities();

  void setup(ObserversController& observers,
             OccurrencesController& occurrences);

  void update();
  void draw();

private:
  void beginEntityUpdate(ParticleObject* entity, const EntityPhysicsParams& params);
  void endEntityUpdate(ParticleObject* entity, const EntityPhysicsParams& params);

  const Params& _params;
  const Bounds& _bounds;
  const DebugParams& _debugParams;
  const State& _state;

  std::shared_ptr<PhysicsWorld> _world;

  std::shared_ptr<BoundsBehavior> _rebound;
  std::shared_ptr<AttractionBehavior<ObserverEntity, OccurrenceEntity>> _observerOccurrenceAttraction;
  std::shared_ptr<AttractionBehavior<OccurrenceEntity, ObserverEntity>> _occurrenceObserverAttraction;
  std::shared_ptr<NoiseForceFieldBehavior<ObserverEntity>> _observerSpatialNoiseForce;
  std::shared_ptr<NoiseForceFieldBehavior<OccurrenceEntity>> _occurrenceSpatialNoiseForce;
  std::shared_ptr<AnchorPointBehavior<ObserverEntity>> _observerAnchorPointAttraction;
  std::shared_ptr<AnchorPointBehavior<OccurrenceEntity>> _occurrenceAnchorPointAttraction;
  std::shared_ptr<DampingBehavior<ObserverEntity>> _observerDamping;
  std::shared_ptr<DampingBehavior<OccurrenceEntity>> _occurrenceDamping;
};

#endif /* PhysicsController_h */