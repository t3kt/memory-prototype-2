//
//  Params.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef Params_h
#define Params_h

#include <ofParameterGroup.h>
#include <ofUtils.h>
#include <string>
#include <typeinfo>
#include <vector>
#include "../core/Common.h"
#include "../core/Events.h"
#include "../core/JsonIO.h"

class TParamBase
: public JsonReadable
, public JsonWritable {
public:
  static const char pathSeparator = '.';

  virtual std::string getKey() const = 0;

  virtual Json::object::value_type toJsonField() const {
    return { getKey(), to_json() };
  }
  virtual void readJsonField(const Json& obj) = 0;
  virtual void resetToDefault() = 0;
  virtual bool hasDefault() const = 0;
  virtual bool isGroup() const = 0;

  virtual std::string asString() const = 0;
  virtual const std::type_info& getTypeInfo() const = 0;
  virtual bool supportsOsc() const = 0;
};

template<typename T, typename ParT>
class TTypedParamBase
: public ofParameter<T>
, public TParamBase {
public:
  TTypedParamBase()
  : _supportsOsc(true) {
    ofParameter<T>::addListener(this,
                                &TTypedParamBase::onChanged);
  }

  virtual ~TTypedParamBase() override {
    ofParameter<T>::removeListener(this,
                                   &TTypedParamBase::onChanged);
  }

  ParT& setKey(std::string key) {
    _key = key;
    return selfRef();
  }

  ParT& setName(std::string name) {
    ofParameter<T>::setName(name);
    return selfRef();
  }

  ParT& setRange(T minValue, T maxValue) {
    ofParameter<T>::setMin(minValue);
    ofParameter<T>::setMax(maxValue);
    return selfRef();
  }

  ParT& setDefaultValue(T defaultValue) {
    _defaultValue = defaultValue;
    _hasDefaultValue = true;
    return selfRef();
  }

  ParT& setValueAndDefault(T value) {
    ofParameter<T>::set(value);
    return setDefaultValue(value);
  }

  const T& operator()() const {
    return ofParameter<T>::get();
  }

  bool hasDefault() const override {
    return _hasDefaultValue;
  }

  const T& getDefaultValue() const {
    return _defaultValue;
  }

  void clearDefaultValue() {
    _defaultValue = {};
    _hasDefaultValue = false;
  }

  void resetToDefault() override {
    if (hasDefault()) {
      ofParameter<T>::set(getDefaultValue());
    }
  }

  std::string getKey() const override {
    if (_key.empty()) {
      return ofToLower(ofParameter<T>::getEscapedName());
    } else {
      return _key;
    }
  }

  bool isGroup() const override { return false; }

  const std::type_info& getTypeInfo() const override {
    return typeid(T);
  }

  std::string asString() const override { return ofParameter<T>::toString(); }

  TEvent<T&> changed;

  Json to_json() const override {
    return JsonUtil::toJson(ofParameter<T>::get());
  }

  void read_json(const Json& val) override {
    this->set(JsonUtil::fromJson<T>(val));
  }

  void readJsonField(const Json& obj) override {
    Json val = obj[getKey()];
    if (!val.is_null()) {
      read_json(val);
    } else if (hasDefault()) {
      ofParameter<T>::set(getDefaultValue());
    } else {
      throw JsonException("Required field missing: " + getKey());
    }
  }

  bool supportsOsc() const override { return _supportsOsc; }

  ParT& setSupportsOsc(bool supportsOsc) {
    _supportsOsc = supportsOsc;
    return selfRef();
  }
  
protected:
  virtual ParT& selfRef() = 0;

private:
  void onChanged(T& value) {
    changed.notifyListeners(value);
  }

  std::string _key;
  bool _hasDefaultValue;
  T _defaultValue;
  bool _supportsOsc;
};

template<typename T>
class TParam
: public TTypedParamBase<T, TParam<T>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<T>::set(getInterpolated(ofParameter<T>::getMin(),
                                        ofParameter<T>::getMax(),
                                        normVal));
  }

  float getNormalizedValue() const {
    return ofMap(ofParameter<T>::get(),
                 ofParameter<T>::getMin(),
                 ofParameter<T>::getMax(),
                 0.0f,
                 1.0f);
  }

protected:
  TParam<T>& selfRef() override { return *this; }
};

template<>
class TParam<bool>
: public TTypedParamBase<bool, TParam<bool>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<bool>::set(normVal > 0);
  }

  float getNormalizedValue() const {
    return ofParameter<bool>::get() ? 1.0f : 0.0f;
  }

  void toggle() {
    ofParameter<bool>::set(!ofParameter<bool>::get());
  }

protected:
  TParam<bool>& selfRef() override { return *this; }
};

using ParamBaseAction = std::function<void(TParamBase&)>;

class Params
: public ofParameterGroup
, public TParamBase
, public NonCopyable {
public:
  Params() {}
  Params(std::string key, std::string label) {
    setKey(key);
    setName(label);
  }

  std::string getKey() const override {
    if (_key.empty()) {
      return ofToLower(ofParameterGroup::getEscapedName());
    } else {
      return _key;
    }
  }

  Params& setKey(std::string key) {
    _key = key;
    return *this;
  }

  Params& setName(std::string name) {
    ofParameterGroup::setName(name);
    return *this;
  }

  void add(Params& params) {
    ofParameterGroup::add(params);
    _paramBases.push_back(&params);
  }

  template<typename T>
  void add(TParam<T>& param) {
    ofParameterGroup::add(param);
    _paramBases.push_back(&param);
  }

  Json to_json() const override;

  void read_json(const Json& val) override;

  void readJsonField(const Json& obj) override;

  virtual void resetToDefault() override {
    for (auto param : _paramBases) {
      param->resetToDefault();
    }
  }

  virtual bool hasDefault() const override { return true; }

  bool isGroup() const override { return true; }
  const std::type_info& getTypeInfo() const override {
    return typeid(Params);
  }

  std::string asString() const override { return ofParameterGroup::toString(); }

  TParamBase* findKey(const std::string& key);

  TParamBase* lookupPath(const std::string& path);

  std::vector<TParamBase*>& getParamBases() { return _paramBases; }

  const std::vector<TParamBase*>& getParamBases() const {
    return _paramBases;
  }

  void performRecursiveParamAction(ParamBaseAction action);

  bool supportsOsc() const override { return _supportsOsc; }

  Params& setSupportsOsc(bool supportsOsc) {
    _supportsOsc = supportsOsc;
    return *this;
  }
private:
  std::string _key;
  std::vector<TParamBase*> _paramBases;
  bool _supportsOsc = true;
};

class ParamsWithEnabled : public Params {
public:
  ParamsWithEnabled() {
    add(enabled
        .setKey("enabled")
        .setName("Enabled")
        .setValueAndDefault(true));
  }

  void setEnabledValueAndDefault(bool val) {
    enabled.setValueAndDefault(val);
  }

  TParam<bool> enabled;
};

#endif /* Params_h */