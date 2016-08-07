//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include <ofMain.h>
#include "ConnectorRenderer.h"

void ObserverOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  float lowCount = _params.connectionCountRange.lowValue();
  float highCount = _params.connectionCountRange.highValue();
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _occurrences) {
    float occurrenceLife = occurrence->getAmountOfObservation();
    float occurrenceAlpha = ofMap(occurrenceLife,
                                  lowCount,
                                  highCount,
                                  0, 1, true);
    if (occurrenceAlpha <= 0) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceAlpha);
    for (const auto& entry : occurrence->connectedObservers()) {
      auto observer = entry.second;
      if (!observer->visible()) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * observer->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void ObserverObserverConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& observer : _entities) {
    if (!observer->alive()) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * observer->alpha());
    for (const auto& other : observer->getConnectedObservers()) {
      if (!other.second->alive()) {
        continue;
      }
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * other.second->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void OccurrenceOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _entities) {
    if (!occurrence->alive()) {
      continue;
    }
    float occurrenceLife = occurrence->getAmountOfObservation();
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceLife);
    for (const auto& other : occurrence->getConnectedOccurrences()) {
      if (!other.second->alive()) {
        continue;
      }
      float otherLife = other.second->getAmountOfObservation();
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * otherLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}
