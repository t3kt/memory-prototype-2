//
//  OccurrencesController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "OccurrencesController.h"

OccurrencesController::OccurrencesController(const OccurrencesController::Params& params,
                                             const Bounds& bounds,
                                             ObserversController& observers,
                                             const State& state,
                                             SimulationEvents& events)
: _params(params)
, _bounds(bounds)
, _events(events)
, _spawnInterval(params.spawnInterval, state)
, _observers(observers) {
}

void OccurrencesController::setup(const State &state, const ColorTheme& colors) {
  _renderer = std::make_shared<OccurrenceRenderer>(_params.renderer, colors, _occurrences);
  _observerOccurrenceConnectorRenderer = std::make_shared<ObserverOccurrenceConnectorRenderer>(_params.connectorRenderer, colors.getColor(ColorId::OCCURRENCE_CONNECTOR), _occurrences);
}

void OccurrencesController::update(State &state) {
  _occurrences.update(state);
  _occurrences.cullDeadObjects([&](shared_ptr<OccurrenceEntity> occurrence) {
    OccurrenceEventArgs e(state, *occurrence);
    _events.occurrenceDied.notifyListeners(e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnOccurrence(state);
  }
  state.occurrenceCount = _occurrences.size();
  _renderer->update(state);
}

void OccurrencesController::draw(const State &state) {
  _renderer->draw(state);
  _observerOccurrenceConnectorRenderer->draw(state);
}

void OccurrencesController::spawnOccurrence(const State &state) {
  ofVec3f pos = _bounds.randomPoint();
  float radius = _params.radius.getValue();
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       state);
  
  bool connected = _observers.registerOccurrence(occurrence);
  
  OccurrenceEventArgs e(state, *occurrence);
  if (connected) {
    occurrence->setVelocity(_params.initialVelocity.getValue());
    ofLogNotice() << "Spawned occurrence: " << *occurrence;
    _occurrences.add(occurrence);
    _events.occurrenceSpawned.notifyListeners(e);
  } else {
    ofLogNotice() << "Nothing in range of occurrence: " << *occurrence;
    _events.occurrenceSpawnFailed.notifyListeners(e);
  }
}

void OccurrencesController::spawnOccurrences(int count,
                                             const State &state) {
  for (int i = 0; i < count; ++i) {
    spawnOccurrence(state);
  }
}
