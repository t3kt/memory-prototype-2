//
//  AppGui.cpp
//

#include <ofxGuiExtended.h>
#include "../app/AppGui.h"

void AppGui::setup() {
  _gui = std::make_shared<ofxGui>();
  _gui->setupFlexBoxLayout();
  _mainPanel = _gui->addPanel();
  _mainPanel->setBackgroundColor(ofFloatColor(1, 1, 1, 0.5));
  auto rootGroup = _mainPanel->addGroup();

  auto rootTabs = rootGroup->addTabs();
  rootTabs->setWidth(150);
  rootTabs->setTabHeight(6);
  rootTabs->setTabWidth(44);
  auto coreTab = rootTabs->addGroup(_appParams.core);
  {
    auto entityTabs = rootTabs->addTabs("Ent");
    entityTabs->setTabHeight(6);
    entityTabs->setTabWidth(44);
    entityTabs->addGroup(_appParams.observers)->setName("Obs");
    entityTabs->addGroup(_appParams.occurrences)->setName("Occ");
    entityTabs->addGroup(_appParams.navigators)->setName("Nav");
  }
  {
    auto visTabs = rootTabs->addTabs("Vis");
    visTabs->setTabHeight(6);
    visTabs->setTabWidth(44);
    visTabs->addGroup(_appParams.animations)->setName("Ani");
    visTabs->addGroup(_appParams.colors)->setName("Col");
    visTabs->addGroup(_appParams.rendering)->setName("Ren");
  }
  rootTabs->addGroup(_appParams.physics)->setName("Phys");
  rootTabs->addGroup(_appParams.debug)->setName("Dbg");

  loadTheme();
  _mainPanel->setWidth(150);
}

void AppGui::loadTheme() {
  _mainPanel->loadTheme("theme.json");
}

bool AppGui::performAction(AppAction action) {
  switch (action) {
    case AppAction::RELOAD_THEME:
      loadTheme();
      return true;
    default:
      return false;
  }
}

void AppGui::draw() {
}
