//
//  MidiMapping.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef MidiMapping_h
#define MidiMapping_h

#include <string>
#include "Common.h"
#include "JsonIO.h"
#include "MappingSet.h"
#include "MidiCommon.h"

class MidiMapping
: public JsonReadable
, public JsonWritable
, public Outputable {
public:
  MidiMapping() {}
  MidiMapping(MidiMappingKey key, std::string path)
  : _key(key)
  , _path(path) {}

  const MidiMappingKey& key() const { return _key; }
  const std::string& path() const { return _path; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

protected:
  std::string typeName() const override { return "MidiMapping"; }
  void outputFields(std::ostream& os) const override;
private:
  MidiMappingKey _key;
  std::string _path;
};

using MidiMappingSet = MappingSet<MidiMapping>;

#endif /* MidiMapping_h */
