//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "ParticleObject.h"

ParticleObject::ParticleObject(ofVec3f pos, const State& state)
: WorldObject(state)
, _velocity(0)
, _force(0) {
  _position = pos;
}

void ParticleObject::setInitialCondition(ofVec3f pos, ofVec3f vel) {
  _position = pos;
  _velocity = vel;
}

void ParticleObject::resetForce() {
  _force.set(0);
}

void ParticleObject::addForce(ofVec3f force) {
  _force += force;
}

void ParticleObject::addDampingForce(float damping) {
  addForce(-_velocity * damping);
}

void ParticleObject::updateVelocityAndPosition(const State &state,
                                               float speed) {
  if (state.timeDelta > 0) {
    _velocity += _force * state.timeDelta;
    _position += _velocity * speed * state.timeDelta;
  }
}

void ParticleObject::outputFields(std::ostream &os) const {
  WorldObject::outputFields(os);
  os << ", velocity: " << _velocity
      << ", force: " << _force;
}

