//
//  OccurrenceSpawner.cpp
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#include <ofMath.h>
#include "../app/AppSystem.h"
#include "../core/Actions.h"
#include "../core/Bounds.h"
#include "../core/Context.h"
#include "../core/Logging.h"
#include "../core/OccurrencesController.h"
#include "../spawning/OccurrenceSpawner.h"

static void logNotice(Logger::Statement statement) {
  AppSystem::get().log().occurrence().logNotice(statement);
}

class OccurrenceSequenceSpawnAction
: public Action {
public:
  OccurrenceSequenceSpawnAction(OccurrenceSpawnerCore& spawner,
                                std::shared_ptr<OccurrenceEntity> prev,
                                int count)
  : _spawner(spawner)
  , _count(count)
  , _previous(prev) { }

  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    if (!_previous->alive()) {
      logNotice([&](ofLog& log) {
        log << "Canceling spawn sequence because entity died: " << *_previous;
      });
      return ActionResult::cancel();
    }
    auto next = _spawner.spawnSequenceStepEntity(_previous);
    if (!next) {
      logNotice([&](ofLog& log) {
        log << "Canceling spawn sequence spawning next entity failed. previous: " << *_previous;
      });
      return ActionResult::cancel();
    }
    _previous = next;
    _count--;
    if (_count <= 0) {
      logNotice([&](ofLog& log) {
        log << "Finished spawn sequence. previous: " << *_previous;
      });
      return ActionResult::cancel();
    }
    float interval = _spawner.params().sequenceInterval.getValue();
    return ActionResult::reschedule(context.time() + interval);
  }
private:
  OccurrenceSpawnerCore& _spawner;
  std::shared_ptr<OccurrenceEntity> _previous;
  int _count;
};

std::shared_ptr<OccurrenceEntity>
OccurrenceSpawnerCore::spawnSequenceStepEntity(std::shared_ptr<OccurrenceEntity> prev) {
  float radius = _params.radius.getValue();
  ofVec3f pos = ofVec3f(_params.sequenceStepDistance.getValue(), 0, 0);
  pos.rotate(ofRandomf() * 360, ofRandomf() * 360, ofRandomf() * 360);
  pos += prev->position();
  pos = _bounds.clampPoint(pos);
  auto entity = spawnEntity(radius, pos, prev);
  if (entity) {
    logNotice([&](ofLog& log) {
      log << "Spawned sequence step: " << *entity;
    });
  } else {
    logNotice([&](ofLog& log) {
      log << "Failed to spawn sequence step from " << *prev;
    });
  }
  return entity;
}

bool OccurrenceSpawnerCore::spawnNewSequence() {
  auto count = _params.sequenceCount.getValue();
  if (count <= 0) {
    return false;
  }
  auto occurrence =
  spawnEntity(_params.radius.getValue(),
              _bounds.randomPoint(),
              nullptr);
  if (!occurrence) {
    return false;
  }
  AppSystem::get().actions()
  .addDelayed(_params.sequenceInterval.getValue(),
              std::make_shared<OccurrenceSequenceSpawnAction>(*this,
                                                              occurrence,
                                                              count));
  return true;
}

int OccurrenceSpawnerCore::spawnEntities() {
  if (_params.sequence.getValue()) {
    return spawnNewSequence() ? 1 : 0;
  }
  float startRadius = _params.radius.getValue();
  ofVec3f startPos = _bounds.randomPoint();
  int chainCount = _params.chainCount.getValue();
  if (!_params.chain.getValue() || chainCount <= 1) {
    auto occurrence = spawnEntity(startRadius,
                                  startPos,
                                  nullptr);
    return occurrence ? 1 : 0;
  }
  float endRadius = _params.radius.getValue();
  ofVec3f endPos = startPos + _params.chainDistance.getValue();
  AppSystem::get().log().occurrence().logNotice([&](ofLog& log) {
    log << "Spawning occurrence chain - count: " << chainCount;
    log << ", radius: " << startRadius << "->" << endRadius;
    log << ", pos: " << startPos << "->" << endPos;
  });
  std::shared_ptr<OccurrenceEntity> prev;
  int i;
  for (i = 0; i < chainCount; ++i) {
    float fraction = ofMap(i,
                           0, chainCount - 1,
                           0, 1);
    auto radius = ofLerp(startRadius,
                         endRadius,
                         fraction);
    auto pos = startPos.getInterpolated(endPos, fraction);
    auto occurrence = spawnEntity(radius,
                                  pos,
                                  prev);
    if (!occurrence) {
      break;
    } else {
      prev = occurrence;
    }
  }
  AppSystem::get().log().occurrence().logNotice([&](ofLog& log) {
    log << "Spawned occurrence chain - actual count: " << (i + 1);
  });
  return i + 1;
}

std::shared_ptr<OccurrenceEntity>
OccurrenceSpawnerCore::spawnEntity(float radius,
                                   const ofVec3f &pos,
                                   std::shared_ptr<OccurrenceEntity> prev) {
  float radiusFraction = ofMap(radius,
                               _params.radius.lowValue.get(),
                               _params.radius.highValue.get(),
                               0, 1);
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       radiusFraction,
                                                       _context.state);
  if (_controller.tryAddEntity(occurrence)) {
    if (prev) {
      prev->addOccurrence(occurrence);
      occurrence->addOccurrence(prev);
    }
    occurrence->setVelocity(_params.initialVelocity.getValue());
    return occurrence;
  }
  return nullptr;
}

void RateOccurrenceSpawner::spawnEntities(int count) {
  for (int i = 0; i < count; ++i) {
    _core.spawnEntities();
  }
}

bool RateOccurrenceSpawner::performAction(AppAction action) {
  switch (action) {
    case AppAction::SPAWN_FEW_OCCURRENCES:
      spawnEntities(5);
      return true;
    case AppAction::SPAWN_MANY_OCCURRENCES:
      spawnEntities(100);
      return true;
    case AppAction::SPAWN_TONS_OF_OCCURRENCES:
      spawnEntities(4000);
      return true;
    default:
      return false;
  }
}
