//
//  ObjectManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#ifndef ObjectManager_h
#define ObjectManager_h

#include <map>
#include <memory>
#include <vector>
#include <iterator>
#include <list>
#include <functional>
#include "WorldObject.h"
#include "State.h"
#include "Events.h"

template <typename T>
class ObjectManager {
public:
  using StorageList = std::list<std::shared_ptr<T>>;
  using Iterator = typename StorageList::iterator;
  using ConstIterator = typename StorageList::const_iterator;
  
  void update(const State& state) {
    for (auto& entity : _objects) {
      entity->update(state);
    }
  }
  
  std::vector<std::shared_ptr<T>> extractDeadObjects() {
    std::vector<std::shared_ptr<T>> deadObjects;
    cullDeadObjects([&](std::shared_ptr<T> entity) {
      deadObjects.push_back(entity);
    });
    return deadObjects;
  }
  
  void cullDeadObjects(std::function<void(std::shared_ptr<T>)> callback) {
    for (auto i = std::begin(_objects);
         i != std::end(_objects);) {
      std::shared_ptr<T>& object = *i;
      if (object->alive()) {
        i++;
      } else {
        callback(object);
        i = _objects.erase(i);
      }
    }
  }
  
  void draw(const State& state) {
    for (const auto& entity : _objects) {
      if (entity->visible()) {
        entity->draw(state);
      }
    }
  }
  
  void add(std::shared_ptr<T> object) {
    _objects.push_back(object);
  }

  template<typename ...Args>
  void add(std::shared_ptr<T> object, Args&... others) {
    add(object);
    add(others...);
  }
  
  bool eraseById(ObjectId id) {
    for (auto i = std::begin(_objects);
         i != std::end(_objects);
         i++) {
      if ((*i)->id == id) {
        _objects.erase(i);
        return true;
      }
    }
    return false;
  }
  
  void performAction(std::function<void(std::shared_ptr<T>)> action) {
    for (auto& entity : _objects) {
      action(entity);
    }
  }

  template<typename Base>
  void performTypedAction(std::function<void(Base*)> action) {
    for (auto& entity : _objects) {
      action(entity.get());
    }
  }

  template<typename Base>
  void performTypedAction(std::function<void(const Base*)> action) const {
    for (const auto& entity : _objects) {
      action(entity.get());
    }
  }
  
  std::size_t size() const {
    return _objects.size();
  }
  
  bool empty() const {
    return _objects.empty();
  }

  Iterator begin() {
    return _objects.begin();
  }

  Iterator end() {
    return _objects.end();
  }

  ConstIterator begin() const {
    return _objects.cbegin();
  }

  ConstIterator end() const {
    return _objects.cend();
  }

private:
  StorageList _objects;
};

#endif /* ObjectManager_h */
