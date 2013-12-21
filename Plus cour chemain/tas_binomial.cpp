#include "GestionMemoire.h"
#include "tas_binomial.h"
#include<iostream>
using namespace std;

Table TableHachage(int nb)
{
	Table Hach;
	Hach = (Table)malloc(sizeof(TTas*));
	Hach->_nb = nb;
	Hach->_table_hash = (Tas*)MonMalloc(sizeof(Arbre)*Hach->_nb);
	Hach->_tas = NULL;
	return Hach;
}

bool EstVideTas(Table table)
{
	return (table->_tas == NULL);
}

Tas Pere(Tas tas)
{
	return tas->_pere;
}

Tas FilsGauche(Tas tas)
{
	return tas->_FG;
}

Tas FrereDroit(Tas tas)
{
	return tas->_FD;
}

Tas ConsVideTas()
{
	return NULL;
}

void ViderTas(Tas tas)
{
	if (tas != NULL)
	{
		ViderTas(tas->_pere);
		ViderTas(tas->_FD);
		ViderTas(tas->_FD);
		MonFree(tas);
	}
}

void ViderTable(Table table)
{
	if (table->_tas != NULL)
	{
		ViderTas(table->_tas);
		ViderTas(table->_min);
	}
	MonFree(table);
}

Tas FusionTasBinomiaux(Tas h1, Tas h2)
{
	Tas h = ConsVideTas();
	Tas queue = ConsVideTas();
	
	while ( h1 != NULL && h2 != NULL){
		if (h1->degre > h2->degre)
		{
			if(queue == NULL)
			{
				queue = h = h2;
			}
			else
			{
				queue->_FD = h2;
				queue = queue->_FD;
			}
			h2= h2->_FD;
		}
		else
		{
			if(queue == NULL)
			{
				queue = h = h1;
			}
			else
			{
				queue->_FD = h1;
				queue = queue->_FD;
			}
			h1= h1->_FD;
		}
	}
	if (h1 == NULL){
		queue->_FD = h2;
	}else{
		queue->_FD = h1;
	}
	
	return h;
}

void LienBinomial(Tas *tas1, Tas *tas2)
{
	(*tas1)->_pere = (*tas2);
	(*tas1)->_FD = (*tas2)->_FG;
	(*tas2)->_FG = (*tas1);
	(*tas2)->degre++;
}

Tas UnionTasBinomiaux(Tas tas1, Tas tas2)
{
	if(!tas2)
	{
		return tas1;
	}
	if(!tas1)
	{
		return tas2;
	}

	Tas avant_x, x, apres_x;
	Tas temp = ConsVideTas();
	temp = FusionTasBinomiaux(tas1, tas2);

	if (temp == NULL)
	{
		return temp;
	}
	else 
	{
		avant_x = NULL;
		x = temp;
		apres_x = (x->_FD);
		while (apres_x != NULL)
		{
			if (((x->degre) != (apres_x->degre)) || ((apres_x->_FD != NULL) && ((apres_x->_FD->degre) == x->degre)))
			{
				avant_x = x;
				x = apres_x;
			}
			else if ((x->_poids) <= (apres_x->_poids))
			{
				x->_FD = apres_x->_FD;
				LienBinomial(&apres_x,&x);
			}
			else
			{
				if (avant_x == NULL)
				{
					temp = apres_x;
				}
				else
				{
					avant_x->_FD = apres_x;
				}
				LienBinomial(&x, &apres_x);
				x = apres_x;
			}
			apres_x = x->_FD;
		}
		return temp;
	}
}	

void Afficher(Tas tas)
{
	if(tas != NULL)
	{
		cout<<"numsommet "<<tas->_num_sommet<<endl;
		cout<<"poids "<<tas->_poids<<endl;
		cout<<"FD "<<tas->_FD<<endl;
		cout<<"FG "<<tas->_FG<<endl;
		cout<<"degre "<<tas->degre<<endl;
		Afficher(tas->_FD);
		Afficher(tas->_FG);
	}
}
	

void Inserer(Table *table, int num_sommet,double cle,int num_pere)
{
	Tas tas2 = (Tas)MonMalloc(sizeof(Arbre));
	tas2->_poids = cle;
	tas2->degre = 0;
	tas2->_num_sommet = num_sommet;
	tas2->_FD = NULL;
	tas2->_pere = NULL;
	tas2->_FG = NULL;
	(*table)->_table_hash[num_sommet] = (tas2);
	(*table)->_tas = UnionTasBinomiaux((*table)->_tas,tas2);
}

Tas Minimum(Table *table) //stocke le minimum dans la table, et retourne le précedent de minimum
{
	double min = 9999999999999 ;
	Tas x = (*table)->_tas;
	Tas y = NULL;
	Tas temp = NULL;
	while(x != NULL)
	{
		if((x->_poids) < min)
		{
			min = x->_poids;
			y = x;
		}
		x = x->_FD;
	}
	(*table)->_min = y;

	x = (*table)->_tas;
	while(x != NULL)
	{
		if(x->_FD == y)
		{
			temp = x;
		}
		x = x->_FD;
	}
	
	return temp;
}

void Inverse(Tas *tas)
{
	Tas queue = NULL;
	Tas suivant = NULL;
	if((*tas) != NULL)
	{
		while((*tas)->_FD)
		{
			suivant = (*tas)->_FD;
			(*tas)->_FD = queue;
			queue = (*tas);
			(*tas) = suivant;
			(*tas)->_pere = NULL;
		}
		(*tas)->_FD = queue;
	}
}

double Extraire_Min_Tas(Table table, int *num_sommet, int *pere)
{
	Tas z = Minimum(&table);
	if (table->_min == NULL)
	{
		(*num_sommet) = -1;
		(*pere) = -1;
	}
	else
	{
		Tas temp = table->_min->_FG;
		if (temp != NULL)
		{
			table->_min->_FG = NULL;
			temp->_pere = NULL;//
		}
		if(z)
		{
			z->_FD = table->_min->_FD;
			table->_min->_FD = NULL;
		}

		Inverse(&temp);
		if (table->_min == table->_tas)
		{
			table->_tas = NULL;
		}
		table->_tas = UnionTasBinomiaux(table->_tas,temp);
		int temp_num = table->_min->_num_sommet;
		int temp_pere = table->_min->_num_pere;
		double temp_cle = table->_min->_poids;
		MonFree(table->_min);

		(*num_sommet) = temp_num;
		(*pere) = temp_cle;
		return temp_cle;
	}
}

double Cle_tas(Table table, int num_sommet)//retourne la valeur du poids
{
	return (table->_table_hash[num_sommet]->_poids);
}

void Diminuer_Cle_Tas(Table table,int num_sommet,double cle)
{
	if(cle < table->_table_hash[num_sommet]->_poids)
	{
		table->_table_hash[num_sommet]->_poids = cle;
		Tas y = table->_table_hash[num_sommet];
		Tas z = table->_table_hash[num_sommet]->_pere;
		while((z != NULL) && (y->_poids < z->_poids))
		{
			double temp_poids = y->_poids;
			int temp_num_sommet = y->_num_sommet;
			int temp_pere = y->_num_pere;
			y->_poids = z->_poids;
			y->_num_sommet = z->_num_sommet;
			y->_num_pere = z->_num_pere;

			z->_poids = temp_poids;
			z->_num_sommet = temp_num_sommet;
			z->_num_pere = temp_pere;

			y = z;
			z = y->_pere;
		}
	}
}
		
bool Est_Dans_Tas(Table table, int num_sommet)
{
	bool res = false;
	for(int i=0; i<table->_nb; i++)
	{
		if(table->_table_hash[i] != NULL)
		{
			if(table->_table_hash[i]->_num_sommet == num_sommet)
			{
				res = true;
			}
		}
	}
	return res;
}

void Set_Pere_Tas(Table *table,int num_sommet,int pere)
{
	(*table)->_table_hash[num_sommet]->_num_pere = pere;
}
