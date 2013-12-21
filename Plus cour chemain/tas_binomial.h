#ifndef _TasBino
#define _TasBino

typedef struct A
{
	A *_pere;
	A *_FG; //fils le plus a gauche
	A *_FD; //frère droit
	int _num_pere,degre,_num_sommet;
	double _poids;
}Arbre, *Tas;

typedef struct tab
{
	Tas _min;
	Tas _tas;
	Tas *_table_hash;
	int _nb;
}TTas, *Table;

	void Afficher(Tas);
	Table TableHachage(int);
	bool EstVideTas(Table);
	Tas Pere(Tas);
	Tas FilsGauche(Tas);
	Tas FrereDroit(Tas);
	Tas ConsVideTas();
	Tas FusionTasBinomiaux(Tas, Tas);
	void LienBinomial(Tas*, Tas*);
	Tas UnionTasBinomiaux(Tas,Tas);
	void Inserer(Table *,int,double,int);
	double Cle_tas(Table,int);
	Tas Minimum(Table*);
	void Inverse(Tas*);
	double Extraire_Min_Tas(Table, int*, int*);
	void Diminuer_Cle_Tas(Table,int,double);
	bool Est_Dans_Tas(Table, int);
	void ViderTas(Tas);
	void Set_Pere_Tas(Table *,int,int);
	void ViderTable(Table);

#endif