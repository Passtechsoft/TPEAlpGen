#include "MT_Vector3.h"
#include <vector>

class KX_Cell;
class KDTree;

typedef std::vector<KX_Cell *> KX_CellList;

class KX_Cell
{
private:
	/// Les cellules adjacentes.
	KX_CellList m_adjacents;

	/// La position de la cellule.
	MT_Vector3 m_position;
	/// La velocité precédente de la cellule, utilisée comme inertie.
	MT_Vector3 m_velocity;

	/// La somme de toutes les velocités externes appliquées à cette cellule.
	MT_Vector3 m_addedVelocity;
	/// Le nombre de velocités externes appliquées à cette cellule.
	unsigned int m_addedVelocityCount;

	/** Evite le double calcul de cellules, si vrai :
	 *    - la cellule ne peut pas être ajouter dans le front;
	 *    - la cellule ne peut recevoir de velocité externe.
	 */ 
	bool m_computed;

public:
	KX_Cell();
	virtual ~KX_Cell();

	/** Trouve toutes les cellules adjacentes grace au KDTree et a la liste de
	 * toutes les cellules.
	 */
	void FindAdjacents(KDTree *tree, KX_CellList& cells);

	/** Ajoute toutes les cellules adjacentes non calculées dans une liste
	 * servant de front.
	 */
	void AppendAjacents(KX_CellList& cells);

	/** Applique sa propre vélocité à toutes les cellules adjacentes non
	 * calculées.
	 */
	void PropagateVelocity();

	/** Enregistre une velocité à appliquer à la fin du calcule.
	 */
	void AddVelocity(MT_Vector3& velocity);

	/** Applique la moyenne des velocités a la position et supprime toutes
	 * les velocités à appliquer.
	 */
	void Translate();

	/** Fait le rendu des cette cellules avec ses liens.
	 */
	void Render(unsigned int color);
};
