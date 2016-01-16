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
	std::vector<MT_Vector3> m_addedVelocityList;

	/** Evite le double calcul de cellules, si vrai :
	 *    - la cellule ne peut pas être ajouter dans le front;
	 *    - la cellule ne peut recevoir de velocité externe.
	 */ 
	bool m_computed;

	/** Vrai quand la cellule fait partie du front, on utilise
	 * cette variable pour eviter les doubles ajout de cellules dans
	 * le front.
	 */
	bool m_inFront;

	/// La couleur du rendu de la cellule.
	MT_Vector3 m_color;

public:
	/** Intialisation de la cellule a sa position originelle.
	 */
	KX_Cell(MT_Vector3 position);
	virtual ~KX_Cell();

	/** Trouve toutes les cellules adjacentes grace au KDTree et a la liste de
	 * toutes les cellules.
	 */
	void FindAdjacents(KDTree *tree, KX_CellList& cells);

	/** Ajoute toutes les cellules adjacentes non calculées dans une liste
	 * servant de front.
	 */
	void AppendAjacents(KX_CellList& cells);

	void AddInFront(KX_CellList& cells);

	/** Applique sa propre vélocité à toutes les cellules adjacentes non
	 * calculées.
	 */
	void PropagateVelocity(unsigned int layer);

	/** Enregistre une velocité à appliquer à la fin du calcule.
	 */
	void AddVelocity(MT_Vector3 velocity, unsigned int layer);

	/** Reservation d'une liste de calque par cellules de collision
	 * pour la vélocité propagée.
	 */
	void ResizeVelocityLayers(unsigned int count);

	/** Applique la moyenne des velocités a la position et supprime toutes
	 * les velocités à appliquer.
	 */
	void Translate(float time);

	/** Remet la cellule à l'état non calculé.
	 */
	void ResetComputed();

	void ResetVelocity();

	/** Mise en place de la couleur de rendu de la cellule.
	 */
	void SetColor(MT_Vector3 color);

	/** Fait le rendu des cette cellules avec ses liens.
	 */
	void Render();

	/** Fait le rendu de la velocité externe par un trait rouge.
	 */
	void RenderVelocity(unsigned int layer);

	/// Renvoi vrai si la cellule à déjà était calculée.
	bool GetComputed() const;

	bool GetInFront() const;

	/// Renvoi la position de la cellule.
	MT_Vector3 GetPosition() const;
};
