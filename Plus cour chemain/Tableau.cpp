#include "tableau.h"
#include <iostream>
using namespace std;

TTab CreerTableau(int nb)
{
	TTab tab;
	tab.nb = 0;
	tab.tab = (TCle*)malloc (sizeof(TCle)*nb);
	for(int i=0; i< nb;i++)
	{
		tab.tab[i]._num_pere=-1;
		tab.tab[i]._num_sommet=-1;
		tab.tab[i]._poids=99999999999;
	}
	return tab;
}

double ExtraireMin(TTab *tab, int *min, int* num_pere)
{
	*min = tab->tab[tab->nb - 1]._num_sommet;
	*num_pere = tab->tab[tab->nb - 1]._num_pere;
	double temp = tab->tab[tab->nb -1]._poids;
	Supprimer(tab,tab->nb - 1);
	return temp;
}

bool EstDans(TTab tab, int num_sommet)
{
	for(int i=0; i<=tab.nb-1; i++)
	{
		if(tab.tab[i]._num_sommet == num_sommet)
			return true;
	}
	return false;
}

double Cle(TTab tab, int num_sommet)
{
	double poids;
	for(int i=0; i <= tab.nb-1; i++)
	{
		if(tab.tab[i]._num_sommet == num_sommet)
			poids = tab.tab[i]._poids;
			
	}
	return poids;
}

void Inserer_Tab(TTab *tab, int num_sommet, double poids)
{
	int i=0;
	while(tab->tab[i]._poids <= poids)
	{
		i++;
	}
	
	TCle temp;
	temp._num_pere = tab->tab[i+1]._num_pere;
	temp._num_sommet = tab->tab[i+1]._num_sommet;
	temp._poids = tab->tab[i+1]._poids;
	TCle temp2;

	for (int j=i+1; j < tab->nb; j++)
	{
		
		temp2._num_pere = tab->tab[j+1]._num_pere;
		temp2._num_sommet = tab->tab[j+1]._num_sommet;
		temp2._poids = tab->tab[j+1]._poids;
			
		tab->tab[j+1]._num_pere = temp._num_pere;
		tab->tab[j+1]._num_sommet = temp._num_sommet;
		tab->tab[j+1]._poids = temp._poids;
		
		temp._num_pere = temp2._num_pere;
		temp._num_sommet = temp2._num_sommet;
		temp._poids = temp2._poids;
	}

	tab->tab[i]._poids = poids;
	tab->tab[i]._num_sommet = num_sommet;
	tab->nb++;
}
		
void DiminuerCle(TTab *t, int num_sommet, double poids)
{
	for(int i=0; i<t->nb;i++)
	{
		if (t->tab[i]._num_sommet == num_sommet)
		{
			if (t->tab[i]._poids >= poids)
			{
				t->tab[i]._poids = poids;
			}
		}
	}
	(*t) = Tri((*t));
}		

void Supprimer (TTab *tab, int num_sommet)
{
	tab->nb = tab->nb - 1;
}

bool Est_Vide_Tab(TTab tab)
{
	return (tab.nb <= 0);
}


TTab Tri(TTab tableau)
{
	int debut = 0;
	int fin = tableau.nb-1;
	TCle temp;

	for(int i = debut; i<= fin; i++)
	{
		for(int j = i+1; j<=fin;j++)
		{
			if(tableau.tab[i]._poids < tableau.tab[j]._poids)
			{
				temp._num_pere = tableau.tab[i]._num_pere;
				temp._num_sommet = tableau.tab[i]._num_sommet;
				temp._poids = tableau.tab[i]._poids;

				tableau.tab[i]._num_pere = tableau.tab[j]._num_pere;
				tableau.tab[i]._num_sommet = tableau.tab[j]._num_sommet;
				tableau.tab[i]._poids = tableau.tab[j]._poids;

				tableau.tab[j]._num_pere = temp._num_pere;
				tableau.tab[j]._num_sommet = temp._num_sommet;
				tableau.tab[j]._poids = temp._poids;

			}

		}
	}
	return tableau;
}
void Affiche_tableau(TTab tableau)
{
	cout<<"----------------------------------------------------------------------------"<<endl;
	for(int i=0; i<tableau.nb;i++)
	{
		cout<<"case "<<i<<endl;
		cout<<"Num-sommet "<<tableau.tab[i]._num_sommet<<endl;
		cout<<"Num-pere "<<tableau.tab[i]._num_pere<<endl;
		cout<<"Poid "<<tableau.tab[i]._poids<<endl;
	}
	cout<<"----------------------------------------------------------------------------"<<endl;
	system("pause");
}

void Vider_Tableau(TTab tableau)
{
	free(tableau.tab);
}

void Set_Pere_Tab(TTab *tab, int num_sommet,int pere)
{
	for(int i=0; i<=tab->nb-1; i++)
	{
		if(tab->tab[i]._num_sommet == num_sommet)
			tab->tab[i]._num_pere = pere;
	}
}