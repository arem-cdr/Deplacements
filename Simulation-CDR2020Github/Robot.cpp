#include"stdafx.h"

#include "Robot.h"

using namespace std;


robot::robot(int nbRobots, team team, sf::Vector2f taille, int idRobot, const Terrain* terrain, int id) 
	: m_target(NULL), 
	m_maxAcceleration(.4f), m_maxDeceleration(1.f), m_maxSpeed(5.f),
	m_team(team),
	m_terrain(terrain),
	id(id)
{
	switch (team)
	{
	case (PURPLE):

		m_pos.Setall(xconv(2550), yconv(705 + idRobot * 500), aconv(90));
		break;

	case(YELLOW):
		m_pos.Setall(xconv(250), yconv(705 + idRobot * 500), aconv(90));
		break;

	default:
		break;
	}

	m_length = taille.x;
	m_width = taille.y;
	m_rightSpeed = 0.f;
	m_leftSpeed = 0.f;
	m_delay = 2000.f;

	// creation de m_shapeTest, m_shapeTarget
	{
		m_shapeTest.setSize(sf::Vector2f((float)m_length, (float)m_width));
		m_shapeTest.setFillColor(sf::Color(20, 100, 20, 170));
		m_shapeTest.setPosition(sf::Vector2f(m_pos.getX() + 100, m_pos.getY()));
		m_shapeTest.setRotation(-degre(m_pos.getangle()) - 90);

		m_shapeTarget.setSize(sf::Vector2f((float)m_length, (float)m_width));
		m_shapeTarget.setOutlineColor(sf::Color(0, 0, 0));
		m_shapeTarget.setOutlineThickness(1);
		m_shapeTarget.setOrigin(sf::Vector2f((float)m_length * 0.5f, (float)m_width * 0.5f));
		if (m_target)
		{
			m_shapeTarget.setPosition(sf::Vector2f(m_target->getX(), m_target->getY()));
			m_shapeTarget.setRotation(-degre(m_target->getangle()) - 90);
		}
		switch (team)
		{
		case (PURPLE):

			m_shapeTarget.setFillColor(sf::Color(0, 0, 255, 150));
			break;

		case(YELLOW):
			m_shapeTarget.setFillColor(sf::Color(220, 220, 0, 150));
			break;

		default:
			break;
		}
	}

	// creation de l'aspect graphique du robot
	{
		switch (team)
		{
		case (PURPLE):
			m_texture.loadFromFile("Ressources/Images/RobotTextureBlue.png");
			break;

		case(YELLOW):
			m_texture.loadFromFile("Ressources/Images/RobotTextureYellow.png");
			break;

		default:
			break;
		}
		
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin(tailleTextureRobotX * 0.5f, tailleTextureRobotY * 0.5f);
		m_sprite.setScale((float) taille.x / tailleTextureRobotX, (float)taille.y / tailleTextureRobotY);
	}

	m_IAPthfinding = new AlgoGen(nbRobots, m_pos, sf::Vector2f(m_length, m_width), m_maxAcceleration, m_maxDeceleration, m_maxSpeed, m_terrain);

	for (int i = 0; i < nbRobots - 1; ++i)
		m_posOtherRobot.push_back(new Point(0.f, 0.f, 0.f));
}

robot::~robot()
{
	if (m_IAPthfinding)
		delete m_IAPthfinding;

	for (size_t i = 0; i < m_posOtherRobot.size(); ++i)
	{
		delete m_posOtherRobot[i];
		m_posOtherRobot[i] = NULL;
	}

	if (this->m_target)
		delete this->m_target;
}

void robot::update(const float dt)
{
	//m_delay = 0;
	if (m_delay <= 0.f)
	{
		m_delay = 0.f;

		if (!m_target)
		{
			setTarget(Point(1000.f, 500.f, 0.f));
			retarget();
		}

		//frottements(dt);

		actualise_position(m_rightSpeed, m_leftSpeed);
	}
	else
	{
		std::cout << "stop" << std::endl;
		m_delay -= dt * 1000.f;
		if (m_delay < 0.f)
			m_delay = 0.f;
	}
}

void robot::updateClavier(float vitD, float vitG) // utile au debug : permet de tester le comportement du robot en deplacant la target au clavier
{
	//actualise_positionTarget(vitD, vitG);
	//actualise_position(vitD,vitG); // on peut aussi controler le robot au clavier en inversant les lignes commentees dans cette fonction

	//m_shapeTarget.setPosition(sf::Vector2f(m_target.getX(), m_target.getY()));
	//m_shapeTarget.setRotation(-degre(m_target.getangle()) - 90);
	//m_shape.setPosition(sf::Vector2f(m_pos.getX(), m_pos.getY()));
	//m_shape.setRotation(-degre(m_pos.getangle()) - 90);
}

void robot::frottements(float dt)
{
	m_leftSpeed *= 1 - 0.005f * dt * FPS;
	m_rightSpeed *= 1 - 0.005f * dt * FPS;
}

void robot::actualise_position(float rightSpeed, float leftSpeed)
{
	// determination du cercle décrit par la trajectoire et de la vitesse du robot sur ce cercle

	if (rightSpeed != leftSpeed) {

		float R = 0; // rayon du cercle decrit par la trajectoire
		float d = 0; // vitesse du robot
		float cx = 0; // position du centre du cercle decrit par la trajectoire
		float cy = 0;

		R = ECART_ROUE / 2 * (rightSpeed + leftSpeed) / (leftSpeed - rightSpeed); // rayon du cercle
		cx = m_pos.getX() + R * sin(m_pos.getangle());
		cy = m_pos.getY() + R * cos(m_pos.getangle());
		d = (leftSpeed + rightSpeed) * 0.5f;

		// mise à jour des coordonnées du robot
		if (leftSpeed + rightSpeed != 0) {
			m_pos.Setangle(m_pos.getangle() - d / R);//m_angle -= d/R;
		}
		else {
			m_pos.Setangle(m_pos.getangle() + rightSpeed * 2.f / ECART_ROUE); //m_angle += rightSpeed*2.0 / ECART_ROUE;
		}

		if (m_pos.getangle() > PI)
		{
			m_pos.Setangle(m_pos.getangle() - 2 * PI); //m_angle -= 2*PI;
		}
		else if (m_pos.getangle() <= -PI)
		{
			m_pos.Setangle(m_pos.getangle() + 2 * PI); //m_angle += 2*PI;
		}

		m_pos.setX(cx - R * sin(m_pos.getangle())); //m_xPos = cx - R * sin(m_angle);
		m_pos.setY(cy - R * cos(m_pos.getangle())); //m_yPos = cy - R * cos(m_angle);
	}
	else if (leftSpeed == rightSpeed) { // cas où la trajectoire est une parfaite ligne droite

		m_pos.setX(m_pos.getX() + leftSpeed * cos(m_pos.getangle())); //m_xPos += leftSpeed * cos(m_angle);
		m_pos.setY(m_pos.getY() - rightSpeed * sin(m_pos.getangle())); //m_yPos -= rightSpeed * sin(m_angle);
	}
}

void robot::actualise_positionTarget(float rightSpeed, float leftSpeed)
{
	// determination du cercle décrit par la trajectoire et de la vitesse du robot sur ce cercle

	//if (rightSpeed != leftSpeed) {

	//	float R = 0; // rayon du cercle decrit par la trajectoire
	//	float d = 0; // vitesse du robot
	//	float cx = 0; // position du centre du cercle decrit par la trajectoire
	//	float cy = 0;

	//	R = ECART_ROUE / 2 * (rightSpeed + leftSpeed) / (leftSpeed - rightSpeed); // rayon du cercle
	//	cx = m_target.getX() + R * sin(m_target.getangle());
	//	cy = m_target.getY() + R * cos(m_target.getangle());
	//	d = (leftSpeed + rightSpeed) * 0.5f;

	//	// mise à jour des coordonnées du robot
	//	if (leftSpeed + rightSpeed != 0) {
	//		m_target.Setangle(m_target.getangle() - d / R);//m_angle -= d/R;
	//	}
	//	else {
	//		m_target.Setangle(m_target.getangle() + rightSpeed * 2.f / ECART_ROUE); //m_angle += rightSpeed*2.0 / ECART_ROUE;
	//	}

	//	if (m_target.getangle() > PI)
	//	{
	//		m_target.Setangle(m_target.getangle() - 2 * PI); //m_angle -= 2*PI;
	//	}
	//	else if (m_target.getangle() <= -PI)
	//	{
	//		m_target.Setangle(m_target.getangle() + 2 * PI); //m_angle += 2*PI;
	//	}

	//	m_target.setX(cx - R * sin(m_target.getangle())); //m_xPos = cx - R * sin(m_angle);
	//	m_target.setY(cy - R * cos(m_target.getangle())); //m_yPos = cy - R * cos(m_angle);
	//}
	//else if (leftSpeed == rightSpeed) { // cas où la trajectoire est une parfaite ligne droite

	//	m_target.setX(m_target.getX() + leftSpeed * cos(m_target.getangle())); //m_xPos += leftSpeed * cos(m_angle);
	//	m_target.setY(m_target.getY() - rightSpeed * sin(m_target.getangle())); //m_yPos -= rightSpeed * sin(m_angle);
	//}
}

bool robot::delay()
{
	if (m_delay > 0)
		return true;
	return false;
}

void robot::updatePosOtherRobots(const std::vector<Point>& posRobots)
{
	int ind = 0;
	for (size_t i = 0; i < posRobots.size(); ++i)
	{
		if (this->m_pos != posRobots[i])
		{
			*this->m_posOtherRobot[ind] = posRobots[i];
			++ind;
		}
	}
}

void robot::play(float time_available, const std::vector<Point>& posRobots)
{
	updatePosOtherRobots(posRobots);

	if (m_target)
		m_IAPthfinding->run(time_available, m_rightSpeed, m_leftSpeed, m_pos, *m_target, m_posOtherRobot);
	else
	{
		m_rightSpeed = 0;
		m_leftSpeed = 0;
	}

	if (m_target && reachTarget() && m_delay <= 0.f)
	{
		retarget();
	}
}

void robot::render(sf::RenderTarget& target)
{
	m_sprite.setPosition(m_pos.getX(), m_pos.getY()); 
	m_sprite.setRotation(-degre(m_pos.getangle()) - 90);

	target.draw(m_sprite);

	if (m_target)
		target.draw(m_shapeTarget);
	//target.draw(m_shapeTest);

	m_IAPthfinding->render(target);
}

std::string robot::posString() const
{
	return std::to_string((int)round(m_pos.getX())) + (string) ", " + std::to_string((int)round(m_pos.getY()));
}

std::string robot::pathFindingString() const
{
	return m_IAPthfinding->stateString();
}

std::string robot::speedString() const
{
	return (string) "vitesse roue droite : " + std::to_string((int)(m_rightSpeed * 10.f)) + (string) ", roue gauche : " + std::to_string((int)(m_leftSpeed * 10.f));
}

bool robot::reachTarget()
{
	return (dist(m_target->getX() - m_pos.getX(), m_target->getY() - m_pos.getY()) < 30
		&& val_abs(properAngleRad(m_target->getangle() - m_pos.getangle())) < 0.05f
		&& val_abs(m_rightSpeed) < 0.1f
		&& val_abs(m_leftSpeed) < 0.1f);
}

void robot::setTarget(const Point target)
{
	if (m_target)
		delete m_target;
	m_target = new Point(target);

	m_shapeTarget.setPosition(sf::Vector2f(m_target->getX(), m_target->getY()));
	m_shapeTarget.setRotation(-degre(m_target->getangle()) - 90);
}

void robot::deleteTarget()
{
	if (m_target)
	{
		delete m_target;
		m_target = NULL;
	}
}

void robot::retarget()
{
	float x = (float)(rand()) * CmToPx(tableLength + 20.f) / RAND_MAX + 230.f;
	float y = (float)(rand()) * CmToPx(tableWidth - 20.f) / RAND_MAX + 200.f;
	float ang = ((float)(rand()) / RAND_MAX - 0.5f) * PI;
	
	m_target->Setall(x, y, ang);
	

	m_delay = 1000.f;

	m_shapeTarget.setPosition(sf::Vector2f(m_target->getX(), m_target->getY()));
	m_shapeTarget.setRotation(-degre(m_target->getangle()) - 90);
}