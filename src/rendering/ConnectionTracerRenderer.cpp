//
//  ConnectionTracerRenderer.cpp
//  memory
//
//  Created by tekt on 10/7/16.
//
//

#include <ofAppRunner.h>
#include <ofGraphics.h>
#include <ofMath.h>
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/ConnectionTracerRenderer.h"

void ConnectionTracerRenderer::update() {
  if (!_params.enabled.get()) {
    return;
  }
  _rawRatio += _context.state.timeDelta * _params.rate.get();
  _ratio = ofWrap(_rawRatio, 0, 2);
  if (_ratio > 1) {
    _ratio = 1.0f - _ratio;
  }
}

void ConnectionTracerRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  auto renderer = ofGetCurrentRenderer();
  renderer->pushStyle();
  renderer->setFillMode(OF_OUTLINE);
  for (const auto& entity : _context.observers) {
    for (const auto& connection : entity->getObserverConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
  }
  for (const auto& entity : _context.occurrences) {
    for (const auto& connection : entity->getObserverConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
  }
  renderer->popStyle();
}

void ConnectionTracerRenderer::drawTracer(ParticleObject& entityA,
                                          ParticleObject& entityB,
                                          std::shared_ptr<ofBaseRenderer>& renderer) {
  if (!entityA.visible() || !entityB.visible()) {
    return;
  }
  const auto& posA = entityA.position();
  const auto& posB = entityB.position();
  auto tracerPos = getInterpolated(posA, posB, _ratio);
  auto color = ofFloatColor::white;

  renderer->drawSphere(tracerPos.x,
                       tracerPos.y,
                       tracerPos.z,
                       _params.drawRadius.get());
}
