#include "Tank.h"

void Tank::init()
{
	type = tank;
	m_initialHealth = 250;
	setHealth(250);
}

Tank::Tank():Player()
{
	init();
}

Tank::Tank(Player& model) : Player(model)
{
	init();
}

Tank::Tank(const char* path) : Player(path)
{
	init();
}


Tank::~Tank()
{
}

float Tank::getTimeSinceLastShield()
{
	return timeSinceLastShield;
}

void Tank::setTimeSinceLastShield(float v)
{
	timeSinceLastShield = v;
}

void Tank::update(float dt)
{
	if (!m_active)
	{
		getCurrentAnimation()->pause();
		return;
	}
	else
		if (!getCurrentAnimation()->checkPlay())
			getCurrentAnimation()->play();

	Player::update(dt);
	XinputController* p1 = (XinputController*)GameEmGine::getController(m_index);
	p1->setStickDeadZone(.2f);

	/// - Tank Special Ability Active - ///
	if(tankShield == true)
	{
		if((time - shieldTime) >= 2.5f)
		{
			if(getHealth() > getInitialHealth())
			{
				setHealth(getInitialHealth());
			}
			tankShield = false;
		}
	}

	/// - Tank Special Ability Inactive - ///

	if(p1->isButtonStroked(CONTROLLER_Y))
		if(tankShield == false)
		{
			if(time - getTimeSinceLastShield() >= 8)
			{
				setHealth(getHealth() + 100);
				shieldTime = time;
				setTimeSinceLastShield(time);
				//puts("Special Ability TANK");
				AudioPlayer::createAudioStream("Audio/tankAbility.wav","Tank Ability");
				AudioPlayer::play();

				tankShield = true;
			}
		}

}
