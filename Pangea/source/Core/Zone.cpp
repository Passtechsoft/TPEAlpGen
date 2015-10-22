#define HORS_ZONE 0
/* *********** struct ZoneBiome:
 */


ostream& operator<<(ostream& output, const Point2D& pt)
{
    output<<"("<<pt.x<<";"<<pt.y<<")";
    return output;
}


ZoneBiome::ZoneBiome(ZoneSetup* setup, int diminutionExtToInter)
		: m_setup(setup), m_diminutionExtToInter(diminutionExtToInter) {}

ZoneBiome::ZoneBiome(ZoneSetup* setup, vector<Point2D> pts, int diminutionExtToInter)
		: m_setup(setup), m_diminutionExtToInter(diminutionExtToInter)
{
	AddPoints(pts, true);
	if(!BuildHitBox())
		ERROR("1::: Impossible! fail du build de la box!");
}

///Cette fonction ajoute intelligement des points à la structure!
/**\param pts Le tableau de points qui sera ajouté à ou qui remplacera m_points
*\param rm Si on doit reset le tableau au lieu de simplement ajouter des vertices, doit valoir true
*\param reversed Indique si le tableau est déjà dans le bon sens ou si
*/
void ZoneBiome::AddPoints(const vector<Point2D>& pts, bool rm, bool finished)
{
	if(rm){
		m_points=pts;
		finished=true;
	}
	else{
		for(unsigned int c=0 : pts)
			m_points.push_back(pts[c]);
    }
	m_finished=finished;

	if(m_finished){
		if(!BuildHitBox())
			ERROR("2::: On a sauvagement bourré le tableau de vertice en le désignant comme complet alors qu'il contenait moins de deux vertices!");
	}
}

///Construit la HitBox simplifiée du mesh, normalement inutile pour l'utilisateur, \n
///car la fonction AddPoint s'occupe de l'appeler automatiquement (si le mesh est \n
///déclaré comme fini
bool ZoneBiome::BuildHitBox()
{
	if(m_points.size()<3)
		return WARNING("On a tenté d'appeler la fonction buildhitbox alors que le mesh n'était pas fini.");

	//On reset le tableau avec le preum's point
	m_hitBox[0]=m_points[0].x;
	m_hitBox[1]=m_points[0].x;
	m_hitBox[2]=m_points[0].y;
	m_hitBox[3]=m_points[0].y;

	/*  Ce for va créer laborieusement une boite organisée
	comme suit: index 0 = plus petit X, index 1 = plus grand X,
	index 2 = plus petit Y, index 3 = plus grand Y
	*/
	for(unsigned int c=0 : m_points)
	{
		if(m_points[c].x<m_hitBox[0])
			m_hitBox[0]=m_points[c].x;
		else if(m_points[c].x>m_hitBox[1])
			m_hitBox[1]=m_points[c].x;

		if(m_points[c].y<m_hitBox[2])
			m_hitBox[2]=m_points[c].y;
		else if(m_points[c].y>m_hitBox[3])
			m_hitBox[3]=m_points[c].y;
	}

	return EXIT_SUCCESS;
}

///La fonction déterminante de ZoneBiome, teste la collision de la zone avec un point.
///\todo rendre la fonction fonctionnelle
///\param pt Le point à tester
///\return Si le point pt est à l'intérieure du mesh 2D de la ZoneBiome
float ZoneBiome::ValeurZ(const Point2D &pt) const
{
	if(!m_finished){
		DEBUG("3::: Un ZoneBiome d'addresse "<<this<<" a été testé via sa methode EnContact\n alors que son finished valait false!");
		return EXIT_FAILURE;
	}

	unsigned int colisions=0;//Un compteur de collisions supposées avec le mesh

	//Test de collision simple sur le hitbox simplifiée de la zone
	if(pt.x < m_hitBox[0] || pt.x > m_hitBox[1] || pt.y < m_hitBox[2] || pt.y > m_hitBox[3])
		return HORS_ZONE;//Aucun modificateur, puisque le point n'est pas dans la zone

	unsigned int moyenneSegmentPrecedent[2]={INT_IMPROBABLE, INT_IMPROBABLE};
	unsigned int coteTrouve[2];//le numero représentant le point de départ dans le tableau de la fonction affine qui représente le point actuel
	float result;//On stocke la valeur théorique que doit prendre le point x pour être à l'interieur
	uchar i=1;//L'incrementation. Sera modifié quand on passera au point final, qui lui est au début du tableau (donc pas à la suite)
	for(unsigned int c=0 : m_points)
	{
		//si on en est au dernier point, on change l'incrementaton pour que c+i vale zero.
		if(c==m_points.size())
			i = -m_points.size();

		//Le bloc de code suivant cherche si le segment actuellement en test est plus proche du point que le précédent
		Point2D pointMoyenne((m_points.at(c).x + m_points.at(c+i).x)/2, (m_points.at(c).x + m_points.at(c+i).x)/2);
		//Si ce point est plus proche que le précédent à avoir remporté le palmares
		if( positif(pt.x-pointMoyenne.x)+positif(pt.y-pointMoyenne.y) <
		moyenneSegmentPrecedent[0]+moyenneSegmentPrecedent[1] )
		{
			coteTrouve[0]=c, coteTrouve[1]=c+i;//alors le point le plus proche (actuellement) est le point c
			moyenneSegmentPrecedent[0]=positif(pt.x-pointMoyenne.x);
			moyenneSegmentPrecedent[1]=positif(pt.y-pointMoyenne.y);
		}

		//Test basique qui va simplement vérifier si le point se trouve dans la bonne zone des X.
		//Si oui, on stocke dans result le y pour lequel ce point et le suivant forment une ligne
		//Et que pt doit couper pour etre à l'intérieure ou à l'extérieure de la zone
		if((m_points.at(c).x<m_points.at(c+i).x  &&  pt.x>m_points.at(c).x  &&  pt.x<m_points.at(c+i).x) ||
			(m_points.at(c).x>m_points.at(c+i).x  &&  pt.x>m_points.at(c+i).x  &&  pt.x<m_points.at(c).x))
		{
			/* Traduction: On imagine une fonction affine, coupée par la condition ci-dessus.
			*Son coeficient directeur, donnée par les deux points, donnent la droite qui relie les
			*deux points. Finalement, sachant que l'échelle sera reprise comme si la droite
			*commençait à l'origine, partant du premier point, on soustrait tout par la position
			*du premier point pour y ramener à l'echelle.
			*/
			result = (pt.x - m_points.at(c+i).y) * (m_points.at(c+i).y - m_points.at(c).y) /
                                                      (m_points.at(c+i).x - m_points.at(c).x);
			if(pt.y-m_points.at(c+i).y > result)
				colisions++;
		}
	}

	//Enfin, si tous les tests ont réussis, on fait l'interpolation des cotés et on retourne Z
	if(colisions==m_points.size()-1)
	{
		//ptTemporaire représente le point pt corrigé de l'origine des droites:
		Point2D ptTemporaire(pt.x-m_points.at(coteTrouve[0]).x, pt.y-m_points.at(coteTrouve[0]).y);

		//On résoud l'equation du segment proche pour retrouver son coeficient directeur, puis on trouve le coefficient directeur de la droite incidente
		float coefDirecteurFonctionSegmentProche = ((m_points.at(coteTrouve[1]).y-m_points.at(coteTrouve[0]).y) / (m_points.at(coteTrouve[1]).x-m_points.at(coteTrouve[0]).x));
		float coefDirecteurFonctionSegmentIncident = -1/coefDirecteurFonctionSegmentProche;
		//Cet algo, un peut plus complexe trouve les coordonnées XY du point le plus proche de la droite grace à la solution des équations de celles ci
		float posX = (-ptTemporaire.y-(coefDirecteurFonctionSegmentIncident*ptTemporaire.x)) /
                         (coefDirecteurFonctionSegmentIncident-coefDirecteurFonctionSegmentProche);
		float posY = posX*coefDirecteurFonctionSegmentProche;//On résoud tout simplement la fonction affine du segment proche
		//La distance entre les deux points!! enfin!!! (pythagore)
		float distanceBetPoints=sqrt(CARRE(positif(posX-pt.x))+CARRE(positif(posY-pt.y)));

		//Formule de l'interpoltion sinusoidale réussie: cos(((x+a)*3)/a)/2+.5
		//dernière ligne droite: le calcul de l'interpolation
		float coefficientInterpolation;
		if(distanceBetPoints>m_diminutionExtToInter)
		coefficientInterpolation=1;
		else//Das fuckin' interpolation
			coefficientInterpolation=(cos(((distanceBetPoints + m_diminutionExtToInter)*3.f)/m_diminutionExtToInter)/2.f+.5f);

		//Enfin, on fait le bruit de perlin:
		return Unoise::perlinNoise(pt.x, pt.y, m_setup->resolution) * m_setup->amplification * coefficientInterpolation;
	}
	return HORS_ZONE;
}
