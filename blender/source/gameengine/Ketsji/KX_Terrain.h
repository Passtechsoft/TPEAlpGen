#include "KX_Cell.h"

struct KX_ColliderInfo
{
	KX_Cell *cell;
	MT_Vector3 velocity;
};

typedef std::vector<KX_ColliderInfo> KX_ColliderInfoList;

class KX_Terrain
{
private:
	/// L'arbre utilisé pour rechercher les cellules adjacentes.
	KDTree *m_tree;
	/// Toutes les cellules existantes.
	KX_CellList m_cells;
	/// Le nombre de cellules en largeur.
	unsigned int m_size;
	/// L'espace entre les cellules.
	float m_interval;
	/// Le temps actuelle + 1 à chaque fin de collision.
	float m_time;

	/// Liste de toutes les cellules fromant le front.
	KX_CellList m_currentFront;
	/// Le numéro d'iteration du front actuelle.
	unsigned int m_frontIteration;

	/// Toutes les cellules en collision. Cette liste est fixe pour le moment.
	KX_ColliderInfoList m_colliders;
	/// Le numéro de la cellule de collision ayant créé le front.
	unsigned int m_currentCollider;

	/** Rconstruction de l'arbre avec la nouvelle position de toutes
	 * les cellules.
	 */
	void RebuildTree();

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
