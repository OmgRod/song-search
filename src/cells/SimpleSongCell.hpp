#pragma once
#include <Geode/Geode.hpp>
#include "../layers/CustomSongBrowser.hpp"

class SimpleSongCell : public CustomSongWidget {
protected:
    CustomSongBrowser* m_browserLayer = nullptr;
    cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
    cocos2d::CCMenu* m_deleteMenu = nullptr;

    bool init(SongInfoObject* song);
    void onPlay(cocos2d::CCObject* sender);
    void onUse(cocos2d::CCObject* sender);
public:
    static SimpleSongCell* create(SongInfoObject* song);

    void setBrowserLayer(CustomSongBrowser* browserLayer);
    CustomSongBrowser* getBrowserLayer();
};