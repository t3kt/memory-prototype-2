//
//  ValueSequence.h
//  memory
//
//  Created by tekt on 8/4/16.
//
//

#ifndef ValueSequence_h
#define ValueSequence_h

#include <array>
#include <functional>
#include <ofMath.h>
#include <ofUtils.h>
#include <ofxChoreograph.h>
#include "../control/Params.h"

// A sequence of parameter values, which can be linearly interpolated
// based on time or some other index.
template<typename T, std::size_t N>
class ValueSequence
: public Params {
public:
  static const std::size_t size = N;

  template<typename P>
  using ParamArray = std::array<TParam<P>, N>;
  using SeqT = ofxChoreograph::Sequence<T>;
  using SeqRefT = ofxChoreograph::SequenceRef<T>;
  using RampT = ofxChoreograph::RampTo<T>;
  using SelfT = ValueSequence<T, N>;

  ValueSequence() {
    std::function<void()> onChange = [&]() {
      rebuildSequence();
    };
    add(startValue
        .setKey("startValue")
        .setName("Start Value"));
    startValue.changed.addVoidListener(onChange, this);
    for (std::size_t i = 0; i < N; ++i) {
      auto& lengthParam = lengths[i];
      add(lengthParam
          .setKey("length" + ofToString(i))
          .setName("Length " + ofToString(i)));
      lengthParam.changed.addVoidListener(onChange, this);
      auto& valueParam = values[i];
      add(valueParam
          .setKey("value" + ofToString(i))
          .setName("Value " + ofToString(i)));
      valueParam.changed.addVoidListener(onChange, this);
    }
    rebuildSequence();
  }

  ~ValueSequence() override {
    startValue.changed.removeListeners(this);
    for (std::size_t i = 0; i < N; ++i) {
      auto& lengthParam = lengths[i];
      lengthParam.changed.removeListeners(this);
      auto& valueParam = values[i];
      valueParam.changed.removeListeners(this);
    }
  }

  SelfT& setKey(std::string key) {
    Params::setKey(key);
    return *this;
  }

  SelfT& setName(std::string name) {
    Params::setName(name);
    return *this;
  }

  SelfT& setValueRanges(T minVal, T maxVal) {
    startValue.setRange(minVal, maxVal);
    for (std::size_t i = 0; i < N; ++i) {
      values[i].setRange(minVal, maxVal);
    }
    return *this;
  }

  SelfT& setLengthRanges(T minVal, T maxVal) {
    for (std::size_t i = 0; i < N; ++i) {
      lengths[i].setRange(minVal, maxVal);
    }
    return *this;
  }

  TParam<T> startValue;
  ParamArray<T> values;
  ParamArray<float> lengths;

  T evaluate(float position) const {
    position = ofClamp(position, 0, _sequence->getDuration());
    return _sequence->getValue(position);
  }

protected:
  SelfT& selfRef() override {
    return *this;
  }
private:
  void rebuildSequence() {
    using ofxChoreograph::RampTo;

    const auto& prevValue = startValue.get();
    _sequence = std::make_shared<SeqT>(prevValue);
    auto& seq = *_sequence;
    for (std::size_t i = 0; i < N; ++i) {
      const auto& valueParam = values[i];
      const auto& lengthParam = lengths[i];
      seq.template then<RampTo>(valueParam.get(),
                                lengthParam.get());
    }
  }

  SeqRefT _sequence;
};

#endif /* ValueSequence_h */
