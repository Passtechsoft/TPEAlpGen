#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

//Mettez cette ligne en commentaire si vous ne voulez pas voir affichés les messages de debug
#define DEBUGMOD

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <sys/time.h>

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>

#include <array>
#include <vector>
#include <map>
#include <queue>
#include <list>


#define PR_WHITE 30 //#seriouslybro
#define PR_RED 31
#define PR_GREEN 32
#define PR_YELLOW 33
#define PR_BLUE 34
#define PR_PURPLE 35
#define PR_LIGHTBLUE 36
#define PR_GREY 37

#define COLORED_PRINT(txt, color) "\033["<<color<<"m"<<txt<<"\033[0m"
#define FONT(txt, color, boldcolor) "\033["<<color<<"m"<<"\033["<<boldcolor<<"m"<<txt<<"\033[0m"

#define STYLE_OUT(txt, surtxt, color) std::cout<<FONT(surtxt, color, 42)<<" in "<<\
	COLORED_PRINT(__PRETTY_FUNCTION__, PR_GREY)<<": "<<txt<<std::endl

#define PIMP(txt) STYLE_OUT("/!\\PIMP SPOTTED/!\\", PR_LIGHTBLUE)
#define DEBUG(txt) STYLE_OUT(txt, "DEBUG", PR_BLUE)
#define INFO(txt) STYLE_OUT(txt, "INFO", PR_GREY)
#define WARNING(txt) STYLE_OUT(txt, "WARNING", PR_YELLOW)
#define ERROR(txt) STYLE_OUT(txt, "ERROR", PR_RED)
#define CRITIC_ERROR(txt) STYLE_OUT(txt, "/!\\CRITIC ERROR/!\\", PR_PURPLE, GREEN)

#ifdef DEBUGMOD
#define affInDebug(message, entree) entree(message)
#else
#define affDebug(message, entree) ;
#endif // DEBUGMOD

#define Getter(nomvariable, suffixe) \
template<typename T>				 \
T Get##suffixe() 					 \
{ 									 \
	return nomvariable;				 \
}

#define Setter(nomvariable, suffixe) \
template<typename T> 				 \
void Set##suffixe(T modifier) 	 	 \
{ 									 \
	nomvariable = modifier;			 \
}

#define GetSet(nomvariable, suffixe) \
Getter(nomvariable, suffixe)		 \
Setter(nomvariable, suffixe)		 \

//Génère un pitit nbre aléatoire
#define RAND(min, max) ( (rand() % (max-min+1))+min )

#define MSLEEP(msecs) usleep(msecs*1000)

//Une simple fonction ²
#define CARRE(var) (var*var)

//Retourne true si le chiffre est entier
#define ISINTEGRER(result) (result==(int)result)

#define ISPOSITIVE(number) (number>=0)

///Cette fonction ne garde qu'un certain nombre de chiffres de la partie entière
#define cutDiz(nombre_, nbIntegrers_) (nombre_/(10*(long)((nombre_/10)*pow(10, nbIntegrers_)))

#define DIVISIBLE_PAR(nbre, diviseur) (nbre/((float)diviseur)==(short)nbre/diviseur)
//Utile dans les cas de tests ou on aimerait obtenir un int tel que jamais un utilisateur n'oserais le taper
#define INT_IMPROBABLE 4294967295


typedef std::map<std::string, std::map<void*, int> > CompteurClasseType;
typedef unsigned char oct_e;//Rest In Pieces :_{
typedef unsigned char uchar;
typedef unsigned short ushort;

std::map<void *, int> *getCptClass(std::string &nom);

#define ptrFlagInc(nomclasse, ptrClasse) compteurClasses[nomclasse].at(ptrClasse) += 1
#define ptrFlagDec(nomclasse, ptrClasse) compteurClasses[nomclasse].at(ptrClasse) -= 1
void affPtrFlag();


///Permet de tester une certaine probabilité
bool probability(int prob, int sur);
///Renvoie le nombre en positif
float positif(int a);
///Donne le mot n°\a word_num dans la chaine chaine, à noter que word_num commence par 0;
std::string getWord(ushort word_num, std::string &chaine);
///\return true si \a subWord est dans \a word
bool isInWord(std::string word, std::string subWord);

///Lis dans le fichier donné une liste de mots suivant ces conditions: si le premier mot de la chaîne \n
///se termine par ":", on l'ignore (il est alors considéré comme une sorte de commentaire) et si le mot \n
///est ";", on s'arrête de rajouter des mots dans le vector. Enfin, on ne retourne qu'une nombre de mots égale\n
///à \a nbreMotsAttendus.
std::vector<std::string> getFileWithSthx(std::ifstream &flux, unsigned int nbreMotsAttendus=INT_IMPROBABLE);

///Uchronia noise namespace
namespace Unoise
{
    typedef std::array<unsigned short, 256> PermTable;
    typedef std::vector<std::vector<float>> ChunkPoints;

    ///Stock a PermTable "Diamond"
    struct DiamondPermTable
    {
        PermTable permTable;
        PermTable permPermTable;
    };

    ///Pas obligatoire, cette fonction permet de changer lambdaPermTable
    void setPermTable(PermTable* perm);

    void setSeed(int);

    ///On génère la table de permutation à partir d'un seed
    void genPermTable(PermTable*, int);

    ///Fonction du bruit de perlin (float x, float z, float resolution, table de permutation):
    float perlinNoise(float x, float z, float res, PermTable* perm=nullptr);

    ///Midpoint displacement noise, ou Diamond square noise (nom latin: carrus diamus)
    ChunkPoints diamondSquareNoise(float x, float y, int amplitude, float res, unsigned short subDivisions, int nseed=-1, std::array<float, 4>* pointsPrincipaux=nullptr, std::array<std::vector<float>, 4>* pointsEnvironnants=nullptr);
}

#endif // UTILITIES_H_INCLUDED

