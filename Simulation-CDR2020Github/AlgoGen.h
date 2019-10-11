#pragma once
#include"Pathfinding.h"
#include"BasicFunctions.h"
#include"PosOtherRobots.h"
#include"Terrain.h"

float properAngleRad(float ang);

class AlgoGen :
	public Pathfinding
{
private:
	sf::Clock m_clockSearchLeft;
	sf::Time m_timeSearchLeft; // temps restant disponible pour la recherche genetique

	float m_sol[NB_SOL + 1][nbToursSimules + 1][2]; //[n° de la sol (nb_sol+1 pour stocker la sol testee)]
														//[avancee dans le temps(nbToursSimules+1 pour stocker le score)][droite, gauche]
	sf::RectangleShape m_hitbox;

	//std::vector <sf::Vector2f*> m_posOther; // pos des autres robots
	const int m_nbRobots;

	PosOtherRobots m_posOther; // pos des autres robots
	bool m_collision; // = true si une collision a lieu lors de la simulation, faux sinon
	float m_malus; // malus utilisé pour noter les solutions generees
	int m_nbMutations; // utile pour des statistiques
	int m_nbMutationsMax;

	const float m_maxSpeed;
	const float m_maxAcceleration;
	const float m_maxDeceleration;

	Point m_posTest; // utilisée pour simuler l'avancee du robot virtuellement sur plusieurs tours
	Point m_posDep; // position de depart a partir de laquelle on va rechercher la trajectoire optimale. Cette position doit rester fixe pendant toute la duree de la recherche

	const Terrain* m_terrain;

	float currentScore; // utile pour debug

public:
	AlgoGen(const int nbRobots, const Point& pos, const sf::Vector2f robotSize,
		const float maxAcceleration, const float maxDeceleration, const float maxSpeed, const Terrain* terrain);
	virtual ~AlgoGen();

	virtual void init(const std::vector <Point*>& posOtherRobots, const Point& actualPos, const Point& target, const float& rightSpeed, const float& leftSpeed);
	virtual void run(float timeAvailable, float& rightSpeed, float& leftSpeed, const Point& actual_pos, const Point& target, const std::vector <Point*>& posOtherRobots = std::vector <Point*>());
	virtual void end(float& rightSpeed, float& leftSpeed);

	virtual void render(sf::RenderTarget& target);
	virtual std::string stateString();


	void startGen(const std::vector <Point*>& robot_pos, const Point& actualPos, const Point& target, const float& rightSpeed, const float& leftSpeed);
	void endGen(float& rightSpeed, float& leftSpeed);
	float evalueSol(const Point& target) const;
	void swapSol(const int sol1, const int sol2);
	int chercheMeilleure(int indiceDepart);
	void genSol(const float& rightSpeed, const float& leftSpeed, const Point& target);
	void crossOver(int ind1, int ind2, int a);
	void mutate();
	void brake(const int ind);
	void accel(const int ind);
	void turnRight(const int ind);
	void turnLeft(const int ind);
	//void retarget();

	void actualisePositionTest(const float& rightSpeed, const float& leftSpeed, int indTour);

	void testCollisionEn(int indTour);

	void score(int indSol);
	void distSolTest(int indSol);
	int testDist();
	bool reachTarget();
	bool reachTargetTest(int i, Point& target);

	float getBestScore() const;
};

