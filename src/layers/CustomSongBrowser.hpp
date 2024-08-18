#pragma once
#include <Geode/Geode.hpp>

class CustomSongBrowser : public geode::Popup<>, TextInputDelegate {
protected:
    CCTextInputNode* m_textInput = nullptr;
    cocos2d::extension::CCScale9Sprite* m_inputBGSprite = nullptr;
    std::vector<std::pair<std::string, SongInfoObject*>> m_downloadedSongs = {};
    std::vector<cocos2d::CCNode*> m_cells = {};
    geode::ScrollLayer* m_scrollLayer = nullptr;
    CustomSongLayer* m_songLayer = nullptr;

    bool setup();
    virtual void textChanged(CCTextInputNode* input);
    void updateList(std::vector<SongInfoObject*>& songs, bool goToTop);
public:
    static CustomSongBrowser* create();

    void setSongLayer(CustomSongLayer* layer);
    CustomSongLayer* getSongLayer();
    void updateAllCellPlaybacks();
    void searchSongs();
};