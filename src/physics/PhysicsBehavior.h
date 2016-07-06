//
//  PhysicsBehavior.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsBehavior_h
#define PhysicsBehavior_h

#include "Params.h"
#include "State.h"
#include "PhysicsWorld.h"
#include "ParticleObject.h"
#include "Bounds.h"

class AbstractPhysicsBehavior {
public:
  class Params : public ::Params {
  public:
    Params()
    : ::Params() {
      add(_enabled
          .setKey("enabled")
          .setName("Enabled")
          .setValueAndDefault(true));
    }

    bool enabled() const { return _enabled.get(); }
    void setEnabled(bool enabled) { _enabled.set(enabled); }
  protected:
    void setDefaultEnabled(bool enabled) {
      _enabled.setValueAndDefault(enabled);
    }
  private:
    TParam<bool> _enabled;
  };

  virtual void applyToWorld(PhysicsWorld* world) = 0;
};

class BoundsBehavior
: public AbstractPhysicsBehavior {
public:
  BoundsBehavior(const Params& params, const Bounds& bounds)
  : _params(params)
  , _bounds(bounds) { }

  void applyToWorld(PhysicsWorld* world) override;

private:
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);

  const Params& _params;
  const Bounds& _bounds;
};

class AbstractAttractionBehavior
: public AbstractPhysicsBehavior {
public:
  class Params
  : public AbstractPhysicsBehavior::Params {
  public:
    Params()
    : AbstractPhysicsBehavior::Params() {
      add(distanceBounds
          .setKey("distanceBounds")
          .setName("Distance Bounds")
          .setParamValuesAndDefaults(0.04, 0.3)
          .setParamRanges(0, 4)
          .setParamNames("Near", "Far"));
      add(forceRange
          .setKey("forceRange")
          .setName("Force Range")
          .setParamValuesAndDefaults(0.0001, 0)
          .setParamRanges(-0.05, 0.05)
          .setParamNames("Near", "Far"));
    }

    FloatValueRange distanceBounds;
    FloatValueRange forceRange;
  };

  AbstractAttractionBehavior(const Params& params)
  : _params(params) { }

  const Params& _params;
};

template<typename E, typename O>
class AttractionBehavior
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(const Params& params)
  : AbstractAttractionBehavior(params) { }
public:
  void applyToWorld(PhysicsWorld* world) override;
};

class AbstractSpatialNoiseForceBehavior
: public AbstractPhysicsBehavior {
public:
  class Params : public AbstractPhysicsBehavior::Params {
  public:
    Params()
    : AbstractPhysicsBehavior::Params() {
      add(_scale
          .setKey("scale")
          .setName("Scale")
          .setValueAndDefault(1)
          .setRange(0, 4));
      add(_rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(0.1)
          .setRange(0, 0.5));
      add(_magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(0.0001)
          .setRange(0, 0.005));
    }

    bool enabled() const { return _enabled.get(); }
    float scale() const { return _scale.get(); }
    float rate() const { return _rate.get(); }
    float magnitude() const { return _magnitude.get(); }

    void setEnabled(bool enabled) { _enabled.set(enabled); }

  private:
    TParam<bool> _enabled;
    TParam<float> _scale;
    TParam<float> _rate;
    TParam<float> _magnitude;
  };

  AbstractSpatialNoiseForceBehavior(const Params& params)
  : _params(params) { }

protected:
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);

  const Params& _params;
};

template<typename E>
class SpatialNoiseForceBehavior
: public AbstractSpatialNoiseForceBehavior {
public:
  SpatialNoiseForceBehavior(const Params& params)
  : AbstractSpatialNoiseForceBehavior(params) { }

  void applyToWorld(PhysicsWorld* world) override;
};


#endif /* PhysicsBehavior_h */
