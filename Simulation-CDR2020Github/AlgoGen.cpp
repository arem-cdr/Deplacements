#include"stdafx.h"

#include "AlgoGen.h"


AlgoGen::AlgoGen(const int nbRobots, const Point& pos, const sf::Vector2f robotSize, 
	const float maxAcceleration, const float maxDeceleration, const float maxSpeed, const Terrain* terrain)
	: Pathfinding(robotSize),
	m_posTest(pos), m_posDep(pos), m_posOther(nbRobots, nbToursSimules), 
	m_clockSearchLeft(), m_timeSearchLeft(),
	m_nbRobots(nbRobots), 
	m_maxSpeed(maxSpeed), m_maxAcceleration(maxAcceleration), m_maxDeceleration(maxDeceleration),
	m_terrain(terrain)
{
	m_clockSearchLeft.restart();
	m_timeSearchLeft = sf::Time(sf::milliseconds(0));

	m_hitbox.setSize(sf::Vector2f(robotSize.x, robotSize.y));
	m_hitbox.setOrigin(sf::Vector2f(robotSize.x / 2.f, robotSize.y / 2.f));
	m_hitbox.setPosition(sf::Vector2f(m_posTest.getX(), m_posTest.getY()));
	m_hitbox.setRotation(-degre(m_posTest.getangle()) - 90);

	m_collision = false;
	m_malus = 0;
	m_nbMutations = 0;
	m_nbMutationsMax = 30000;

	currentScore = 0;

	for (int i = 0; i < NB_SOL + 1; i++)
	{
		for (int j = 0; j < nbToursSimules + 1; j++)
		{
			for (int k = 0; k < 2; ++k)
			{
				m_sol[i][j][k] = 0;
			}
		}
	}
}

AlgoGen::~AlgoGen()
{
	
}

void AlgoGen::init(const std::vector <Point*>& posOtherRobots, const Point& actualPos, const Point& target, const float& rightSpeed, const float& leftSpeed)
{
	m_malus = 0;
	m_posTest.Setall(actualPos.getX(), actualPos.getY(), actualPos.getangle());
	currentScore = evalueSol(target);

	m_posOther.updatePosOtherRobots(posOtherRobots, actualPos);
	startGen(posOtherRobots, actualPos, target, rightSpeed, leftSpeed);
}

void AlgoGen::run(float timeAvailable, float& rightSpeed, float& leftSpeed, const Point& actual_pos, const Point& target, const std::vector <Point*>& posOtherRobots)
{
	sf::Clock timeLeft;
	timeLeft.restart();

	while (timeLeft.getElapsedTime().asMilliseconds() < timeAvailable)
	{
		if (m_timeSearchLeft.asMilliseconds() > 0)
		{
			for (int i = 0; i < 20; ++i)
			{
				genSol(rightSpeed, leftSpeed, target);
			}

			m_timeSearchLeft = sf::Time(sf::milliseconds(temps_gen)) - m_clockSearchLeft.getElapsedTime();
		}
		else
		{
			end(rightSpeed, leftSpeed); // Application du resultat trouvé
			init(posOtherRobots, actual_pos, target, rightSpeed, leftSpeed); // Preparation a la prochaine recherche

			m_clockSearchLeft.restart();
			m_timeSearchLeft = sf::Time(sf::milliseconds(temps_gen));
		}
	}
}

void AlgoGen::end(float& rightSpeed, float& leftSpeed)
{
	endGen(rightSpeed, leftSpeed);

	m_posTest.Setall(m_posDep.getX(), m_posDep.getY(), m_posDep.getangle());

	for (int i = 0; i < nbToursSimules; i++) // affichage du robot vert
	{
		actualisePositionTest(m_sol[0][i][0] * TEMPS_GEN / 17, m_sol[0][i][1] * TEMPS_GEN / 17, i);
	}

	m_shapeDebug.setPosition(m_posTest.getX(), m_posTest.getY());
	m_shapeDebug.setRotation(-degre(m_posTest.getangle()) - 90);
}

void AlgoGen::render(sf::RenderTarget& target)
{
	//target.draw(m_shapeDebug);
	return void();
}

void AlgoGen::startGen(const std::vector <Point*>& robot_pos, const Point& actualPos, const Point& target, const float& rightSpeed, const float& leftSpeed) // setup tout avant de lancer la recherche de trajectoires
{
	m_posTest.Setall(actualPos.getX(), actualPos.getY(), actualPos.getangle());

	//initialise les variables, prepare aux mutations

	m_posTest.Setall(actualPos.getX(), actualPos.getY(), actualPos.getangle());
	actualisePositionTest(rightSpeed * TEMPS_GEN * 60 / 1000, leftSpeed * TEMPS_GEN * 60 / 1000, -1);
	m_posDep.Setall(m_posTest.getX(), m_posTest.getY(), m_posTest.getangle());

	//remise a 0 des scores des sol
	for (int i = 0; i < NB_SOL; i++)
	{
		m_sol[i][nbToursSimules][0] = 0;
		m_sol[i][nbToursSimules][1] = 0;
	}

	m_nbMutations = 0;

	//creation de la population de depart

	//decalage de la meilleure ancienne sol d'une unite de temps
	for (int j = 0; j < nbToursSimules - 1; j++)
	{
		m_sol[0][j][0] = m_sol[0][j + 1][0];
		m_sol[0][j][1] = m_sol[0][j + 1][1];
	}
	m_sol[0][nbToursSimules][0] = 0;
	m_sol[0][nbToursSimules][1] = 0;


	for (int i = 0; i < nbToursSimules; i++) //tourne sur place a droite
	{
		m_sol[1][i][0] = -0.5f;
		m_sol[1][i][1] = 0.5f;
	}

	for (int i = 0; i < nbToursSimules; i++) //reste sur place
	{
		m_sol[2][i][0] = 0;
		m_sol[2][i][1] = 0;
	}

	for (int i = 0; i < nbToursSimules; i++) //tourne sur place a droite
	{
		m_sol[3][i][0] = 0.5f;
		m_sol[3][i][1] = -0.5f;
	}

	for (int i = 0; i < nbToursSimules; i++) //avance tout droit
	{
		m_sol[4][i][0] = 2;
		m_sol[4][i][1] = 2;
	}

	if (NB_SOL > 5)
	{
		for (int i = 5; i < NB_SOL; i++) //les autres sol sont mises à 0, on peut ajouter d'autres mecanismes de generation de sol initiales si besoin
		{
			for (int j = 0; j < nbToursSimules + 1; j++) //avance tout droit
			{
				m_sol[i][j][0] = 0;
				m_sol[i][j][1] = 0;
			}
		}
	}

	//rends les sol generees conformes
	for (int j = 0; j < NB_SOL; j++)
	{
		if (m_sol[j][0][0] > m_maxSpeed)
			m_sol[j][0][0] = m_maxSpeed;
		if (m_sol[j][0][1] > m_maxSpeed)
			m_sol[j][0][1] = m_maxSpeed;
		if (m_sol[j][0][0] < -m_maxSpeed)
			m_sol[j][0][0] = -m_maxSpeed;
		if (m_sol[j][0][1] < -m_maxSpeed)
			m_sol[j][0][1] = -m_maxSpeed;

		if (m_sol[j][0][0] >= 0)
		{
			if (m_sol[j][0][0] > rightSpeed + m_maxAcceleration)
				m_sol[j][0][0] = rightSpeed + m_maxAcceleration;
			if (m_sol[j][0][0] < rightSpeed - m_maxDeceleration)
				m_sol[j][0][0] = rightSpeed - m_maxDeceleration;
			if (m_sol[j][0][1] > leftSpeed + m_maxAcceleration)
				m_sol[j][0][1] = leftSpeed + m_maxAcceleration;
			if (m_sol[j][0][1] < leftSpeed - m_maxDeceleration)
				m_sol[j][0][1] = leftSpeed - m_maxDeceleration;
		}
		else
		{
			if (m_sol[j][0][0] < rightSpeed - m_maxAcceleration)
				m_sol[j][0][0] = rightSpeed - m_maxAcceleration;
			if (m_sol[j][0][0] > rightSpeed + m_maxDeceleration)
				m_sol[j][0][0] = rightSpeed + m_maxDeceleration;
			if (m_sol[j][0][1] < leftSpeed - m_maxAcceleration)
				m_sol[j][0][1] = leftSpeed - m_maxAcceleration;
			if (m_sol[j][0][1] > leftSpeed + m_maxDeceleration)
				m_sol[j][0][1] = leftSpeed + m_maxDeceleration;
		}

		for (int i = 1; i < nbToursSimules; i++)
		{
			if (m_sol[j][i][0] > m_maxSpeed)
				m_sol[j][i][0] = m_maxSpeed;
			if (m_sol[j][i][1] > m_maxSpeed)
				m_sol[j][i][1] = m_maxSpeed;
			if (m_sol[j][i][0] < -m_maxSpeed)
				m_sol[j][i][0] = -m_maxSpeed;
			if (m_sol[j][i][1] < -m_maxSpeed)
				m_sol[j][i][1] = -m_maxSpeed;

			if (m_sol[j][i][0] >= 0)
			{
				if (m_sol[j][i][0] > m_sol[j - 1][i][0] + m_maxAcceleration)
					m_sol[j][i][0] = m_sol[j - 1][i][0] + m_maxAcceleration;
				if (m_sol[j][i][0] < m_sol[j - 1][i][0] - m_maxDeceleration)
					m_sol[j][i][0] = m_sol[j - 1][i][0] - m_maxDeceleration;
				if (m_sol[j][i][1] > m_sol[j - 1][i][1] + m_maxAcceleration)
					m_sol[j][i][1] = m_sol[j - 1][i][1] + m_maxAcceleration;
				if (m_sol[j][i][1] < m_sol[j - 1][i][1] - m_maxDeceleration)
					m_sol[j][i][1] = m_sol[j - 1][i][1] - m_maxDeceleration;
			}
			else
			{
				if (m_sol[j][i][0] < m_sol[j - 1][i][0] - m_maxAcceleration)
					m_sol[j][i][0] = m_sol[j - 1][i][0] - m_maxAcceleration;
				if (m_sol[j][i][0] > m_sol[j - 1][i][0] + m_maxDeceleration)
					m_sol[j][i][0] = m_sol[j - 1][i][0] + m_maxDeceleration;
				if (m_sol[j][i][1] < m_sol[j - 1][i][1] - m_maxAcceleration)
					m_sol[j][i][1] = m_sol[j - 1][i][1] - m_maxAcceleration;
				if (m_sol[j][i][1] > m_sol[j - 1][i][1] + m_maxDeceleration)
					m_sol[j][i][1] = m_sol[j - 1][i][1] + m_maxDeceleration;
			}
		}
	}

	//teste les solutions de depart et les classe

	//simulation des sol de depart
	for (int j = 0; j < NB_SOL; j++)
	{
		m_malus = 0;
		m_collision = false;

		m_posTest.Setall(m_posDep.getX(), m_posDep.getY(), m_posDep.getangle());

		//simulation des tours

		for (int i = 0; i < nbToursSimules; i++)
		{
			actualisePositionTest(m_sol[j][i][0] * TEMPS_GEN * 60 / 1000, m_sol[j][i][1] * TEMPS_GEN * 60 / 1000, i);
			m_sol[j][nbToursSimules][0] += evalueSol(target) / nbToursSimules; // on fait la moyenne du score de toutes les positions par lesquelle est passe le robot
		}

		if (evalueSol(target) < 900.f) // si le robot est encore loin de la target, le score de la solution est finalement seulement la position finale
		{
			m_sol[j][nbToursSimules][0] = evalueSol(target) - 500;
		}

		if (m_collision)
		{
			m_sol[j][nbToursSimules][0] = -10000.f; // on met les sol de depart avec un score le plus bas possible
			if (j == 2)
				m_sol[j][nbToursSimules][0] = -9000.f; // sauf la sol "reste sur place", afin qu'elle soit preferee en cas de collision
		}

		//distSolTest(j); tests
	}

	//triage des sol
	for (int i = 0; i < NB_SOL; i++)
		swapSol(chercheMeilleure(i), i);
}

void AlgoGen::endGen(float& rightSpeed, float& leftSpeed)
{
	// mise à jour des vitesse des roues du robot
	m_collision = false;
	m_posTest.Setall(m_posDep.getX(), m_posDep.getY(), m_posDep.getangle());

	actualisePositionTest(m_sol[0][0][0] * TEMPS_GEN / 17, m_sol[0][0][1] * TEMPS_GEN / 17, 0);

	/*if (!m_collision)
	{
		rightSpeed = m_sol[0][0][0];
		leftSpeed = m_sol[0][0][1];
	}
	else
	{
		rightSpeed = 0;
		leftSpeed = 0;
	}*/

	m_nbMutationsMax = m_nbMutations;

	rightSpeed = m_sol[0][0][0];
	leftSpeed = m_sol[0][0][1];
}

float AlgoGen::evalueSol(const Point& target) const // fonction cruciale : c'est grace a la note retournee par cette fonction que l'on va evaluer les performances d'une solution
{
	float d = dist(target.getX() - m_posTest.getX(), target.getY() - m_posTest.getY()); // la distance entre la pos actuelle du robot et sa target
	float a = 512;
	if (d < 5) // on considere a seulement si le robot est suffisamment proche de la cible
	{
		a = val_abs(properAngleRad(target.getangle() - m_posTest.getangle())) * 160.f; // la difference d'angle entre la pos actuelle du robot et sa target
	}
	if (d < 0 || a < 0 || m_malus < 0)
	{ 
		std::cout << "Major Error in evalueSol" << std::endl;
	}

	return 1000 - d - a - m_malus; //la note max est de 1000. m_malus augmente en fonction des obstacles rencontres par le robot
}

void AlgoGen::swapSol(const int sol1, const int sol2)
{
	//echange les positions de sol1 et sol2. Optimisable en le faisant avec des pointeurs. Cette optimisation est d'autant pluys nécessaire que NB_SOL est grand
	if (sol1 != sol2)
	{
		//copie de m_sol[sol1] dans un tableau temporaire
		float solTemp[nbToursSimules + 1][2];

		for (int i = 0; i < nbToursSimules + 1; i++)
		{
			solTemp[i][0] = m_sol[sol1][i][0];
			solTemp[i][1] = m_sol[sol1][i][1];
		}

		//copie de m_sol[sol2] dans m_sol[sol1]
		for (int i = 0; i < nbToursSimules + 1; i++)
		{
			m_sol[sol1][i][0] = m_sol[sol2][i][0];
			m_sol[sol1][i][1] = m_sol[sol2][i][1];
		}

		//copie du tableau temporaire dans m_sol[sol2]
		for (int i = 0; i < nbToursSimules + 1; i++)
		{
			m_sol[sol2][i][0] = solTemp[i][0];
			m_sol[sol2][i][1] = solTemp[i][1];
		}
	}
}

int AlgoGen::chercheMeilleure(int indiceDepart)
{
	//cherche la meilleure sol parmi celles apres l'indice de depart dans m_sol
	int a = indiceDepart;

	for (int i = indiceDepart + 1; i < NB_SOL; i++)
	{
		if (m_sol[a][nbToursSimules][0] < m_sol[i][nbToursSimules][0])
			a = i;
	}
	return a;
}

void AlgoGen::genSol(const float& rightSpeed, const float& leftSpeed, const Point& target)
{
	// Cette fonction genere une nouvelle solution et la teste

	// generation d'une nouvelle solution
	{
		float a = (float)rand() / RAND_MAX * 10.f;
		int ind1 = (int)floor(a * a / 100.f * NB_SOL); // genere un entier entre 0 et NB_SOL-1 inclus, avec une proba plus grande pour les petits entiers
		a = (float)rand() / RAND_MAX * 10.f;
		int ind2 = (int)floor(a * a / 100.f * NB_SOL);
		int indCr = (int)floor(rand() / RAND_MAX * (nbToursSimules - 1) + 1);

		crossOver(ind1, ind2, 0);
		mutate();
		while ((float)rand() / RAND_MAX < 0.3f)
			mutate();

		m_nbMutations++;

		//teste si la sol generee est conforme
		{
			if (m_sol[NB_SOL][0][0] > m_maxSpeed)
				m_sol[NB_SOL][0][0] = m_maxSpeed;
			if (m_sol[NB_SOL][0][1] > m_maxSpeed)
				m_sol[NB_SOL][0][1] = m_maxSpeed;
			if (m_sol[NB_SOL][0][0] < -m_maxSpeed)
				m_sol[NB_SOL][0][0] = -m_maxSpeed;
			if (m_sol[NB_SOL][0][1] < -m_maxSpeed)
				m_sol[NB_SOL][0][1] = -m_maxSpeed;

			if (m_sol[NB_SOL][0][0] >= 0)
			{
				if (m_sol[NB_SOL][0][0] > rightSpeed + m_maxAcceleration)
					m_sol[NB_SOL][0][0] = rightSpeed + m_maxAcceleration;
				if (m_sol[NB_SOL][0][0] < rightSpeed - m_maxDeceleration)
					m_sol[NB_SOL][0][0] = rightSpeed - m_maxDeceleration;
				if (m_sol[NB_SOL][0][1] > leftSpeed + m_maxAcceleration)
					m_sol[NB_SOL][0][1] = leftSpeed + m_maxAcceleration;
				if (m_sol[NB_SOL][0][1] < leftSpeed - m_maxDeceleration)
					m_sol[NB_SOL][0][1] = leftSpeed - m_maxDeceleration;
			}
			else
			{
				if (m_sol[NB_SOL][0][0] < rightSpeed - m_maxAcceleration)
					m_sol[NB_SOL][0][0] = rightSpeed - m_maxAcceleration;
				if (m_sol[NB_SOL][0][0] > rightSpeed + m_maxDeceleration)
					m_sol[NB_SOL][0][0] = rightSpeed + m_maxDeceleration;
				if (m_sol[NB_SOL][0][1] < leftSpeed - m_maxAcceleration)
					m_sol[NB_SOL][0][1] = leftSpeed - m_maxAcceleration;
				if (m_sol[NB_SOL][0][1] > leftSpeed + m_maxDeceleration)
					m_sol[NB_SOL][0][1] = leftSpeed + m_maxDeceleration;
			}

			for (int i = 1; i < nbToursSimules; i++)
			{
				if (m_sol[NB_SOL][i][0] > m_maxSpeed)
					m_sol[NB_SOL][i][0] = m_maxSpeed;
				if (m_sol[NB_SOL][i][1] > m_maxSpeed)
					m_sol[NB_SOL][i][1] = m_maxSpeed;
				if (m_sol[NB_SOL][i][0] < -m_maxSpeed)
					m_sol[NB_SOL][i][0] = -m_maxSpeed;
				if (m_sol[NB_SOL][i][1] < -m_maxSpeed)
					m_sol[NB_SOL][i][1] = -m_maxSpeed;

				if (m_sol[NB_SOL][i][0] >= 0)
				{
					if (m_sol[NB_SOL][i][0] > m_sol[NB_SOL][i - 1][0] + m_maxAcceleration)
						m_sol[NB_SOL][i][0] = m_sol[NB_SOL][i - 1][0] + m_maxAcceleration;
					if (m_sol[NB_SOL][i][0] < m_sol[NB_SOL][i - 1][0] - m_maxDeceleration)
						m_sol[NB_SOL][i][0] = m_sol[NB_SOL][i - 1][0] - m_maxDeceleration;
					if (m_sol[NB_SOL][i][1] > m_sol[NB_SOL][i - 1][1] + m_maxAcceleration)
						m_sol[NB_SOL][i][1] = m_sol[NB_SOL][i - 1][1] + m_maxAcceleration;
					if (m_sol[NB_SOL][i][1] < m_sol[NB_SOL][i - 1][1] - m_maxDeceleration)
						m_sol[NB_SOL][i][1] = m_sol[NB_SOL][i - 1][1] - m_maxDeceleration;
				}
				else
				{
					if (m_sol[NB_SOL][i][0] < m_sol[NB_SOL][i - 1][0] - m_maxAcceleration)
						m_sol[NB_SOL][i][0] = m_sol[NB_SOL][i - 1][0] - m_maxAcceleration;
					if (m_sol[NB_SOL][i][0] > m_sol[NB_SOL][i - 1][0] + m_maxDeceleration)
						m_sol[NB_SOL][i][0] = m_sol[NB_SOL][i - 1][0] + m_maxDeceleration;
					if (m_sol[NB_SOL][i][1] < m_sol[NB_SOL][i - 1][1] - m_maxAcceleration)
						m_sol[NB_SOL][i][1] = m_sol[NB_SOL][i - 1][1] - m_maxAcceleration;
					if (m_sol[NB_SOL][i][1] > m_sol[NB_SOL][i - 1][1] + m_maxDeceleration)
						m_sol[NB_SOL][i][1] = m_sol[NB_SOL][i - 1][1] + m_maxDeceleration;
				}
			}
		}
	}

	//test et evaluation de la nouvelle solution 
	{
		m_malus = 0;
		m_collision = false;

		m_posTest.Setall(m_posDep.getX(), m_posDep.getY(), m_posDep.getangle());

		//simulation des tours
		m_sol[NB_SOL][nbToursSimules][0] = 0;

		for (int i = 0; i < nbToursSimules; i++)
		{
			//if (m_sol[NB_SOL][i][0] < -2.f && m_sol[NB_SOL][i][1] < -2.f) // malus si marche arriere
			//	m_malus += abs(m_sol[NB_SOL][i][0] + m_sol[NB_SOL][i][1]) * 1.f;
			if (m_sol[NB_SOL][i][0] * m_sol[NB_SOL][i][1] < -5.f) // malus si tourne sur lui meme
				m_malus += abs(m_sol[NB_SOL][i][0] - m_sol[NB_SOL][i][1]) * 0.5f;

			actualisePositionTest(m_sol[NB_SOL][i][0] * TEMPS_GEN * 60 / 1000, m_sol[NB_SOL][i][1] * TEMPS_GEN * 60 / 1000, i);

			if (m_collision) // si une collision s'est produite, on ne prend pas en compte la solution generee
			{
				return;
			}

			m_sol[NB_SOL][nbToursSimules][0] += evalueSol(target) / nbToursSimules;
		}


		if (evalueSol(target) < 400) // si la sol trouvee est eloignee de la target, on ne prend en compte que la sol finale
			m_sol[NB_SOL][nbToursSimules][0] = evalueSol(target) - 500;
	}

	//distSolTest(NB_SOL); //ne sert a rien, amelioration potentielle

	//on place la nouvelle solution parmi les solutions existantes par une recherche dicotomique
	{
		int classement = NB_SOL;
		int dico1 = 0;
		int dico2 = NB_SOL;

		if (m_sol[NB_SOL][nbToursSimules][0] > m_sol[NB_SOL - 1][nbToursSimules][0]) // petite optimisation : souvent la sol generee sera trop mauvaise pour etre gardee
		{
			while (dico2 - dico1 > 1)
			{
				if (m_sol[NB_SOL][nbToursSimules][0] > m_sol[(int)((dico1 + dico2) * 0.5)][nbToursSimules][0])
				{
					dico2 = (int)((dico1 + dico2) * 0.5f);
				}
				else
				{
					dico1 = (int)((dico1 + dico2) * 0.5f);
				}
			}
			if (m_sol[NB_SOL][nbToursSimules][0] > m_sol[dico1][nbToursSimules][0])
			{
				classement = dico1;
			}
			else
			{
				classement = dico2;
			}
		}

		//verification de la distance de la sol generee par rapport aux autres sol. /!\ PAS FONCTIONNEL
		//int d = NB_SOL; //vaut NB_SOL si aucune des autre sol n'est proche de celle generee
		//if (classement < NB_SOL)
		//{
		//	d = testDist();
		//}

		//on reconstitue alors le tableau des solutions classé avec la nouvelle solution
		while (/*classement<=d && */classement != NB_SOL)
		{
			swapSol(classement, NB_SOL);
			classement++;
		}
	}
}

void AlgoGen::crossOver(int ind1, int ind2, int a)
{
	//mixe les sol ind1 et ind2 pour en creer une nouvelle

	for (int i = 0; i < a; i++)
	{
		m_sol[NB_SOL][i][0] = m_sol[ind1][i][0];
		m_sol[NB_SOL][i][1] = m_sol[ind1][i][1];
	}

	for (int i = a; i < nbToursSimules; i++)
	{
		m_sol[NB_SOL][i][0] = m_sol[ind2][i][0];
		m_sol[NB_SOL][i][1] = m_sol[ind2][i][1];
	}
}

void AlgoGen::mutate()
{
	float r = (float)rand() / RAND_MAX;
	int a = (int)exp((float)rand() * log(nbToursSimules + 1) / RAND_MAX) - 1; //genere un entier entre 0 et nbToursSimules

	if (r < 0.25)
		brake(a);
	else if (r < 0.5)
		accel(a);
	else if (r < 0.75)
		turnLeft(a);
	else if (r < 1) // inutile dans ce cas
		turnRight(a);
}

void AlgoGen::brake(const int ind)
{
	for (int i = ind; i < nbToursSimules; i++)
	{
		m_sol[NB_SOL][i][0] -= 1 * (1 - (float)m_nbMutations / 35000);
		m_sol[NB_SOL][i][1] -= 1 * (1 - (float)m_nbMutations / 35000);
	}
}

void AlgoGen::accel(const int ind)
{
	for (int i = ind; i < nbToursSimules; i++)
	{
		m_sol[NB_SOL][i][0] += 1 * (1 - (float)m_nbMutations / 35000);
		m_sol[NB_SOL][i][1] += 1 * (1 - (float)m_nbMutations / 35000);
	}
}

void AlgoGen::turnRight(const int ind)
{
	for (int i = ind; i < nbToursSimules; i++)
	{
		m_sol[NB_SOL][i][0] += 1 * (1 - (float)m_nbMutations / 35000);
		m_sol[NB_SOL][i][1] -= 1 * (1 - (float)m_nbMutations / 35000);
	}
}

void AlgoGen::turnLeft(const int ind)
{
	for (int i = ind; i < nbToursSimules; i++)
	{
		m_sol[NB_SOL][i][0] -= 1 * (1 - (float)m_nbMutations / 35000);
		m_sol[NB_SOL][i][1] += 1 * (1 - (float)m_nbMutations / 35000);
	}
}

void AlgoGen::actualisePositionTest(const float& rightSpeed, const float& leftSpeed, int indTour)
{
	// determination du cercle décrit par la trajectoire et de la vitesse du robot sur ce cercle
	if (rightSpeed != leftSpeed) {

		float R = 0; // rayon du cercle decrit par la trajectoire
		float d = 0; // vitesse du robot
		float cx = 0; // position du centre du cercle decrit par la trajectoire
		float cy = 0;

		R = ECART_ROUE / 2 * (rightSpeed + leftSpeed) / (leftSpeed - rightSpeed); // rayon du cercle
		cx = m_posTest.getX() + R * sin(m_posTest.getangle());
		cy = m_posTest.getY() + R * cos(m_posTest.getangle());
		d = (leftSpeed + rightSpeed) * 0.5f;

		// mise à jour des coordonnées du robot
		if (leftSpeed + rightSpeed != 0) {
			m_posTest.Setangle(m_posTest.getangle() - d / R);//m_angle -= d/R;
		}
		else {
			m_posTest.Setangle(m_posTest.getangle() + rightSpeed * 2.f / ECART_ROUE); //m_angle += rightSpeed*2.0 / ECART_ROUE;
		}

		if (m_posTest.getangle() > PI)
		{
			m_posTest.Setangle(m_posTest.getangle() - 2.f * PI); //m_angle -= 2*PI;
		}
		else if (m_posTest.getangle() <= -PI)
		{
			m_posTest.Setangle(m_posTest.getangle() + 2.f * PI); //m_sangle += 2*PI;
		}

		m_posTest.setX(cx - R * sin(m_posTest.getangle())); //m_xPos = cx - R * sin(m_angle);
		m_posTest.setY(cy - R * cos(m_posTest.getangle())); //m_yPos = cy - R * cos(m_angle);
	}
	else if (leftSpeed == rightSpeed) { // cas où la trajectoire est une parfaite ligne droite
		m_posTest.setX(m_posTest.getX() + leftSpeed * cos(m_posTest.getangle())); //m_xPos += leftSpeed * cos(m_angle);
		m_posTest.setY(m_posTest.getY() - rightSpeed * sin(m_posTest.getangle())); //m_yPos -= rightSpeed * sin(m_angle);
	}
	
	m_hitbox.setPosition(sf::Vector2f(m_posTest.getX(), m_posTest.getY()));
	m_hitbox.setRotation(-degre(m_posTest.getangle()) - 90);

	m_collision = m_terrain->collision(m_hitbox);

	if (indTour >= 0) // i = -1 dans startGen, on ne veut pas checker les collisions
	{
		testCollisionEn(indTour);
	}
}

void AlgoGen::testCollisionEn(int indTour) // à revoir pour utiliser les collisions de sprite
{
	m_posOther.testCollisionOtherRobots(m_posTest, indTour, m_malus, m_collision, val_abs(m_sol[NB_SOL][indTour][0] + m_sol[NB_SOL][indTour][0]), m_robotSize);
}

void AlgoGen::score(int indSol)
{
	m_sol[indSol][nbToursSimules][0] = (m_sol[indSol][nbToursSimules][0] * 0 + m_sol[indSol][nbToursSimules][1] * 2) / 2;
}

void AlgoGen::distSolTest(int indSol)
{
	m_sol[indSol][nbToursSimules][1] = 0;

	for (int i = 0; i < nbToursSimules; i++)
	{
		m_sol[indSol][nbToursSimules][1] += val_abs(m_sol[indSol][i][0] - m_sol[indSol][i][1]);
	}

	m_sol[indSol][nbToursSimules][1] *= sgn(m_sol[indSol][0][0] - m_sol[indSol][0][1] + m_sol[indSol][1][0] - m_sol[indSol][1][1] + m_sol[indSol][2][0] - m_sol[indSol][2][1]);
}

int AlgoGen::testDist()
{
	for (int i = 0; i < nbToursSimules; i++)
	{
		if (val_abs(m_sol[i][nbToursSimules][1] - m_sol[NB_SOL][nbToursSimules][1]) < 4)
		{
			return i;
		}
	}
	return -1;
}

bool AlgoGen::reachTargetTest(int i, Point& target)
{
	return dist(target - m_posTest) < 7 // proximite avec la target
		&& val_abs(properAngleRad(target.getangle() - m_posTest.getangle())) < 0.03 // angle tres proche
		&& val_abs(m_sol[NB_SOL][i][0]) < 0.3 // robot à l'arret
		&& val_abs(m_sol[NB_SOL][i][0]) < 0.3
		&& m_collision == false;
}

float AlgoGen::getBestScore() const
{
	return m_sol[NB_SOL][nbToursSimules][0];
}

std::string AlgoGen::stateString()
{
	return (std::string) "Score actuel : " + std::to_string((int) currentScore);
}


