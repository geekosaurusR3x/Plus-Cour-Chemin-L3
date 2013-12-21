#include "graphe.h"
#include <iostream>

int Menu();
void Charger_Graphe(Graphe *);
void Fichier_en_Graphe(FILE*, Graphe *);
void Sauver_Graphe(Graphe);
void Graphe_en_Fichier(FILE*, Graphe);
void Genration(Graphe *);
void Rand_Graphe(Graphe *,int,bool,int);
void Rand_Graphe_Annexe(Graphe *,int,int,double);
void Dijkstra(Graphe);
double Dijkstra_tas(Graphe ,int,int);
double Dijkstra_tab(Graphe ,int,int);