#define _USE_MATH_DEFINES
#include "Boss.h"
#include "Assault.h"



Model* Boss::minion;//78

void Boss::init()
{
	if(!minion)
		minion = new Minion("Models/Minion/SmallRobot/SmallRobot.obj");
	minion->setToRender(true);

	missles.resize(4);
	missles[0] = (new Model("Models/Missile/BossMissile.obj"));
	//GAME::addModel(missles[0]);
	missles[1] = (new Model(*missles[0]));
	//GAME::addModel(missles[1]);
	missles[2] = (new Model(*missles[0]));
	//GAME::addModel(missles[2]);
	missles[3] = (new Model(*missles[0]));
	//GAME::addModel(missles[3]);

	lazer = new Model("Models/lazer/lazer.obj");
	lazer->setColour(1, 0, 0);
	//GAME::addModel(lazer);

	m_lifeBar = new Model("Models/BloodBar/PinkBar/blood.obj");//72
	m_baseBar = new Model("Models/BloodBar/PinkBarLighter/blood.obj");//73

	m_baseBar->setToRender(true);

	//Boss Blood Bar
	//m_baseBar->getTransformer().setPosition(this->getTransformer().getPosition() + Coord3D{ m_baseBar->getWidth() / 2, this->getHeight() - 10 , 0 });
	m_baseBar->getTransformer().setPosition(10.5f, 20.0f, 20.0f);
	m_baseBar->getTransformer().setRotation(Coord3D(0, 90, 0));
	m_baseBar->getTransformer().setScale(0.8f, 0.8f, 2.0f);

	//m_lifeBar->getTransformer().setPosition(this->getTransformer().getPosition() + Coord3D{ m_lifeBar->getWidth() / 2  ,this->getHeight() - 10, 0 });
	m_lifeBar->getTransformer().setPosition(10.5f, 20.0f, 20.0f);
	m_lifeBar->getTransformer().setRotation(Coord3D(0, 90, 0));
	m_lifeBar->getTransformer().setScale(0.8f, 0.8f, 2.0f);

	GameEmGine::addModel(m_baseBar);
	GameEmGine::addModel(m_lifeBar);


	//Boss Animation
	static Animation missleShoot, laserPoint, slam;
	missleShoot.addDir("Models/BOSS/missleShoot/");
	laserPoint.addDir("Models/BOSS/laserPoint/");
	slam.addDir("Models/BOSS/slam/");
	addAnimation("missleShoot", &missleShoot);
	addAnimation("laserPoint", &laserPoint);
	addAnimation("slam", &slam);
	setAnimation("missleShoot");
	missleShoot.setAnimationSpeed(0.3f);
	missleShoot.repeat(true);
	getCurrentAnimation()->play();

	//m_baseBar->addChild(m_lifeBar);
	//m_lifeBar->getTransformer().setPosition(getTransformer().getPosition() + Coord3D{13.0f,18.5f,0.0f});
	//m_lifeBar->getTransformer().setRotation(Coord3D(0, 90, 0));
	//m_lifeBar->getTransformer().setScale(0.8f, 0.8f, 2.5f);


	//addChild(m_baseBar);
	//addChild(m_lifeBar);


	m_initialHealth = m_health = 5000.f;
}

Boss::Boss(): Model()
{
	init();
}

Boss::Boss(Model& model) : Model(model)
{
	init();
}

Boss::Boss(const char* path) : Model(path)
{
	init();
}

Boss::~Boss()
{
	//boss's blood bar
	//GameEmGine::removeModel;
	//Model* m_lifeBar;
	//Model* m_baseBar;
	//float 	m_time = 0, m_bossFlashTime = 0;
	//bool m_active = true, m_dead = false;
	//float m_initialHealth, m_health;

	//static Model* minion;
	//int attackDamage;
	//float attackCooldown;
	//Player** targets;
}

void Boss::setPlayers(Player* players[4])
{
	targets = players;
}

bool Boss::randAttacks()
{
	//Randomly Pick an attack (attacks should be other functions, this semester just model attacks)
	//Use Attack
	//If Player(s) standing too close use stomp OR flop attack instead
	//Wait x Amount of time before repeating
	return false;
}

float Boss::getHealth()
{
	return m_health;
}

void Boss::setHealth(float v)
{
	m_health = v;
}

std::vector<Model*>& Boss::getMissials()
{
	return missles;
}

void Boss::update(float dt)
{
	if(!m_active)
	{
		getCurrentAnimation()->pause();
		return;
	}
	else
		if(!getCurrentAnimation()->checkPlay())
			getCurrentAnimation()->play();

	m_time += dt;

	static clock_t  lastDelay[4];
	static float  curveroni[4], delay[4]{10,10,10,10};
	static bool hasTarget[4];
	static Coord3D cat[4];
	static Coord3D  pointPosition[4];

	static Model* missileRadius[4]
	{
		(new Model("Models/Target Circle/TargetCircle.obj")),(new Model("Models/Target Circle/TargetCircle.obj")),
		(new Model("Models/Target Circle/TargetCircle.obj")),(new Model("Models/Target Circle/TargetCircle.obj"))
	};

	missileRadius[0]->setColour(1, 0, 0);
	missileRadius[1]->setColour(0, 0, 1);
	missileRadius[2]->setColour(0, 1, 0);
	missileRadius[3]->setColour({255,255,110});
	static  float missileSpeed = 0.01f;

	if(!m_dead)
	{
		for(int a = 0; a < 4; a++)
		{

			missles[a]->getTransformer().setScale(1.5f); //every missile shot is scaled up a bit since the original is small 
			pointPosition[a] = getTransformer().getPosition();
			if(targets[a]->isActive())
				if(!targets[a]->dead)
				{
					float ans;
					static Coord3D bossTarget[4];

					//gets a target for model (players 1, 2, 3 or 4) randomly
					if((ans = (clock() - lastDelay[a]) / (float)CLOCKS_PER_SEC) >= delay[a])
					{
						if(!curveroni[a])
						{
							GameEmGine::addModel(missles[a]);
							GameEmGine::addModel(missileRadius[a]);
							bossTarget[a] = targets[a]->getTransformer().getPosition();
						}
						curveroni[a] += 28.f /(getTransformer().getPosition()- bossTarget[a]).distance() * missileSpeed;
					}

					if(!hasTarget[a])
					{

						delay[a] = (rand() % 2 + 1) + (float(rand() % 100) / 100);
						hasTarget[a] = true;

					}

					if(curveroni[a] >= 1)
					{
						//Missile to Player Collisions
						for(int t = 0; t < 4; t++)
							targets[t]->hitByEnemy(missles[a], 35);

						curveroni[a] = 0;
						lastDelay[a] = clock();
						hasTarget[a] = false;

						GameEmGine::removeModel(missles[a]);
						GameEmGine::removeModel(missileRadius[a]);
						missles[a]->getTransformer().setPosition(getTransformer().getPosition());
					}

					Coord3D
						p1[4],
						p2[4],
						c1[4],
						c2[4];
					if(hasTarget[a])
					{
						p1[a] = getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 2.0f),//start point
							p2[a] = bossTarget[a],//end point 
							c1[a] = p1[a] - Coord3D{0, 100, 100},//control point
							c2[a] = p2[a] - Coord3D{0, 150, 100};//control point

						cat[a] = catmull
						(
							c1[a],
							p1[a],
							p2[a],
							c2[a],
							curveroni[a]
						);

						pointPosition[a] = cat[a];
					}

					if(hasTarget[a])
					{
						missles[a]->getTransformer().setPosition(pointPosition[a].x, pointPosition[a].y, pointPosition[a].z);
						missileRadius[a]->getTransformer().setPosition(bossTarget[a] + Coord3D{0,.06f + .02f * a,0});
						missileRadius[a]->getTransformer().setScale(catmull(-7.f, 1.f, 0.7f, -7.f, curveroni[a]));
					}

					static bool slam = false;
					//Player comes near Boss, gets teleported backwards
					if(collision2D(targets[a]))
					{
						getAnimation("missleShoot")->stop();
						setAnimation("slam");
						getCurrentAnimation()->stop();
						getCurrentAnimation()->setAnimationSpeed(3.f);
						getCurrentAnimation()->repeat(false);
						getCurrentAnimation()->play();
						slam = true;

						if(getAnimation("slam")->getFrameNumber() == 5)
						{
							targets[a]->hitByEnemy(this);
							//targets[a]->getTransformer().setPosition(targets[a]->getTransformer().getPosition().x, targets[a]->getTransformer().getPosition().y, getTransformer().getPosition().z - 15);
							//targets[a]->setHealth(targets[a]->getHealth() - 35);
						}
					}

					if(getCurrentAnimation() == getAnimation("slam") && getAnimation("slam")->hasEnded())
					{
						setAnimation("missleShoot");
						getCurrentAnimation()->play();
						getCurrentAnimation()->repeat(true);
					}


				}
		}

		if(m_health <= m_initialHealth / 2)
		{
			getAnimation("missleShoot")->setAnimationSpeed(0.15f);
			missileSpeed = 0.02f;
		}

		//Boss health bar calculation
		m_lifeBar->getTransformer().setScale(0.8f, 0.8f, 2.0f * (m_health / m_initialHealth));
		//m_baseBar->getTransformer().setPosition(getTransformer().getPosition() + Coord3D{0, getHeight(), 0});

		//eliminates the possibility of the bar being too large
		if(m_health > m_initialHealth)
			m_initialHealth = m_health;

		/// - Boss Spawns Minions - ///
		//TODO: More Minions, Have boss spawn minions, Make minions have unifrom move speed. Lerp between colours??

		static float minionDelay, minionDelayCounter;

		if(minionDelayCounter += dt > minionDelay)
			if(minions.size() < 10)
			{

				minions.push_back(new Minion(*minion));
				GameEmGine::addModel(minions.back());

				minions.back()->setColour({200, 50, 50});
				minions.back()->getTransformer().setPosition(float(rand() % 15 + rand() % 1000 * .001f) * -(rand() % 2), 0, -float(rand() % 2 + rand() % 100 * .001f)); // Random spawns in bottom right of screen
				while(minions.back()->collision2D(targets[0]) ||
					minions.back()->collision2D(targets[1]) ||
					minions.back()->collision2D(targets[2]) ||
					minions.back()->collision2D(targets[3]))
					minions.back()->getTransformer().setPosition(float(rand() % 15 + rand() % 1000 * .001f) * -(rand() % 2), 0, -float(rand() % 2 + rand() % 100 * .001f)); // Random spawns in bottom right of screen

				minions.back()->getTransformer().setScale(0.4f, 0.6f, 0.4f);

				minionDelay = float(rand() % 9) + rand() % 1001 * .001f;//delay from 0 to 9 seconds
			}

		///~ unclump all minions ~///
		for(int a = 0; a < (int)minions.size(); a++)
		{
			for(int b = 0; b < a; b++)
				if(collision2D(minions[a - 1], minions[a]))
				{
					minions[a]->move(false);
					break;
				}
				else
					minions[a]->move(true);

			minions[a]->update(dt);
		}

		if(m_health <= 0)
		{
			//GAME::gameWin = true;
			m_dead = true;
			for(auto& a : minions)
				GameEmGine::removeModel(a);

			for(auto& a : missles)
				GameEmGine::removeModel(a);

			for(int a = 0; a < 4; a++)
			{
				GameEmGine::removeModel(missileRadius[a]);
				delete missileRadius[a];
			}
			minions.clear();
			missles.clear();
			GameEmGine::removeModel(this);
		}

		for(int a = 0; a < (int)minions.size(); a++)
		{
			if(minions[a]->getHealth() <= 0)
			{
				GameEmGine::removeModel(minions[a]);
				minions.erase(minions.begin() + a);

				minionDelay = minionDelayCounter;
				a--;
			}
		}
	}

	/// - Boss Flashes when hit by Bullets - ///
	if(bossFlash == true)
	{
		m_bossFlashTime = m_time;
		this->setColour({255,0,0});
		m_lifeBar->setColour(1, 1, 1);
		bossFlash = false;
	}

	if(bossFlash == false && (m_time - m_bossFlashTime >= 0.15f))
	{
		this->setColour(1, 1, 1);
		m_lifeBar->setColour({255,0,255});
	}
	//shootLazer(0);
}

void Boss::shootLazer(int playerIndex)
{
	Coord3D start = getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 0.0f)
		, end = targets[playerIndex]->getTransformer().getPosition();

	float distance = (end - start).distance();
	static float counter, amount = .01f;

	counter += amount;

	if(counter > 1 || counter < 0)
	{
		counter = counter > 0 ? 1.f : 0;

		amount *= -1;
	}

	float angle[3];
	angle[0] = acosf((start.x * end.x + start.y * end.y + start.z * end.z) /
		(sqrtf(start.x * start.x
			+ start.y * start.y + start.z * start.z) * sqrtf(end.x * end.x + end.y * end.y + end.z * end.z)));

	angle[1] = acosf((start.x * end.x + start.z * end.z) /
		(sqrtf(start.x * start.x
			+ start.z * start.z) * sqrtf(end.x * end.x + end.z * end.z))) * (180 / (float)M_PI);

	angle[2] = acosf((start.y * end.y + start.z * end.z) /
		(sqrtf(start.y * start.y
			+ start.z * start.z) * sqrtf(end.y * end.y + end.z * end.z))) * (180 / (float)M_PI);

	//angle[0] += (end.y - start.y < 0 ? (180 - angle[0]) * 2 : 0);//90 represents the start angle
	//angle[1] += (end.z - start.z < 0 ? (180 - angle[1]) * 2 : 0);//90 represents the start angle
	//angle[2] += (end.x - start.x < 0 ? (180 - angle[2]) * 2 : 0);//90 represents the start angle


	lazer->getTransformer().setPosition(start);
	lazer->getTransformer().setScale(.5f, lerp(0.0f, distance, counter), .5f);

	float anglex = cosf(angle[0]) * (360 / (float)M_PI),
		angley = tanf(angle[0]) * (360 / (float)M_PI),
		anglez = sinf(angle[0]) * (360 / (float)M_PI);
	angley, anglez;
	lazer->getTransformer().setRotation({anglex,0,0});

}

void Boss::setActive(bool active)
{
	m_active = active;
}

bool Boss::isActive()
{
	return m_active;
}

bool Boss::hitByEnemy(Model * mod, float damage)
{
	if(!m_active)
		return false;

	if(collision2D(this, mod))
	{
		AudioPlayer::createAudioStream("Audio/bossHit.wav", "Boss Hit");
		AudioPlayer::play();
		setHealth(getHealth() - damage);
		bossFlash = true;
		return true;
	}
	return false;
}