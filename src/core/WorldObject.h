//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include "Common.h"
#include <iostream>
#include <map>
#include <memory>

typedef long ObjectId;

class State;

class WorldObject : public Outputable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId id;
  
  virtual void update(const State& state) {}
  virtual void draw(const State& state) {}
  
  void output(std::ostream& os) const override;

  bool alive() const { return _alive; }

  void kill() { _alive = false; }
  
  virtual bool visible() const { return this->alive(); }
protected:
  virtual std::string typeName() const;
  virtual void outputFields(std::ostream& os) const;
private:
  bool _alive;
};

template<typename E>
class EntityMap {
public:
  using EntityPtr = std::shared_ptr<E>;
  using Storage = std::map<ObjectId, EntityPtr>;
  using Iterator = typename Storage::iterator;
  using ConstIterator = typename Storage::const_iterator;

  EntityMap() {}

  void add(EntityPtr entity) {
    _map[entity->id] = entity;
  }

  bool remove(EntityPtr entity) {
    return remove(entity->id);
  }

  bool remove(ObjectId id) {
    auto iter = _map.find(id);
    if (iter == _map.end()) {
      return false;
    }
    _map.erase(iter);
    return true;
  }

  std::size_t size() const {
    return _map.size();
  }

  bool empty() const {
    return _map.empty();
  }

  Iterator begin() { return _map.begin(); }
  Iterator end() { return _map.end(); }
  ConstIterator begin() const { return _map.begin(); }
  ConstIterator end() const { return _map.end(); }
private:
  Storage _map;
};

#endif /* WorldObject_h */
