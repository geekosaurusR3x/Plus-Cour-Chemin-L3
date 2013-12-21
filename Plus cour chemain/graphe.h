#ifndef _GRAPHE
#define _GRAPHE

    typedef struct Voisin
    {
        int _num_sommet;
        double _poid;
		struct Voisin *_v;
		bool _marque;
    } GVoisin, *Graph_Voisin;

    
	typedef struct Sommet
    {
        int _nb_voisin;
		int _num_sommet;
		struct Voisin *_v;
    } GSommet, *Graph_Sommet;

	typedef struct graphe
	{
		int _nb_sommet;
		Graph_Sommet *_tab_sommet;
	} Ggraphe, *Graphe;

	//partie pour les voisins
	Graph_Voisin NewVoisin(int,double);
	bool Est_Vide_Voisin(Graph_Voisin);
	double Get_Poid_Voisin(Graph_Voisin);
	bool Est_Marque(Graph_Voisin);
	void Marque(Graph_Voisin *);
	Graph_Voisin Voisin_Suivant(Graph_Voisin);
	int Num_Sommet_Voisin(Graph_Voisin);
	void New_Voisin_Suivant(Graph_Voisin *,Graph_Voisin);
	void Vider_Voisin(Graph_Voisin);
	
	//partie pour les sommets
	Graph_Sommet New_Graph_Sommet(int);
	int Get_Nb_Voisin(Graph_Sommet);
	Graph_Voisin Get_Voisin(Graph_Sommet);
	void Add_Graph_Sommet_Voisin(Graph_Sommet *, int, double);
	void Supp_Graph_Sommet(Graph_Sommet);

	//partie pour les graphes
	Graphe New_Graphe();
	bool Est_Vide_Graphe(Graphe);
	void New_Sommet(Graphe *,int);
	void New_voisin(Graphe *,int,int,double);
	int Get_Nb_Sommet(Graphe);
	Graph_Sommet Get_Sommet(Graphe,int);
	bool Est_Dans(Graphe, int);
	void Afficher_Graphe(Graphe);
	void Vider_Graphe(Graphe);

	

#endif