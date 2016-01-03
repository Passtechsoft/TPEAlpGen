#include "KX_Cell.h"

class KX_Terrain
{
private:
	/// L'arbre utilisé pour rechercher les cellules adjacentes.
	KDTree *m_tree;
	/// Toutes les cellules existantes.
	KX_CellList *m_cells;
	/// Le nombre de cellules en largeur.
	unsigned int m_size;

	/// Liste de toutes les cellules fromant le front.
	KX_CellList m_currentFront;
	/// Le numéro d'iteration du front actuelle.
	unsigned int m_frontIteration;

	/// Toutes les cellules en collision. Cette liste est fixe pour le moment.
	KX_CellList m_colliders;
	/// Le numéro de la cellule de collision ayant créé le front.
	unsigned int m_currentCollider;

public:
	KX_Terrain();
	virtual ~KX_Terrain();

	/** Créer ou modifie un front existant, propage la velocité des cellules
	 * du front et les déplace si le front est finit.
	 */
	void NextFront();

	/** Fait le rendu des cellules et de leurs liens.
	 */
	void Render();
};
