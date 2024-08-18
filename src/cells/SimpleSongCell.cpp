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
    m_selectSongBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png"), this, menu_selector(SimpleSongCell::onUse));

    m_buttonMenu->addChild(m_playbackBtn);
    m_buttonMenu->addChild(m_selectSongBtn);

    m_buttonMenu->setPosition({ this->getContentWidth() / 2.7f, 1 });
    m_buttonMenu->updateLayout();

    m_deleteMenu = CCMenu::create();
    m_deleteMenu->setPosition(this->getContentWidth() + 5.f, 1);
    addChild(m_deleteMenu);

    auto deleteSpr = CCSprite::createWithSpriteFrameName("GJ_deleteSongBtn_001.png");
    deleteSpr->setScale(0.4f);
    m_deleteBtn = CCMenuItemSpriteExtra::create(deleteSpr, this, menu_selector(CustomSongWidget::onDelete));
    m_deleteMenu->addChild(m_deleteBtn);

    return true;
}

void SimpleSongCell::onPlay(CCObject* sender) {
    //onPlayback(sender);
    m_browserLayer->updateAllCellPlaybacks();
}

void SimpleSongCell::onUse(CCObject* sender) {
    auto widget = getChildOfType<CustomSongWidget>(m_browserLayer->getSongLayer()->m_mainLayer, 0);
    widget->m_songInfoObject = this->m_songInfoObject;
    widget->m_songInfoObject->retain();
    widget->getSongInfoIfUnloaded();
    widget->onSelect(nullptr);
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