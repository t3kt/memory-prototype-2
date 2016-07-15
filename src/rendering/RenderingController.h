//
//  RenderingController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef RenderingController_h
#define RenderingController_h

#include <ofEasyCam.h>
#include "Colors.h"
#include "Common.h"
#include "Params.h"
#include "PostProcController.h"
#include "State.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class CameraParams : public Params {
public:
  CameraParams() {
    add(_spinEnabled
        .setKey("spinEnabled")
        .setName("Spin Enabled")
        .setValueAndDefault(false));
    add(_spinRate
        .setKey("spinRate")
        .setName("Spin Rate")
        .setValueAndDefault(ofVec3f(2, 4, 5))
        .setRange(ofVec3f(-10), ofVec3f(10)));
  }

  bool spinEnabled() const { return _spinEnabled.get(); }
  const ofVec3f& spinRate() const { return _spinRate.get(); }

private:
  TParam<bool> _spinEnabled;
  TParam<ofVec3f> _spinRate;
};

class FogParams : public ParamsWithEnabled {
public:
  FogParams() {
    add(_density
        .setKey("density")
        .setName("Density")
        .setValueAndDefault(0.001f)
        .setRange(0, 0.004f));
    add(_useBackgroundColor
        .setKey("useBackgroundColor")
        .setName("Use Background Color")
        .setValueAndDefault(true));
    add(distance
        .setKey("distance")
        .setName("Distance")
        .setParamValuesAndDefaults(0.3, 0.9)
        .setParamRanges(0, 8));
    setEnabledValueAndDefault(true);
  }

  float density() const { return _density.get(); }
  bool useBackgroundColor() const { return _useBackgroundColor.get(); }

  ValueRange<float> distance;

private:
  TParam<float> _density;
  TParam<bool> _useBackgroundColor;
};

class RenderingController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(camera
          .setKey("camera")
          .setName("Camera"));
      add(fog
          .setKey("fog")
          .setName("Fog"));
      add(postProc
          .setKey("postProc")
          .setName("Post Processing"));
    }

    CameraParams camera;
    FogParams fog;
    PostProcController::Params postProc;
  };

  RenderingController(const Params& params,
                      const ColorTheme& colors);

  void setup();
  void update(const State& state);
  void beginDraw(const State& state);
  void endDraw(const State& state);
  void resetCamera();
#ifdef ENABLE_SYPHON
  void pushToSyphon(ofxSyphonServer& syphonServer);
#endif

private:
  void beginFog();
  void endFog();

  const Params& _params;
  const ColorTheme& _colors;
  const ofFloatColor& _backgroundColor;
  const ofFloatColor& _fogColor;
  ofEasyCam _cam;
  std::shared_ptr<PostProcController> _postProc;
  //  ofLight _light;
  ofVec3f _rotation;
};

#endif /* RenderingController_h */
