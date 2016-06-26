//
//  AppParameters.h
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#ifndef __behavior__AppParameters__
#define __behavior__AppParameters__

#include <ofParameterGroup.h>
#include <ofxPanel.h>

#include "ObserversController.h"
#include "OccurrencesController.h"

class MemoryAppParameters {
public:
  MemoryAppParameters();
  
  void initGui(ofxPanel& gui);
  
  ofParameterGroup paramGroup;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
};

#endif /* defined(__behavior__AppParameters__) */
