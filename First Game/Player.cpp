#include "Player.h"
#include <algorithm>


Model
* Player::bullet,
* Player::redBar, * Player::blueBar, * Player::greenBar, * Player::yellowBar,
* Player::baseRedBar, * Player::baseBlueBar, * Player::baseGreenBar, * Player::baseYellowBar;

void Player::init(int index)
{
	if (!bullet)
		Player::bullet = new Model("Models/Bullet/bullet.obj");
	if (!redBar)
		Player::redBar = new Model("Models/BloodBar/RedBar/blood.obj");//64
	if (!blueBar)
		Player::blueBar = new Model("Models/BloodBar/BlueBar/blood.obj");//65
	if (!greenBar)
		Player::greenBar = new Model("Models/BloodBar/GreenBar/blood.obj");//66
	if (!yellowBar)
		Player::yellowBar = new Model("Models/BloodBar/YellowBar/blood.obj");//67
	if (!baseRedBar)
		Player::baseRedBar = new Model("Models/BloodBar/RedBarLighter/blood.obj");//68
	if (!baseBlueBar)
		Player::baseBlueBar = new Model("Models/BloodBar/BlueBarLighter/blood.obj");//69
	if (!baseGreenBar)
		Player::baseGreenBar = new Model("Models/BloodBar/GreenBarLighter/blood.obj");//70
	if (!baseYellowBar)
		Player::baseYellowBar = new Model("Models/BloodBar/YellowBarLighter/blood.obj");//71

	squash = new Animation();
	squash->addDir("Models/RIP/Rip Ani/");
	squash->setAnimationSpeed(.2f);
	//	squash[a].repeat(true);

	graveStone = new Model("Models/RIP/Rip Ani/RIP1.obj");

	ringID = new Model("Models/ID/Identifier.obj");
	bulletCircle = new Model("Models/BulletCircle/BulletCircle.obj");

	ringID->addChild(bulletCircle);
	this->addChild(ringID);

	ringID->getTransformer().setRotation({ 0,-90.f, 0 });
	ringID->getTransformer().setScale(.75f);
	gun = new Model("Models/AssaultModel/Weapon/AssaultClassGun.obj");

	if (type == PlayerType::assault)
	{
		Animation* walk = new Animation, *idle = new Animation;
		walk->addDir("Models/Class/Assault/Run/");
		idle->addDir("Models/Class/Assault/Idle/");
		addAnimation("walk", walk);
		addAnimation("idle", idle);
		setAnimation("idle");
		walk->repeat(true);
		walk->setAnimationSpeed(0.05f);
		idle->repeat(true);
		idle->setAnimationSpeed(.25);
		idle->play();
	}
	else if (type == PlayerType::tank)
	{
		Animation* walk = new Animation, *idle = new Animation;
		walk->addDir("Models/Class/Tank/Run/");
		idle->addDir("Models/Class/Tank/Idle/");
		addAnimation("walk", walk);
		addAnimation("idle", idle);
		setAnimation("idle");
		walk->repeat(true);
		walk->setAnimationSpeed(0.05f);
		idle->repeat(true);
		idle->setAnimationSpeed(.25);
		idle->play();
	}
	else if (type == PlayerType::medic)
	{
		Animation* walk = new Animation, *idle = new Animation;
		walk->addDir("Models/Class/Medic/Run/");
		idle->addDir("Models/Class/Medic/Idle/");
		addAnimation("walk", walk);
		addAnimation("idle", idle);
		setAnimation("idle");
		walk->repeat(true);
		walk->setAnimationSpeed(0.05f);
		idle->repeat(true);
		idle->setAnimationSpeed(.25);
		idle->play();
	}
	else
	{
		Animation* walk = new Animation, *idle = new Animation;
		walk->addDir("Models/Class/Specialist/Run/");
		idle->addDir("Models/Class/Specialist/Idle/");
		addAnimation("walk", walk);
		addAnimation("idle", idle);
		setAnimation("idle");
		walk->repeat(true);
		walk->setAnimationSpeed(0.05f);
		idle->repeat(true);
		idle->setAnimationSpeed(.25);
		idle->play();
	}

	dead = false;
	setPlayerIndex(index);
}

Player::Player(int index) :Model()
{
	init(index);
}

Player::Player(Player& model) : Model(model)
{
	type = model.type;
	init(0);
}

Player::Player(const char* path) :Model(path)
{
	init(0);
}

Player::~Player()
{
}

void Player::setPlayerIndex(int index)
{
	m_index = index;
	GameEmGine::removeModel(m_baseBar);

	m_baseBar->removeChild(m_lifeBar);
	switch (m_index)
	{
	case 0:
		setColour({ 255,0,0 });
		m_baseBar = baseRedBar;
		m_lifeBar = redBar;
		ringID->setColour(1.f, 0, 0);
		graveStone->setColour({255,50,50});
		bulletCircle->setColour({ 255,50,50});
		break;
	case 1:
		setColour({ 0,0,255 });
		m_baseBar = baseBlueBar;
		m_lifeBar = blueBar;
		ringID->setColour(0, 0, 1.f);
		graveStone->setColour({50,50,200});
		bulletCircle->setColour({ 50,50,200});
		break;
	case 2:
		setColour({ 0,255,0 });
		m_baseBar = baseGreenBar;
		m_lifeBar = greenBar;
		ringID->setColour(0, 1.f, 0);
		graveStone->setColour({50,200,50});
		bulletCircle->setColour({ 50,200,50});
		break;
	case 3:
		setColour({ 255,255,0 });
		m_baseBar = baseYellowBar;
		m_lifeBar = yellowBar;
		ringID->setColour(1.f, 1.f, 0.4314f);
		graveStone->setColour({200,200,50});
		bulletCircle->setColour({ 200,200,50});
	}

	ringID->getTransformer().setPosition(Coord3D{ 0, .06f + .02f * index + .01f,0 });

	//m_lifeBar->getTransformer().setScale(0.08f, 0.08f, 0.065f);
	//m_lifeBar->getTransformer().setPosition(0.35f, 1.6f, 0.0f);
	m_baseBar->addChild(m_lifeBar);
	m_baseBar->getTransformer().setScale(0.08f, 0.08f, 0.065f);
	m_baseBar->getTransformer().setRotation({ 0, 90.f, 0 });
	m_baseBar->getTransformer().setPosition(getTransformer().getPosition() + Coord3D{ 0.35f,1.6f,0.0f });


	GameEmGine::addModel(m_baseBar);


}

float Player::getHealth()
{
	return m_health;
}

void Player::setHealth(float v)
{
	m_health = v;
}

int Player::getBulletCount()
{
	return m_bulletCount;
}

void Player::setBulletCount(int v)
{
	m_bulletCount = v;
}

float Player::getTimeSinceLastMissile()
{
	return m_timeSinceLastMissile;
}

void Player::setTimeSinceLastMissile(float v)
{
	m_timeSinceLastMissile = v;
}

void Player::hitByEnemy(Model* mod, float damage)
{
	if (!m_active)
		return;

	XinputController* p1 = (XinputController*)GameEmGine::getController(m_index);
	if (collision2D(ringID, mod))
	{
		//curveroni[a] = 1;
		//CandyMan->getMissial(a)->getTransformer().setPosition(mod[8]->getCenter());
		float tempHealth = getHealth() - damage;
		setHealth(tempHealth < 0 ? 0 : tempHealth);

		//Coord3D test = ;

		GLclampf(damage * .023f);
		p1->setVibration(glm::clamp(damage * .023f, 0.f, 1.f), glm::clamp(damage * .023f, 0.f, 1.f));
		onHitShakeTimer = time;
		onHitShakeDir = glm::clamp(damage * .0175f, 0.f, 1.f);
	}
}

void Player::onPlayArea(Model * mod)
{
	if (!collision2D(mod))
		getTransformer().setPosition(
			abs(getTransformer().getPosition().x) > mod->getWidth() / 2 ? getTransformer().getPosition().x < 0 ? -mod->getWidth() / 2 : mod->getWidth() / 2 : getTransformer().getPosition().x,
			0,
			abs(getTransformer().getPosition().z) > mod->getDepth() / 2 ? getTransformer().getPosition().z < 0 ? -mod->getDepth() / 2 : mod->getDepth() / 2 : getTransformer().getPosition().z);
}

bool Player::bulletCollisions(Model * mod)
{
	///- Bullet Collisions -///
	bool bulletHit = false;
	for (unsigned b = 0; b < bullets.size(); b++)
		if (bullets[b])
		{
			if (collision2D(bullets[b], mod))
				bulletHit = true;


			if (bulletHit)
			{
				GameEmGine::removeModel(bullets[b]);
				bullets.erase(bullets.begin() + b);
				timer.erase(timer.begin() + b);
				velocity.erase(velocity.begin() + b);
				break;
			}
		}

	return bulletHit;
}

void Player::update(float dt)
{
	if (!m_active)
	{
		getCurrentAnimation()->pause();
		return;
	}
	time += dt;

	XinputController* p1 = (XinputController*)GameEmGine::getController(m_index);
	p1->setStickDeadZone(.2f);

	if (GameEmGine::isControllerConnected(m_index))
	{
		if (!dead)
		{
			//angle of right stick
			if (p1->getSticks()[RS].x || p1->getSticks()[RS].y)
			{

				angle = acosf(p1->getSticks()[RS].x /
					sqrtf(p1->getSticks()[RS].x * p1->getSticks()[RS].x
						+ p1->getSticks()[RS].y * p1->getSticks()[RS].y)) * (180 / (float)M_PI);
				angle += (p1->getSticks()[RS].y < 0 ? (180 - angle) * 2 : 0) + 90;//90 represents the start angle
				angle = fmodf(angle, 360);
			}

			//If player dies
			if (getHealth() <= 0)
			{
				dead = true;
				AudioPlayer::createAudioStream("Audio/dead.wav","Death Sound");
				AudioPlayer::play();
				graveStone->setColour(getColour());
				graveStone->getTransformer().setScale(0.75f * 2, 1 * 2, 0.5 * 2), graveStone->getTransformer().setPosition(getTransformer().getPosition()), graveStone->getTransformer().setRotation({ 0.0f,270.0f,0.0f });
				GameEmGine::addModel(graveStone);
				graveStone->addAnimation("squash", squash);

				graveStone->setAnimation("squash");
				p1->resetVibration();
				GameEmGine::removeModel(this);
				dead = true;
				//GAME::removeModel(mod[44] + a);
			}

			/// - Player Shooting - ///
			if (p1->getTriggers().RT >= .95 && !gunControlLaw)
			{
				if (reloading == false)
				{
					if (getBulletCount() > 0)
					{
						gunControlLaw = true; //gun Control Law makes it so the guns function "manualy" instead of "fully automatic"

						bullets.push_back(new Model(*bullet));
						GameEmGine::addModel(bullets.back());
						bullets.back()->setColour(getColour());//bullet color = players color
						Coord3D pos = getTransformer().getPosition();
						bullets.back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
						bullets.back()->getTransformer().setScale(.25f);
						bullets.back()->getTransformer().setRotation({ 90 , angle ,0 });

						float cosVal = cos((float)(fmodf(angle - 90, 360) * (M_PI / 180)));
						float sinVal = sin((float)(fmodf(angle - 90, 360) * (M_PI / 180)));

						velocity.push_back(Coord3D());
						velocity.back() = Coord3D(cosVal * move * 3, 0, sinVal * move * 3);

						timer.push_back(0);
						std::string tag = "Shots Fired" + std::to_string(bullets.size());
						AudioPlayer::createAudioStream("Audio/pew.wav",tag);
						AudioPlayer::play();
						bulletCircle->setColour(1, 1, 1);//set bullet circle colour to white
						m_bulletCount--;//subtract from bullet count
						shotBuzzTimer = time;
						p1->setVibration(.6f, .6f);
					}
				}
			}
			else if (p1->getTriggers().RT < .95 && gunControlLaw)
			{
				switch (m_index)
				{
				case 0:
					bulletCircle->setColour({ 255,50,50,150 });
					break;
				case 1:
					bulletCircle->setColour({ 50,50,200,150 });
					break;
				case 2:
					bulletCircle->setColour({ 50,200,50,150 });
					break;
				case 3:
					bulletCircle->setColour({ 200,200,50,150 });
					break;
				default:
					break;
				}
				gunControlLaw = false;
			}

			


			/// - Button Presses on controller - ///
			if ((p1->isButtonPressed(CONTROLLER_X)) || (getBulletCount() <= 0))
			{
				if (reloading == false)
					reloadTimer = time;

				reloading = true;
			}

			if (reloading == true)
			{
				//put a bar here that lerps up to full or make circle become full
				if ((time - reloadTimer) >= 2)
				{
					setBulletCount(30);
					reloading = false;
					puts("RELOADING!!!\n");
				}
				else
				{
					//Put heptic feedback here
					puts("Can't Reload yet!\n");
				}
			}

			/// - Left Trigger to Dash - ///
			if (p1->getTriggers().LT >= .95)
			{
				//get total time and put it into duration variable

				if (time - cooldown >= 3)
				{
					if (f == true)
					{
						duration = time;
						f = false;
					}
					move = 0.5f;
					if (time - 0.1f >= duration)
					{
						AudioPlayer::createAudioStream("Audio/dash.wav","Wave Dash");
						EmGineAudioPlayer::setVolume(2);
						AudioPlayer::play();
						move = 0.1f;
						//If getTriggers() up then coolDown = time;
						cooldown = time;
						f = true;
					}
				}

			}

			else//Do the same with the LT button, have it so will only work every X seconds.
			{
				move -= .001f;
				if (move <= .1f)
					move = .1f;
				//f = false;
				dashControl = false;
			}

			getTransformer().setRotation({ 0,angle, 0 });
			getTransformer().translateBy(p1->getSticks()[LS].x * move, 0, p1->getSticks()[LS].y * move); //move player
			float speed = p1->getSticks()[LS].x * p1->getSticks()[LS].x + p1->getSticks()[LS].y * p1->getSticks()[LS].y;



			//Player Animations
			if (!speed && getAnimation("walk")->checkPlay())
			{
				getAnimation("walk")->pause();
				getAnimation("idle")->play();
				setAnimation("idle");
			}

			else if (speed && !getAnimation("walk")->checkPlay())
			{
				getAnimation("idle")->pause();
				getAnimation("walk")->play();
				setAnimation("walk");
			}


			if (dead)
			{
				GameEmGine::removeModel(this);
				GameEmGine::removeModel(m_lifeBar);
				GameEmGine::removeModel(m_baseBar);

				GameEmGine::removeModel(ringID);
				GameEmGine::removeModel(gun);
				GameEmGine::removeModel(bulletCircle);

				for (int a = 0; a < (int)bullets.size(); a++)
				{
					GameEmGine::removeModel(bullets[a]);
					bullets.erase(bullets.begin());
					velocity.erase(velocity.begin());
					timer.erase(timer.begin());
					a--;
				}
			}

			//Update each player's Bullet Circle
			bulletCircle->getTransformer().setScale((getBulletCount() / 30.0f) * 0.9f);

			//checks if bullet timer is up
			for (unsigned b = 0; b < bullets.size(); b++)
				if (bullets[b])
				{
					timer[b] += dt;
					bullets[b]->getTransformer().translateBy(velocity[b].x, velocity[b].y, velocity[b].z);

					if (timer[b] >= 1)
					{
						GameEmGine::removeModel(bullets[b]);
						bullets.erase(bullets.begin() + b);
						velocity.erase(velocity.begin() + b);
						timer.erase(timer.begin() + b);
						break;
					}
				}


		}
		//reset all vibration
		if (float(time - onHitShakeTimer) > onHitShakeDir)
			if (float(time - shotBuzzTimer) > shotBuzzDir)
				p1->resetVibration();
	}

	//Update each player's Blood Bar
	m_lifeBar->getTransformer().setScale(1.f, 1.f, ((float)getHealth() / (float)getInitialHealth()));
	m_baseBar->getTransformer().setPosition(getTransformer().getPosition() + Coord3D{ 0.35f,1.6f,0.0f });
}

void Player::setActive(bool active)
{
	m_active = active;
}

bool Player::isActive()
{
	return m_active;
}

float Player::getInitialHealth()
{
	return m_initialHealth;
}
