#pragma once
#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "Scene.h"

class SceneMenu :public Scene
{
private:
	std::vector<std::string> m_menuStrings;
	sf::Text m_menuText;
	sf::Font m_menuFont;
	std::vector<std::string> m_levelPaths;
	size_t m_menuIndex = 0;

	void init();

	void onEnd()							override;
	void update()							override;
	void sRender()							override;
	void sDoAction(const Action&)			override;
	const ActionMap& getActionMap() const	override;

	void setSound();
public:
	SceneMenu(std::shared_ptr<GameEngine>);
};

#endif