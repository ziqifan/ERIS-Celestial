#include "Turret.h"

Turret::Turret()
{
}

Turret::Turret(const char* str):Model(str)
{
}

Turret::~Turret()
{
}

void Turret::setLifeTime(float t)
{
	m_lifeTime = t;
}

float Turret::getLifeTime()
{
	return m_lifeTime;
}

void Turret::setHealth(int v)
{
	m_health = v;
}

int Turret::getHealth()
{
	return m_health;
}
void Turret::update(float dt)
{
	dt;
	static float Time = 0;
	static float bulletTime = 0;
	Time += dt;
	static Model* bullet = new Model("Models/Bullet/bullet.obj");
	if (Time - bulletTime >= 1)
	{
		bullets.push_back(new Model(*bullet));
		GameEmGine::addModel(bullets.back());
		bullets.back()->setColour(1, 1, 1);//bullet color = players color
		Coord3D pos = this->getTransformer().getPosition();
		bullets.back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
		bullets.back()->getTransformer().setScale(.25f);

		velocity.push_back(Coord3D());
		velocity.back() = (m_minions[0]->getTransformer().getPosition() - getTransformer().getPosition()).normal();
		velocity.back().y = 0;
		bulletTime = Time;
		//timer.push_back(0);
		std::string tag = "Turret Shots Fired" + std::to_string(bullets.size());
		AudioPlayer::createAudioStream("Audio/pew.wav",tag);
		AudioPlayer::play();
	}

	for (int a = 0; a < (int)bullets.size(); a++)
	{
		bullets[a]->getTransformer().translateBy(velocity[a]);
		//if(collision bullets[a], minion)
	}

	
}

void Turret::setMinions(std::vector<Minion*> minions)
{
	this->m_minions = minions;
}
