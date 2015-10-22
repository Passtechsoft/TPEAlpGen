#include "utilities.h"
#include <math.h>
#include <cstdlib>


//La multimap stockera les classes par nom, puis s'en suit une map de pointeur sur types variables qui donnent sur le nombre d'allocations désalocs.
CompteurClasseType compteurClasses;
std::map<void *, int> *getCptClass(std::string &nom)
{
	return &compteurClasses[nom];
}

void affPtrFlag()
{
	typedef std::pair<CompteurClasseType::iterator, std::map<void *, int>::iterator > LaPaire;
	std::vector<LaPaire> v;
	for (CompteurClasseType::iterator it = compteurClasses.begin(); it != compteurClasses.end(); it++) {
		for(std::map<void *, int>::iterator ite = it->second.begin(); ite != it->second.end(); ite++) {
			if(ite->second != 0)
				v.push_back(LaPaire(it, ite));
		}
	}

	if (v.size() > 0)
		DEBUG("Attention! Les classes suivantes contiennent des fuites de mémoires!");

	for (unsigned int c = 0; c != v.size(); ++c)
		DEBUG("		L'instance " << v[c].second->first << " de la classe " << v[c].first->first << " possède " <<v[c].second->second <<" fuites mémoires.");
}

namespace Uchronia{class ZChunk;}

using namespace std;

bool probability(int prob, int sur)
{
    return RAND(0, sur)<=prob;
}

string getWord(ushort word_num, string &chaine)
{
    string word;
    int compteur=0;
    for(unsigned int c=0; c!=chaine.size(); ++c)
    {
        if(compteur==word_num)
            word.push_back(chaine[c]);
        else if(compteur>word_num)
            break;

        if(chaine[c]==' ')
            compteur++;
    }
    if(word.empty())
        WARNING("10::: le mot demandé (n°"<<word_num<<") est introuvable.");
    return word;
}

bool isInWord(std::string word, std::string subWord)
{
	unsigned int degresLectureSubWord=0;

	bool trouve=false;
	for(unsigned int c=0; c!=word.size() && !trouve; ++c)
	{
		if(word[c]==subWord[degresLectureSubWord])
			degresLectureSubWord++;
		else
			degresLectureSubWord=0;

		if(degresLectureSubWord == subWord.size())
			trouve=true;
	}
	return trouve;
}

std::vector<std::string> getFileWithSthx(std::ifstream &flux, unsigned int nbreMotsAttendus)
{
	std::string word;
	std::vector<std::string> instructions;
	flux>>word;

	if(!isInWord(word, ":"))
		instructions.push_back(word);

	flux>>word;
	unsigned int c=0;
	for(;word!=";" && flux; ++c)
	{
		instructions.push_back(word);
		flux>>word;
	}

	if(c >= nbreMotsAttendus){
		WARNING(":7:: Le nombre de mots reçu a dépassé le nombre de mots attendus de "
					<<c-nbreMotsAttendus+1<<" mots, il y a peut être une erreur dans le fichier "
					"de configuration.");
	}
	if(!flux)
		WARNING(":7:: Cette recherche a été poussée jusqu'au bout du fichier sans rencontrer de \" ;\", ce n'est pas normal.");

	return instructions;
}

float positif(int a)
{
    if(a<0)
        return a*-1;
    return a;
}


namespace Unoise
{
    PermTable* lambdaPermTable=nullptr;//La table de permutation de base*
    int seed=42;//Le seed!!!

    void setSeed(int graine)
    {
        seed=graine;
        srand(seed);
    }

    void setPermTable(PermTable* perm)
        {lambdaPermTable=perm;}


    /// \param pttable pointeur via une table de permutation qui sera générée avec le seed
    /// \param seed le seed
    void genPermTable(PermTable* pttable, int seed)
    {
        cout<<"9:::      Génération d'une table de permutation"<<endl;
        setSeed(seed);

        unsigned short buffer;
        bool finish=false;
        //On crée toutes les 256 cases de la table de permutation
        for(unsigned short c=0; c!=256; ++c)
        {
            //Tant qu'on a pas trouvé un nombre qui n'est pas dans la liste
            while(!finish)
            {
                finish = true;//Alors on a pas fini
                buffer = rand() % (255 + 1);//On calcul un nouveau nombre

                //Tant qu'on est pas arrivé au nombre qu'on est sensé calculer actuellement:
                for(unsigned short d=0; d!=c; ++d){
                    if(buffer == pttable->at(d))//Si par malheur on a déjà généré ce nombre
                        finish = false;//On recommence
                }

                if(finish == true)//En revanche si tout va au mieux
                    pttable->at(c) = buffer;//On donne la valeur générée à la case du tableau qu'on cherchait à remplir
            }
            finish=false;
        }
    }

    struct point2Dui
    {
		point2Dui(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
		unsigned int x,y;
    };

	///Cette fonction va donner à l'utilisateur la hauteur des quatres points les plus proches
	///\note Elle n'est faite que pour être utilisée par DiamondSquareNoise
	///\warning Si cette fonction occure un segfault, celà vient probablement du fait que pas ou tableau est à une valeur érronnée
	///\param tableau Une référence vers le chunk père du point
	///\param pas Le nombre de points par lequel on est autorisé à parcourir le chunk (de un \n en un pour les chunk finis, de quatre en quatre pour les chunks qu'il reste à subdiviser deux fois)
	///\param phase Détermine si le point est en phase carré ou diamond, ce qui nous donne un indice sur les points l'environnant
	std::vector<float> getPositionsAround(ChunkPoints* tableau, point2Dui point, int pas, std::array<std::vector<float>, 4>* pointsEnvironnants=nullptr, bool phase=1)
	{
		std::vector<float> positions;
		//Si on est en phase carré, alors on peut se permettre de tout envoyer
		if(phase)
		{
			positions.push_back(tableau->at(point.x-pas).at(point.y-pas));
			positions.push_back(tableau->at(point.x+pas).at(point.y-pas));
			positions.push_back(tableau->at(point.x-pas).at(point.y+pas));
			positions.push_back(tableau->at(point.y-pas).at(point.y-pas));
		}
		//Si on est en phase diamond et que les points environnants sont définis, on fait les tests supplémentaires
		else if(!phase && pointsEnvironnants)
		{
			if(point.x-pas < 0)
				positions.push_back(pointsEnvironnants->at(3).at(point.y));
			else
				positions.push_back(tableau->at(point.x-pas).at(point.y-pas));

			if(point.x+pas >= tableau->size())
				positions.push_back(pointsEnvironnants->at(1).at(point.y));
			else
				positions.push_back(tableau->at(point.x+pas).at(point.y-pas));

			if(point.y-pas < 0)
				positions.push_back(pointsEnvironnants->at(0).at(point.x));
			else
				positions.push_back(tableau->at(point.x-pas).at(point.y+pas));

			if(point.y+pas >= tableau->size())
				positions.push_back(pointsEnvironnants->at(2).at(point.x));
			else
				positions.push_back(tableau->at(point.y-pas).at(point.y-pas));
		}
		//Si on est juste en phase diamond, on met le nombre de points nécessaires
		else
		{
			if(point.x-pas >= 0)
				positions.push_back(tableau->at(point.x-pas).at(point.y-pas));

			else if(point.x+pas < tableau->size())
				positions.push_back(tableau->at(point.x+pas).at(point.y-pas));

			else if(point.y-pas >= 0)
				positions.push_back(tableau->at(point.x-pas).at(point.y+pas));

			else if(point.y+pas < tableau->size())
				positions.push_back(tableau->at(point.y-pas).at(point.y-pas));
		}

		return positions;
	}

    ///Cet algo génère sur une zone memoire donnée un bruit de type midpoint displacement
    ///\note Vous trouverez des informations sur le fonctionnement interne de cette fonction dans le fichier algoinfos rubrique diamondSquareNoise
    ///\warning Il est interdit de mettre pos à 1 ou à 0, car ça signifierait que le nombre de point par coté vaut 2
    ///\param x La position du chunk virtuelle X, inutile dans les versions primitives de la fonction
    ///\param y	La position du chunk virtuelle Y, inutile dans les versions primitives de la fonction
    ///\param amplitude La hauteur maximale d'un point (la valeur minimale est l'opposée de l'amplitude
    ///\param res La résolution, c'est à dire la rapidité de la réduction du bruit
    ///\param subDivisions Le nombre de subdivisions appliqués, pour trouver le nombre de points par coté ça fait, calculez: pow(2, subDivisions)+1
	 ChunkPoints diamondSquareNoise(float x, float y, int amplitude, float res, ushort subDivisions, int nseed, std::array<float, 4>* pointsPrincipaux, std::array<std::vector<float>, 4>* pointsEnvironnants)
	{
		if(seed<0)
			seed=nseed;
		ChunkPoints chunkPoints;//le tableau qui contient tous les nouveaux points du chunk

		uint nbrePointsCote=pow(2, subDivisions)+1;
		#define nbrPtsCotePaire (nbrePointsCote-1) //Utile dans de nombreux calculs où nous n'avons pas besoins de prendre en compte le point qui viens fermer le bord

		chunkPoints.resize(nbrePointsCote);
		for(uint c=0; c!=nbrePointsCote; c++)
			chunkPoints[c].resize(nbrePointsCote);

		//On teste si le paramètre pointsenvironnants est correctement initialisé
		if(pointsEnvironnants){
		uint tmpNbrePts;
		for(uint c=0; c!= pointsEnvironnants->size(); ++c)
			tmpNbrePts+=pointsEnvironnants->at(c).size();
		if(tmpNbrePts != nbrPtsCotePaire*4)
		{
			WARNING("diamondSquareNoise: le paramètre pointsEnvirronants ne contient pas\n le nombre attendu de points, il ne sera pas utilisé.");
			pointsEnvironnants=nullptr;//Ce qui va permettre de faire comme si ce paramètre déficient n'avait pas été entré
		}
		}


		INFO("Le chunk de subdivision "<<subDivisions<<" généré vaut "<<nbrePointsCote<<" points de coté pour un total de "<<CARRE(nbrePointsCote)<<" points.");

		if(pointsPrincipaux==nullptr)
		{
			//Durant cette première phase, nous créons les quatres sommets de base du chunk
			chunkPoints[0][0]=							RAND(-amplitude, amplitude);
			chunkPoints[nbrePointsCote][0]=				RAND(-amplitude, amplitude);
			chunkPoints[0][nbrePointsCote]=				RAND(-amplitude, amplitude);
			chunkPoints[nbrePointsCote][nbrePointsCote]=RAND(-amplitude, amplitude);
		}
		else
		{
			chunkPoints[0][0]=							pointsPrincipaux->at(0);
			chunkPoints[nbrePointsCote][0]=				pointsPrincipaux->at(1);
			chunkPoints[0][nbrePointsCote]=				pointsPrincipaux->at(2);
			chunkPoints[nbrePointsCote][nbrePointsCote]=pointsPrincipaux->at(3);
		}
		if(subDivisions==0)
			return chunkPoints;


		float moyennePointsEnvironnants;//cette variable fait la [explicite] en utilisant le point de traitement actuel comme base.
		for(uint c=1; c!=subDivisions+1; ++c)
		{
			//Il y a deux phases par subdivisions: d'abors la division carrée, puis la division diamond
			uint nbrePointsActuels=pow(2, c)+1;
			uint pas=nbrPtsCotePaire/(nbrePointsActuels-1);

			//Di d=0; alors c'est la division carrée, sinon c'est la division diamond
			for(char d=1; d!=-1; --d)
			{
				bool i=1;
				for(uint y=0; y!=nbrePointsActuels; ++y, ++i)
				{
					for(uint x=i/2; x!=nbrePointsActuels; ++x)
					{
						moyennePointsEnvironnants=0;

						point2Dui position(pas/2*d+ x*pas, pas/2*d+ y*pas);

						//Ici, on fait la moyenne des points environnants, puis on exécute l'opération pour définir la nouvelle hauteur du point
						vector<float> v=getPositionsAround(&chunkPoints, position, pas, pointsEnvironnants, (bool)d);
						for(uint c=0; c!=v.size(); ++c)
							moyennePointsEnvironnants+=v[c];
						moyennePointsEnvironnants/=v.size();

						chunkPoints[position.x][position.y] = (float)RAND(-amplitude, amplitude) / ((float)c*1.f/res) * (float)amplitude  + moyennePointsEnvironnants;
					}
					if(i == true)
						i=false;
				}
			}
		}

        return chunkPoints;
    }

    /**
    * \param x La position X
    * \param y La position Z
    * \param res La douceur du terrain
    * \param perm Le pointeur vers la table de permutation
    * \warning La fonction est défective
    */
    float perlinNoise(float x, float y, float res, PermTable *perm)
    {
        if(perm==nullptr)
        {
            if(lambdaPermTable==nullptr){
                lambdaPermTable=new PermTable;
                genPermTable(lambdaPermTable, seed);
            }
            perm=lambdaPermTable;
        }

        float tempX,tempY;
        int x0,y0,ii,jj,gi0,gi1,gi2,gi3;
        float unit = 1.0f/sqrt(2);
        float tmp,s,t,u,v,Cx,Cy,Li1,Li2;
        float gradient2[][2] = {{unit,unit},{-unit,unit},{unit,-unit},{-unit,-unit},{1,0},{-1,0},{0,1},{0,-1}};

        //Adapter pour la résolution
        x /= res;
        y /= res;

        //On récupère les positions de la grille associée à (x,y)
        x0 = (int)(x);
        y0 = (int)(y);

        //Masquage
        ii = x0 & 255;
        jj = y0 & 255;

        //Pour récupérer les vecteurs
        gi0 = perm->at(ii + perm->at(jj)) % 8;
        gi1 = perm->at(ii + 1 + perm->at(jj)) % 8;
        gi2 = perm->at(ii + perm->at(jj + 1)) % 8;
        gi3 = perm->at(ii + 1 + perm->at(jj + 1)) % 8;

        //on récupère les vecteurs et on pondère
        tempX = x-x0;
        tempY = y-y0;
        s = gradient2[gi0][0]*tempX + gradient2[gi0][1]*tempY;

        tempX = x-(x0+1);
        tempY = y-y0;
        t = gradient2[gi1][0]*tempX + gradient2[gi1][1]*tempY;

        tempX = x-x0;
        tempY = y-(y0+1);
        u = gradient2[gi2][0]*tempX + gradient2[gi2][1]*tempY;

        tempX = x-(x0+1);
        tempY = y-(y0+1);
        v = gradient2[gi3][0]*tempX + gradient2[gi3][1]*tempY;


        //Lissage
        tmp = x-x0;
        Cx = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

        Li1 = s + Cx*(t-s);
        Li2 = u + Cx*(v-u);

        tmp = y - y0;
        Cy = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

        return Li1 + Cy*(Li2-Li1);
    }
}
