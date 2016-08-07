//
//  ObserverPreRenderer.cpp
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#include "Context.h"
#include "ObserverEntity.h"
#include "ObserverPreRenderer.h"

ObserverPreRenderer::ObserverPreRenderer(const Params& params,
                                         Context& context)
: EntityPreRenderer(params,
                    context,
                    context.observers) { }

void ObserverPreRenderer::update() {
  auto fadeIn = _fadeIn.getPhrase();
  for (auto& entity : _entities) {
    auto alpha = entity->getRemainingLifetimeFraction();
    auto age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      alpha *= fadeIn->getValue(age);
    }
    alpha = ofClamp(alpha, 0, 1);
    entity->setAlpha(alpha);
  }
}
