#include "scene_state.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_basic_movement.h"
#include "../components/cmp_state_machine.h"
#include "../enemy_states.h"
#include <LevelSystem.h>
#include "../game.h"
#include "../components/cmp_music.h"

using namespace std;
using namespace sf;

MusicPlayer s3;

void StateScene::Load()
{
	s1.stop();
	s3.play3(2, true);
	auto player = makeEntity();
	player->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2));
	auto s = player->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(10.0f);
	s->getShape().setFillColor(Color::Red);
	player->addComponent<BasicMovementComponent>();

	auto enemy = makeEntity();
	enemy->setPosition(Vector2f(50.0f, 50.0f));
	s = enemy->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(10.0f);
	s->getShape().setFillColor(Color::Blue);

	auto sm = enemy->addComponent<StateMachineComponent>();
	sm->addState("normal", make_shared<NormalState>(player));
	sm->addState("near", make_shared<NearState>(player));
	sm->changeState("normal");

}

void StateScene::UnLoad() { Scene::UnLoad(); }

void StateScene::Update(const double& dt)
{
	if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
		s1.play1(0, true);
		Engine::ChangeScene(&menu);
	}
	Scene::Update(dt);
}

void StateScene::Render()
{
	Scene::Render();
}