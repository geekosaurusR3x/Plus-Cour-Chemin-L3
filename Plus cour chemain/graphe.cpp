#include <stdlib.h>
#include <iostream>
#include "graphe.h"
using namespace std;

//partie pour les voisin
Graph_Voisin NewVoisin(int num_sommet,double poid)
{
	Graph_Voisin retour = (Graph_Voisin)malloc(sizeof(GVoisin));
	retour->_num_sommet = num_sommet;
	retour->_poid = poid;
	retour->_marque = false;
	retour->_v = NULL;

	return retour;
}

bool Est_Vide_Voisin(Graph_Voisin V)
{
	return (V==NULL);
}

double Get_Poid_Voisin(Graph_Voisin V)
{
	return V->_poid;
}

bool Est_Marque(Graph_Voisin V)
{
	return V->_marque;
}

void Marque(Graph_Voisin *V)
{
	(*V)->_marque = true;
}

Graph_Voisin Voisin_Suivant(Graph_Voisin V)
{
	return V->_v;
}

int Num_Sommet_Voisin(Graph_Voisin V)
{
	return V->_num_sommet;
}

void New_Voisin_Suivant(Graph_Voisin *V, Graph_Voisin new_v)
{
	(*V)->_v = new_v;
}

void Vider_Voisin(Graph_Voisin V)
{
	if (V != NULL)
	{
		Vider_Voisin(V->_v);
	}
	free(V);
}

//partie pour les sommets
Graph_Sommet New_Graph_Sommet(int num_sommet)
{
	Graph_Sommet retour = (Graph_Sommet)malloc(sizeof(GSommet));
	retour->_nb_voisin = 0;
	retour->_num_sommet = num_sommet;
	retour->_v = NULL;

	return retour;
}

int Get_Nb_Voisin(Graph_Sommet S)
{
	return S->_nb_voisin;
}

Graph_Voisin Get_Voisin(Graph_Sommet S)
{
	return S->_v;
}

void Add_Graph_Sommet_Voisin(Graph_Sommet *S, int num_sommet, double poid)
{
	if ((*S)->_v == NULL)
	{
		(*S)->_nb_voisin++;
		(*S)->_v = NewVoisin(num_sommet,poid);
	}
	else
	{
		Graph_Voisin temp = (*S)->_v;
		bool trouve = false;
		while(temp->_v != NULL)
		{
			if(temp->_num_sommet == num_sommet)
			{
				trouve = true;
			}
			temp = temp->_v;
		}
		if(temp->_num_sommet == num_sommet)
		{
			trouve = true;
		}
		if (!trouve)
		{
			(*S)->_nb_voisin++;
			New_Voisin_Suivant(&temp,NewVoisin(num_sommet,poid));
		}
	}

}

void Supp_Graph_Sommet(Graph_Sommet S)
{
	Vider_Voisin(S->_v);
	free(S);
}

//partie pour les graphes
Graphe New_Graphe()
{
	Graphe temp = (Graphe)malloc(sizeof(Ggraphe));
	temp->_nb_sommet = 0;
	temp->_tab_sommet = NULL;
	return temp;
}

bool Est_Vide_Graphe(Graphe G)
{
	return (G == NULL) || (G->_nb_sommet<0);
}

void New_Sommet(Graphe *G,int num_sommet)
{
	(*G)->_nb_sommet++;
	(*G)->_tab_sommet = (Graph_Sommet *)realloc((*G)->_tab_sommet,(*G)->_nb_sommet*sizeof(GSommet));
	Graph_Sommet temp = New_Graph_Sommet(num_sommet);
	(*G)->_tab_sommet[num_sommet] = temp;
}

void New_voisin(Graphe *G,int num_sommet,int num_voisin,double poids)
{
	Graph_Sommet temp = (*G)->_tab_sommet[num_sommet];
	Add_Graph_Sommet_Voisin(&temp,num_voisin,poids);
}

int Get_Nb_Sommet(Graphe G)
{
	return G->_nb_sommet;
}

Graph_Sommet Get_Sommet(Graphe G,int num_sommet)
{
	return G->_tab_sommet[num_sommet];
}


bool Est_Dans(Graphe G, int num)
{
	return ((G->_nb_sommet-1) >= num);
}

void Afficher_Graphe(Graphe G)
{
	cout<<"Graphe"<<endl;
	cout<<"Nombre sommet : "<<G->_nb_sommet<<endl;
	for (int i = 0; i < G->_nb_sommet;i++)
	{
		cout<<"Sommet : "<<G->_tab_sommet[i]->_num_sommet<<endl;
		cout<<"Nombre de voisins : "<<G->_tab_sommet[i]->_nb_voisin<<endl;
		Graph_Voisin temp = G->_tab_sommet[i]->_v;
		while ( temp != NULL)
		{
			cout<<"Numerot sommet voisin : "<<temp->_num_sommet<<" Poid de l'arrete "<<temp->_poid<<" -> ";
			temp = temp->_v;
		}
		cout<<endl;
	}
}

void Vider_Graphe(Graphe G)
{
	if( G != NULL)
	{
		for (int i = 0; i<G->_nb_sommet; i++)
		{
			Supp_Graph_Sommet(G->_tab_sommet[i]);
		}
		free(G->_tab_sommet);
		free(G);
	}
}