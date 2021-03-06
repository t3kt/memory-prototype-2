//
//  SimulationApp.cpp
//

#include <ofSystemUtils.h>
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/CommandsController.h"
#include "../core/EventLogging.h"
#include "../scenes/ScenesController.h"

void SimulationApp::setup() {
  _commands =
  _components.add<CommandsController>();

  _paramsController =
  _components.add<ParametersController>(_appParams,
                                        _context);

  _eventLoggers = std::make_shared<EventLoggers>();

  _appParams.debug.logging.enabled.changed += [this]() {
    updateLogState();
  };

  updateLogState();

  _actions =
  _components.add<ActionsController>(_context);

  _midi = _components.add<MidiController>(_appParams);

  _osc = _components.add<OscController>(_appParams,
                                        *_commands);
  _timing =
  _components.add<TimingController>(_appParams.core.timing,
                                    _context);

  _physics =
  _components.add<PhysicsController>(_appParams.physics,
                                     _appParams.debug,
                                     _context);

  _nodes =
  _components.add<NodesController>(_context,
                                   _events);

  _observers =
  _components.add<ObserversController>(_appParams.observers,
                                       _physics->bounds(),
                                       _context,
                                       _events);

  _occurrences =
  _components.add<OccurrencesController>(_appParams.occurrences,
                                         _physics->bounds(),
                                         *_observers,
                                         _context,
                                         _events);

  _animations =
  _components.add<AnimationsController>(_appParams.animations,
                                        _events,
                                        _context);

  _navigators =
  _components.add<NavigatorsController>(_context,
                                        _appParams.navigators,
                                        _events);

  _renderingController =
  _components.add<RenderingController>(_appParams.rendering,
                                        *_window,
                                        _context);

  _inspectionController =
  _components.add<InspectionController>(_appParams.debug.inspect,
                                        _context,
                                        *_window);

  _statusController =
  _components.add<StatusInfoController>(_appParams.debug,
                                        _context);

  _scenes =
  _components.add<ScenesController>(_context,
                                    *_actions);

  _gui = _components.add<AppGui>(_appParams);

  _commands->registerCommand("toggleLogging", "Toggle Logging",
                             [&](const CommandArgs&) {
    _appParams.debug.logging.enabled.toggle();
    return true;
  })
  .withButton(true)
  .withKeyMapping('l');
  _commands->registerCommand("dumpEntityState", "Dump Entity State", [&](const CommandArgs&) {
    dumpEntityState();
    return true;
  })
  .withButton(true);
  _commands->registerCommand("loadEntityState", "Load Entity State", [&](const CommandArgs&) {
    loadEntityState();
    return true;
  })
  .withButton(true);
  _commands->registerCommand("saveEntityState", "Save Entity State", [&](const CommandArgs&) {
    saveEntityState();
    return true;
  })
  .withButton(true);
}

void SimulationApp::updateLogState() {
  bool enabled = _appParams.debug.logging.enabled.get();
  ofSetLogLevel(enabled ? OF_LOG_NOTICE : OF_LOG_ERROR);
  auto& events = AppSystem::get().simulation().getEvents();
  if (enabled) {
    _eventLoggers->attach(events);
  } else {
    _eventLoggers->detach(events);
  }
}

void SimulationApp::update() {
  _components.update();
}

void SimulationApp::draw() {
  _renderingController->beginDraw();

  _renderingController->draw();
  _animations->draw();
  _navigators->draw();
  _physics->draw();

  _renderingController->endDraw();

  _statusController->draw();
  _inspectionController->draw();
  _gui->draw();
}

void SimulationApp::dumpEntityState() {
  _context.writeJsonTo(std::cout);
}

void SimulationApp::loadEntityState() {
  FileAction action = [&](ofFileDialogResult& file) {
    _context.readFromFile(file.getPath());
    return true;
  };
  AppSystem::get().performFileLoadAction(action,
                                         "Load Entity State",
                                         "entityState.json");
}

void SimulationApp::saveEntityState() {
  FileAction action = [&](ofFileDialogResult& file) {
    _context.writeJsonTo(file.getPath());
    return true;
  };
  AppSystem::get().performFileSaveAction(action,
                                         "Save Entity State",
                                         "entityState.json");
}

void SimulationApp::keyPressed(ofKeyEventArgs& event) {
  if (_commands->keyMap().handleKeyPressed(event)) {
    return;
  }
  AppSystem::get().handleKeyPressed(event);
}
