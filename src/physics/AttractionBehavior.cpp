//
//  AttractionBehavior.cpp
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#include "AttractionBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

template<>
EntityMap<OccurrenceEntity>& AttractionBehavior<ObserverEntity, OccurrenceEntity>::getEntityOthers(ObserverEntity *entity) {
  return entity->getConnectedOccurrences();
}

template<>
EntityMap<ObserverEntity>& AttractionBehavior<OccurrenceEntity, ObserverEntity>::getEntityOthers(OccurrenceEntity *entity) {
  return entity->connectedObservers();
}

ofVec3f AbstractAttractionBehavior::calcAttractionForce(ParticleObject *entity, const ofVec3f &otherPosition) {
  ofVec3f posDiff = otherPosition - entity->position();
  float lowBound = _params.distanceBounds.lowValue();
  float highBound = _params.distanceBounds.highValue();
  float magnitude = _params.signedMagnitude();
  float dist = posDiff.length();
  if (dist < lowBound || dist > highBound) {
    return ofVec3f(0);
  }
  posDiff.normalize();
  float mag = ofMap(dist,
                    lowBound, highBound,
                    magnitude, 0,
                    true);
  return posDiff * mag;
}

void AbstractAttractionBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::green);
}

void AbstractAttractionBehavior::debugDrawEntity(ParticleObject *entity,
                                                 const ofVec3f &force) {
  drawForceArrow(entity->position(),
                 force);
  debugDrawEntityRange(entity);
}

void AbstractAttractionBehavior::debugDrawEntityRange(ParticleObject *entity) {
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetColor(ofFloatColor(0.9, 0.1, 0.1, 0.01));
  ofDrawSphere(entity->position(), _params.distanceBounds.lowValue());
  ofSetColor(ofFloatColor(0.1, 0.1, 0.9, 0.03));
  ofDrawSphere(entity->position(), _params.distanceBounds.highValue());
  ofPopStyle();
}

void AbstractAttractionBehavior::endDebugDraw() {
  ofPopStyle();
}