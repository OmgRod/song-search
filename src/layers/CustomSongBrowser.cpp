#include "CustomSongBrowser.hpp"
#include "../util/string.hpp"
#include "../cells/SimpleSongCell.hpp"

using namespace geode::prelude;

bool CustomSongBrowser::setup() {

    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    m_bgSprite->setVisible(false);

    m_textInput = CCTextInputNode::create(200.f, 50.f, "Song Name", "bigFont.fnt");
    m_textInput->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQSRTUVWXYZ0123456789-_+=$!#&*\"\':@()[]{}.,;/?\\~^ <>%");
    m_textInput->setPosition(winSize.width / 2, director->getScreenTop() - 35.f);
    m_textInput->setDelegate(this);
    m_textInput->setLabelPlaceholderColor({200, 200, 200});
    m_mainLayer->addChild(m_textInput, 2);

    m_inputBGSprite = CCScale9Sprite::create("GJ_square05.png");
    m_inputBGSprite->setContentSize({ 400.f, 60.f });
    m_inputBGSprite->setPosition(m_textInput->getPosition());
    m_mainLayer->addChild(m_inputBGSprite, 1);

    CCArrayExt<SongInfoObject*> songs = MusicDownloadManager::sharedState()->getDownloadedSongs();

    for(SongInfoObject* song : songs) {
        auto searchValue = fmt::format("{} {}", song->m_songName, song->m_artistName);
        m_downloadedSongs.push_back({ searchValue, song });
    }

    m_closeBtn->setPosition(0, 0);
    m_buttonMenu->setPosition(director->getScreenLeft() + 25, director->getScreenTop() - 25);

    m_scrollLayer = ScrollLayer::create({winSize.width, winSize.height - 43.f});
    m_scrollLayer->m_scrollLimitBottom = 25;
    m_scrollLayer->m_contentLayer->setPositionX(winSize.width / 2);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true)
            ->setGap(3.f)
            ->setAutoGrowAxis(winSize.height)
    );
    m_mainLayer->addChild(m_scrollLayer, 0);

    searchSongs();

    return true;
}

void CustomSongBrowser::reloadDownloaded() {
    m_downloadedSongs.clear();

    CCArrayExt<SongInfoObject*> songs = MusicDownloadManager::sharedState()->getDownloadedSongs();

    for(SongInfoObject* song : songs) {
        auto searchValue = fmt::format("{} {}", song->m_songName, song->m_artistName);
        m_downloadedSongs.push_back({ searchValue, song });
    }

    searchSongs();
}

void CustomSongBrowser::textChanged(CCTextInputNode* input) {
    searchSongs();
}

void CustomSongBrowser::searchSongs() {

    std::vector<SongInfoObject*> songs = {};

    if(m_downloadedSongs.empty()) {
        if(m_noSongs) return;
        auto winSize = CCDirector::get()->getWinSize();
        m_noSongs = CCLabelBMFont::create("You have no songs installed :(", "bigFont.fnt");
        m_noSongs->setPosition(winSize / 2);
        m_noSongs->setScale(0.5f);
        m_mainLayer->addChild(m_noSongs);
    } else {
        auto search = util::string::toLowercase(m_textInput->getString());

        for(std::pair<std::string, SongInfoObject*> item : m_downloadedSongs) {

            if(!search.empty()) {
                auto lowerName = util::string::toLowercase(item.first);
                if(lowerName.find(search) != std::string::npos) {
                    songs.push_back(item.second);
                } else {
                    log::debug("Failed to find {} in {}", search, lowerName);
                }
            } else {
                songs.push_back(item.second);
            }

        }
    }

    updateList(songs, true);
}

void CustomSongBrowser::updateList(std::vector<SongInfoObject*>& songs, bool goToTop) {
    auto winSize = CCDirector::get()->getWinSize();

    m_scrollLayer->m_contentLayer->removeAllChildren();
    m_cells.clear();
    if(songs.empty()) {
        return;
    }

    for(SongInfoObject* item : songs) {
        auto cell = SimpleSongCell::create(item);
        cell->setBrowserLayer(this);
        cell->updatePlaybackBtn();
        m_cells.push_back(cell);
        m_scrollLayer->m_contentLayer->addChild(cell);
    }

    m_scrollLayer->m_contentLayer->updateLayout();
    if(goToTop) m_scrollLayer->scrollToTop();
}

void CustomSongBrowser::updateAllCellPlaybacks() {
    if(m_cells.empty())
        return;
    for(CCNode* cell : m_cells) {
        if(cell)
            static_cast<SimpleSongCell*>(cell)->updatePlaybackBtn();
    }
}

void CustomSongBrowser::setSongLayer(CustomSongLayer* songLayer) {
    this->m_songLayer = songLayer;
}

CustomSongLayer* CustomSongBrowser::getSongLayer() {
    return this->m_songLayer;
}

CustomSongBrowser* CustomSongBrowser::create() {
    auto ret = new CustomSongBrowser();
    if(ret->init(0, 0)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}