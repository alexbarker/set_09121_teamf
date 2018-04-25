#include "engine.h"
#include "../game.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <system_resources.h>
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_music.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_basic_movement.h"
#include "../components/cmp_state_machine.h"
#include "../components/cmp_physics.h"
#include "system_renderer.h"
#include "scene_level6.h"
#include "../add_entity.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

sf::Sprite backgroundSprite6a;
sf::Texture backgroungTexture6a;
sf::Vector2u backgroundSize6a;  //Added to store texture size.
sf::Vector2u windowSize6a;   //Added to store window size.
sf::Sprite titleSprite6b;
sf::Texture titleTexture6b;
sf::Vector2u titleSize6b;
sf::Vector2u windowSize6b;
int fadeCounter6 = 0;

void Level6Scene::SetTitle() {
	titleTexture6b = *Resources::load<Texture>("title.png");
	float x1 = Engine::GetWindow().getSize().x;
	float y1 = Engine::GetWindow().getSize().y;
	titleSize6b = titleTexture6b.getSize(); //Get size of texture.
	windowSize6b = Engine::GetWindow().getSize();             //Get size of window.
	float ScaleX = (float)windowSize6b.x / titleSize6b.x;
	float ScaleY = (float)windowSize6b.y / titleSize6b.y;     //Calculate scale.
	titleSprite6b.setTexture(titleTexture6b);
	titleSprite6b.setPosition(windowSize6b.x / 9.2, windowSize6b.y / 12.5);
	titleSprite6b.setScale(ScaleX / 6, ScaleY / 6);
	titleSprite6b.setOrigin(titleSize6b.x / 2, titleSize6b.y / 2);
}

void Level6Scene::SetBackground() {
	backgroungTexture6a = *Resources::load<Texture>("main1.png");
	float x = Engine::GetWindow().getSize().x;
	float y = Engine::GetWindow().getSize().y;
	backgroundSize6a = backgroungTexture6a.getSize(); //Get size of texture.
	windowSize6a = Engine::GetWindow().getSize();             //Get size of window.
	float ScaleX = (float)windowSize6a.x / backgroundSize6a.x;
	float ScaleY = (float)windowSize6a.y / backgroundSize6a.y;     //Calculate scale.
	backgroundSprite6a.setTexture(backgroungTexture6a);
	backgroundSprite6a.setPosition(0, 0);
	backgroundSprite6a.setScale(ScaleX, ScaleY);
	backgroundSprite6a.setOrigin(0, 0);
}

void Level6Scene::Load() {
	s2.playing();

	float x2 = Engine::getWindowSize().x;
	float y2 = Engine::getWindowSize().y;
	Engine::GetWindow().setSize(sf::Vector2u(x2, y2));
	Engine::GetWindow().display();

	float temp = y2 / 44;

	ls::loadLevelFile("res/level6.txt", temp);
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * temp);
	ls::setOffset(Vector2f(x2 / 4.72, ho));

	SetBackground();
	SetTitle();

	player = AddEntity::makePlayer(this, Vector2f(x2 / 2, y2 / 2));

	AddEntity::makeWalls(this);
}

void Level6Scene::UnLoad() {
	float x2 = Engine::GetWindow().getSize().x;
	float y2 = Engine::GetWindow().getSize().y;
	Engine::GetWindow().setView(sf::View(sf::FloatRect(0, 0, x2, y2)));
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void Level6Scene::Update(const double& dt) {

	const auto pp = player->getPosition();
	if (ls::getTileAt(pp) == ls::END) {
		Engine::ChangeScene((Scene*)&level7);
	}

	Event event;
	while (Engine::GetWindow().pollEvent(event)) {
		if (event.type == sf::Event::Resized) {
			UnLoad();
			Engine::GetWindow().setSize(sf::Vector2u(event.size.width, event.size.height));
			Engine::GetWindow().display();
			Load();
		}
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::R)) {
		UnLoad();
		Load();
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
		s1.play1(0, true);
		Engine::ChangeScene(&menu);
	}
	Scene::Update(dt);
}

void Level6Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();

	if (fadeCounter6 <= 250) {
		titleSprite6b.setColor(sf::Color(255, 255, 255, fadeCounter6));
		fadeCounter6--;
		Renderer::queue(&backgroundSprite6a);
		Renderer::queue(&titleSprite6b);
	}
	else {
		fadeCounter6 = 0;
		Renderer::queue(&backgroundSprite6a);
		Renderer::queue(&titleSprite6b);
	}
}
