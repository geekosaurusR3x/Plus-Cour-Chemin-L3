#include "graph_voisin.h"

#ifndef _Graphe_Sommet
#define _Graphe_Sommet

    typedef struct Sommet
    {
        int _nb_voisin;
		int _num_sommet;
		struct Voisin *_v;
    } GSommet;

	typedef GSommet* Graph_Sommet;

	Graph_Sommet New_Graph_Sommet(int);
	void Add_Graph_Sommet_Voisin(Graph_Sommet, int, double);
	void Supp_Graph_Sommet(Graph_Sommet);



#endif