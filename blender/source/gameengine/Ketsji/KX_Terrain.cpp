#include "KX_Terrain.h"

extern "C" {
	#include "BLI_kdtree.h"
}

KX_Terrain::KX_Terrain()
	:m_tree(NULL),
	m_size(10),
	m_interval(1.0f),
	m_currentCollider(0)
{
	// Initialisation des cellules en nid d'abeilles.
	for (unsigned int y = 0; y < m_size; ++y) {
		// Ne nombre de cellules en x : alternance de m_size et m_size - 1.
		unsigned int maxx = m_size - y % 2;
		// Le décalage en x de la première cellule : alternance 0 et m_interval / 2.
		float gapx = (y % 2) ? (m_interval / 2.0f) : 0.0f;
		for (unsigned int x = 0; x < maxx; ++x) {
			MT_Vector3 position(x * m_interval + gapx, y * m_interval, 0.0f);
			KX_Cell *cell = new KX_Cell(position);
			m_cells.push_back(cell);

			// AJout d'une cellule de collision
			if (x == 0 && y == 0) {
				m_colliders.push_back(cell);
			}
		}
	}
}

KX_Terrain::~KX_Terrain()
{
	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		delete *it;
	}
	if (m_tree) {
		BLI_kdtree_free(m_tree);
	}
}

void KX_Terrain::RebuildTree()
{
	// Suppression de l'ancien arbre.
	if (m_tree) {
		BLI_kdtree_free(m_tree);
	}
	// Instancation d'un nouvelle arbre.
	m_tree = BLI_kdtree_new(m_cells.size());

	for (unsigned int i = 0, size = m_cells.size(); i < size; ++i) {
		KX_Cell *cell = m_cells[i];
		/* Insertion des cellules dans l'arbre en fonction de leur indice 
		 * dans la liste des cellules.
		 */
		BLI_kdtree_insert(m_tree, i, cell->GetPosition().getValue());
	}

	/* Organisation des noeuds (= position cellule) dans l'arbre pour optimiser
	 * la recherche.
	 */
	BLI_kdtree_balance(m_tree);
}

void KX_Terrain::NextFront()
{
	// Construction d'un nouveau front si celui ci est vide.
	if (m_currentFront.empty()) {
		/* La cellule de collision actuelle est après la dernière.
		 * Cela signifie aussi que toutes les propagations des velocités des
		 * cellules en collisions ont été effectué.
		 * On peut donc appliquer la velocité au positions des cellules,
		 * reconstruire l'arbre et rechercher les cellules adjacentes.
		 */
		if (m_currentCollider >= m_colliders.size()) {
			// On remet à la première cellule.
			m_currentCollider = 0;

			// Reconstruction de l'arbre.
			RebuildTree();

			for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
				KX_Cell *cell = *it;
				// Aplication de la vélocité sur la position de la cellule.
				cell->Translate();
				// Recherche de toutes les cellules adjacentes à celle ci.
				cell->FindAdjacents(m_tree, m_cells);
			}
		}

		KX_Cell *collider = m_colliders[m_currentCollider];

		// Application d'une velocity par défaut aux cellules de collision.
		const float scale = 0.1f;
		MT_Vector3 velocity(1.0f, 1.0f, 0.0f);
		collider->AddVelocity(velocity * scale);

		// Création du front originel contenant que la cellule en collision.
		m_currentFront.push_back(collider);
		++m_currentCollider;
	}

	for (KX_CellList::iterator it = m_currentFront.begin(), end = m_currentFront.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		/* Porpagation de la vélocité de cette cellule au cellules
		 * adjacentes pas encore calculées.
		 */
		cell->PropagateVelocity();
	}

	// Nouveau front temporaire.
	KX_CellList temporaryFront;
	// Ajout des cellules adjacentes au front non calculées.
	for (KX_CellList::iterator it = m_currentFront.begin(), end = m_currentFront.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->AppendAjacents(temporaryFront);
	}

	// Replacment de l'ancien front.
	m_currentFront = temporaryFront;
}

void KX_Terrain::Render()
{
	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->SetColor(MT_Vector3(1.0f, 0.0f, 0.0f));
	}

	for (KX_CellList::iterator it = m_currentFront.begin(), end = m_currentFront.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->SetColor(MT_Vector3(1.0f, 0.0f, 1.0f));
	}

	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->Render();
	}
}
