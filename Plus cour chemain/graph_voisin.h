#ifndef _GRAPH_VOISIN
#define _GRAPH_VOISIN

    typedef struct Voisin
    {
        int _num_sommet;
        double _poid;
		struct Voisin *_v;
    } GVoisin;

    typedef GVoisin* Graph_Voisin;

	Graph_Voisin NewVoisin(int,double);
	bool Est_Vide_Voisin(Graph_Voisin V);
	Graph_Voisin Voisin_Suivant(Graph_Voisin);
	void New_Voisin_Suivant(Graph_Voisin *,Graph_Voisin);
	int Num_Sommet_Voisin(Graph_Voisin);
	double Poid_Arrete_Voisin(Graph_Voisin);

	void Vider_Voisin(Graph_Voisin);

#endif