//
//  PostProcController.h
//  memory
//
//  Created by tekt on 7/13/16.
//
//

#ifndef PostProcController_h
#define PostProcController_h

#include <memory>
#include <ofCamera.h>
#include <ofxPostProcessing.h>
#include "Common.h"
#include "Params.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class EdgePassParams : public ParamsWithEnabled {
public:
  EdgePassParams() {
    add(hue
        .setKey("hue")
        .setName("Hue")
        .setValueAndDefault(0.5)
        .setRange(0, 1));
    add(saturation
        .setKey("saturation")
        .setName("Saturation")
        .setValueAndDefault(0.5)
        .setRange(0, 1));
    setEnabledValueAndDefault(false);
  }

  TParam<float> hue;
  TParam<float> saturation;
};

class DofPassParams : public ParamsWithEnabled {
public:
  DofPassParams() {
    add(focus
        .setKey("focus")
        .setName("Focus")
        .setValueAndDefault(0.985)
        .setRange(0.99, 1.01));
    add(aperture
        .setKey("aperture")
        .setName("Aperture")
        .setValueAndDefault(0.8)
        .setRange(0, 1.2));
    add(maxBlur
        .setKey("maxBlur")
        .setName("Max Blur")
        .setValueAndDefault(0.6)
        .setRange(0, 1));
    setEnabledValueAndDefault(false);
  }

  TParam<float> focus;
  TParam<float> aperture;
  TParam<float> maxBlur;
};

class PostProcParams : public ParamsWithEnabled {
public:
  PostProcParams() {
    add(fxaa
        .setKey("fxaa")
        .setName("FXAA"));
    add(edge
        .setKey("edge")
        .setName("Edge"));
    add(dof
        .setKey("dof")
        .setName("Depth of Field"));
    add(rimHighlight
        .setKey("rimHighlight")
        .setName("Rim Highlight"));
    add(bloom
        .setKey("bloom")
        .setName("Bloom"));
    setEnabledValueAndDefault(true);
    fxaa.setEnabledValueAndDefault(true);
    rimHighlight.setEnabledValueAndDefault(false);
    bloom.setEnabledValueAndDefault(true);
  }

  ParamsWithEnabled fxaa;
  EdgePassParams edge;
  DofPassParams dof;
  ParamsWithEnabled rimHighlight;
  ParamsWithEnabled bloom;
};

class PostProcController {
public:
  using Params = PostProcParams;

  PostProcController(const Params& params)
  : _params(params) {}

  void setup();
  void updateResolution(ofVec2f size);

  void update();

  void beginDraw(ofCamera& cam);
  void endDraw(ofCamera& cam);
  
#ifdef ENABLE_SYPHON
  void pushToSyphon(ofxSyphonServer& syphonServer);
#endif

private:
  const Params& _params;
  ofxPostProcessing _postProc;
  std::shared_ptr<FxaaPass> _fxaaPass;
  std::shared_ptr<EdgePass> _edgePass;
  std::shared_ptr<DofPass> _dofPass;
  std::shared_ptr<RimHighlightingPass> _rimHighlightPass;
  std::shared_ptr<BloomPass> _bloomPass;
};

#endif /* PostProcController_h */
