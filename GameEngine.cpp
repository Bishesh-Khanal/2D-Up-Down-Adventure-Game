#include "GameEngine.h"
#include "Scene.h"
#include "SceneMenu.h"
#include <fstream>
#include <sstream>

GameEngine::GameEngine()
{
    std::cout << "Created the game engine" << std::endl;
    init("bin/assets.txt");
}

void GameEngine::init(const std::string& path)
{
    std::ifstream myFiles(path);
    if (!myFiles.is_open())
    {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(myFiles, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream lineStream(line);
        std::string assetType, nameAsset, pathAsset;
        size_t frameCount;
        float animationSpeed;

        if (lineStream >> assetType >> nameAsset >> pathAsset >> frameCount >> animationSpeed)
        {
            if (assetType == "Texture")
            {
                getAssets().addTexture(nameAsset, pathAsset);
            }
            else if (assetType == "Font")
            {
                getAssets().addFont(nameAsset, pathAsset);
            }
            else if (assetType == "Sound")
            {
                getAssets().addSound(nameAsset, pathAsset);
            }
            else if (assetType == "Animation")
            {
                getAssets().addAnimations(nameAsset, Animation(nameAsset, getAssets().getTexture(pathAsset), frameCount, animationSpeed));
            }
        }
        else
        {
            std::cerr << "Malformed line" << std::endl;
        }
    }

    getAssets().debugTextures();
    getAssets().debugSounds();
    getAssets().debugFonts();
    getAssets().debugAnimations();

    m_window.create(sf::VideoMode(m_widthW, m_heightW), "2D UP DOWN GAME", sf::Style::Close);
    m_window.setFramerateLimit(60);

    std::cout << "Game engine initialized successfully." << std::endl;
}


void GameEngine::run(std::shared_ptr<GameEngine> game)
{
    std::cout << "Running the game engine" << std::endl;
    changeScene("MENU", std::make_shared<SceneMenu>(game));

    while (m_running)
    {
        update();
        sUserInput();
        currentScene()->sRender();

        currentScene()->m_currentFrame++;
    }
}

const std::shared_ptr<Scene>& GameEngine::currentScene()
{
    return m_scenes[m_currentScene];
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
{
    m_scenes[sceneName] = scene;
    m_currentScene = sceneName;
    //currentScene();
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }

            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }

        auto mousePos = sf::Mouse::getPosition(m_window);
        Vec2 mpos(mousePos.x, mousePos.y);

        if (event.type == sf::Event::MouseButtonPressed)
        {
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Left: { currentScene()->doAction(Action("LEFT_CLICK", "START", mpos)); break; }
            case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "START", mpos)); break; }
            case sf::Mouse::Right: { currentScene()->doAction(Action("RIGHT_CLICK", "START", mpos)); break; }
            default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Left: { currentScene()->doAction(Action("LEFT_CLICK", "END", mpos)); break; }
            case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "END", mpos)); break; }
            case sf::Mouse::Right: { currentScene()->doAction(Action("RIGHT_CLICK", "END", mpos)); break; }
            default: break;
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {
            currentScene()->doAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
        }

        if (event.type == sf::Event::Resized)
        {
            m_widthW = m_window.getSize().x;
            m_heightW = m_window.getSize().y;
            m_window.close();
            m_window.create(sf::VideoMode(m_widthW, m_heightW), "Game");
        }
    }
}

void GameEngine::update()
{
    currentScene()->update();
}

Assets& GameEngine::getAssets()
{
    return m_assets;
}