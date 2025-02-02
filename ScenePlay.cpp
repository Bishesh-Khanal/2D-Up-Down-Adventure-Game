#include "ScenePlay.h"
#include "GameEngine.h"
#include "SceneMenu.h"
#include "Physics.h";

#include <fstream>
#include <sstream>

void ScenePlay::init(const std::string& levelPath)
{
	std::cout << "Game started" << std::endl;
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Escape, "MENU");
	registerAction(sf::Keyboard::Space, "PAUSE");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");
	registerAction(sf::Keyboard::B, "SHOOT");

	m_gridText.setCharacterSize(16);
	m_gridText.setFont(m_game->getAssets().getFont("Algerian"));

	m_playText.setFont(m_game->getAssets().getFont("Arial"));
	m_playText.setFillColor(sf::Color::Black);
	m_playText.setStyle(sf::Text::Bold);

	for (auto& entity : m_entities.getEntities())
	{
		entity->destroy();
	}

	//loadLevel(levelPath);

	m_drawTextures = true;
	m_drawCollision = false;
	m_drawGrid = false;
	m_verticalResolved = false;
	m_horizontalResolved = false;

	m_game->m_window.setView(m_game->m_window.getDefaultView());

	m_viewSet = false;

	spawnPlayer();

	m_playerView.setCenter(sf::Vector2f(m_player->getComponent<CTransform>().pos.x, m_player->getComponent<CTransform>().pos.y));
	m_playerView.setSize(sf::Vector2f(768, m_game->m_heightW));
	m_landScapeView.setCenter(sf::Vector2f(m_player->getComponent<CTransform>().pos.x, m_player->getComponent<CTransform>().pos.y));
	m_landScapeView.setSize(sf::Vector2f(768, m_game->m_heightW));
}

void ScenePlay::setView(sf::View& view, float offset)
{
	auto& physical = m_player->getComponent<CTransform>();
	if (!m_viewSet)
	{
		if (physical.pos.x >= (m_game->m_widthW) / 2)
		{
			m_viewSet = true;
			m_playerView.reset(sf::FloatRect(0, 0, m_game->m_widthW, m_game->m_heightW));
			m_landScapeView.reset(sf::FloatRect(0, 0, m_game->m_widthW, m_game->m_heightW));
		}
	}

	if (m_viewSet)
	{
		if (!m_horizontalResolved)
		{
			float viewLeft = view.getCenter().x - view.getSize().x / 2;
			float viewRight = view.getCenter().x + view.getSize().x / 2;
			if (!m_paused)
			{
				if (physical.velocity.x < 0 && viewLeft >= 0)
				{
					view.move(sf::Vector2f(offset, 0));
				}
				else if (physical.velocity.x > 0 && viewRight <= m_game->m_worldWidth)
				{
					view.move(sf::Vector2f(offset, 0));
				}
				else if (viewLeft < 0)
				{
					m_viewSet = false;
				}
			}
		}
		m_game->m_window.setView(view);
	}
}

Vec2 ScenePlay::windowToWorld(const Vec2& windowPos) const
{
	auto& view = m_game->m_window.getView();

	float wx = view.getCenter().x - (m_game->m_window.getSize().x / 2);
	float wy = view.getCenter().y - (m_game->m_window.getSize().y / 2);

	return Vec2(windowPos.x + wx, windowPos.y + wy);
}

void ScenePlay::spawnPlayer()
{
	m_sound.setBuffer(m_game->getAssets().getSound("Start"));
	m_sound.play();

	m_player = m_entities.addEntity("Player");

	m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("StandSide"), false);

	m_player->addComponent<CState>("StandSide");

	m_player->addComponent<CTransform>(Vec2(gridtoMidPixel(0, 2, m_player)));

	m_player->addComponent<CBoundingBox>(Vec2(m_player->getComponent<CAnimation>().animation.getSize().x, m_player->getComponent<CAnimation>().animation.getSize().y));
}

Vec2 ScenePlay::gridtoMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	if (!entity->hasComponent<CAnimation>()) {
		throw std::runtime_error("Entity missing required CAnimation component.");
	}

	auto& animationSize = entity->getComponent<CAnimation>().animation.getSize();

	float centerX = gridX + (animationSize.x / (2.f * m_gridSize.x));
	float centerY = (gridY + 1) - (animationSize.y / (2.f * m_gridSize.y));

	return Vec2(centerX * m_gridSize.x, centerY * m_gridSize.y);
}

void ScenePlay::loadLevel(const std::string& level_path)
{
	std::cout << level_path << std::endl;
	std::fstream myFiles(level_path);
	if (!myFiles.is_open())
	{
		std::cerr << "Failed to open the file: " << level_path << std::endl;
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
		std::string assetType, nameAsset;
		float xAsset, yAsset;

		if (lineStream >> assetType >> nameAsset >> xAsset >> yAsset)
		{
			auto entity = m_entities.addEntity(assetType);
			entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(nameAsset), false);

			entity->addComponent<CTransform>(gridtoMidPixel(xAsset, yAsset, entity));

			if (assetType == "Tile")
			{
				entity->addComponent<CBoundingBox>(Vec2(entity->getComponent<CAnimation>().animation.getSize().x, entity->getComponent<CAnimation>().animation.getSize().y));
				entity->addComponent<CDraggable>(true);
			}

		}
		else
		{
			std::cerr << "Malformed line" << std::endl;
		}
	}
}

ScenePlay::ScenePlay(std::shared_ptr<GameEngine> game, const std::string& level_path)
	: Scene(std::move(game))
	, m_levelPath(level_path)
{
	ScenePlay::init(m_levelPath);
}

void ScenePlay::sAnimation()
{
	auto& animComponent = m_player->getComponent<CAnimation>().animation;
	const auto& stateComponent = m_player->getComponent<CState>();

	if (stateComponent.state == "RunUp")
	{
		animComponent = m_game->getAssets().getAnimation("RunUp");
	}
	if (stateComponent.state == "RunSide")
	{
		animComponent = m_game->getAssets().getAnimation("RunSide");
	}
	if (stateComponent.state == "RunDown")
	{
		animComponent = m_game->getAssets().getAnimation("RunDown");
	}
	if (stateComponent.state == "StandUp")
	{
		animComponent = m_game->getAssets().getAnimation("StandUp");
	}
	if (stateComponent.state == "StandSide")
	{
		animComponent = m_game->getAssets().getAnimation("StandSide");
	}
	if (stateComponent.state == "StandDown")
	{
		animComponent = m_game->getAssets().getAnimation("StandDown");
	}

	animComponent.m_sprite.setScale(m_player->getComponent<CTransform>().angle, 1.0f);

	for (auto& entity : m_entities.getEntities())
	{
		if (entity->tag() == "Player")
		{
			entity->getComponent<CAnimation>().animation.m_gameFrame = m_currentFrame;
		}
		entity->getComponent<CAnimation>().animation.update();
		if (entity->getComponent<CAnimation>().animation.hasEnded() && entity->getComponent<CAnimation>().destroy)
		{
			entity->destroy();
		}
	}
}

void ScenePlay::sDebug()
{
	m_game->m_window.setView(m_game->m_window.getDefaultView());
	for (auto& entity : m_entities.getEntities("Control"))
	{
		entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
		m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
	}
	m_playText.setString("SCORE: " + std::to_string(m_score));
	m_playText.setCharacterSize(30);
	m_playText.setPosition(1350.0f, 0.0f);
	m_game->m_window.draw(m_playText);

	if (m_drawTextures)
	{

		// VIEW FOR THE LANDSCAPE......................................

		setView(m_landScapeView, m_player->getComponent<CTransform>().velocity.x / 10);
		for (auto& entity : m_entities.getEntities("Land"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}


		// VIEW FOR THE PLAYER AND DECORATING ENTITIES

		setView(m_playerView, m_player->getComponent<CTransform>().velocity.x);
		for (auto& entity : m_entities.getEntities("Dec"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}
		for (auto& entity : m_entities.getEntities("Tile"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}
		for (auto& entity : m_entities.getEntities("Bullet"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}
		for (auto& entity : m_entities.getEntities("Coin"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}
		for (auto& entity : m_entities.getEntities("Player"))
		{
			entity->getComponent<CAnimation>().animation.getSprite().setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CAnimation>().animation.getSprite());
		}
	}

	if (m_drawGrid)
	{
		sf::VertexArray gridLines(sf::Lines);

		for (int x = 0; x <= m_game->m_worldWidth; x += m_gridSize.x)
		{
			gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color::White));
			gridLines.append(sf::Vertex(sf::Vector2f(x, m_game->m_heightW), sf::Color::White));
		}

		for (int y = 0; y <= m_game->m_heightW; y += m_gridSize.y)
		{
			gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color::White));
			gridLines.append(sf::Vertex(sf::Vector2f(m_game->m_worldWidth, y), sf::Color::White));
		}

		m_game->m_window.draw(gridLines);
		m_gridText.setFillColor(sf::Color::White);


		for (int y = 0; y < m_game->m_heightW / m_gridSize.y; ++y)
		{
			for (int x = 0; x < m_game->m_worldWidth / m_gridSize.x; ++x)
			{
				std::ostringstream label;
				label << "(" << x << "," << y << ")";
				m_gridText.setString(label.str());

				m_gridText.setPosition(x * m_gridSize.x + 5, y * m_gridSize.y + 5);

				m_game->m_window.draw(m_gridText);
			}
		}
	}

	if (m_drawCollision)
	{
		for (auto& entity : m_entities.getEntities())
		{
			if (entity->hasComponent<CBoundingBox>())
			{
				entity->getComponent<CBoundingBox>().rectangle.setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
				m_game->m_window.draw(entity->getComponent<CBoundingBox>().rectangle);
			}
		}
	}
}

void ScenePlay::spawnBullet()
{
	if (m_player->getComponent<CInput>().canshoot)
	{
		m_sound.setBuffer(m_game->getAssets().getSound("Bullet"));
		m_sound.play();

		auto entity = m_entities.addEntity("Bullet");

		entity->addComponent<CAnimation>(m_game->getAssets().getAnimation("Bullet"), false);

		Vec2 velocity(5.0f, 0.0f);

		velocity *= m_player->getComponent<CTransform>().angle;

		entity->addComponent<CTransform>(m_player->getComponent<CTransform>().pos, velocity);

		entity->addComponent<CBoundingBox>(Vec2(entity->getComponent<CAnimation>().animation.getSize().x, entity->getComponent<CAnimation>().animation.getSize().y));

		entity->addComponent<CLifespan>(50);
	}
	m_player->getComponent<CInput>().canshoot = false;
}

void ScenePlay::sLifeSpan()
{
	for (auto& entity : m_entities.getEntities())
	{
		if (entity->hasComponent<CLifespan>())
		{
			entity->getComponent<CLifespan>().remaining--;
			if (entity->getComponent<CLifespan>().remaining <= 0)
			{
				entity->destroy();
			}
		}
	}
}

bool IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto ePos = e->getComponent<CTransform>().pos;
	auto size = e->getComponent<CAnimation>().animation.getSize();

	float dx = fabs(pos.x - ePos.x);
	float dy = fabs(pos.y - ePos.y);

	return (dx <= size.x / 2) && (dy <= size.y / 2);
}

void ScenePlay::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "MENU")
		{
			onEnd();
		}
		if (action.name() == "TOGGLE_TEXTURE")
		{
			m_drawTextures = !m_drawTextures;
		}
		if (action.name() == "TOGGLE_COLLISION")
		{
			m_drawCollision = !m_drawCollision;
		}
		if (action.name() == "TOGGLE_GRID")
		{
			m_drawGrid = !m_drawGrid;
		}
		if (action.name() == "PAUSE")
		{
			m_paused = !m_paused;
		}
		if (action.name() == "SHOOT")
		{
			m_player->getComponent<CInput>().canshoot = false;
		}

		if (action.name() == "LEFT_CLICK")
		{
			Vec2 worldPos = windowToWorld(action.pos());
			for (auto& e : m_entities.getEntities())
			{
				if (e->hasComponent<CDraggable>() && IsInside(worldPos, e))
				{
					std::cout << "Clicked Entity: " << e->getComponent<CAnimation>().animation.getName() << std::endl;
					e->getComponent<CDraggable>().dragging = !e->getComponent<CDraggable>().dragging;
				}
			}
		}

		if (action.name() == "MOUSE_MOVE")
		{
			m_mPos = action.pos();
			m_mShape.setPosition(m_mPos.x, m_mPos.y);
		}

		if (action.name() == "RIGHT")
		{
			m_player->getComponent<CState>().state = "RunSide";
			m_player->getComponent<CInput>().right = true;
			m_player->getComponent<CTransform>().angle = 1.0f;
		}

		if (action.name() == "LEFT")
		{
			m_player->getComponent<CState>().state = "RunSide";
			m_player->getComponent<CInput>().left = true;
			m_player->getComponent<CTransform>().angle = -1.0f;
		}

		if (action.name() == "UP")
		{
			m_player->getComponent<CState>().state = "RunUp";
			m_player->getComponent<CInput>().up = true;
		}

		if (action.name() == "DOWN")
		{
			m_player->getComponent<CState>().state = "RunDown";
			m_player->getComponent<CInput>().down = true;
		}
	}

	if (action.type() == "END")
	{
		if (action.name() == "RIGHT")
		{
			m_player->getComponent<CState>().state = "StandSide";
			m_player->getComponent<CInput>().right = false;
		}

		if (action.name() == "LEFT")
		{
			m_player->getComponent<CState>().state = "StandSide";
			m_player->getComponent<CInput>().left = false;
		}

		if (action.name() == "UP")
		{
			m_player->getComponent<CState>().state = "StandUp";
			m_player->getComponent<CInput>().up = false;
		}

		if (action.name() == "DOWN")
		{
			m_player->getComponent<CState>().state = "StandDown";
			m_player->getComponent<CInput>().down = false;
		}

		if (action.name() == "SHOOT")
		{
			m_player->getComponent<CInput>().canshoot = true;
		}
	}
}

void ScenePlay::sDragAndDrop()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->hasComponent<CDraggable>() && e->getComponent<CDraggable>().dragging)
		{
			if (e->getComponent<CAnimation>().animation.getName() == "Block")
			{
				Vec2 worldPos = windowToWorld(m_mPos);
				e->getComponent<CTransform>().pos = worldPos;
			}
		}
	}
}

const ActionMap& ScenePlay::getActionMap() const
{
	return m_actionMap;
}

void ScenePlay::sCollision() {
	const float EPSILON = 1e-5f;

	m_verticalResolved = false;
	m_horizontalResolved = false;
	Vec2 totalAdjustment(0.0f, 0.0f);

	for (auto& bullet : m_entities.getEntities("Bullet"))
	{
		for (auto& tile : m_entities.getEntities("Tile"))
		{
			Vec2 overlap = Physics::GetOverlap(bullet, tile);
			if (overlap != Vec2(0.0f, 0.0f))
			{
				bullet->destroy();
				if (tile->getComponent<CAnimation>().animation.getName() == "Brick")
				{
					m_sound.setBuffer(m_game->getAssets().getSound("Explosion"));
					m_sound.play();
					tile->getComponent<CBoundingBox>().has = false;
					tile->getComponent<CAnimation>().destroy = true;
					tile->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation("Explosion");
				}
				break;
			}
		}
	}

	for (auto& tile : m_entities.getEntities("Tile")) {
		Vec2 overlap = Physics::GetOverlap(m_player, tile);
		if (overlap != Vec2(0.0f, 0.0f)) {
			if (tile->getComponent<CAnimation>().animation.getName() == "Flag")
			{
				for (auto& entity : m_entities.getEntities())
				{
					entity->destroy();
				}

				size_t pos = m_levelPath.find_last_of("0123456789");
				size_t start = pos;
				while (start > 0 && std::isdigit(m_levelPath[start - 1])) {
					--start;
				}

				std::string numberStr = m_levelPath.substr(start, pos - start + 1);

				int number = std::stoi(numberStr);

				number = (number % 3) + 1;

				std::string incrementedLevel = m_levelPath.substr(0, start) + std::to_string(number) + ".txt";
				m_game->changeScene("PLAY" + std::to_string(number), std::make_shared<ScenePlay>(m_game, incrementedLevel));
			}
			else
			{
				Vec2 prevOverlap = Physics::GetPreviousOverlap(m_player, tile);
				auto& playerTransform = m_player->getComponent<CTransform>();
				auto& tileTransform = tile->getComponent<CTransform>();
				auto& tileAnimation = tile->getComponent<CAnimation>();

				if (!m_verticalResolved && prevOverlap.x > 0) {
					if (playerTransform.pos.y > tileTransform.pos.y) {
						playerTransform.velocity.y = overlap.y;
						if (tileAnimation.animation.getName() == "Brick") {
							m_sound.setBuffer(m_game->getAssets().getSound("Explosion"));
							m_sound.play();
							tile->getComponent<CBoundingBox>().has = false;
							tileAnimation.destroy = true;
							tileAnimation.animation = m_game->getAssets().getAnimation("Explosion");
						}
						else if (tileAnimation.animation.getName() == "Question") {
							m_sound.setBuffer(m_game->getAssets().getSound("Coin"));
							m_sound.play();

							m_score++;

							tileAnimation.animation = m_game->getAssets().getAnimation("Question2");
							auto coin = m_entities.addEntity("Coin");
							coin->addComponent<CAnimation>(m_game->getAssets().getAnimation("CoinSpin"), true);
							coin->addComponent<CTransform>(gridtoMidPixel((tileTransform.pos.x - 32) / 64, (tileTransform.pos.y - 32) / 64 - 1, coin));
						}
					}
					else {
						playerTransform.pos.y -= overlap.y;
					}
					m_verticalResolved = true;
				}

				if (!m_horizontalResolved && prevOverlap.y > 0) {
					if (playerTransform.pos.x > tileTransform.pos.x) {
						playerTransform.pos.x += overlap.x;
					}
					else {
						playerTransform.pos.x -= overlap.x;
					}
					m_horizontalResolved = true;
				}

				if (m_verticalResolved && m_horizontalResolved) {
					break;
				}
			}
		}
	}


	auto& playerTransform = m_player->getComponent<CTransform>();
	playerTransform.pos.x += totalAdjustment.x;
	playerTransform.pos.y += totalAdjustment.y;

}



void ScenePlay::update() {
	m_entities.update();
	if (!m_paused) {
		sMovement();
		sCollision();
		sAnimation();
		spawnBullet();
		sLifeSpan();
		sDragAndDrop();
	}
}


void ScenePlay::onEnd()
{
	for (auto& entity : m_entities.getEntities())
	{
		entity->destroy();
	}
	m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void ScenePlay::sMovement()
{
	Vec2 playerVelocity(0, 0);

	if (m_player->getComponent<CInput>().up)
	{
		playerVelocity.y = -2.0;
	}

	else if (m_player->getComponent<CInput>().down)
	{
		playerVelocity.y = 2.0;
	}

	else if (m_player->getComponent<CInput>().right)
	{
		playerVelocity.x = 2.0;
	}

	else if (m_player->getComponent<CInput>().left)
	{
		playerVelocity.x = -2.0;
	}

	if (m_player->getComponent<CTransform>().pos.x - m_player->getComponent<CBoundingBox>().halfSize.x <= 0)
	{
		playerVelocity.x = 0.1f;
	}
	/*
	if (m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CBoundingBox>().halfSize.x >= m_game->m_widthW)
	{
		playerVelocity.x = -0.1f;
	}
	*/

	m_player->getComponent<CTransform>().velocity = playerVelocity;

	for (auto& e : m_entities.getEntities())
	{
		if (e->getComponent<CTransform>().velocity.y > 10)
		{
			e->getComponent<CTransform>().velocity.y = 10;
		}
		if (e->getComponent<CTransform>().velocity.x > 10)
		{
			e->getComponent<CTransform>().velocity.x = 10;
		}
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
	}
}

void ScenePlay::sRender()
{
	m_game->m_window.clear(sf::Color(220, 222, 217));
	sDebug();

	m_mShape.setFillColor(sf::Color::Red);
	m_mShape.setRadius(4);
	m_mShape.setOrigin(2, 2);

	Vec2 worldPos = windowToWorld(m_mPos);
	m_mShape.setPosition(worldPos.x, worldPos.y);
	m_game->m_window.draw(m_mShape);

	if (m_paused)
	{
		m_playText.setString("PAUSED");
		m_playText.setCharacterSize(100);
		if (m_viewSet)
		{
			m_playText.setPosition(m_playerView.getCenter().x - 192, m_playerView.getCenter().y - 64);
		}
		else
		{
			m_playText.setPosition(m_game->m_window.getDefaultView().getCenter().x - 192, m_game->m_window.getDefaultView().getCenter().y - 64);
		}
		m_game->m_window.draw(m_playText);
	}

	m_game->m_window.display();
}