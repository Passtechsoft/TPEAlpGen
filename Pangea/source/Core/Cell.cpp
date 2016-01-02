#include "Cell.h"
#include <cmath>

using namespace std;

Cell::Cell()
{}

Cell::Cell(const Vector2f &position, World *mondeptr)
    : m_position(position)
{
	computeAttributes();
}

void Cell::computeVelocity()
{
    if (m_velocity = 0)
        return;

    delete m_voisines;
    m_voisines = m_tableauCells->findCells(this, m_distanceCorrecte);
    if (m_voisinzs.size() == 0)
        return;

    vector2f velocity;
    vector2f tmpVel;
    bool quit;
    //This is the loop that supervize the cell-to-cell computation (see queryConstrains to know more)
    while(!quit)
    {
        quit=true;

        for (uint c=0; c!= m_voisines.size(); ++c) {
            tmpVel = velocity;
            if(!m_voisine[c]->queryConstrains(tmpVel, this))
                quit=false;
            else {//Ici on subis les contraintes rendues
                m_deformX = abs(tmpVel.x, m_velocity.x);
                m_deformY = abs(tmpVel.y, m_velocity.y);
				
				m_backVelocity += tmpVel;
            }
        }
    }
    //We tell to all our daugter cells that computation is done
    for(uint c=0; c!= m_voisines.size(); ++c) {
        m_voisines[c]->queryConstrains(tmpVel, this, true);
    }
    
    m_velocity
}

void Cell::applyVelocity()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

///\param velocity The velocity to compute
///\param parent The parent cell.
///\param reset Set true when the mother cell have received all the velocity she was waiting for.
bool Cell::queryConstrains(vector2f &velocity, Cell *parent, float distance, bool reset)
{
	if(reset) {
		m_parsed = false;
		return true;
	}
	bool parsed=false;
	m_parsed= true;
	for(uint c=0; c!=m_voisines.size(); ++c) {
		tmpVel = velocity;
            if(!m_voisine[c]->queryConstrains(tmpVel, this, distance))
                parsed=false;
            else {//Ici on subis les contraintes rendues
                m_deformX = abs(tmpVel.x, m_velocity.x);
                m_deformY = abs(tmpVel.y, m_velocity.y);
				
				velocity += tmpVel;
            }
	}
	
	return parsed;
}

void Cell::computeAttributes()
{
    m_rayon =
    float volume = PI*m_rayon*m_rayon;
    float coef=0;
    for(uint r=0; r!= m_roches.size(); ++r)
    {
        coef+=(float)m_roches[c].first->masseVolumique*(volume/(float)(m_roches[c].second/100));
    }
}

float Cell::getConstrainResult()
{
	m_queryed = false;
	m_emmiting = false;
}

void Cell::setColor(float R, float G, float B)
{
	m_colR = R;
	m_colG = G;
	m_colB = B;
}
///\todo calculer plus finement les pourcentages des différents trucs.
void Cells::addRock(Roche *rock, uint pourcentage)
{
    uint percents=0;
    for(uint c=0; c!=m_roches.size(); ++c) {
        percents+=m_roches[c].second;
        m_roches[c].second -= pourcentage/100 * m_roches[c].second;
    }
    if(percents != 100)
        WARNING("La somme des pourcentages de la cellule ("<<m_position.x<<";"<<m_position.y<<") vaut "<<percents);
	m_rocks.push_back(std::pair<Roche *, uint>(rock, pourcentage));
	computeAttributes();
}

float Cell::distance(Cell &cell1, Cell &cell2)
{
	return abs(quare_root( pow(cell2.GetPosition().y-cell1.GetPosition().y,2) +
						   pow(cell2.GetPosition().x-cell2.GetPosition().x,2) ));
}
