//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include <functional>
#include <iostream>
#include <memory>
#include "Common.h"
#include "JsonIO.h"
#include "Serialization.h"

typedef int ObjectId;

const ObjectId NO_OBJECT_ID = -1;

enum class EntityType {
  ANIMATION,
  NAVIGATOR,
  OBSERVER,
  OCCURRENCE,
};

template<typename T>
EntityType getEntityType();

class Info;

class WorldObject
: public Outputable
, public Serializable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId& id() const { return _id; }

  bool alive() const { return _alive; }

  void kill() { _alive = false; }
  
  virtual bool visible() const { return this->alive() && _alpha > 0; }

  float alpha() const { return _alpha; }
  void setAlpha(float alpha) { _alpha = alpha; }

  virtual EntityType entityType() const = 0;

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;
  virtual std::string typeName() const override;

  virtual void fillInfo(Info& info) const;

  using ObjectPtrAction =
  std::function<void(std::shared_ptr<WorldObject>)>;
  virtual void performActionOnConnected(ObjectPtrAction action) {}

  virtual bool hasConnections() const { return false; }
protected:
  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
private:
  float _alpha;
  bool _alive;
  ObjectId _id;
};

#endif /* WorldObject_h */
