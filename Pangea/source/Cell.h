class Cell
{
public:
	enum NatureType {
		NAT_BORD,
		NAT_SOCLE_OCEAN,
		NAT_COUCHE_INTERSTI,
		NAT_SOCLE_CONTI,
		NAT_SOCLE_CONTI_ERO,
	};

	Cell(unsigned int color,
		 NatureType nature,
		 float courbure,
		 float vieil,
		 float altitude);
	~Cell();

private:
	unsigned int m_color;
	NatureType m_nature;
	float m_courbure;
	unsigned int m_memoire;
	float m_vieil;
	float m_altitude;
};
