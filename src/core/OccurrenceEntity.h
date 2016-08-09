//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <iostream>
#include <ofTypes.h>
#include "Common.h"
#include "Context.h"
#include "ParticleObject.h"
#include "State.h"
#include "ValueSupplier.h"
#include "WorldObject.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  static std::shared_ptr<OccurrenceEntity> createEmpty() {
    return std::shared_ptr<OccurrenceEntity>(new OccurrenceEntity());
  }

  OccurrenceEntity(ofVec3f pos,
                   float radius,
                   float radiusFraction,
                   const State& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }
  
  void removeObserver(ObjectId id) {
    _connectedObservers.erase(id);
  }

  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
    _connectedOccurrences.add(occurrence);
  }

  void removeOccurrence(ObjectId id) {
    _connectedOccurrences.erase(id);
  }
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }

  void detachConnections();
  
  float getAmountOfObservation() const { return _amountOfObservation; }

  float getAge(const State& state) const { return state.time - _startTime; }
  
  float originalRadius() const { return _originalRadius; }

  float originalRadiusFraction() const { return _originalRadiusFraction; }

  float actualRadius() const { return _actualRadius; }

  const EntityMap<ObserverEntity>& connectedObservers() const {
    return _connectedObservers;
  }

  EntityMap<ObserverEntity>& connectedObservers() {
    return _connectedObservers;
  }

  const EntityMap<OccurrenceEntity>& getConnectedOccurrences() const {
    return _connectedOccurrences;
  }

  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }

  EntityType entityType() const override { return EntityType::OCCURRENCE; }

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;

  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  std::string typeName() const override { return "OccurrenceEntity"; }
protected:
  void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
  virtual void addSerializedRefs(Json::object& obj,
                                 const SerializationContext& context) const override;
  
private:
  OccurrenceEntity() { }

  void setAmountOfObservation(float amount) {
    _amountOfObservation = amount;
  }

  void setActualRadius(float radius) {
    _actualRadius = radius;
  }

  float _originalRadius;
  float _originalRadiusFraction;
  float _actualRadius;
  float _startTime;
  float _amountOfObservation;
  EntityMap<ObserverEntity> _connectedObservers;
  EntityMap<OccurrenceEntity> _connectedOccurrences;

  friend class OccurrencesController;
};

template<>
EntityType getEntityType<OccurrenceEntity>() { return EntityType::OCCURRENCE; }

void readOccurrenceRefs(EntityMap<OccurrenceEntity>& entities,
                        const Json& arr,
                        Context& context);

#endif /* OccurrenceEntity_h */
