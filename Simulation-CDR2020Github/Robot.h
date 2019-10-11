#ifndef ROBOT1_H
#define ROBOT1_H

#include"AlgoGen.h"
#include"Capteurs.h"
#include"Point.h"
#include"BasicFunctions.h"
#include"constantes.h"
#include"RenderObject.h"
#include"Terrain.h"

class robot : public RenderObject
{
private:
	//Point m_pos; // position du robot
	int id;

	Point* m_target;	// position visee
	float m_length;	// dimensions du rectangle représentant le robot
	float m_width;
	float m_leftSpeed; // vitesses des roues droite et gauche
	float m_rightSpeed;
	const float m_maxAcceleration;
	const float m_maxDeceleration;
	const float m_maxSpeed;

	sf::RectangleShape m_shapeTest;
	sf::RectangleShape m_shapeTarget;

	float m_delay;
	const team m_team;

	const Terrain* m_terrain;

	sf::Sprite m_sprite;
	sf::Texture m_texture;

	Pathfinding* m_IAPthfinding;
	std::vector <Capteurs*> capteurs;
	std::vector<Point*> m_posOtherRobot;

	// Functions
	void actualise_position(float rightSpeed, float leftSpeed);
	void actualise_positionTarget(float rightSpeed, float leftSpeed);
	void frottements(float dt);

	void retarget();
	bool reachTarget();
	void setTarget(const Point target);
	void deleteTarget();

	bool delay();

	void updatePosOtherRobots(const std::vector<Point>& posRobots);

public:
	robot(int nbRobots, team team, sf::Vector2f taille, int idRobot, const Terrain* terrain, int id);
	virtual ~robot();

	void update(const float dt);
	void updateClavier(float vitD, float vitG);
	void play(float time_available, const std::vector<Point>& posRobots);
	void render(sf::RenderTarget& target);

	std::string posString() const;
	std::string pathFindingString() const;
	std::string speedString() const;
};

#endif // ROBOT1_H
