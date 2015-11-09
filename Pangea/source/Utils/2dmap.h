#include "utilities.h"

///Ce type de map est fait pour retrouver un élément dans un arbre de noeuds en deux dimensions \n
///Il stocke donc en réalité deux maps qu'il va utiliser de façon transparente pour trouver les objets les plus proches
///\warning Le dévelloppeur étant un fainéant examplaire, il ne l'a implémenté que pour utiliser ses cellules pour l'instant
class 2dmap
{
    public:
    2dmap();
    Cell* FindCell(float x, float y);
    void AddCell(Cell* cellule);

    private:
    std::map<float, std::vector<Cell*> > m_tableauX;
    std::map<float, std::vector<Cell*> > m_tableauY;
};

Cell* operator[](2dmap, float x, float y:wq)
