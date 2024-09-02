#pragma once
#include <Geode/Geode.hpp>
#include "../layers/CustomSongBrowser.hpp"

class SimpleSongCell : public CustomSongWidget {
protected:
    CustomSongBrowser* m_browserLayer = nullptr;
    cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
    bool isHovering = false;

    bool init(SongInfoObject* song);
    void onPlay(cocos2d::CCObject* sender);
    void onDelete(CCObject* sender);

    void update(float delta);
    void onHover();
    void onHoverExit();
public:
    static SimpleSongCell* create(SongInfoObject* song);

    void setBrowserLayer(CustomSongBrowser* browserLayer);
    CustomSongBrowser* getBrowserLayer();
    void onSelect(cocos2d::CCObject* sender);
};