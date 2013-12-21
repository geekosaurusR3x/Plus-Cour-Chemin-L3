#include <stdlib.h>
#include "GestionMemoire.h"
#include "graph_Sommet.h"
#include "graph_voisin.h"

Graph_Sommet New_Graph_Sommet(int num_sommet)
{
	Graph_Sommet retour = (Graph_Sommet)MonMalloc(sizeof(GSommet));
	retour->_nb_voisin = 0;
	retour->_num_sommet = num_sommet;
	retour->_v = NULL;

	return retour;
}

void Add_Graph_Sommet_Voisin(Graph_Sommet S, int num_sommet, double poid)
{
	S->_nb_voisin++;
	Graph_Voisin v = NewVoisin(num_sommet,poid);
	if (S->_v == NULL)
	{
		S->_v = v;
	}
	else
	{
		Graph_Voisin temp = S->_v;
		while(!Est_Vide_Voisin(Voisin_Suivant(temp)))
		{
			temp = Voisin_Suivant(temp);
		}
		New_Voisin_Suivant(&temp,v);
	}

}

void Supp_Graph_Sommet(Graph_Sommet S)
{
	Vider_Voisin(S->_v);
	MonFree(S);
}