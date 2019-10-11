#include"stdafx.h"

#include "Simulation.h"
#include "constantes.h"


using namespace std;

/**

Simulation du comportement du robot pour la coupe de france de robotique avec AREM.

Autheur : Corentin COURTOT


/!\ j'ai utilisé un repère ayant pour origine le coin en haut à droite de la table lorsque vous lancez le programme, l'axe des x est vers la droite, l'axe des y vers le bas.

Notes explicatives sur le fonctionnement de l'algorithme génétique :
- L'algorithme discrétise le temps en plusieurs "tours" de durée modifiable, 150 ms ici. Toutes les 150 ms, la recherche est réinitialisée avec de nouvelles entrées.
entrées : position du robot, position des ennemis
sortie : vitesse doite et gauche des roues
- Les gênes considérés pour cet algorithme génétique sont les vitesses des roues droite et gauche. J'ai appelé "solution" une combinaison de vitesses à appliquer aux roues lors de la simulation.
L'état de ces gènes est stocké dans le tableau à 3 dimensions m_sol[i][j][k], où i désigne le numero de la solution, j le tour pour lequel les vitesses seront appliquées aux roues, et k vaut 0 s'il désigne la vitesse de  la roue droite, 0 s'il désigne la gauche.
Par exemple, la 3ème solution appliquera au tour 7 la vitesse m_sol[2][6][0] à la roue droite. Les solutions sont classées dans ce tableau selon leur score.
Lors de l'appel de genSol, une nouvelle solution sera créée dans m_sol[NB_SOL], et sera ensuite classée parmi les autres selon son score.

Petits éclaircissements sur les attributs de la classe robot :
- J'ai appelé "solution" tout couple de vitesses apliquées sur les roues droite et gauche au cours de la simulation.
L'état de l'espèce est stocké dans le tableau de dimension 3 m_sol[i][j][k].
i désigne le numero de la solution considérée, j le tour d'application de la vitesse considérée, et k vaut soit 0 soit 1 selon que l'on désigne la vitesse de la roue droite ou de la roue gauche.
Par exemple, la vitesse appliquée à la roue droite pour la 3ème solution au 5ème tour de la simulation sera m_sol[2][4][0].
Les solutions sont classées selon leur score de la meilleure à la pire. Une nouvelle solution est créée à l'indice m_sol[NB_SOL] puis est classée parmi les autres solutions ensuite.
- Il existe plusieurs attributs décrivant une position.
* m_pos désigne la position actuelle du robot.
* m_posDep désigne la position de départ à partir de laquelle on va rechercher une trajectoire opptimale.
C'est la position que le robot va occuper lorsque le temps imparti pour la recherche sera écoulé. De cette manière, toutes les solutions partiront de la même position.
Cette position est déterminée à chaque appel de StartGen à partir de la position actuelle et des vitesse qui vont être appliquées au cours des 150 prochaines ms.
* m_posTest est utilisée pour tester une solution : elle est initialisée à m_posDep avant la simulation, et on l'actualise à chaque tour.

Améliorations possibles :
- Actuellement, lorsqu'une solution devient la meilleure solution, elle est très souvent choisie comme solution de base à partir de laquelle faire des mutations pour générer des nouvelles solutions, donc m_sol est rapidement rempli de solutions très proches les unes des autres
Ceci est problématique car cela réduit les chances de s'échapper d'un minimum local, et cela est visible lorsque l'on place un robot ennemi à une position bloquant le chemin direct, tout en laissant d'autres opportunités de passage :
le robot ne trouve pas le deuxieme chemin et prefere souvent rester sur place plutot que de contourner le robot adverse. Cette diversité des solutions s'apparente à la diversité génétique d'une espèce.
Pour pallier à ce problème, j'ai voulu avec l'aide de Guillaume Chauvon instaurer une idée de "distance" entre les solutions : deux solutions trop similaires ne peuvent pas cohabiter dans m_sol. La meilleure des 2 solutions serait alors choisie, la 2eme retiree du tableau.
Les fonctions distSolTest et testDist() avaient vocation à concrétiser cette idée, je n'ai néanmoins pas eu le temps de les coder. J'ai laissé les ébauches, si ca peut etre utile à qqu.
- Accessoirement, mettre m_sol sous forme de pointeur pourrait être une optimisation pertinente, surtout si NB_SOL est grand (voir Robot.cpp, l.487).

**/

//sf::RenderWindow window;


int main()
{
	Simulation simulation;

	simulation.run();

	return 0;
}

