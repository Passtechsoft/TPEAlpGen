#include "../Interface/utilities.h"

class Zone
{
public:
	Zone();
private:

}; 
/**La zone, ou "biome", représente un polygone composé d'un nuage de points reliés permettant de
	*définir des parties de terrain où la génération s'effectuera selon des règles particulières
	*à la zone, stocke "l'image vectorielle" d'une zone du terrain.
	* /!\ Attention! Les vertices doivent être données dans un ordre précis, de sorte que l'on n'ait pas besoin
	*de donner d'index. Passez un tableau qui suit cette norme et vous gagnerez en temps et en patience:
	*- Rule 1: Commencez par le vertex le plus à gauche, reliez-le par le haut jusqu'au vertex le plus à droite,
	*vous pouvez désormais terminer avec les vertices du bas. Vous n'avez pas besoin de rajouter le 1er vertex.
    */
class ZoneBiome
{
public:
	ZoneBiome(ZoneSetup* setup, int pourcentDominutionExtInter=5);
	ZoneBiome(ZoneSetup* setup, vector<Point2D> pts, int pourcentDominutionExtInter=5);

	void AddPoints(const vector<Point2D>& pts, bool rm=true, bool finished=false);

	///Si ledit point est en contact, on renvoie la modif asociée à sa hauteur
	float ValeurZ(const Point2D &pt) const;

	///Construit la HitBox simplifiée du mesh
	bool BuildHitBox();

	//Setters/getters:
	///Permet de set m_pourcentDominutionExtInter, rebuild l'interieur
	void setDiminution(int diminutionExtInter){m_diminutionExtToInter=diminutionExtInter;};

	//Retro satanas!!!! :
	bool isAdditionnable=true;
	
private:
	//La configuration que suit cette zone:
	ZoneSetup* m_setup;
	float m_hitBox[4];//Décrit dans BuildHitBox()
	bool m_finished=false;//Si l'utilisateur a averti que le mesh est complet

	//Les points:
	vector<Point2D> m_points;

	//le nombre de métres qui séparent l'exterieur de la "bordure interieur"
	float m_diminutionExtToInter=10.;
};
///\todo Implementer EnContact
//Surcharge de l'opérateur == permettant de vérifier si un point est à l'intérieure d'une zone
bool operator==(const Point2D& p, const ZoneBiome& zone);    
