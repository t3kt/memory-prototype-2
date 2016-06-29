//
//  ParamsGui.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef ParamsGui_h
#define ParamsGui_h

#include "Params.h"
#include <ofxDatGui.h>

class AbstractParamsGui
: public ParamsGui {
public:
  AbstractParamsGui(Params& params);

  ofxDatGuiFolder* getGuiFolder() override { return _folder; }

  virtual void setup() override;
protected:
  virtual void addControls() = 0;

  ofxDatGuiToggle* addToggle(ofParameter<bool>& param);
  ofxDatGuiSlider* addSlider(ofParameter<float>& param);
  ofxDatGuiColorPicker* addColorPicker(ofParameter<ofFloatColor>& param);

  virtual void onToggleEvent(ofxDatGuiToggleEvent e);
  virtual void onSliderEvent(ofxDatGuiSliderEvent e);
  virtual void onColorPickerEvent(ofxDatGuiColorPickerEvent e);

private:
  Params& _baseParams;
  ofxDatGuiFolder* _folder;
};

#endif /* ParamsGui_h */