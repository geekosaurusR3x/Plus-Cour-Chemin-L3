#include <stdlib.h>
#include "GestionMemoire.h"
#include "graph_voisin.h"

Graph_Voisin NewVoisin(int num_sommet,double poid)
{
	Graph_Voisin retour = (Graph_Voisin)MonMalloc(sizeof(Graph_Voisin));
	retour->_num_sommet = num_sommet;
	retour->_poid = poid;
	retour->_v = NULL;

	return retour;

}

bool Est_Vide_Voisin(Graph_Voisin V)
{
	return (V==NULL);
}

Graph_Voisin Voisin_Suivant(Graph_Voisin V)
{
	return V->_v;
}

void New_Voisin_Suivant(Graph_Voisin *V, Graph_Voisin new_v)
{
	(*V)->_v = new_v;
}

int Num_Sommet_Voisin(Graph_Voisin V)
{
	return V->_num_sommet;
}

double Poid_Arrete_Voisin(Graph_Voisin V)
{
	return V->_poid;
}

void Vider_Voisin(Graph_Voisin V)
{
	if (!Est_Vide_Voisin(V->_v))
	{
		Vider_Voisin(V->_v);
	}
	MonFree(V);
}