//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "../core/Info.h"
#include "../core/ParticleObject.h"

ParticleObject::ParticleObject(ofVec3f pos,
                               const ClockState& state)
: WorldObject(state)
, _position(pos)
, _velocity(0)
, _force(0)
, _startPosition(pos)
, _size(1) { }

void ParticleObject::resetForce() {
  _force.set(0);
}

void ParticleObject::addForce(ofVec3f force) {
  _force += force;
}

void ParticleObject::updateVelocityAndPosition(const ClockState &state,
                                               float speed) {
  if (state.timeDelta > 0) {
    _velocity += _force * state.timeDelta;
    _position += _velocity * speed * state.timeDelta;
  }
}

void ParticleObject::outputFields(std::ostream &os) const {
  WorldObject::outputFields(os);
  os << ", position: " << _position
      << ", velocity: " << _velocity
      << ", force: " << _force;
}

void ParticleObject::fillInfo(Info &info) const {
  WorldObject::fillInfo(info);
  info.add("position:", _position);
  info.add("velocity:", _velocity);
  info.add("force:", _force);
}

void ParticleObject::addSerializedFields(ofJson &obj,
                                         const SerializationContext &context) const {
  WorldObject::addSerializedFields(obj, context);
  ofxTCommon::JsonUtil::mergeInto(obj, {
    {"position", ofxTCommon::JsonUtil::toJson(_position)},
    {"velocity", ofxTCommon::JsonUtil::toJson(_velocity)},
    // omit force since it's calculated
    {"startPosition", ofxTCommon::JsonUtil::toJson(_startPosition)},
  });
}

void ParticleObject::deserializeFields(const ofJson &obj,
                                       const SerializationContext &context) {
  WorldObject::deserializeFields(obj, context);
  _position = ofxTCommon::JsonUtil::fromJson<ofVec3f>(obj["position"]);
  _velocity = ofxTCommon::JsonUtil::fromJsonField<ofVec3f>(obj, "velocity", ofVec3f::zero());
  _startPosition = ofxTCommon::JsonUtil::fromJson<ofVec3f>(obj["startPosition"]);
}

