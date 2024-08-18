#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongLayer.hpp>
#include "layers/CustomSongBrowser.hpp"

using namespace geode::prelude;

struct $modify(CustomSongLayer) {
    void onSongBrowser(cocos2d::CCObject* sender) {
        auto browser = CustomSongBrowser::create();
        browser->setSongLayer(this);
        browser->show();
    }
};