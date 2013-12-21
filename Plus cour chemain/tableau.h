#ifndef _TABLEAU_
#define _TABLEAU_

typedef struct
{
	double _poids;
	int _num_sommet;
	int _num_pere;
}TCle;

typedef struct
{
	int nb;
	TCle *tab;
}TTab;

TTab CreerTableau(int);
double ExtraireMin(TTab*, int*, int*);
bool EstDans(TTab, int);
double Cle(TTab, int);
void Inserer_Tab(TTab*, int, double);
void DiminuerCle(TTab *, int , double);
void Supprimer(TTab *,int);
bool Est_Vide_Tab(TTab);
TTab Tri(TTab);
void Affiche_tableau(TTab);
void Vider_Tableau(TTab);
void Set_Pere_Tab(TTab *,int,int);


#endif _TABLEAU_