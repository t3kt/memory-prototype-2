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
#include <ofVec3f.h>
#include "../core/Common.h"
#include "../core/ObjectId.h"
#include "../core/JsonIO.h"
#include "../core/Serialization.h"

enum class EntityType {
  ANIMATION,
  NAVIGATOR,
  NODE,
  OBSERVER,
  OCCURRENCE,
};

class Info;

class WorldObject
: public Outputable
, public Serializable
, public NonCopyable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId& id() const { return _id; }

  virtual bool isParticle() const { return false; }

  bool alive() const { return _alive; }

  void kill();
  
  virtual bool visible() const { return this->alive() && alpha() > 0; }

  float alpha() const { return _color.a; }
  void setAlpha(float alpha) { _color.a = alpha; }

  const ofFloatColor& color() const { return _color; }
  void setColor(const ofFloatColor& color) { _color = color; }

  const ofVec3f& screenPos() const { return _screenPos; }

  void setScreenPos(const ofVec3f& pos) { _screenPos = pos; }

  virtual EntityType entityType() const = 0;

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;
  virtual std::string typeName() const override;

  virtual void fillInfo(Info& info) const;

  using ObjectPtrRefAction =
  std::function<void(std::shared_ptr<WorldObject>)>;
  virtual void performActionOnConnected(ObjectPtrRefAction action) {}

  virtual const ofVec3f& position() const = 0;

  virtual bool hasConnections() const { return false; }
  virtual void detachConnections() { }
protected:
  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
private:
  ofFloatColor _color;
  bool _alive;
  ObjectId _id;
  ofVec3f _screenPos;
};

#endif /* WorldObject_h */
