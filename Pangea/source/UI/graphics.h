#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

#include "usualydefined.h"
#include "utilities.h"

#define PRIORITY_BACKGROUND0 1000000
#define PRIORITY_BACKGROUND1 500000
#define PRIORITY_BEFORECENTER 100000
#define PRIORITY_CENTER 50000
#define PRIORITY_TOP 10000
#define PRIORITY_TOTT 1000 //Top Of The Top

class Timer : public sf::Clock
{
public:
	Timer(){sf::Clock();};
	///Restart le timer uniquement la première fois ou quand stop a été appelé juste avant
	void start();
	///Permet un nouvel appel à start
	void stop();
private:
	bool m_started=false;
};

class SpriteList
{
public:
    SpriteList(){};
    sf::Sprite* AddSprite(sf::Sprite sprite, std::string id, unsigned int priority);
    void RemoveSprite(std::string id);

    static SpriteList* getGeneralSpriteList();
    std::multimap<unsigned int, sf::Sprite*>* getPriorityMap(){return &priorityMap;};
private:

    std::map<std::string, sf::Sprite*> indexMap;
    std::multimap<unsigned int, sf::Sprite*> priorityMap;
};

class TextureList
{
public:
    TextureList(){};
    TextureList(std::string filePath);
    void loadTextures(std::string filePath);
    void showErrors(std::string outfilen="");

    sf::Texture* getTexture(std::string textureID);
    static TextureList* getGeneralTextureList();

private:
    std::string m_errors;
    std::map<std::string, sf::Texture> m_textureMap;
};

#endif // GENERAL_H_INCLUDED
