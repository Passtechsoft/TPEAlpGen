#include "KX_Cell.h"

extern "C" {
	#include "BLI_kdtree.h"
}

#include "glew-mx.h"

KX_Cell::KX_Cell(MT_Vector3 position):
	m_position(position),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_addedVelocity(0.0f, 0.0f, 0.0f),
	m_addedVelocityCount(0),
	m_computed(false),
	m_color(0.0f, 0.0f, 0.0f)
{
}

KX_Cell::~KX_Cell()
{
}


void KX_Cell::FindAdjacents(KDTree *tree, KX_CellList& cells)
{
	// suppresion des anciennes cellules adjacentes.
	m_adjacents.clear();

	// Futur tableau des position des cellules adjacentes.
	KDTreeNearest *nearests = NULL;

	const float radius = 1.3f;
	// On recherche toutes les cellules dans un certain rayon.
	unsigned int found = BLI_kdtree_range_search(tree, m_position.getValue(), &nearests, radius);

	for (unsigned int i = 0; i < found; ++i) {
		const KDTreeNearest& nearest = nearests[i];
		// Retrouve la cellule par rapport a sont indice lors de son insertion dans l'arbre.
		KX_Cell *cell = cells[nearest.index];
		// Pas de relations avec soi même.
		if (cell != this) {
			m_adjacents.push_back(cell);
		}
	}

	if (nearests) {
		// TODO : trouver un moyen de suppresion.
		/*for (unsigned int i = 0; i < found; ++i) {
			delete &nearests[i];
		}*/
	}
}

void KX_Cell::AppendAjacents(KX_CellList& cells)
{
	for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		if (!cell->GetComputed()) {
			cells.push_back(cell);
		}
	}
}

void KX_Cell::PropagateVelocity()
{
	for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		if (!cell->GetComputed()) {
			MT_Vector3 velocity = m_addedVelocity / m_addedVelocityCount;
			cell->AddVelocity(velocity);
		}
	}

	// La cellule a transmit sa velocité, elle ne doit plus intervenir dans un calcul maintenant.
	m_computed = true;
}

void KX_Cell::AddVelocity(MT_Vector3 velocity)
{
	m_addedVelocity += velocity;
	++m_addedVelocityCount;
}

void KX_Cell::Translate()
{
	if (m_addedVelocityCount != 0) {
		m_velocity = m_addedVelocity / m_addedVelocityCount;
		m_position += m_velocity;
	}

	// Reinitialisation de la velocité externe.
	m_addedVelocityCount = 0;
	m_addedVelocity = MT_Vector3(0.0f, 0.0f, 0.0f);

	// On remet la cellule en non calculé pour la prochaine frame.
	m_computed = false;
}

void KX_Cell::SetColor(MT_Vector3 color)
{
	m_color = color;
}

void KX_Cell::Render()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);

		glColor3fv(m_color.getValue());
		glVertex3fv(m_position.getValue());

	glEnd();

	glBegin(GL_LINES);

		glColor3f(0.0f, 1.0f, 0.0f);
		for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
			KX_Cell *cell = *it;
			glVertex3fv(m_position.getValue());
			glVertex3fv(cell->GetPosition().getValue());
		}

	glEnd();
}

bool KX_Cell::GetComputed() const
{
	return m_computed;
}

MT_Vector3 KX_Cell::GetPosition() const
{
	return m_position;
}

