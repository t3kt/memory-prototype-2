//
//  MidiEvents.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef MidiEvents_h
#define MidiEvents_h

#include <ofxTJsonIO.h>
#include <vector>
#include "../control/MappingSet.h"
#include "../control/MidiCommon.h"
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/SimulationEvents.h"

class MidiRouter;

class MidiEventBinding;

class MidiEventMapping
: public Outputable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  MidiEventMapping()
  : _eventType(SimulationEventType::ANIMATION_SPAWNED)
  , _value(0) { }

  MidiEventMapping(SimulationEventType eventType,
                   MidiMappingKey key,
                   int value,
                   bool autoOff = true)
  : _eventType(eventType)
  , _key(key)
  , _value(value)
  , _autoOff(true) { }

  SimulationEventType eventType() const { return _eventType; }
  const MidiMappingKey& key() const { return _key; }
  int value() const { return _value; }
  bool autoOff() const { return _autoOff; }

  ofJson toJson() const override;
  void readJson(const ofJson& obj) override;

  std::string typeName() const override { return "MidiEventMapping"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  MidiMappingKey _key;
  SimulationEventType _eventType;
  int _value;
  bool _autoOff;
};

using MidiEventMappingSet = MappingSet<MidiEventMapping>;

class MidiEventRouter
: public NonCopyable {
public:
  using BindingList = std::vector<std::shared_ptr<MidiEventBinding>>;

  MidiEventRouter(MidiRouter& router)
  : _router(router) { }

  void setup();

  void attach(SimulationEvents& events);
  void detach(SimulationEvents& events);
private:
  void loadMappings();
  void initBindings();
  void addBinding(const MidiEventMapping& mapping);

  MidiRouter& _router;
  MidiEventMappingSet _mappings;
  BindingList _bindings;
};

#endif /* MidiEvents_h */
