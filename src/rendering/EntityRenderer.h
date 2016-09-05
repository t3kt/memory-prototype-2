//
//  EntityRenderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

#include <ofGraphics.h>
#include <ofxChoreograph.h>
#include "../core/AnimationObject.h"
#include "../core/ObjectManager.h"
#include "../core/Params.h"
#include "../rendering/Colors.h"

class Context;

class EntityPreRendererParams : public Params {
public:
  EntityPreRendererParams() {
    add(fadeIn
        .setKey("fadeIn")
        .setName("Fade In"));
  }

  RampFactory<float>::Params fadeIn;
};

template<typename T, typename P = EntityPreRendererParams>
class EntityPreRenderer {
public:
  using Params = P;

  EntityPreRenderer(const P& params,
                    const ColorTheme& colors,
                    Context& context,
                    ObjectManager<T>& entities)
  : _params(params)
  , _colors(colors)
  , _context(context)
  , _entities(entities)
  , _fadeIn(params.fadeIn) { }

  virtual void update() = 0;
protected:
  const P& _params;
  const ColorTheme& _colors;
  Context& _context;
  ObjectManager<T>& _entities;
  RampFactory<float> _fadeIn;
};

#endif /* EntityRenderer_h */
