#include "KX_Cell.h"
#include "glew-mx.h"

KX_Cell::KX_Cell(MT_Vector3 position):
	m_position(position),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_addedVelocity(0.0f, 0.0f, 0.0f),
	m_addedVelocityCount(0),
	m_computed(false)
{
}

KX_Cell::~KX_Cell()
{
}


void KX_Cell::FindAdjacents(KDTree *tree, KX_CellList& cells)
{
}

void KX_Cell::AppendAjacents(KX_CellList& cells)
{
	cells.insert(cells.begin(), m_adjacents.begin(), m_adjacents.end());
}

void KX_Cell::PropagateVelocity()
{
	for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		if (!cell->GetComputed()) {
			cell->AddVelocity(m_velocity);
		}
	}

	// La cellule a transmit sa velocité, elle ne doit plus intervenir dans un calcul maintenant.
	m_computed = true;
}

void KX_Cell::AddVelocity(MT_Vector3& velocity)
{
	m_addedVelocity += velocity;
	++m_addedVelocityCount;
}

void KX_Cell::Translate()
{
	m_velocity = m_addedVelocity / m_addedVelocityCount;
	m_position += m_velocity;
}

void KX_Cell::Render(MT_Vector3 color)
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);

		glColor3fv(color.getValue());
		glVertex3fv(m_position.getValue());

	glEnd();
}

bool KX_Cell::GetComputed() const
{
	return m_computed;
}
