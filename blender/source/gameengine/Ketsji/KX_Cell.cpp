#include "KX_Cell.h"
#include <cmath>

extern "C" {
	#include "BLI_kdtree.h"
	#include "BLI_math.h"
	#include "MEM_guardedalloc.h"
}

#include "glew-mx.h"

KX_Cell::KX_Cell(MT_Vector3 position):
	m_position(position),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_computed(false),
	m_inFront(false),
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

	const float radius = std::sqrt(2.0f);
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
		MEM_freeN(nearests);
	}
}

void KX_Cell::AppendAjacents(KX_CellList& cells)
{
	for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->AddInFront(cells);
	}
	m_inFront = false;
}

void KX_Cell::AddInFront(KX_CellList& cells)
{
	if (!m_computed && !m_inFront) {
		cells.push_back(this);
		m_inFront = true;
	}
}

#define USE_DIRECTION
// #define USE_DISTANCE

void KX_Cell::PropagateVelocity(unsigned int layer)
{
	// La vélocité de cette cellules.
	MT_Vector3 originalVelocity = m_addedVelocityList[layer];

	// La cellule a transmit sa velocité, elle ne doit plus intervenir dans un calcul maintenant.
	m_computed = true;

	if (MT_fuzzyZero(originalVelocity)) {
		return;
	}

	KX_CellList adjacentActiveCellList;

	unsigned int adjacentCellCount = 0;
	for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		if (!cell->GetComputed() && !cell->GetInFront()) {
			adjacentActiveCellList.push_back(cell);
			++adjacentCellCount;
		}
	}

	// Le facteur appliqué sur la force de chaque cellules.
	float cellFactorList[adjacentCellCount];
	MT_Vector3 cellDirectionList[adjacentCellCount];

	for (unsigned int i = 0; i < adjacentCellCount; ++i) {
		KX_Cell *cell = adjacentActiveCellList[i];

		MT_Vector3 direction = (cell->GetPosition() - m_position);
		float angle = direction.angle(originalVelocity);
		float dist = direction.length();
		float v1 = originalVelocity.length();
		// L'axe Z deduit par le plan formé par les 2 cellules et le vecteur de force.
		MT_Vector3 zaxis = direction.cross(originalVelocity);
		// L'axe Y perpendiculaire à la force et sur le même plan.
		MT_Vector3 yaxis = zaxis.cross(originalVelocity);

		float factor = 0.0f;
		// L'angle est proche de 0 ou pi car l'absolu de son cosinus
		if (MT_fuzzyZero(std::sin(angle))) {
			factor = 1.0f;
		}
		else if (MT_fuzzyZero(std::cos(angle))) {
			factor = 1.0f;
		}
		else {
			// Application du centre de rotation instantané d'un solide.
			// Le premier rayon.
			float r1 = std::abs(dist / std::cos(MT_PI / 2.0f - angle));
			// Le second rayon.
			float r2 = std::abs(dist * std::tan(MT_PI / 2.0f - angle));

			// La rotation du solide en rad/s.
			float om = v1 / r1;
			float v2 = om * r2;

			/** on calcule le facteur avec le cosinux de l'angle entre le vecteur
			* de force et le vecteur vers la cellule adjacente.
			*/
			factor = v2;
			if (factor < 0.0f) {
				std::cout << "negative, factor : " << factor << ", r2 : " << r2 << ", om : " << om << std::endl;
			}
		}
		cellFactorList[i] = factor;
		cellDirectionList[i] = direction;
	}

	/** Puis on applique enfin la velocité.
	 */
	for (unsigned int i = 0; i < adjacentCellCount; ++i) {
		KX_Cell *cell = adjacentActiveCellList[i];
		float factor = cellFactorList[i];
		MT_Vector3 velocity;

#ifdef USE_DIRECTION
		MT_Vector3 direction = cellDirectionList[i];
		velocity = direction.normalized() * factor;
#  ifdef USE_DISTANCE
		float dist = direction.length();
		float ratio = std::sqrt(2.0f) / dist;
		velocity += direction * ratio;
#  endif
#else
		velocity = originalVelocity * factor;
#endif

		cell->AddVelocity(velocity, layer);
	}
}

void KX_Cell::AddVelocity(MT_Vector3 velocity, unsigned int layer)
{
	if (MT_fuzzyZero(velocity)) {
		return;
	}

	float velocityDiffDist = (velocity - m_velocity).length();

	float factor = 0.0f;
	if (!MT_fuzzyZero(velocityDiffDist)) {
		factor = 1.0f; //(-pow(velocityDiffDist - 2, 3)) / 50.0f + 0.8f;
	}

// 	CLAMP(factor, 0.0f, 0.8f);

	velocity *= factor;
	m_addedVelocityList[layer] += velocity;
}

void KX_Cell::Translate(float time)
{
	for (unsigned int layer = 0; layer < m_addedVelocityList.size(); ++layer) {
		MT_Vector3 addedVelocity = m_addedVelocityList[layer];

		m_position += addedVelocity;
		m_velocity = addedVelocity;
	}
}

void KX_Cell::ResetComputed()
{
	// On remet la cellule en non calculé pour la prochaine frame.
	m_computed = false;
}

void KX_Cell::ResetVelocity()
{
	m_velocity = MT_Vector3(0.0f, 0.0f, 0.0f);
}

void KX_Cell::ResizeVelocityLayers(unsigned int count)
{
	m_addedVelocityList.clear();
	m_addedVelocityList.resize(count, MT_Vector3(0.0f, 0.0f, 0.0f));
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

		glColor3f(0.0f, 0.0f, 1.0f);
		for (KX_CellList::iterator it = m_adjacents.begin(), end = m_adjacents.end(); it != end; ++it) {
			KX_Cell *cell = *it;
			glVertex3fv(m_position.getValue());
			glVertex3fv(cell->GetPosition().getValue());
		}

	glEnd();
}

void KX_Cell::RenderVelocity(unsigned int layer)
{
	for (unsigned int i = 0; i < m_addedVelocityList.size(); ++i) {
		MT_Vector3 velocity = (m_addedVelocityList[i]) * 10.0f;
		MT_Vector3 position = m_position + MT_Vector3(0, 0, i * 0.01f);

		glColor3f(1.0f, ((float)i) / m_addedVelocityList.size(), 0.0f);
		glBegin(GL_LINES);
			glVertex3fv(position.getValue());
			glVertex3fv((position + velocity).getValue());
		glEnd();
	}
}

bool KX_Cell::GetComputed() const
{
	return m_computed;
}

bool KX_Cell::GetInFront() const
{
	return m_inFront;
}

MT_Vector3 KX_Cell::GetPosition() const
{
	return m_position;
}
