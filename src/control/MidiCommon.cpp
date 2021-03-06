//
//  MidiCommon.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <boost/functional/hash.hpp>
#include "../control/MidiCommon.h"

using namespace ofxTCommon;

EnumTypeInfo<MidiMessageType> MidiMessageTypeInfo {
  {"cc", MidiMessageType::CONTROL_CHANGE},
  {"noteOn", MidiMessageType::NOTE_ON},
  {"noteOff", MidiMessageType::NOTE_OFF},
  {"other", MidiMessageType::OTHER},
};

template<>
const EnumTypeInfo<MidiMessageType>& ofxTCommon::getEnumInfo() {
  return MidiMessageTypeInfo;
}

std::ostream& operator<<(std::ostream& os,
                         const MidiMessageType& messageType) {
  return os << MidiMessageTypeInfo.toString(messageType);
}

MidiMessageType statusToMessageType(const MidiStatus& status) {
  switch (status) {
    case MidiStatus::MIDI_CONTROL_CHANGE:
      return MidiMessageType::CONTROL_CHANGE;
    case MidiStatus::MIDI_NOTE_ON:
      return MidiMessageType::NOTE_ON;
    case MidiStatus::MIDI_NOTE_OFF:
      return MidiMessageType::NOTE_OFF;
    default:
      return MidiMessageType::OTHER;
  }
}

MidiMappingKey MidiMappingKey::create(const MidiDeviceId& device,
                                      const ofxMidiMessage& message) {
  return MidiMappingKey(device,
                        statusToMessageType(message.status),
                        message.channel,
                        message.control);
}

void MidiMappingKey::outputFields(std::ostream& os) const {
  os << "dev: " << _device
     << ", type: " << _type
     << ", chan: " << _channel
     << ", cc: " << _cc;
}

bool operator==(const MidiMappingKey& lha, const MidiMappingKey& rha) {
  return lha._device == rha._device
      && lha._type == rha._type
      && lha._channel == rha._channel
      && lha._cc == rha._cc;
}

std::size_t MidiMappingKey::hash() const {
  std::size_t seed = 0;
  boost::hash_combine(seed, _device);
  boost::hash_combine(seed, _type);
  boost::hash_combine(seed, _channel);
  boost::hash_combine(seed, _cc);
  return seed;
}

ofJson MidiMappingKey::toJson() const {
  return {
    {"device", JsonUtil::toJson(_device)},
    {"channel", JsonUtil::toJson(_channel)},
    {"type", JsonUtil::enumToJson(_type)},
    {"cc", JsonUtil::toJson(_cc)},
  };
}

void MidiMappingKey::readJson(const ofJson &obj) {
  JsonUtil::assertIsObject(obj);
  _device = JsonUtil::fromJson<MidiDeviceId>(obj["device"]);
  _channel = JsonUtil::fromJson<MidiChannel>(obj["channel"]);
  _type = JsonUtil::enumFromJson<MidiMessageType>(obj["type"]);
  _cc = JsonUtil::fromJson<int>(obj["cc"]);
}

void MidiReceivedEventArgs::outputFields(std::ostream &os) const {
  os << "dev:" << device
  << ", msg: " << const_cast<ofxMidiMessage&>(message).toString();
}
