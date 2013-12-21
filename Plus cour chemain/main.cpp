// Plus cour chemin.cpp : Defines the entry point for the console application.
//
#include "graphe.h"
#include "tableau.h"
#include "tas_binomial.h"
#include "GestionMemoire.h"
#include "fonction.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	long int    NombreOctetsAlloues ;
	int        NbPointeursNonLiberes ;
	int menu = -1;
	Graphe graphe = NULL;

	while( menu != 11)
	{
		menu = Menu();
		switch (menu)
		{
		case 1:
			Vider_Graphe(graphe);
			graphe = New_Graphe();
			Charger_Graphe(&graphe);
			break;
		case 2:
			Vider_Graphe(graphe);
			graphe = New_Graphe();
			Genration(&graphe);
			break;
		case 3:
			if (!Est_Vide_Graphe(graphe))
			{
				Afficher_Graphe(graphe);
			}
			else
			{
				cout<<"Erreure : le graphe est vide"<<endl;
			}
			break;
		case 4:
			if (!Est_Vide_Graphe(graphe))
			{
				Sauver_Graphe(graphe);
			}
			else
			{
				cout<<"Erreure : le graphe est vide"<<endl;
			}
			break;
		case 5:
			if (!Est_Vide_Graphe(graphe))
			{
				Dijkstra(graphe);
			}
			else
			{
				cout<<"Erreure : le graphe est vide"<<endl;
			}
			break;
		default:
			cout<<"Fonction non presente"<<endl;
			break;
		}
	}


	//Vidage memoire
	if (!Est_Vide_Graphe(graphe))
	{
		Vider_Graphe(graphe);
	}
	//verification de la memoire
	VerificationMemoire (&NbPointeursNonLiberes, &NombreOctetsAlloues) ;
	cout << "Il reste " << NbPointeursNonLiberes << " pointeurs non libérés, ce qui represente " << NombreOctetsAlloues << " octets" << endl ;
	
	system("pause");
	return 0;
}

