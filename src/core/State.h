//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "../analysis/SimulationStats.h"
#include "../core/Common.h"

class State
: public Outputable
, public NonCopyable {
public:
  State()
  : running(true) { }
  
  float time;
  float timeDelta;
  SimulationStats stats;
  bool running;
  std::string typeName() const override { return "State"; }
protected:
  void outputFields(std::ostream& os) const override;
};

class ChangeFlag : public Outputable {
public:
  ChangeFlag(std::string name)
  : _name(name)
  , _status(false) {}

  const std::string& name() const { return _name; }

  void set() { _status = true; }
  void reset() { _status = false; }

  operator bool() const { return _status; }

  std::string typeName() const override { return "ChangeFlag"; }
protected:
  void outputFields(std::ostream& os) const override;
private:
  const std::string _name;
  bool _status;
};

class ChangeFlags : public Outputable {
public:
  ChangeFlags();

  void clear();

  ChangeFlag boundsChanged;

  std::string typeName() const override { return "ChangeFlags"; }
protected:
  void outputFields(std::ostream& os) const override;
};

#endif /* defined(__behavior__State__) */
