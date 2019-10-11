#include"stdafx.h"

#include "Simulation.h"
#include "constantes.h"


using namespace std;

/**

Simulation du comportement du robot pour la coupe de france de robotique avec AREM.

Autheur : Corentin COURTOT


/!\ j'ai utilis� un rep�re ayant pour origine le coin en haut � droite de la table lorsque vous lancez le programme, l'axe des x est vers la droite, l'axe des y vers le bas.

Notes explicatives sur le fonctionnement de l'algorithme g�n�tique :
- L'algorithme discr�tise le temps en plusieurs "tours" de dur�e modifiable, 150 ms ici. Toutes les 150 ms, la recherche est r�initialis�e avec de nouvelles entr�es.
entr�es : position du robot, position des ennemis
sortie : vitesse doite et gauche des roues
- Les g�nes consid�r�s pour cet algorithme g�n�tique sont les vitesses des roues droite et gauche. J'ai appel� "solution" une combinaison de vitesses � appliquer aux roues lors de la simulation.
L'�tat de ces g�nes est stock� dans le tableau � 3 dimensions m_sol[i][j][k], o� i d�signe le numero de la solution, j le tour pour lequel les vitesses seront appliqu�es aux roues, et k vaut 0 s'il d�signe la vitesse de  la roue droite, 0 s'il d�signe la gauche.
Par exemple, la 3�me solution appliquera au tour 7 la vitesse m_sol[2][6][0] � la roue droite. Les solutions sont class�es dans ce tableau selon leur score.
Lors de l'appel de genSol, une nouvelle solution sera cr��e dans m_sol[NB_SOL], et sera ensuite class�e parmi les autres selon son score.

Petits �claircissements sur les attributs de la classe robot :
- J'ai appel� "solution" tout couple de vitesses apliqu�es sur les roues droite et gauche au cours de la simulation.
L'�tat de l'esp�ce est stock� dans le tableau de dimension 3 m_sol[i][j][k].
i d�signe le numero de la solution consid�r�e, j le tour d'application de la vitesse consid�r�e, et k vaut soit 0 soit 1 selon que l'on d�signe la vitesse de la roue droite ou de la roue gauche.
Par exemple, la vitesse appliqu�e � la roue droite pour la 3�me solution au 5�me tour de la simulation sera m_sol[2][4][0].
Les solutions sont class�es selon leur score de la meilleure � la pire. Une nouvelle solution est cr��e � l'indice m_sol[NB_SOL] puis est class�e parmi les autres solutions ensuite.
- Il existe plusieurs attributs d�crivant une position.
* m_pos d�signe la position actuelle du robot.
* m_posDep d�signe la position de d�part � partir de laquelle on va rechercher une trajectoire opptimale.
C'est la position que le robot va occuper lorsque le temps imparti pour la recherche sera �coul�. De cette mani�re, toutes les solutions partiront de la m�me position.
Cette position est d�termin�e � chaque appel de StartGen � partir de la position actuelle et des vitesse qui vont �tre appliqu�es au cours des 150 prochaines ms.
* m_posTest est utilis�e pour tester une solution : elle est initialis�e � m_posDep avant la simulation, et on l'actualise � chaque tour.

Am�liorations possibles :
- Actuellement, lorsqu'une solution devient la meilleure solution, elle est tr�s souvent choisie comme solution de base � partir de laquelle faire des mutations pour g�n�rer des nouvelles solutions, donc m_sol est rapidement rempli de solutions tr�s proches les unes des autres
Ceci est probl�matique car cela r�duit les chances de s'�chapper d'un minimum local, et cela est visible lorsque l'on place un robot ennemi � une position bloquant le chemin direct, tout en laissant d'autres opportunit�s de passage :
le robot ne trouve pas le deuxieme chemin et prefere souvent rester sur place plutot que de contourner le robot adverse. Cette diversit� des solutions s'apparente � la diversit� g�n�tique d'une esp�ce.
Pour pallier � ce probl�me, j'ai voulu avec l'aide de Guillaume Chauvon instaurer une id�e de "distance" entre les solutions : deux solutions trop similaires ne peuvent pas cohabiter dans m_sol. La meilleure des 2 solutions serait alors choisie, la 2eme retiree du tableau.
Les fonctions distSolTest et testDist() avaient vocation � concr�tiser cette id�e, je n'ai n�anmoins pas eu le temps de les coder. J'ai laiss� les �bauches, si ca peut etre utile � qqu.
- Accessoirement, mettre m_sol sous forme de pointeur pourrait �tre une optimisation pertinente, surtout si NB_SOL est grand (voir Robot.cpp, l.487).

**/

//sf::RenderWindow window;


int main()
{
	Simulation simulation;

	simulation.run();

	return 0;
}

