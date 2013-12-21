#include "graphe.h"
#include "fonction.h"
#include "tas_binomial.h"
#include "tableau.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace std;

int Menu()
{
	int retour;
	cout<<"---------------------------------------------------------------"<<endl;
	cout<<"0  Charger une scene"<<endl;
	cout<<"1  Charger graphe"<<endl;
	cout<<"2  Generer un graphe"<<endl;
	cout<<"3  Afficher le graphe"<<endl;
	cout<<"4  Sauver le graphe"<<endl;
	cout<<"5  Algo Dijkstra"<<endl;
	cout<<"11 Quitter"<<endl;
	cout<<"---------------------------------------------------------------"<<endl;
	cin>>retour;
	return retour;
}

void Charger_Graphe(Graphe *G)
{
	FILE* Fichier;
	char* NomFichier = (char*)malloc(255*sizeof(char));
	cout<<"Veuillez donner le nom du fichier a charger"<<endl;
	cin>>NomFichier;

	Fichier = fopen(NomFichier,"rt");
	if (Fichier == NULL)
	{
		cout<<"Echec du chargement du fichier"<<endl;
	}
	else
	{
		cout<<"Ouverture du fichier"<<endl;
		Fichier_en_Graphe(Fichier,G);
		cout<<"Graphe charge"<<endl;
		fclose(Fichier);
	}
	free(NomFichier);
}

void Fichier_en_Graphe(FILE* Fichier,Graphe *G)
{
	int nb_sommet;
	int num_sommet = 0;
	int nb_voisin = 0;
	int num_voisin;
	double pd_arrete;
	if (Fichier)
	{
		fscanf(Fichier,"%ld", &nb_sommet);
		//parse les lignes pour les Sommet et voisin;

		while(fscanf(Fichier,"%ld", &nb_voisin) >0) //tant qu'on ne lit pas au dela du fichier
		{
			if (!Est_Dans((*G),num_sommet))
			{
				New_Sommet(G,num_sommet);
			}
			for(int i = 0;i < nb_voisin;i++)
            {
				fscanf(Fichier,"%ld%lf", &num_voisin, &pd_arrete);
				New_voisin(G,num_sommet,num_voisin,pd_arrete);
				if (!Est_Dans((*G),num_voisin))
				{
					for (int j=i+1;j<=num_voisin;j++)
					{
						New_Sommet(G,j);
					}
				}
				New_voisin(G,num_voisin,num_sommet,pd_arrete);
            }
			num_sommet++;
		}
	}
}

void Sauver_Graphe(Graphe G)
{
	FILE* Fichier;
	char* NomFichier = (char*)malloc(255*sizeof(char));
	cout<<"Veuillez donner le nom du fichier a sauver"<<endl;
	cin>>NomFichier;

	Fichier = fopen(NomFichier,"wt");
	if (Fichier == NULL)
	{
		cout<<"Echec du chargement du fichier"<<endl;
	}
	else
	{
		cout<<"Ouverture du fichier"<<endl;
		Graphe_en_Fichier(Fichier,G);
		cout<<"Graphe sauver"<<endl;
		fclose(Fichier);
	}
	free(NomFichier);
}

void Graphe_en_Fichier(FILE *Fichier,Graphe G)
{
	int nb_sommet = Get_Nb_Sommet(G);
	Graph_Sommet temp,temp3;
	Graph_Voisin temp2,z;
	int nb_voisin,nb_voisin_marque;
	bool marque;
	fprintf(Fichier,"%i\n",nb_sommet);
	for(int i = 0; i < (nb_sommet-1); i++)
	{
		temp = Get_Sommet(G,i);
		nb_voisin = Get_Nb_Voisin(temp);
		nb_voisin_marque = 0;
		temp2 = Get_Voisin(temp);
		for(int j = 1; j < nb_voisin;j++)
		{
			if(Est_Marque(temp2))
			{
				nb_voisin_marque++;
			}
			else
			{
				marque = false;
				temp3 = Get_Sommet(G,Num_Sommet_Voisin(temp2));
				z = Get_Voisin(temp3);
				while(!Est_Vide_Voisin(z))
				{
					if (Num_Sommet_Voisin(z) == j)
					{
						marque = true;
						break;
					}
					z = Voisin_Suivant(z);
				}
				if (marque)
				{
					Marque(&z);
					cout<<z->_marque<<endl;
				}
			}
			temp2 = Voisin_Suivant(temp2);
		}
		nb_voisin = (nb_voisin-1) - nb_voisin_marque;
		fprintf(Fichier,"%i\n",(nb_voisin));
		nb_voisin = Get_Nb_Voisin(temp);
		temp2 = Get_Voisin(temp);
		for(int j = 1; j < nb_voisin;j++)
		{
			cout<<"Le voisin "<<temp2->_num_sommet<<" Marque : "<<temp2->_marque<<endl;
			if(!Est_Marque(temp2))
			{
				cout<<"Ecriture dans le fichier alors"<<endl;
				fprintf(Fichier,"%i %f\n",Num_Sommet_Voisin(temp2),Get_Poid_Voisin(temp2));
			}
			temp2 = Voisin_Suivant(temp2);
		}
	}
}

void Genration(Graphe *G)
{
	int nb_sommet,densite;
	bool conex;
	cout<<"Tapez le nombre de sommet"<<endl;
	cin>>nb_sommet;
	cout<<"Connex 1 / Non Connex 0"<<endl;
	cin>>conex;
	cout<<"Densité en % (ex pour 20% ecrire 20)"<<endl;
	cin>>densite;	
	Rand_Graphe(G,nb_sommet,conex,densite);
}

void Rand_Graphe(Graphe *G,int nb_sommet,bool conex,int densite)
{
	if (conex)
	{
		Rand_Graphe_Annexe(G,0,nb_sommet-1,densite);
	}
	else
	{
		Rand_Graphe_Annexe(G,0,(nb_sommet/2)-1,densite);
		Rand_Graphe_Annexe(G,(nb_sommet/2),nb_sommet-1,densite);
	}
}

void Rand_Graphe_Annexe(Graphe *G,int num_sommet_dep, int num_sommet_fin, double densite)
{
	double pd_arrete;
	int num_sommet_voisin;
	int nb_voisin =((num_sommet_fin-num_sommet_dep)*(densite/100));
	cout<<"Num sommet depart "<<num_sommet_dep<<" Num sommet fin "<<num_sommet_fin<<endl;
	cout<<"Nombre de voisin par sommet "<<nb_voisin<<endl;
	for(int i = num_sommet_dep; i <= num_sommet_fin; i++)
	{
		pd_arrete = rand()%(100);
		if (!Est_Dans((*G),i))
		{
			New_Sommet(G,i);
		}
		if((i+1)<= num_sommet_fin)
		{
			if (!Est_Dans((*G),i+1))
			{
				New_Sommet(G,i+1);
			}
			New_voisin(G,i,i+1,pd_arrete);
			New_voisin(G,i+1,i,pd_arrete);
		}
		pd_arrete = rand()%(100);

		for(int j = num_sommet_dep; j <= nb_voisin ;j++)
		{
			num_sommet_voisin = rand()%(num_sommet_fin-num_sommet_dep)+num_sommet_dep;
			for (int z=i+2;z <= num_sommet_voisin;z++)
			{
				if(!Est_Dans((*G),z))
				{
					New_Sommet(G,z);
				}
			}
			New_voisin(G,i,num_sommet_voisin,pd_arrete);
			New_voisin(G,num_sommet_voisin,i,pd_arrete);
		}
	}
}

void Dijkstra(Graphe G)
{
	bool type;
	int depart,arriver;
	double longeur_chemin;

	cout<<"Tas binomiaux 1/ Tableaux 0"<<endl;
	cin>>type;
	cout<<"Numero Sommet depart"<<endl;
	cin>>depart;
	cout<<"Numero Sommet arrive"<<endl;
	cin>>arriver;
	
	int temp = clock();
	if(type)
	{
		longeur_chemin = Dijkstra_tas(G,depart,arriver);
	}
	else
	{
		longeur_chemin = Dijkstra_tab(G,depart,arriver);
	}
	temp = clock()-temp;
	cout<<"longeur du chemin : "<<longeur_chemin<<endl;
	cout<<"Temps de calcul : "<<temp<<" millisecondes"<<endl;
}

double Dijkstra_tas(Graphe G, int depart, int arriver)
{
	double cle = 0;
	Table tas_bino = TableHachage(Get_Nb_Sommet(G));
	for (int i = 0; i < Get_Nb_Sommet(G); i++)
	{
		Inserer(&tas_bino,i,999999,NULL);
	}
	Diminuer_Cle_Tas(tas_bino, depart, 0);
	int min,pere,num_sommet_voisin;
	int *retour = (int*)malloc(Get_Nb_Sommet(G)*sizeof(int));
	Graph_Voisin temp;
	while(!EstVideTas(tas_bino))
	{
		cle = Extraire_Min_Tas(tas_bino,&min,&pere);
		cout<<"Sommet "<<min<<" Pere "<<pere<<" Cle "<<cle<<endl;
		retour[min] = pere;
		temp = Get_Voisin(Get_Sommet(G,min));
		while(!Est_Vide_Voisin(temp))
		{
			num_sommet_voisin = Num_Sommet_Voisin(temp);
			cout<<"Sommet "<<num_sommet_voisin<<endl;
			cout<<"Cle "<<Cle_tas(tas_bino,num_sommet_voisin)<<endl;
			cout<<"Est dans tas "<<Est_Dans_Tas(tas_bino,num_sommet_voisin)<<endl;
			if(Est_Dans_Tas(tas_bino,num_sommet_voisin) 
				&& (Cle_tas(tas_bino,num_sommet_voisin) > cle + Get_Poid_Voisin(temp)))
			{
				cout<<"Diminiuer cle"<<endl;
				Diminuer_Cle_Tas(tas_bino,num_sommet_voisin,Cle_tas(tas_bino,num_sommet_voisin)+cle);
				cout<<"Pere "<<min<<endl;
				Set_Pere_Tas(&tas_bino,num_sommet_voisin,min);
			}
			temp = Voisin_Suivant(temp);
		}
	}

	ViderTable(tas_bino);
	int temp_sommet = arriver;
	double poids = 0;
	while (temp_sommet != depart)
	{
		temp = Get_Voisin(Get_Sommet(G,temp_sommet));
		while(!Est_Vide_Voisin(temp))
		{
			if(Num_Sommet_Voisin(temp) == retour[temp_sommet])
			{
				poids = poids+Get_Poid_Voisin(temp);
			}
			temp = Voisin_Suivant(temp);
		}
		temp_sommet = retour[temp_sommet];
	}

	free(retour);
	return poids;
}


double Dijkstra_tab(Graphe G, int depart, int arriver)
{
	double cle = 0;
	TTab tableau = CreerTableau(Get_Nb_Sommet(G));
	for (int i = 0; i < Get_Nb_Sommet(G); i++)
	{
		Inserer_Tab(&tableau,i,99999);
	}
	DiminuerCle(&tableau, depart, 0);
	int min,pere,num_sommet_voisin;
	Graph_Voisin temp;
	int *retour = (int*)malloc(Get_Nb_Sommet(G)*sizeof(int));
	while(!Est_Vide_Tab(tableau))
	{
		cle = ExtraireMin(&tableau,&min,&pere);
		retour[min] = pere;
		temp = Get_Voisin(Get_Sommet(G,min));
		while(!Est_Vide_Voisin(temp))
		{
			num_sommet_voisin = Num_Sommet_Voisin(temp);
			if(EstDans(tableau,num_sommet_voisin) && 
				(Cle(tableau,num_sommet_voisin) > (cle + Get_Poid_Voisin(temp))))
			{
				DiminuerCle(&tableau,num_sommet_voisin,(cle+Get_Poid_Voisin(temp)));
				Set_Pere_Tab(&tableau,num_sommet_voisin,min);
			}
			temp = Voisin_Suivant(temp);
		}
	}
	
	Vider_Tableau(tableau);
	int temp_sommet = arriver;
	double poids = 0;
	while (temp_sommet != depart)
	{
		temp = Get_Voisin(Get_Sommet(G,temp_sommet));
		while(!Est_Vide_Voisin(temp))
		{
			if(Num_Sommet_Voisin(temp) == retour[temp_sommet])
			{
				poids = poids+Get_Poid_Voisin(temp);
			}
			temp = Voisin_Suivant(temp);
		}
		temp_sommet = retour[temp_sommet];
	}

	free(retour);
	return poids;
}
