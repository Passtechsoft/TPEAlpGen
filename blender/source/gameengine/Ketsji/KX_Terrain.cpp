#include "KX_Terrain.h"
#include <cmath>
#include <fenv.h>

extern "C" {
	#include "BLI_kdtree.h"
}

// #define USE_RAND

KX_Terrain::KX_Terrain()
	:m_tree(NULL),
	m_sizeX(300),
	m_sizeY(300),
	m_interval(1.0f),
	m_time(0.0f)
{
	float yinterval = std::sin(MT_PI / 3.0f);
	// Initialisation des cellules en nid d'abeilles.
	for (unsigned int y = 0; y < m_sizeY; ++y) {
		// Ne nombre de cellules en x : alternance de m_size et m_size - 1.
		unsigned int maxx = m_sizeX - y % 2;
		// Le décalage en x de la première cellule : alternance 0 et m_interval / 2.
		float gapx = (y % 2) ? (m_interval / 2.0f) : 0.0f;
		for (unsigned int x = 0; x < maxx; ++x) {
			/* L'ajout de bruit dans la position des cellules permet une certaine stabilité
			 * car le CIR ne fonctionne pas avec un angle parfais de 0 ou pi/2.
			 */
			const float randmax = 0.2f;
			float randx = 
#ifdef USE_RAND
				(float)rand() / (float) (RAND_MAX / randmax);
#else
				0.0f;
#endif
			float randy = 
#ifdef USE_RAND
				(float)rand() / (float) (RAND_MAX / randmax);
#else
				0.0f;
#endif
			MT_Vector3 position(x * m_interval + gapx + randx,
								y * m_interval * yinterval + randy, 
								0.0f);
			KX_Cell *cell = new KX_Cell(position);
			m_cells.push_back(cell);
		}
	}

	const float scale = .2f;

	const unsigned int colliders = 2;
	unsigned int indices[colliders] = {
		2, /*5,*/
		(unsigned int)m_cells.size() - 1
// 		(unsigned int)m_cells.size() - 5
	};

	MT_Vector3 direction(0.2f, 0.5f, 0.0f);
	MT_Vector3 velocities[colliders] = {
		direction * scale,
// 		direction * scale,
// 		direction * -scale,
		direction * -scale
	};

	for (unsigned int i = 0; i < 1; /*m_sizeX*/ ++i) {
		KX_ColliderInfo info;
		info.cell = m_cells[/*indices[i]*/i];
		info.velocity = direction * scale; //velocities[i];

		m_colliders.push_back(info);
	}

	// Pour être sure qu'on initialise toutes les cellules.
	m_currentCollider = m_colliders.size() + 1;
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

// #define STOP_ON_FIRST_FRAME

void KX_Terrain::NextFront()
{
	feenableexcept(FE_INVALID | FE_OVERFLOW);
	// Construction d'un nouveau front si celui ci est vide.
	if (m_currentFront.empty()) {
		/* La cellule de collision actuelle est après la dernière.
		 * Cela signifie aussi que toutes les propagations de velocités des
		 * cellules en collisions ont été effectué.
		 * On peut donc appliquer la velocité au positions des cellules,
		 * reconstruire l'arbre et rechercher les cellules adjacentes.
		 */
		if (m_currentCollider >= m_colliders.size()) {
			// Reconstruction de l'arbre.
			RebuildTree();

#ifdef STOP_ON_FIRST_FRAME
			if (m_time > 1.0f) {
				fedisableexcept(FE_INVALID | FE_OVERFLOW);
				return;
			}
			else {
				for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
					KX_Cell *cell = *it;
					// Aplication de la vélocité sur la position de la cellule.
					cell->Translate(m_time);
					// Reservation des calques.
					cell->ResizeVelocityLayers(m_colliders.size());
					// Recherche de toutes les cellules adjacentes à celle ci.
					cell->FindAdjacents(m_tree, m_cells);
				}
			}
			std::cout << "=================== NEXT FRAME ====================" << std::endl;
#else
			for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
				KX_Cell *cell = *it;
				// Aplication de la vélocité sur la position de la cellule.
				cell->Translate(m_time);
				// Reservation des calques.
				cell->ResizeVelocityLayers(m_colliders.size());
				// Recherche de toutes les cellules adjacentes à celle ci.
				cell->FindAdjacents(m_tree, m_cells);
			}
#endif

			// On remet à la première cellule.
			m_currentCollider = 0;
			// On incremnte le temps.
			m_time += 1.0f;
		}

		// Toutes les cellules sont remit dans l'état non calculé.
		for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
			KX_Cell *cell = *it;
			cell->ResetComputed();
		}

		const KX_ColliderInfo& colliderInfo = m_colliders[m_currentCollider];
		KX_Cell *collider = colliderInfo.cell;
		MT_Vector3 velocity = colliderInfo.velocity;

		// Application d'une velocity par défaut aux cellules de collision.
		collider->AddVelocity(velocity, m_currentCollider);

		// Création du front originel contenant que la cellule en collision.
		m_currentFront.push_back(collider);
		++m_currentCollider;
	}

	for (KX_CellList::iterator it = m_currentFront.begin(), end = m_currentFront.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		/* Porpagation de la vélocité de cette cellule au cellules
		 * adjacentes pas encore calculées.
		 */
		cell->PropagateVelocity(m_currentCollider - 1);
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
	fedisableexcept(FE_INVALID | FE_OVERFLOW);
}

// #define DRAW_ALL_COLLIDERS
void KX_Terrain::Render()
{
	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->SetColor(MT_Vector3(1.0f, 0.0f, 1.0f));
	}

	for (KX_CellList::iterator it = m_currentFront.begin(), end = m_currentFront.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->SetColor(MT_Vector3(1.0f, 1.0f, 0.0f));
		cell->RenderVelocity(m_currentCollider - 1);
	}

#ifdef DRAW_ALL_COLLIDERS
	for (unsigned int i = 0; i < m_colliders.size(); ++i) {
		KX_Cell *collider = m_colliders[i].cell;
		collider->SetColor(MT_Vector3(0.0f, 0.0f, 0.0f));
	}
#else
	KX_Cell *collider = m_colliders[m_currentCollider - 1].cell;
	collider->SetColor(MT_Vector3(0.0f, 0.0f, 0.0f));
	collider->RenderVelocity(m_currentCollider - 1);
#endif

	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->Render();
	}
}
