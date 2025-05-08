#include "SceneMenu.h"
#include "GameEngine.h"
#include "ScenePlay.h"

void SceneMenu::init()
{
	std::cout << "Menu Opened" << std::endl;

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::P, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_levelPaths[0] = "bin/Levels/lvl1.txt";
	m_levelPaths[1] = "bin/Levels/lvl2.txt";
	m_levelPaths[2] = "bin/Levels/lvl3.txt";

	std::cout << "Hovering on level 1" << std::endl;
}

SceneMenu::SceneMenu(std::shared_ptr<GameEngine> game)
	: Scene(std::move(game))
	, m_menuStrings(3)
	, m_levelPaths(3)
{
	SceneMenu::init();
}

const ActionMap& SceneMenu::getActionMap() const
{
	return m_actionMap;
}

void SceneMenu::setSound()
{
	switch (m_menuIndex)
	{
	case 0:
		m_sound.setBuffer(m_game->getAssets().getSound("Level_1"));
		break;
	case 1:
		m_sound.setBuffer(m_game->getAssets().getSound("Level_2"));
		break;
	case 2:
		m_sound.setBuffer(m_game->getAssets().getSound("Level_3"));
		break;
	default:
		m_sound.setBuffer(m_game->getAssets().getSound("Level_1"));
	break;
	}
}

void SceneMenu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			//m_sound.setBuffer(m_game->getAssets().getSound("Bullet"));
			//m_sound.play();
			if (m_menuIndex > 0)
			{
				m_menuIndex--;
			}
			else
			{
				m_menuIndex = m_menuStrings.size() - 1;
			}
			setSound();
			m_sound.play();
			std::cout << "Hovering " << action.name() << " to level " << m_menuIndex + 1 << std::endl;
		}
		else if (action.name() == "DOWN")
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
			setSound();
			m_sound.play();
			std::cout << "Hovering " << action.name() << " to level " << m_menuIndex + 1 << std::endl;
		}
		else if (action.name() == "PLAY")
		{
			m_game->changeScene("PLAY", std::make_shared<ScenePlay>(m_game, m_levelPaths[m_menuIndex]));
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}


void SceneMenu::update()
{
	m_entities.update();
}

void SceneMenu::sRender()
{
	m_game->m_window.clear(sf::Color(220, 222, 217));
	m_game->m_window.setView(m_game->m_window.getDefaultView());

	m_menuFont = m_game->getAssets().getFont("Algerian");
	m_menuText.setFont(m_menuFont);
	m_menuText.setString("MENU SCREEN");
	m_menuText.setCharacterSize(30);
	m_menuText.setFillColor(sf::Color(119, 119, 119));
	m_menuText.setPosition(0.0f, 0.0f);
	m_menuText.setStyle(sf::Text::Bold);

	m_game->m_window.draw(m_menuText);

	for (int i = 0; i < m_menuStrings.size(); i++)
	{
		m_menuText.setString("LEVEL " + std::to_string(i + 1));
		m_menuText.setCharacterSize(100);
		if (i == m_menuIndex)
		{
			m_menuText.setFillColor(sf::Color::Black);
		}
		else
		{
			m_menuText.setFillColor(sf::Color(119, 119, 119));
		}
		m_menuText.setPosition(0.0f, (i + 1) * 100);
		m_menuText.setStyle(sf::Text::Bold);

		m_game->m_window.draw(m_menuText);
	}

	m_menuText.setString("W: UP        S: DOWN        P: PLAY        Esc: QUIT");
	m_menuText.setCharacterSize(30);
	m_menuText.setFillColor(sf::Color(119, 119, 119));
	m_menuText.setPosition(0.0f, 1000.0f);
	m_menuText.setStyle(sf::Text::Bold);

	m_game->m_window.draw(m_menuText);

	m_game->m_window.display();
}

void SceneMenu::onEnd()
{
	std::cout << "Quit game" << std::endl;
	m_game->m_window.close();
	m_game->m_running = false;
}