#include "SimpleSongCell.hpp"

using namespace geode::prelude;

bool SimpleSongCell::init(SongInfoObject* song) {
    m_songInfoObject = song;

    m_bgSprite = CCScale9Sprite::create("GJ_square01.png");
    m_bgSprite->setContentSize({ 350.f, 50 });
    addChild(m_bgSprite, -1);

    this->setContentSize(m_bgSprite->getContentSize());

    m_songLabel = CCLabelBMFont::create(m_songInfoObject->m_songName.c_str(), "bigFont.fnt");
    m_songLabel->setAnchorPoint({ 0, 0.5f });
    m_songLabel->setPosition(-this->getContentWidth() / 2.f + 8.f, this->getContentHeight() / 2.f - 13.5f);
    m_songLabel->limitLabelWidth(250.f, 0.5f, 0.0f);
    addChild(m_songLabel);

    m_artistLabel = CCLabelBMFont::create(m_songInfoObject->m_artistName.c_str(), "goldFont.fnt");
    m_artistLabel->setAnchorPoint({0, 1});
    m_artistLabel->setPosition({ -this->getContentWidth() / 2.f + 8.f, this->getContentHeight() / 2.f - 21.6f });
    m_artistLabel->setScale(0.6f);
    addChild(m_artistLabel);

    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setLayout(RowLayout::create()->setAxisReverse(true));
    addChild(m_buttonMenu);

    m_playbackBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_playMusicBtn_001.png"), this, menu_selector(SimpleSongCell::onPlay));
    m_selectSongBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png"), this, menu_selector(SimpleSongCell::onSelect));

    auto deleteSpr = CCSprite::createWithSpriteFrameName("GJ_deleteSongBtn_001.png");
    deleteSpr->setScale(0.5f);
    m_deleteBtn = CCMenuItemSpriteExtra::create(deleteSpr, this, menu_selector(SimpleSongCell::onDelete));
    m_deleteBtn->setPosition(m_bgSprite->getContentSize().width / 2.f + 12.f, 15.5f);
    m_deleteBtn->setVisible(false);

    auto deleteMenu = CCMenu::create();
    deleteMenu->addChild(m_deleteBtn);
    deleteMenu->setPosition(0, 0);
    addChild(deleteMenu);

    m_buttonMenu->addChild(m_playbackBtn);
    m_buttonMenu->addChild(m_selectSongBtn);

    m_buttonMenu->setPosition({ this->getContentWidth() / 2.7f, 1 });
    m_buttonMenu->updateLayout();

    scheduleUpdate();

    return true;
}

void SimpleSongCell::onPlay(CCObject* sender) {
    onPlayback(sender);
    m_browserLayer->updateAllCellPlaybacks();
}

void SimpleSongCell::onDelete(CCObject* sender) {
    createQuickPopup("Delete Song", "Are you sure you would like to <cr>delete</c> this song?", "No", "Yes", [this](auto, bool btn2) {
        if(btn2) {
            auto path = MusicDownloadManager::sharedState()->pathForSong(m_songInfoObject->m_songID);
            remove(path.c_str());
            auto widget = m_browserLayer->getSongLayer()->m_mainLayer->getChildByType<CustomSongWidget>(0);
            widget->m_songInfoObject = this->m_songInfoObject;
            widget->m_songInfoObject->retain();
            widget->updateSongInfo();
            m_browserLayer->reloadDownloaded();
        }
    });
}

void SimpleSongCell::onSelect(CCObject* sender) {
    auto widget = m_browserLayer->getSongLayer()->m_mainLayer->getChildByType<CustomSongWidget>(0);
    widget->m_songInfoObject = this->m_songInfoObject;
    widget->m_songInfoObject->retain();
    widget->getSongInfoIfUnloaded();
    widget->onSelect(sender);
}

void SimpleSongCell::setBrowserLayer(CustomSongBrowser* browserLayer) {
    m_browserLayer = browserLayer;
}

CustomSongBrowser* SimpleSongCell::getBrowserLayer() {
    return m_browserLayer;
}

SimpleSongCell* SimpleSongCell::create(SongInfoObject *song) {
    auto ret = new SimpleSongCell();
    if(ret->init(song)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void SimpleSongCell::onHover() {
    m_deleteBtn->setVisible(true);
}

void SimpleSongCell::onHoverExit() {
    m_deleteBtn->setVisible(false);
}

void SimpleSongCell::update(float delta) {

    auto mousePos = getMousePos();

    auto pos = getParent()->convertToWorldSpace(getPosition());
    auto size = m_bgSprite->getContentSize();
    auto scale = m_bgSprite->getScale();
    auto anchorPoint = m_bgSprite->getAnchorPoint();

    float anchorYSubtract = size.height * scale * anchorPoint.y;

    CCRect rect = CCRect(0, pos.y - anchorYSubtract, size.width * scale, size.height * scale);

    if(mousePos.y >= rect.getMinY() && mousePos.y <= rect.getMaxY()) {
        if(!isHovering){
            onHover();
            isHovering = true;
        }
    } else {
        if(isHovering){
            onHoverExit();
            isHovering = false;
        }
    }
}
