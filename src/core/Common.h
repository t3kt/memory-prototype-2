//
//  Common.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Common__
#define __behavior__Common__

#include <ofVec3f.h>
#include <ofColor.h>
#include <ofxChoreograph.h>
#include <iostream>
#include <map>
#include <stdexcept>

#ifdef TARGET_OSX
#define ENABLE_SYPHON
#endif

using Time = choreograph::Time;

ofVec3f createSignedNoiseVec3f(const ofVec3f& position);
ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs);
ofVec3f createRandomVec3f(float max);
ofVec3f wrapVec(ofVec3f vec, float min, float max);

template<typename T>
T getInterpolated(const T& a, const T& b, float amount);

class Outputable {
public:
  virtual ~Outputable() {}
  virtual void output(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Outputable& obj);

template<typename T>
class EnumTypeInfo {
public:
  EnumTypeInfo(std::initializer_list<std::pair<std::string, T>> entries) {
    for (std::pair<std::string, T> entry : entries) {
      _stringToEnum.insert(entry);
      _enumToString.insert(std::make_pair(entry.second, entry.first));
    }
  }

  bool tryParseString(const std::string& str, T* result) {
    auto iter = _stringToEnum.find(str);
    if (iter == _stringToEnum.end()) {
      return false;
    } else {
      *result = iter->second;
      return true;
    }
  }
  bool tryToString(const T& value, std::string* result) {
    auto iter = _enumToString.find(value);
    if (iter == _enumToString.end()) {
      return false;
    } else {
      *result = iter->second;
      return true;
    }
  }
  std::string toString(T value) {
    std::string name;
    if (!tryToString(value, &name)) {
      throw std::invalid_argument("Unknown enum value");
    }
    return name;
  }
  T parseString(const std::string& str) {
    T value;
    if (!tryParseString(str, &value)) {
      throw std::invalid_argument("Unknown enum value: " + str);
    }
    return value;
  }
private:
  std::map<std::string, T> _stringToEnum;
  std::map<T, std::string> _enumToString;
};

#endif /* defined(__behavior__Common__) */
