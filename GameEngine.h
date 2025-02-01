#pragma once
#include "Assets.h"

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> Scenes;

class GameEngine
{
private:
	friend class SceneMenu;
	friend class ScenePlay;

	Scenes					m_scenes;
	sf::RenderWindow		m_window;
	Assets					m_assets;
	std::string				m_currentScene;
	size_t					m_simulationSpeed = 1;
	bool					m_running = true;
	float					m_widthW = 1500.0f;
	float					m_heightW = 1200.0f;
	float					m_worldWidth = 3000.0f;

	void init(const std::string&);
	const std::shared_ptr<Scene>& currentScene();
	void update();

	Assets& getAssets();
	void changeScene(const std::string&, std::shared_ptr<Scene>);

	void sUserInput();

public:
	GameEngine();
	void run(std::shared_ptr<GameEngine>);
	void quit();
};