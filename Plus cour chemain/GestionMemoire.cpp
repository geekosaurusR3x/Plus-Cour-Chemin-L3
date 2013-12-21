/***************************************************
	ATTENTION :

	1) En cas de tentative de libérer un pointeur non alloué ou déjà libéré, un message est 
	affiché (voir l'unique cout dans la suite). Si l'appli n'est pas en mode console, utiliser
	un autre moyen pour faire apparaître le message d'erreur

	2) NE PAS UTILISER LES EXCEPTIONS autrement que dans des
	cas d'urgence. Sinon, il est probable que la gestion de la mémoire sera déclarée
	maladroite même si elle ne l'est pas.

	3) Retirez l'inclusion à stdafx.h ci-dessous si vous n'êtes pas sous Visual C++ ou si vous
	n'utilisez pas les entêtes précompilés

	4) Quand le programme est débuggué, sortir cette unité du projet, remplacer les éventuels 
	MonMalloc par des malloc et les éventuels MonFree par des free, supprimer les inclusions à 
	GestionMemoire.h et passer en mode release pour gagner encore en rapidité

***************************************************/

//#include "stdafx.h"				// SEULEMENT sous Visual C++ en mode "entêtes précompilés"
#include "GestionMemoire.h"
#include <iostream>

using namespace std ;





// Booléen disant s'il faut ou non comptabiliser une allocation
// ou une désallocation
static bool FautCompter = true ;


#define CONTENEURS_DLL

//*****************************************************************************
//
// Classe Conteneurs
//
//*****************************************************************************

template <class X> class CONTENEURS_DLL  Conteneurs
{
public :

	//*****************************************************************************
	//	Constructeurs et destructeur
	//*****************************************************************************

	Conteneurs ()
	: nb_Elements (0)
	{
	}

	Conteneurs (const Conteneurs<X> &C)
	: nb_Elements (C.nb_Elements)
	{
	}

	Conteneurs (unsigned long Nb_Elem)
	: nb_Elements (Nb_Elem)
	{
	}

	virtual ~Conteneurs ()
   {
   }


	//*****************************************************************************
	//	Accesseurs
	//*****************************************************************************

	unsigned long Nb_Elements () const;


	//*****************************************************************************
	//	Methodes
	//*****************************************************************************

	bool Est_Vide () const;
	virtual void Fusion (	const Conteneurs<X> &CSource,
									Conteneurs<X> &CResultat) const;
   virtual bool Est_Dans (const X &Elem) const		= 0;
   virtual bool Est_Dans ( const X&, unsigned long&) const	= 0 ;
	virtual Conteneurs<X>& Detruit ();
	virtual void Tout_Parcourir (
	void (* Fonction) (	X &, void *,unsigned long),
								void *Donnee = NULL ,
                        unsigned long PosDebut=0 ) const		= 0;

	//*****************************************************************************
	//	Surcharge des operateurs
	//*****************************************************************************

	const Conteneurs<X>& operator = (const Conteneurs<X> &);
	const Conteneurs<X>& operator () (const Conteneurs<X> &);
	/*virtual*/ const Conteneurs<X>& operator + (const Conteneurs<X> &);
	bool operator <= (const Conteneurs<X> &);
	bool operator == (const Conteneurs<X> &);

	friend ostream& CONTENEURS_DLL operator << (ostream &, const Conteneurs<X> &);


protected :

		unsigned long	nb_Elements;
};



template <class X> unsigned long Conteneurs<X>::Nb_Elements () const
{
	return nb_Elements;
}


template <class X> bool Conteneurs<X>::Est_Vide () const
{
	return (nb_Elements == 0);
}

template <class X> Conteneurs<X>& Conteneurs<X>::Detruit ()
{
	nb_Elements = 0;

	return *this;
}


template <class X> void Conteneurs<X>::Fusion (
	const Conteneurs<X> &, Conteneurs<X> &) const
{
}


template <class X> ostream& operator << (ostream &s, const Conteneurs<X> &C)
{
	s << "Conteneurs (Nb_Elements : " << C.nb_Elements << ")";

	return s;
}


template <class X> const Conteneurs<X>& Conteneurs<X>::operator = (
	const Conteneurs<X> &O)
{
	if (&O != this)
		nb_Elements = O.nb_Elements;

	return *this;
}


template <class X> const Conteneurs<X>& Conteneurs<X>::operator () (
	const Conteneurs<X> &CSource)
{
	if (&CSource != this)
		nb_Elements = CSource.nb_Elements;


	return *this;
}


template <class X> const Conteneurs<X>& Conteneurs<X>::operator + (
	const Conteneurs<X> &CSource)
{
	nb_Elements += CSource.nb_Elements;

	return *this;
}


template <class X> bool Conteneurs<X>::operator <= (const Conteneurs<X> &C2)
{
	return this->nb_Elements <= C2.nb_Elements;
}


template <class X> bool Conteneurs<X>::operator == (const Conteneurs<X> &C2)
{
	return this->nb_Elements == C2.nb_Elements;
}

//*****************************************************************************
//
// Module	: Tables
//
// Fichier	: Tables.h
//
// Auteur	: Wies Eric
//
// Date creat.	: Mon Sep  9 16:03:08 MET DST 1996
//
// Version	: 1.0
//
// Modifification : 02/07/99 Heschung Fred,
//						  - InsertionParcours
//
// Commentaires	: Interface de la classe Tables.
//			Gestion de tableaux generiques et dynamiques
//
//*****************************************************************************
// Includes
// Classe Tables
//	Constructeurs et destructeur
//	Accesseurs
//	Methodes
//	Surcharge des operateurs
//	Partie privee
//*****************************************************************************


//*****************************************************************************
//
// Classe Tables
//
//*****************************************************************************

template <class X> class CONTENEURS_DLL Tables	: public Conteneurs<X>
{
public :


	//*****************************************************************************
	//	Constructeurs et destructeur
	//*****************************************************************************

	Tables (unsigned long Taille_Init = 0, unsigned long Pas_Agrandir = 10)
   : Conteneurs<X> (Taille_Init), pas_Agrandir (Pas_Agrandir), tab (NULL) // new X [Taille_Init])
	{
		if (Taille_Init > 0)
			tab = new X [Taille_Init];

		if (pas_Agrandir < 1)
			pas_Agrandir = 10;
	}

	Tables (const Tables<X> &T)
	: Conteneurs<X> (T), pas_Agrandir (T.pas_Agrandir),
	tab (NULL)
	{
		// delete [] tab ; Christian : j'enlève ce delete le 5-08-2000
	   if (nb_Elements > 0)	// Christian le 25 mars 204
			tab=new X[nb_Elements] ;

   	// Correction Christian le 05.08.2000 : je remplace le memcpy par une boucle
	   for (unsigned long i = 0 ; i < nb_Elements ; i++)
   		tab [i] = T.tab [i] ;
			// memcpy (tab,T.tab, nb_Elements*sizeof (X));
	}

	Tables (const Conteneurs<X> &CSource)
	: Conteneurs<X> (CSource), pas_Agrandir (10),tab (NULL)
	{
		// delete [] tab ; Christian : j'enlève ce delete le 5-08-2000
		if (nb_Elements > 0) {	// Christian le 17-07-2004
	   	tab=new X [nb_Elements] ;
			CSource.Tout_Parcourir (Placer_Element, (void *) this);
      }
	}

	virtual ~Tables ()
	{
		if (tab != NULL)
			delete [] tab;
	}


	//*****************************************************************************
	//	Accesseurs
	//*****************************************************************************

	unsigned long Pas_Agrandir () const;

	Tables<X>& Changer_Nb_Elements (unsigned long);
   // Change le nb de cases ALLOUEES à la valeur demandée.

	Tables<X>& Pas_Agrandir (unsigned long);


	//*****************************************************************************
	//	Methodes
	//*****************************************************************************

	bool Est_Dans (const X &) const;
	bool Est_Dans (const X &, unsigned long&) const;
   Conteneurs<X>& Detruit ();
   void Tout_Parcourir (	void (*Fonction) (X &, void *,unsigned long),
      							void *Donnee = NULL , unsigned long PosDebut=0 ) const;
	void Fusion (const Conteneurs<X> &,Conteneurs<X> &) const;


	//*****************************************************************************
	//	Surcharge des operateurs
	//*****************************************************************************

	const Tables<X>& operator = (const Tables<X> &);
	const Tables<X>& operator = (const Conteneurs<X> &);
	const Tables<X>& operator + (const Conteneurs<X> &);
	const Tables<X>& operator () (const Conteneurs<X> &);
	X& operator [] (unsigned long);
	const X& operator [] (unsigned long) const ;

   friend bool CONTENEURS_DLL  operator <= (const Tables<X> &, const Tables<X> &);
   friend bool CONTENEURS_DLL  operator == (const Tables<X> &, const Tables<X> &);

   friend ostream& CONTENEURS_DLL  operator << (ostream &, const Tables<X> &);


	private :

		static void Placer_Element (X &, void *,unsigned long);

		unsigned long	pas_Agrandir;

		X	*tab;
};


template <class X> unsigned long Tables<X>::Pas_Agrandir () const
{
	return pas_Agrandir;
}


template <class X> Tables<X>& Tables<X>::Pas_Agrandir (unsigned long Nouv_Pas)
{
	if (Nouv_Pas > 0)
		pas_Agrandir = Nouv_Pas;

	return *this;
}

template <class X> X& Tables<X>::operator [] (unsigned long Position)
{
	if (Position >= nb_Elements)
	{
		unsigned long	Nouv_Taille	= ((Position / pas_Agrandir) + 1) * pas_Agrandir;
		Changer_Nb_Elements (Nouv_Taille);
	}

	return tab  [Position];
}


template <class X> const X& Tables<X>::operator [] (unsigned long Position) const
{
	return tab  [Position];
}


template <class X> Tables<X>& Tables<X>::Changer_Nb_Elements (unsigned long Max_Elem)
{
	if ( nb_Elements != Max_Elem )
   {
		if (Max_Elem > 0)
		{
#ifdef DEBUG_TABLES
static unsigned long NbAppel=0 ;
NbAppel++ ;
Console<<"NbReallocations="<<(double)NbAppel<<"\n" ;
#endif DEBUG_TABLES

			X	*Tab_Temp	= new X [Max_Elem];
			unsigned long	Nb_ElemCopies;

			if (nb_Elements < Max_Elem)
				Nb_ElemCopies = nb_Elements;
			else
				Nb_ElemCopies = Max_Elem;

			if ( tab != NULL )
			{
  				for ( unsigned long i=0 ; i<Nb_ElemCopies ; i++)
            	Tab_Temp[i]=tab[i] ;

				delete [] tab;
         }

			tab = Tab_Temp;

		}
      else
      {
			if ( tab != NULL )
				delete [] tab;

         tab = NULL ;
      }

		nb_Elements = Max_Elem;
	}

	return *this;
}


template <class X> void Tables<X>::Tout_Parcourir (
	void (* Fonction) (X &, void *,unsigned long),
   void *Donnee , unsigned long PosDebut ) const
{
	for (unsigned long i = 0; i < nb_Elements; i ++)
		Fonction (tab [i], Donnee,i+PosDebut);
}


template <class X> void Tables<X>::Placer_Element (X &Elem, void *Donnee, unsigned long i )
// ATT. : il faut que le ième elt de tableau soit alloué
{
	Tables<X>	*T		= (Tables<X> *) Donnee;
	T -> tab [i] = Elem;
}


template <class X> bool Tables<X>::Est_Dans (const X &Elem) const
{
	unsigned long	Pos	= 0;
	bool	Trouve	= false;

	while ((! Trouve) && (Pos < nb_Elements))
		if (tab [Pos] == Elem)
			Trouve = true;
		else
			Pos ++;

	return Trouve;
}

template <class X> bool Tables<X>::Est_Dans (const X &Elem , unsigned long& Pos ) const
{
	Pos	= 0;
	bool	Trouve	= false;

	while ((! Trouve) && (Pos < nb_Elements))
		if (tab [Pos] == Elem)
			Trouve = true;
		else
			Pos ++;

	return Trouve;
}


template <class X> Conteneurs<X>& Tables<X>::Detruit ()
{
	if ( tab!=NULL )
   {
		// Christian le 05.08.2000 : je mets delete [] au lieu de delete
	   delete [] tab ;
      tab=NULL ;
   }
	nb_Elements=0 ;

	return *this;
}

template <class X> void Tables<X>::Fusion (
	const Conteneurs<X> &CSource, Conteneurs<X> &CResultat) const
{
	if ((Conteneurs<X> *) this != &CResultat)
		Tout_Parcourir (Placer_Element, (void *) &CResultat);

	if (&CSource  != &CResultat)
		CSource.Tout_Parcourir (Placer_Element, (void *) &CResultat);
}


template <class X> ostream& operator << (ostream &s, const Tables<X> &T)
{
	s << "Tables (Herite de : " << (Conteneurs<X> &) T;
	s << ", Pas_Agrandir : " << T.pas_Agrandir << ")";
   for ( unsigned long i=0 ; i<T.nb_Elements ; i++ )
   {
   	s<<((Tables<X>&)T)[i]<<"	" ;
   }

	return s;
}


template <class X> const Tables<X>& Tables<X>::operator = (const Tables<X> &O)
{
	if (&O != this)
	{
		delete [] tab;

		Conteneurs<X>::operator = (O);

      if (nb_Elements > 0) {	// Christian le 17-07-2004
			tab = new X [nb_Elements];

   	   for (unsigned long i = 0 ; i < nb_Elements ; i++)
      		tab [i] = O.tab [i] ;
      }
      else
      	tab = NULL ;
	}

	return *this;
}


template <class X> const Tables<X>& Tables<X>::operator = (const Conteneurs<X> &CSource)
{
	if (&CSource != this)
	{
		Conteneurs<X>::operator = (CSource);

		delete [] tab;
		pas_Agrandir = 10;
      nb_Elements=CSource.Nb_Elements() ;

		if (nb_Elements > 0) {	// Christian le 17-07-2004
			tab = new X [nb_Elements];

			CSource.Tout_Parcourir (Placer_Element, (void *) this);
      }
      else
      	tab = NULL ;
	}

	return *this;
}


template <class X> const Tables<X>& Tables<X>::operator () (
	const Conteneurs<X> &CSource)
{
	if (&CSource != this)
	{
		if (tab != NULL)
			delete [] tab;

		tab = NULL;

		pas_Agrandir = 10;

		Conteneurs<X>::operator () (CSource);

		CSource.Tout_Parcourir (Placer_Element, (void *) this);
	}

	return *this;
}


template <class X> const Tables<X>& Tables<X>::operator + (
	const Conteneurs<X> &CSource)
{
	CSource.Tout_Parcourir (Placer_Element, (void *) this);

	return *this;
}


template <class X> bool operator <= (const Tables<X> &T1, const Tables<X> &T2)
{
	bool	Trouve	= ((Conteneurs<X> &) T1 <= (Conteneurs<X> &) T2);
	unsigned long	i	= 0;

	while ((i < T1.Nb_Elements ()) && (Trouve))
	{
		Trouve = T2.Est_Dans (T1 [i]);
		i ++;
	}

	return Trouve;
}


template <class X> bool operator == (const Tables<X> &T1, const Tables<X> &T2)
{
	bool	Trouve	= ((Conteneurs<X> &) T1 == (Conteneurs<X> &) T2);
	unsigned long	i	= 0;

	while ((i < T1.Nb_Elements ()) && (Trouve))
	{
		Trouve = T2.Est_Dans (T1 [i]);
		i ++;
	}

	return Trouve;
}

//*****************************************************************************
//
// Module	: Tableaux
//
// Fichier	: Tableaux.h
//
// Auteur	: Heschung Frédéric
//
// Date creat.	: 01/07/99
//
// Version	: 1.0
//
// Commentaires	: Interface de la classe Tableaux, dérive de la classe Tables
//			Gestion de tableaux generiques et dynamiques
//			Il faut avoir implémenté les operateurs "=" et "==" sur la classe X.
//
//*****************************************************************************
// Includes
// Classe Tableaux
//	Constructeurs et destructeur
//	Accesseurs
//	Methodes
//	Surcharge des operateurs
//	Partie privee
//*****************************************************************************


//*****************************************************************************
//
// Classe Tableaux
//
//*****************************************************************************

template <class X> class CONTENEURS_DLL  Tableaux	: public Tables<X>
{
	public :

	//*****************************************************************************
	//	Constructeurs et destructeur
   // Ils sont tous les 4 définis ci-dessous pour obliger le compilateur à
   // les compiler. En effet, les 3 sont indispensables même s'ils ne sont
   // pas appelés explicitement.
	//*****************************************************************************

	Tableaux (unsigned long Taille_Init = 0, unsigned long Pas_Agrandir = 10)
	: Tables<X>(Taille_Init,Pas_Agrandir) , nb_Effectifs(0)
   {
   }

	Tableaux (const Tableaux<X> &T )
	: Tables<X>(T) , nb_Effectifs(T.nb_Effectifs)
	{
   }

	Tableaux (const Conteneurs<X> &CSource )
	: Tables<X>(CSource.Nb_Elements(),1) , nb_Effectifs(0)
	{
		CSource.Tout_Parcourir(Placer_Element,(void*)this) ;
	}

	~Tableaux ()
   {
   }

//*****************************************************************************
//	Accesseurs
//*****************************************************************************

      unsigned long Nb_Effectifs () const ;

		Tableaux<X>& Changer_Nb_Elements (unsigned long Nb) ;
      // Si Nb est < au nb effectif d'éléments, ce dernier est réduit.
      // Si Nb est > au nb d'éléments ALLOUES, ce dernier augmente.
      // Sinon, rien ne se passe.

//*****************************************************************************
//	Methodes
//*****************************************************************************

		bool Est_Dans (const X &Elem) const;
      bool Est_Dans (const X &, unsigned long&) const ;
      void Supprimer (unsigned long) ;
      void Supprimer (const X&) ;
      void Inserer	(const X&,unsigned long) ;
      void Inversion () ;


		Conteneurs<X>& Detruit ();

		void Tout_Parcourir (void(*Fonction)(X &, void *,unsigned long) ,
      							void* Donnee=NULL , unsigned long PosDebut=0 ) const ;

		void Fusion (const Conteneurs<X>& CSource,Conteneurs<X>& CResultat ) const ;
		// Concatène this et CSource dans CResultat

//*****************************************************************************
//	Surcharge des operateurs
//*****************************************************************************

		const Tableaux<X>& operator = (const Tableaux<X> &);
		const Tableaux<X>& operator = (const Conteneurs<X> &);
		const Tableaux<X>& operator + (const Conteneurs<X> &);
		const Tableaux<X>& operator + (const Tableaux<X> &);

		const Tableaux<X>& operator () (const Conteneurs<X> &);
		X& operator [] (unsigned long Position);
		const X& operator [] (unsigned long Position) const;


		friend bool CONTENEURS_DLL  operator <= (const Tableaux<X> &, const Tableaux<X> &);
      friend bool CONTENEURS_DLL  operator == (const Tableaux<X> &, const Tableaux<X> &);

		friend ostream& CONTENEURS_DLL operator << (ostream &, const Tableaux<X> &);

//*****************************************************************************
//	Partie privee
//*****************************************************************************

	private :

		static void Placer_Element (X &, void *,unsigned long); // Ajout en queue ac allocation
      															 // d'un nouvel élt

		unsigned long nb_Effectifs ; // Correspond au nb d'élts effectivement
	// stockés CONSECUTIVEMENT ds le tableau
};


template <class X>
bool operator== ( const Tableaux<X>& T1 , const Tableaux<X>& T2 )
{
	return T1.nb_Effectifs==T2.nb_Effectifs && (Tables<X>&)T1==(Tables<X>&)T2 ;
}


template <class X>
unsigned long Tableaux<X>::Nb_Effectifs () const
{
	return nb_Effectifs ;
}

template <class X>
Tableaux<X>& Tableaux<X>::Changer_Nb_Elements ( unsigned long Nb )
{

	if ( Nb < nb_Effectifs )
   	nb_Effectifs=Nb ;
   else if (Nb > nb_Elements )
	   Tables<X>::Changer_Nb_Elements(Nb) ;
   return *this ;
}


template <class X>
void Tableaux<X>::Supprimer ( const X& Elem )
{
	unsigned long pos ;
	if ( Est_Dans(Elem,pos) )
   	Supprimer(pos) ;
}


template <class X>
void Tableaux<X>::Placer_Element (X &Elem, void *Donnee, unsigned long i)
// Ajoute un élt à la fin du tableau et l'initialise avec Elem
{
	Tableaux<X>	*T		= (Tableaux<X> *) Donnee;
	T->operator[](i) = Elem;
}


template <class X>
Conteneurs<X>& Tableaux<X>::Detruit ()
{
	nb_Effectifs=0 ;
	Tables<X>::Detruit() ;
   return (*this) ;
}


template <class X>
const Tableaux<X>& Tableaux<X>::operator = (const Tableaux<X>& T )
{
	if ( &T!=this )
   {
 		Tables<X>::operator=(T) ;
   	nb_Effectifs=T.nb_Effectifs ;
   }
   return *this ;
}


template <class X>
const Tableaux<X>& Tableaux<X>::operator = ( const Conteneurs<X>& CSource )
{
	if (&CSource != this)
	{
		Conteneurs<X>::operator=(CSource) ;
		Changer_Nb_Elements(CSource.Nb_Elements()) ;
		CSource.Tout_Parcourir(Placer_Element, (void *) this);
	}
	return *this;
}


template <class X>
X& Tableaux<X>::operator [] (unsigned long Position)
// On suppose que si un accès au nb ième elements +1 est fait par cette fonction,
// c'est qu'il s'agit d'un ajout, càd que cette fonction est appelée en LValue.
// Sinon, le nb_Effectifs est erroné
{
	if (Position == nb_Effectifs)
	{
   	if ( nb_Effectifs==Nb_Elements() )
      		Changer_Nb_Elements(Nb_Elements()+Pas_Agrandir());
		nb_Effectifs++ ;
	}
   return Tables<X>::operator[](Position) ;
}


template <class X>
const X& Tableaux<X>::operator [] (unsigned long Position) const
// Seuls les accès aux éléments existants sont autorisés
{
   return Tables<X>::operator[](Position) ;
}


template <class X>
void Tableaux<X>::Tout_Parcourir (
								void (* Fonction) (X &, void *,unsigned long),
                        void *Donnee , unsigned long PosDebut ) const
// Parcourt tout le tableau et applique à chaque élt la fonction "Fonction"
{
	unsigned long nb=nb_Effectifs ;
	for (unsigned long i = 0; i < nb; i++)
		Fonction ( ( (Tableaux<X>&)(*this) )[i] , Donnee,i+PosDebut);
}


template <class X>
bool Tableaux<X>::Est_Dans (const X &Elem) const
{
	unsigned long	Pos	= 0;
	bool	Trouve	= false;

	while ( !Trouve && Pos<nb_Effectifs )
   {
		if ( ( (Tableaux<X>&)(*this) )[Pos]==Elem )
			Trouve = true;
		else
			Pos ++;
   }

	return Trouve;
}


template <class X>
bool Tableaux<X>::Est_Dans ( const X& Elem, unsigned long& Pos ) const
{
	bool Trouve=false ;
	Pos=0 ;

   while ( !Trouve && Pos<nb_Effectifs )
   {
   	if ( ( (Tableaux<X>&)(*this) )[Pos]==Elem )
	      Trouve=true ;
      else Pos++ ;
   }

   return Trouve ;
}


template <class X>
void Tableaux<X>::Supprimer ( unsigned long position )
{
	if ( position<nb_Effectifs )
	{
   	for ( unsigned long i=position ; i<nb_Effectifs-1 ; i++ )
   	(*this)[i]=(*this)[i+1] ;

		nb_Effectifs-- ;
   }
}


template <class X>
void Tableaux<X>::Inserer ( const X& Elem , unsigned long pos )
{
//	if ((pos == nb_Effectifs) && (nb_Effectifs == Nb_Elements()))
	if (nb_Effectifs == Nb_Elements())
   	Changer_Nb_Elements(Nb_Elements()+Pas_Agrandir());
      // Ceci est obligatoire ICI. Sinon, la realloc est faite
      // dans (*this)[i] (ci-dessous). Dans ce cas, le termes de
      // droite de l'affectation fait référence à un
      // élément qui disparaît au moment où le terme de gauche
      // provoque la réallocation.

	unsigned long nb=nb_Effectifs ;
	for ( unsigned long i=nb ; i>pos ; i-- )
   {
   	(*this)[i]=(*this)[i-1] ;
   } // nb_Effectifs est incrémenté ds []
   (*this)[pos]=Elem ;
}


template <class X>
void Tableaux<X>::Inversion ()
{
	X aux ;
	for ( unsigned long i=0 ; i<nb_Effectifs/2 ; i++ )
   {
   	aux=(*this)[i] ;
   	(*this)[i]=(*this)[nb_Effectifs-i-1] ;
      (*this)[nb_Effectifs-i-1]=aux ;
   }
}


template <class X>
void Tableaux<X>::Fusion (const Conteneurs<X> &CSource,Conteneurs<X> &CResultat) const
// Concatène this et CSource dans CResultat
{
	// Dommage de ne pouvoir dimensionner le Conteneurs résultat  à la somme
   // des tailles de Nb_Effectifs () et CSource.Nb_Effectifs ()

	if ((Conteneurs<X> *) this != &CResultat)
		Tout_Parcourir (Placer_Element, (void *) &CResultat);

	if (&CSource  != &CResultat)
		// MODIF C. Minich le 7.12.99 : ajout du troisieme parametre ci-dessous
		CSource.Tout_Parcourir (Placer_Element, (void *) &CResultat,((Tableaux<X> &) CResultat).nb_Effectifs);
}


template <class X>
const Tableaux<X>& Tableaux<X>::operator () ( const Conteneurs<X> & CSource )
{
	if (&CSource != this)
	{
		Tables<X>::Detruit() ;
		Conteneurs<X>::operator () (CSource);
		CSource.Tout_Parcourir (Placer_Element, (void *) this);
	}
	return *this;
}


template <class X>
const Tableaux<X>& Tableaux<X>::operator + (const Conteneurs<X>& CSource)
{
	CSource.Tout_Parcourir (Placer_Element, (void *) this,nb_Effectifs);

	return *this;
}


template <class X>
const Tableaux<X>& Tableaux<X>::operator + (const Tableaux<X>& CSource)
{
	Changer_Nb_Elements (Nb_Effectifs ()+CSource.Nb_Effectifs ()) ;
	CSource.Tout_Parcourir (Placer_Element, (void *) this,nb_Effectifs);

	return *this;
}


template <class X>
bool operator<= ( const Tableaux<X>& T1 , const Tableaux<X>& T2 )
{
	bool	Trouve	= ((Conteneurs<X> &) T1 <= (Conteneurs<X> &) T2);
	unsigned long	i	= 0;

	while ( i < T1.Nb_Effectifs() && Trouve )
	{
		Trouve = T2.Est_Dans(T1[i]) ;
		i ++;
	}

	return Trouve;
}


template <class X> ostream& operator << (ostream &s, const Tableaux<X> &T)
{
	s<<"Taille : "<<T.Nb_Elements()<<" , "<<"Nb_Effectifs : "<<T.nb_Effectifs ;
   s<<"\n" ;
	for ( unsigned long i=0 ; i<T.Nb_Effectifs() ; i++ )
   {
   	s<< ((Tables<X>&)T) [i] <<"	" ;
	}

	return s;
}



///////////////////////////////////////////////////////////////////////
//
// La classe TablesEfficaces implemente efficacement une table,
// associant des clés à des valeurs. Elle réalise TRES efficacement :
// - l'insertion d'un couple (clé,valeur)
// - la suppression d'un couple (clé,valeur)
// - la recherche de la valeur associée à une clé.
//
// Les valeur associées aux clés sont fournies sous la forme de pointeurs
// non typés, qui sont les adresses des valeurs associées aux clés. La classe
// TablesEfficaces ne fait strctement rien de ces pointeurs, si ce n'est de
// les mémoriser lors de l'insertion d'un couple et de les restituer lors
// de la recherche.
//
// Ce fichier contient les interfaces ET IMPLEMENTATIONS des classes
// NoeudsAVL, FeuillesAVL, NoeudsNTAVL (toutes 3 sans intérêt pour
// l'utilisateur) et de la classe TablesEfficaces, qui est LA
// classe importante.
//
// Aucun service d'affichage de TablesEfficaces n'est proposé. Pour afficher, faut donc
// demander la construction du tableau des valeurs et l'afficher.
// Aucun service de construction par copie n'est proposé. Faut donc
// passer les TablesEfficaces par référence ou par adresse.
// Aucun service d'égalité n'est proposé.
//
// Pour le détail des services disponibles, rechercher "Classe TablesEfficaces".
//
// La structure de données sous-jacente est l'arbre à hauteurs équilibrées (AVL).
//
//
// La classe TypeCleTable DOIT offrir les services suivants :
//
// - constructeur par défaut
// - constructeur par copie
// - operateur =
// - operateur < : friend bool operator < (const TypeCleTable &P1,const TypeCleTable &P2)
// - operateur > : friend bool operator > (const TypeCleTable &P1,const TypeCleTable &P2)
// - operateur == : friend bool operator == (const TypeCleTable &P1,const TypeCleTable &P2)
//
///////////////////////////////////////////////////////////////////////


#define ArbresAVL NoeudsAVL<TypeCleTable>*

// Classe pour la gestion des défauts mémoire
class EMemoirePleine : public exception
{
public :
	EMemoirePleine (const char *const& m)
   : exception (m)
   {}
} ;


////////////////////////////////////////////////////
//
// Classe NoeudsAVL, classe mère de
//	- NoeudsNTAVL (Noeuds Non Terminaux d'AVL)
// - FeuillesAVL (Feuilles d'AVL)
//
////////////////////////////////////////////////////

template <class TypeCleTable> class CONTENEURS_DLL NoeudsAVL
{
public :
	NoeudsAVL (const TypeCleTable &c, void *val, int FactEq) ;
   virtual ~NoeudsAVL () ;

   virtual ArbresAVL	Inserer (const TypeCleTable &c, void *info, bool &Grandi, bool &Succes) = 0 ;
	// Insère la clé c et l'info associée si la clé ne figure pas déjà
   // dans l'arbre.
	// Succes est vrai si l'insertion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   virtual ArbresAVL	Supprimer (const TypeCleTable &c, bool &Diminue, bool &Succes, void *&val) = 0 ;
	// Supprime la clé c si elle figure dans l'arbre. Dans ce cas, le
   // pointeur restitué est celui qui était associé à la clé.
	// Succes est vrai si la suppresion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   virtual void		Min (TypeCleTable &c, void *&val) = 0 ;
   virtual void		Max (TypeCleTable &c, void *&val) = 0 ;
   // Restituent la plus petite ou la plus grande clé et le pointeur associé

   virtual void 		RemplirTableau (	bool OuiPourLesCles, 
													Tableaux <TypeCleTable> &TC,
													bool OuiPourLesValeurs,
													Tableaux <void *> &TP) = 0 ;
   // Ajoute au(x) tableau(x) les clés de l'arbre dans l'ordre infixé ou les
   // valeurs qui leurs sont associées ou les deux.
	// Attention : il s'agit bien d'ajout !! Les tableaux ne perdent pas leur
	// éventuel contenu.

   virtual ArbresAVL FilsGauche () = 0 ;
   virtual ArbresAVL FilsDroit () = 0 ;

   virtual ArbresAVL Dupliquer () = 0 ;
   // La duplication ne provoque pas celle des valeurs associées aux clés

   virtual bool		EstValide (int &h) = 0 ;

   virtual bool		EstDans (const TypeCleTable &c, void *&val) = 0 ;
   // Restitue vrai ssi la clé c apparaît dans l'arbre. Dans ce cas,
   // le pointeur restitué est celui qui est associé à la clé.

   virtual bool ModifierValeur( const TypeCleTable& c, void*& val ) =0 ;
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.

	int			fe ;
   TypeCleTable		cle ;
   void			*valeur ;

private :
   NoeudsAVL (const NoeudsAVL <TypeCleTable> &) {}
	const NoeudsAVL <TypeCleTable> & operator = (const NoeudsAVL <TypeCleTable> &o) {return o;}
	friend bool CONTENEURS_DLL operator == (const NoeudsAVL <TypeCleTable> &o1, const NoeudsAVL <TypeCleTable> &o2) {return false ;}
	friend ostream & CONTENEURS_DLL operator << (ostream &s, const NoeudsAVL <TypeCleTable> &o) {return s;}
} ;


////////////////////////////////////////////////////
//
// Classe FeuillesAVL
//
////////////////////////////////////////////////////

template <class TypeCleTable> class CONTENEURS_DLL FeuillesAVL : public NoeudsAVL <TypeCleTable>
{
public :
	FeuillesAVL (const TypeCleTable &c, void *val) ;
   ~FeuillesAVL () ;

   ArbresAVL	Inserer (const TypeCleTable &c, void *val, bool &Grandi, bool &Succes) ;
	// Insère la clé c et l'info associée si la clé ne figure pas déjà
   // dans l'arbre.
	// Succes est vrai si l'insertion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   ArbresAVL	Supprimer (const TypeCleTable &c, bool &Diminue, bool &Succes, void *&val) ;
	// Supprime la clé c si elle figure dans l'arbre. Dans ce cas, le
   // pointeur restitué est celui qui était associé à la clé.
	// Succes est vrai si la suppresion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   void			Min (TypeCleTable &c, void *&val) ;
   void			Max (TypeCleTable &c, void *&val) ;
   // Restituent la plus petite ou la plus grande clé et le pointeur associé

   ArbresAVL 	FilsGauche () ;
   ArbresAVL 	FilsDroit () ;

   void 			RemplirTableau (	bool OuiPourLesCles, 
											Tableaux <TypeCleTable> &TC,
											bool OuiPourLesValeurs,
											Tableaux <void *> &TP) ;
   // Ajoute au(x) tableau(x) les clés de l'arbre dans l'ordre infixé ou les
   // valeurs qui leurs sont associées ou les deux.
	// Attention : il s'agit bien d'ajout !! Les tableaux ne perdent pas leur
	// éventuel contenu.

   ArbresAVL 	Dupliquer () ;
   // La duplication ne provoque pas celle des valeurs associées aux clés

   bool			EstValide (int &h) ;

   bool			EstDans (const TypeCleTable &c, void *&val) ;
   // Restitue vrai ssi la clé c apparaît dans l'arbre. Dans ce cas,
   // le pointeur restitué est celui qui est associé à la clé.

   bool ModifierValeur( const TypeCleTable& c, void*& val ) ;
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.
} ;


////////////////////////////////////////////////////
//
// Classe NoeudsNTAVL
//
////////////////////////////////////////////////////

template <class TypeCleTable> class CONTENEURS_DLL NoeudsNTAVL : public NoeudsAVL <TypeCleTable>
{
public :
	NoeudsNTAVL (const TypeCleTable &c, void *val, int FactEq, ArbresAVL FilsG, ArbresAVL FilsD) ;
   ~NoeudsNTAVL () ;

   ArbresAVL	Inserer (const TypeCleTable &c, void *val, bool &Grandi, bool &Succes) ;
	// Insère la clé c et l'info associée si la clé ne figure pas déjà
   // dans l'arbre.
	// Succes est vrai si l'insertion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   ArbresAVL	Supprimer (const TypeCleTable &c, bool &Diminue, bool &Succes, void *&val) ;
	// Supprime la clé c si elle figure dans l'arbre. Dans ce cas, le
   // pointeur restitué est celui qui était associé à la clé.
	// Succes est vrai si la suppresion a été réalisée.
	// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
   // L'arbre initial ne peut plus être utilisé après l'appel.

   void			Min (TypeCleTable &c, void *&val) ;
   void			Max (TypeCleTable &c, void *&val) ;
   // Restituent la plus petite ou la plus grande clé et le pointeur associé

   ArbresAVL 	FilsGauche () ;
   ArbresAVL 	FilsDroit () ;

   void 			RemplirTableau (	bool OuiPourLesCles, 
											Tableaux <TypeCleTable> &TC,
											bool OuiPourLesValeurs,
											Tableaux <void *> &TP) ;
   // Ajoute au(x) tableau(x) les clés de l'arbre dans l'ordre infixé ou les
   // valeurs qui leurs sont associées ou les deux.
	// Attention : il s'agit bien d'ajout !! Les tableaux ne perdent pas leur
	// éventuel contenu.

   ArbresAVL 	Dupliquer () ;
   // La duplication ne provoque pas celle des valeurs associées aux clés

   bool			EstValide (int &h) ;

   bool			EstDans (const TypeCleTable &c, void *&val) ;
   // Restitue vrai ssi la clé c apparaît dans l'arbre. Dans ce cas,
   // le pointeur restitué est celui qui est associé à la clé.

   bool ModifierValeur( const TypeCleTable& c, void*& val ) ;
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.


private :
	ArbresAVL	fg ;
   ArbresAVL	fd ;

   ArbresAVL EquilibrageGaucheApresInsertion () ;
   ArbresAVL EquilibrageDroitApresInsertion () ;
   ArbresAVL EquilibrageGaucheApresSuppression (bool &Diminue) ;
   ArbresAVL EquilibrageDroitApresSuppression (bool &Diminue) ;
} ;


////////////////////////////////////////////////////
//
// Classe TablesEfficaces
//
////////////////////////////////////////////////////

template <class TypeCleTable> class CONTENEURS_DLL TablesEfficaces
{
public :
	TablesEfficaces () ;
	~ TablesEfficaces () ;

   bool	EstVide () ;

   void	Vider () ;
	// Retire du conteneur toutes les clés qu'il contient. Les pointeurs
   // associés aux clés ne sont pas désalloués

   bool	Inserer (const TypeCleTable &c, void *val) ;
	// Insère la clé c et l'info associée si la clé ne figure pas déjà
   // dans la table.
   // Le résultat est vrai ssi l'insertion a été possible, c'est à dire
   // que c n'était pas déjà dans l'arbre.
   // Le pointeur est l'adresse d'un quelconque objet que l'on souhaite
   // associer à la clé.

   bool	Supprimer (const TypeCleTable &c, void *&val) ;
	// Supprime la clé c si elle figure dans la table. Dans ce cas, le
   // résultat est vrai et le pointeur restitué est celui qui était
   // associé à la clé.

   void 	EnTableau (	bool OuiPourLesCles, 
							Tableaux <TypeCleTable> &TC,
							bool OuiPourLesValeurs,
							Tableaux <void *> &TP) const ;
   // Initialise le tableau TC avec les clés contenues dans le conteneur
   // par ordre croissant et TP avec les pointeurs associés à chaque clé.
   // TP [i] est le pointeur associé à la clé TC [i]
	// Le précédent contenu des tableaux est perdu.
	// TC n'est initialisé que si le premier booléen est vrai, TP que si le 
	// deuxième l'est.

   void	Min (TypeCleTable &c, void *&val) ;
   void	Max (TypeCleTable &c, void *&val) ;
   // Restituent la plus petite ou la plus grande clé et le pointeur associé

   int	NombreElements () const ;

   bool	EstDans (const TypeCleTable &c, void *&val) ;
   // Restitue vrai ssi la clé c apparaît dans l'arbre. Dans ce cas,
   // le pointeur restitué est celui qui est associé à la clé.

	const TablesEfficaces & operator = (const TablesEfficaces &o) ;
   // L'affectation ne provoque pas la duplication des valeurs associées
   // aux clés : les pointeurs sont ré-utilisés tels quels.

	bool ModifierValeur( const TypeCleTable& c, void*& val ) ;
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.


private :
	TablesEfficaces (const TablesEfficaces &o) {}
	friend bool CONTENEURS_DLL operator == (const TablesEfficaces &o1, const TablesEfficaces &o2) {return false ;}
	friend ostream & CONTENEURS_DLL operator << (ostream &s, const TablesEfficaces &o) {return s;}
   bool	EstValide () ;

	ArbresAVL		a ;
   unsigned long	nombreElements ;
} ;


////////////////////////////////////////////////////
//
// Implementation de la classe NoeudsAVL
//
////////////////////////////////////////////////////

template <class TypeCleTable>
NoeudsAVL <TypeCleTable>::NoeudsAVL (const TypeCleTable &c, void *val, int FactEq)
:	fe (FactEq), cle (c), valeur (val)
{
}


template <class TypeCleTable> NoeudsAVL <TypeCleTable>::~NoeudsAVL ()
{
}


////////////////////////////////////////////////////
//
// Implementation de la classe FeuillesAVL
//
////////////////////////////////////////////////////

template <class TypeCleTable> FeuillesAVL <TypeCleTable>::FeuillesAVL (const TypeCleTable &c, void *val)
:	NoeudsAVL <TypeCleTable> (c,val,0)
{
}


template <class TypeCleTable> FeuillesAVL <TypeCleTable>::~FeuillesAVL ()
{
}


template <class TypeCleTable> ArbresAVL	FeuillesAVL <TypeCleTable>::Inserer (const TypeCleTable &c, void *val, bool &Grandi, bool &Succes)
// Insère la clé c et l'info associée si la clé ne figure pas déjà
// dans l'arbre.
// Succes est vrai si l'insertion a été réalisée.
// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
// L'arbre initial ne peut plus être utilisé après l'appel.
{
	if (c == cle)
   {
   	Grandi = false ;
      Succes = false ;
      return this ;
   }
   else
   {
	   Grandi = Succes = true ;

		if (c < cle)
	   {
   		ArbresAVL	r = new NoeudsNTAVL<TypeCleTable> (cle,valeur,1,this,NULL) ;

         if ( ! r) {
         	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans FeuillesAVL <TypeCleTable>::Inserer") ;
            throw *E ;
         }

      	cle = c ;
         valeur = val ;
	      return r ;
   	}
	   else
   	{
   		ArbresAVL	r = new NoeudsNTAVL<TypeCleTable> (cle,valeur,-1,NULL,this) ;

         if ( ! r) {
         	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans FeuillesAVL <TypeCleTable>::Inserer") ;
            throw *E ;
         }

      	cle = c ;
         valeur = val ;
   	   return r ;
	   }
   }
}


template <class TypeCleTable> ArbresAVL	FeuillesAVL <TypeCleTable>::Supprimer (const TypeCleTable &c, bool &Diminue, bool &Succes, void *&val)
// Supprime la clé c si elle figure dans l'arbre. Dans ce cas, le
// pointeur restitué est celui qui était associé à la clé.
// Succes est vrai si la suppresion a été réalisée.
// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
// L'arbre initial ne peut plus être utilisé après l'appel.
{
	if (c == cle)
   {
   	val = valeur ;
   	delete this ;
      Diminue = Succes = true ;
      return NULL ;
   }
   else
   {
		Diminue = Succes = false ;
      val = NULL ;
      return this ;
   }
}


// Restituent la plus petite ou la plus grande clé et le pointeur associé
template <class TypeCleTable> void	FeuillesAVL <TypeCleTable>::Min (TypeCleTable &c, void *&val)
{
	c = cle ; val = valeur ;
}


// Restituent la plus petite ou la plus grande clé et le pointeur associé
template <class TypeCleTable> void FeuillesAVL <TypeCleTable>::Max (TypeCleTable &c, void *&val)
{
	c = cle ; val = valeur ;
}


template <class TypeCleTable> ArbresAVL FeuillesAVL <TypeCleTable>::FilsGauche ()
{
	return NULL ;
}


template <class TypeCleTable> ArbresAVL FeuillesAVL <TypeCleTable>::FilsDroit ()
{
	return NULL ;
}


template <class TypeCleTable> 
void FeuillesAVL <TypeCleTable>::RemplirTableau (	bool OuiPourLesCles, 
																	Tableaux <TypeCleTable> &TC,
																	bool OuiPourLesValeurs,
																	Tableaux <void *> &TP)
   
// Ajoute au(x) tableau(x) les clés de l'arbre dans l'ordre infixé ou les
// valeurs qui leurs sont associées ou les deux.
// Attention : il s'agit bien d'ajout !! Les tableaux ne perdent pas leur
// éventuel contenu.
{
	if (OuiPourLesCles)
		TC [TC.Nb_Effectifs ()] = cle ;
	if (OuiPourLesValeurs)
		TP [TP.Nb_Effectifs ()] = valeur ;
}


template <class TypeCleTable> ArbresAVL FeuillesAVL <TypeCleTable>::Dupliquer ()
// La duplication ne provoque pas celle des valeurs associées aux clés
{
	ArbresAVL a = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

	if ( ! a) {
   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans FeuillesAVL <TypeCleTable>::Dupliquer") ;
      throw *E ;
   }

	return a ;
}


template <class TypeCleTable> bool	FeuillesAVL <TypeCleTable> :: EstValide (int &h)
{
	h = 1 ;
	return fe == 0 ;
}


template <class TypeCleTable> bool			FeuillesAVL <TypeCleTable> :: EstDans (const TypeCleTable &c, void *&val)
{
	if (c == cle)
   {
   	val = valeur ;
      return true ;
   }
   else
   {
   	val = NULL ;
		return false ;
   }
}


template <class TypeCleTable> bool FeuillesAVL <TypeCleTable> :: ModifierValeur( const TypeCleTable& c, void*& val )
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.
{
	if (c == cle)
   {
   	valeur = val ;
      return true ;
   }
   else
   {
	 return false ;
   }
}

////////////////////////////////////////////////////
//
// Implementation de la classe NoeudsNTAVL
//
////////////////////////////////////////////////////

template <class TypeCleTable> NoeudsNTAVL <TypeCleTable>::NoeudsNTAVL (const TypeCleTable &c, void *val, int FactEq, ArbresAVL FilsG, ArbresAVL FilsD)
:	NoeudsAVL <TypeCleTable> (c,val,FactEq), fg (FilsG), fd (FilsD)
{
}


template <class TypeCleTable> NoeudsNTAVL <TypeCleTable>::~NoeudsNTAVL ()
{
	delete fg ;
   delete fd ;
}


template <class TypeCleTable> ArbresAVL	NoeudsNTAVL <TypeCleTable>::Inserer (const TypeCleTable &c, void *val, bool &Grandi, bool &Succes)
// Insère la clé c et l'info associée si la clé ne figure pas déjà
// dans l'arbre.
// Succes est vrai si l'insertion a été réalisée.
// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
// L'arbre initial ne peut plus être utilisé après l'appel.
{
	if (c < cle)
   {
   	if (fg == NULL)
      {
      	// FD ne peut être nul aussi sinon, on serait sur une feuille
         // ==> FE = -1 avant insertion et passe à 0 après
      	fg = new FeuillesAVL <TypeCleTable> (c,val) ;

			if ( ! fg) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Inserer") ;
      		throw *E ;
		   }

         fe = 0 ;
         Grandi = false ;
         Succes = true ;
         return this ;
      }
      else
	   {
   		fg = fg->Inserer (c,val,Grandi,Succes) ;
         if (Grandi)
         	switch (fe)
            {
            case -1 : Grandi = false ;
            			 fe = 0 ;
            			 return this ;
            case 0  : // Grandi = true ;
							 fe = 1 ;
                      return this ;
            case +1 : {
            			 ArbresAVL AE = EquilibrageGaucheApresInsertion () ;
            			 Grandi = false ;
                      return AE ;
                      }
            default : return NULL ;
            }
         else
         	return this ;
	   }
   }
   else if (c > cle)
   {
   	if (fd == NULL)
      {
      	// FG ne peut être nul aussi sinon, on serait sur une feuille
         // ==> FE = 1 avant insertion et passe à 0 après
      	fd = new FeuillesAVL <TypeCleTable> (c,val) ;

			if ( ! fd) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Inserer") ;
      		throw *E ;
		   }

         fe = 0 ;
         Grandi = false ;
         Succes = true ;
         return this ;
      }
      else
	   {
   		fd = fd->Inserer (c,val,Grandi,Succes) ;
         if (Grandi)
         	switch (fe)
            {
            case +1 : Grandi = false ;
            			 fe = 0 ;
            			 return this ;
            case 0  : // Grandi = true ;
							 fe = -1 ;
                      return this ;
            case -1 : {
            			 ArbresAVL AE = EquilibrageDroitApresInsertion () ;
            			 Grandi = false ;
                      return AE ;
                      }
            default : return NULL ;
            }
         else
         	return this ;
	   }
   }
   else
   {
   	Grandi = Succes = false ;
   	return this ;
   }
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresInsertion ()
{
	// Puisqu'il y a besoin d'un équilibrage, le fe du fils gauche ne peut
   // être nul ==> il vaut 1 ou -1
	if (fg->fe == 1)
   {
   	// L'insertion a eu lieu "sous" le fils gauche du fils gauche.
      NoeudsNTAVL <TypeCleTable>		*NR = (NoeudsNTAVL <TypeCleTable> *) fg ;
      NR->fe = 0 ;
      // Si le fd de this est vide, alors le fd du fg l'est aussi et le fils droit
      // de l'arbre reconstruit sera UNE FEUILLE --> 1 cas particulier
      if (fd == NULL)
      {
      	NR->fd = new FeuillesAVL <TypeCleTable> (cle,valeur) ;
			if ( ! NR->fd) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresInsertion") ;
      		throw *E ;
		   }
      }
      else
      {
	      fg = NR->fd ;
   	   fe = 0 ;
      	NR->fd = this ;
      }
      return NR ;
   }
   else
   {
   	NoeudsNTAVL <TypeCleTable>	*FG = (NoeudsNTAVL <TypeCleTable> *) fg ;
   	// L'insertion a eu lieu "sous" le fils droit du fils gauche ==> faut
      // voir si le fils gauche était réduit à une feuille avant insertion
   	if (FG->fd->fe == 0)
      {
      	// Le fils gauche était une feuille et a maintenant la nouvelle
         // cle comme fils droit
			fd = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

			if ( ! fd) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresInsertion") ;
      		throw *E ;
		   }

			cle = FG->fd->cle ;
         valeur = FG->fd->valeur ;
         fe = 0 ;
         FG->fd->cle = fg->cle ;
         FG->fd->valeur = fg->valeur ;
         fg = FG->fd ;
         FG->fd = NULL ;
         delete FG ;
         return this ;
      }
      else
      {
      	// Le fils gauche avait un fils droit non vide et équilibré. Ce
         // fils droit est maintenant déséquilibré du côté où a eu lieu
         // l'insertion
         NoeudsNTAVL <TypeCleTable>		*FGD = (NoeudsNTAVL <TypeCleTable> *) FG->fd ;

         fg = FGD->fd ;
         FGD->fd = this ;
         FG->fd = FGD->fg ;
         FGD->fg = FG ;

         if (FGD->fe == 1)	// l'insertion a eu lieu sous le fg du fd du fg
         {
         	FGD->fg->fe = 0 ; FGD->fd->fe = -1 ;
         }
         else
         {
         	FGD->fg->fe = 1 ; FGD->fd->fe = 0 ;
         }
         FGD->fe = 0 ;

         return FGD ;
      }
   }
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresInsertion ()
{
	// Puisqu'il y a besoin d'un équilibrage, le fe du fils droit ne peut
   // être nul ==> il vaut 1 ou -1
	if (fd->fe == -1)
   {
   	// L'insertion a eu lieu "sous" le fils droit du fils droit

      NoeudsNTAVL <TypeCleTable>		*NR = (NoeudsNTAVL <TypeCleTable> *) fd ;
      NR->fe = 0 ;

      // Si le fg de this est vide, alors le fg du fd l'est aussi et le fils gauche
      // de l'arbre reconstruit sera UNE FEUILLE --> 1 cas particulier
      if (fg == NULL)
      {
      	NR->fg = new FeuillesAVL <TypeCleTable> (cle,valeur) ;
			if ( ! NR->fg) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresInsertion") ;
      		throw *E ;
		   }
      }
      else
      {
	      fd = NR->fg ;
   	   fe = 0 ;
      	NR->fg = this ;
      }
      return NR ;
   }
   else
   {
   	NoeudsNTAVL <TypeCleTable>	*FD = (NoeudsNTAVL <TypeCleTable> *) fd ;
   	// L'insertion a eu lieu "sous" le fils gauche du fils droit ==> faut
      // voir si le fils droit était réduit à une feuille avant insertion
   	if (FD->fg->fe == 0)
      {
      	// Le fils droit était une feuille et a maintenant la nouvelle
         // cle comme fils gauche
			fg = new FeuillesAVL <TypeCleTable> (cle,valeur) ;
			if ( ! fg) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresInsertion") ;
      		throw *E ;
		   }
			cle = FD->fg->cle ;
			valeur = FD->fg->valeur ;
         fe = 0 ;
         FD->fg->cle = fd->cle ;
         FD->fg->valeur = fd->valeur ;
         fd = FD->fg ;
         FD->fg = NULL ;
         delete FD ;
         return this ;
      }
      else
      {
      	// Le fils droit avait un fils gauche non vide et équilibré. Ce
         // fils gauche est maintenant déséquilibré du côté où a eu lieu
         // l'insertion
         NoeudsNTAVL <TypeCleTable>		*FDG = (NoeudsNTAVL <TypeCleTable> *) FD->fg ;

         fd = FDG->fg ;
         FDG->fg = this ;
         FD->fg = FDG->fd ;
         FDG->fd = FD ;

         if (FDG->fe == 1)	// l'insertion a eu lieu sous le fg du fg du fd
         {
         	FDG->fg->fe = 0 ; FDG->fd->fe = -1 ;
         }
         else
         {
         	FDG->fg->fe = 1 ; FDG->fd->fe = 0 ;
         }
         FDG->fe = 0 ;

         return FDG ;
      }
   }
}


template <class TypeCleTable> ArbresAVL	NoeudsNTAVL <TypeCleTable>::Supprimer (const TypeCleTable &c, bool &Diminue, bool &Succes, void *&val)
// Supprime la clé c si elle figure dans l'arbre. Dans ce cas, le
// pointeur restitué est celui qui était associé à la clé.
// Succes est vrai si la suppresion a été réalisée.
// L'arbre restitué utilise intégralement la mémoire de l'arbre initial.
// L'arbre initial ne peut plus être utilisé après l'appel.
{
	if (c < cle)	// Suppression à gauche
   	if (fg)
	   {
   		fg = fg->Supprimer (c,Diminue,Succes,val) ;

			// 1er cas : il n'y a pas de fils droit et le fils gauche disparaît
         // --> le noeud devient une feuille
      	if ((fg == NULL) && (fe == 1))
	      {
   	   	// Le noeud courant redevient une feuille car il a perdu son unique fils
      		FeuillesAVL <TypeCleTable>	*f = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

				if ( ! f) {
			   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Supprimer") ;
   	   		throw *E ;
			   }

         	Diminue = true ;
	         delete this ;
   	      return f ;
      	}

         // Deuxième cas : le fils droit existe et le fils gauche diminue.
	      else if (Diminue)
	         switch (fe)
            {
            case 1 : fe = 0 ; // Diminue reste vrai
							return this ;
            case 0 : fe = -1 ;
            			Diminue = false ;
							return this ;
            default:	// deséquilibre
            			ArbresAVL	AE = EquilibrageGaucheApresSuppression (Diminue) ;
                     return AE ;
            }
         else
         	return this ;
      }
      else
      {
      	Diminue = Succes = false ;
         return this ;
      }

   else if (c > cle)	// Suppression à droite
   	if (fd)
	   {
   		fd = fd->Supprimer (c,Diminue,Succes,val) ;

			// 1er cas : il n'y a pas de fils gauche et le fils droit disparaît
         // --> le noeud devient une feuille
      	if ((fd == NULL) && (fe == -1))
	      {
   	   	// Le noeud courant redevient une feuille car il a perdu son unique fils
      		FeuillesAVL <TypeCleTable>	*f = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

				if ( ! f) {
			   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Supprimer") ;
   	   		throw *E ;
			   }

         	Diminue = true ;
	         delete this ;
   	      return f ;
      	}

         // Deuxième cas : le fils gauche existe et le fils droit diminue.
	      else if (Diminue)
	         switch (fe)
            {
            case -1: fe = 0 ; // Diminue reste vrai
							return this ;
            case 0 : fe = 1 ;
            			Diminue = false ;
							return this ;
            default:	// deséquilibre
            			ArbresAVL	AE = EquilibrageDroitApresSuppression (Diminue) ;
                     return AE ;
            }
         else
         	return this ;
      }
      else
      {
      	Diminue = Succes = false ;
         return this ;
      }

   else	// Suppression de la racine. Il y a au moins un fils car
   		// on n'est pas dans une feuille
		if (fe >= 0)
	   {
      	val = valeur ;

			// remplace la racine par le max du fg et l'y supprime
			fg->Max (cle,valeur) ;
			fg = fg->Supprimer (cle,Diminue,Succes,valeur) ;

      	if ((fg == NULL) && (fe == 1))
	      {
   	   	// Le noeud courant redevient une feuille car il a perdu son unique fils
      		FeuillesAVL <TypeCleTable>	*f = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

				if ( ! f) {
			   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Supprimer") ;
   	   		throw *E ;
			   }

            // Diminue et Succes sont déjà vrais
	         delete this ;
   	      return f ;
      	}
         else
         {
         	if (Diminue)
				{
   	         fe-- ;
      	   	if (fe == -1)
						Diminue = false ;
            	// else Diminue reste vrai
	         }

   	      // Succes reste true
      	   return this ;
         }
   	}
      else
	   {
      	val = valeur ;

			// remplace la racine par le min du fd et l'y supprime
			fd->Min (cle,valeur) ;
			fd = fd->Supprimer (cle,Diminue,Succes,valeur) ;

      	if ((fd == NULL) && (fe == -1))
	      {
   	   	// Le noeud courant redevient une feuille car il a perdu son unique fils
      		FeuillesAVL <TypeCleTable>	*f = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

				if ( ! f) {
			   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Supprimer") ;
   	   		throw *E ;
			   }

            // Diminue et Succes sont déjà vrais
	         delete this ;
   	      return f ;
      	}
         else
         {
	         if (Diminue)
   	         fe = 0 ;
      	      // Diminue reste vrai

	         // Succes reste true
   	      return this ;
         }
   	}
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::FilsGauche ()
{
	return fg ;
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::FilsDroit ()
{
	return fd ;
}


// Restitue la plus grande clé et le pointeur associé
template <class TypeCleTable> void NoeudsNTAVL <TypeCleTable>::Max (TypeCleTable &c, void *&val)
{
	ArbresAVL	a = this ;

   while (a->FilsDroit () != NULL)
   	a = a->FilsDroit () ;

	c = a->cle ;
	val = a->valeur ;
}


// Restitue la plus petite clé et le pointeur associé
template <class TypeCleTable> void NoeudsNTAVL <TypeCleTable>::Min (TypeCleTable &c, void *&val)
{
	ArbresAVL	a = this ;

   while (a->FilsGauche () != NULL)
   	a = a->FilsGauche () ;

	c = a->cle ;
	val = a->valeur ;
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresSuppression (bool &Diminue)
// Le fils droit est de 2 plus haut que le gauche
{
	NoeudsNTAVL <TypeCleTable>	*FD = (NoeudsNTAVL <TypeCleTable> *) fd,
   						*FDG = (NoeudsNTAVL <TypeCleTable> *) FD->fg ;

   if (FD->fe <= 0)
   {
   	// Il se peut que le fg de l'arbre reconstruit soit une feuille
      // ==> un cas particulier
      if (FDG == NULL)
      {
      	FD->fg = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

			if ( ! FD->fg) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresSuppression") ;
  	   		throw *E ;
		   }

         fd = NULL ;
         delete this ;
         FD->fe = 0 ;
         Diminue = true ;
      }
      else
      {
	   	this->fd = FDG ;
   	   FD->fg = this ;

   	   if (FD->fe == -1)
	      {
				fe = 0 ; FD->fe = 0 ; Diminue = true ;
	      }
   	   else
      	{
				fe = -1 ; FD->fe = 1 ; Diminue = false ;
	      }
      }

      return FD ;
   }
   else
   	// Il se peut que les fils de l'arbre reconstruit soient des feuilles
      // --> un cas particulier
      if (fg == NULL)
      {
      	ArbresAVL r = new NoeudsNTAVL <TypeCleTable> (	FDG->cle,FDG->valeur,0,
         												new FeuillesAVL <TypeCleTable> (cle,valeur),
         												new FeuillesAVL <TypeCleTable> (fd->cle,fd->valeur)) ;

			if ( ! r) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageGaucheApresSuppression") ;
  	   		throw *E ;
		   }

         delete this ;
         Diminue = true ;
         return r ;
      }
      else
      {
			fd = FDG->fg ;
      	FD->fg = FDG->fd ;
	      FDG->fg = this ;
   	   FDG->fd = FD ;

	      switch (FDG->fe)
   	   {
      	case -1 : FDG->fg->fe = 1 ; FDG->fd->fe = 0 ;
         			 break ;
	      case  0 : FDG->fg->fe = 0 ; FDG->fd->fe = 0 ;
         			 break ;
   	   default : FDG->fg->fe = 0 ; FDG->fd->fe = -1 ;
         			 break ;
      	}

	      Diminue = true ; FDG->fe = 0 ;
      	return FDG ;
   	}
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresSuppression (bool &Diminue)
// Le fils gauche est de 2 plus haut que le droit
{
	NoeudsNTAVL <TypeCleTable>	*FG = (NoeudsNTAVL <TypeCleTable> *) fg,
   						*FGD = (NoeudsNTAVL <TypeCleTable> *) FG->fd ;

   if (FG->fe >= 0)
   {
   	// Il se peut que le fd de l'arbre reconstruit soit une feuille
      // ==> un cas particulier
      if (FGD == NULL)
      {
      	FG->fd = new FeuillesAVL <TypeCleTable> (cle,valeur) ;

			if ( ! FG->fd) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresSuppression") ;
  	   		throw *E ;
		   }

         fg = NULL ;
         delete this ;
         FG->fe = 0 ;
         Diminue = true ;
      }
      else
      {
	   	this->fg = FGD ;
   	   FG->fd = this ;

      	if (FG->fe == 1)
	      {
				fe = 0 ; FG->fe = 0 ; Diminue = true ;
      	}
	      else
   	   {
				fe = 1 ; FG->fe = -1 ; Diminue = false ;
	      }
   	}

      return FG ;
   }
   else
   	// Il se peut que les fils de l'arbre reconstruit soient des feuilles
      // --> un cas particulier
      if (fd == NULL)
      {
      	ArbresAVL r = new NoeudsNTAVL <TypeCleTable> (	FGD->cle,FGD->valeur,0,
         												new FeuillesAVL <TypeCleTable> (fg->cle,fg->valeur),
         												new FeuillesAVL <TypeCleTable> (cle,valeur)) ;

			if ( ! r) {
		   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::EquilibrageDroitApresSuppression") ;
  	   		throw *E ;
		   }

         delete this ;
         Diminue = true ;
         return r ;
      }
      else
      {
			fg = FGD->fd ;
   	   FG->fd = FGD->fg ;
      	FGD->fg = FG ;
	      FGD->fd = this ;

   	   switch (FGD->fe)
      	{
	      case -1 : FGD->fg->fe = 1 ; FGD->fd->fe = 0 ;
         			 break ;
   	   case  0 : FGD->fg->fe = 0 ; FGD->fd->fe = 0 ;
         			 break ;
      	default : FGD->fg->fe = 0 ; FGD->fd->fe = -1 ;
         			 break ;
	      }

   	   Diminue = true ; FGD->fe = 0 ;
	      return FGD ;
      }
}


template <class TypeCleTable> 
void NoeudsNTAVL <TypeCleTable>::RemplirTableau (	bool OuiPourLesCles, 
																	Tableaux <TypeCleTable> &TC,
																	bool OuiPourLesValeurs,
																	Tableaux <void *> &TP)
   
// Ajoute au(x) tableau(x) les clés de l'arbre dans l'ordre infixé ou les
// valeurs qui leurs sont associées ou les deux.
// Attention : il s'agit bien d'ajout !! Les tableaux ne perdent pas leur
// éventuel contenu.
{
	if (fg)
   	fg->RemplirTableau (OuiPourLesCles,TC,OuiPourLesValeurs,TP) ;

	if (OuiPourLesCles)
		TC [TC.Nb_Effectifs ()] = cle ;

	if (OuiPourLesValeurs)
		TP [TP.Nb_Effectifs ()] = valeur ;

	if (fd)
   	fd->RemplirTableau (OuiPourLesCles,TC,OuiPourLesValeurs,TP) ;
}


template <class TypeCleTable> ArbresAVL NoeudsNTAVL <TypeCleTable>::Dupliquer ()
// La duplication ne provoque pas celle des valeurs associées aux clés
{
	ArbresAVL	a = new NoeudsNTAVL <TypeCleTable> (cle,valeur,fe,
							   									 (fg)?fg->Dupliquer ():NULL,
                     							      	 (fd)?fd->Dupliquer ():NULL) ;

	if ( ! a) {
   	EMemoirePleine *E = new EMemoirePleine ("Plus de memoire dans NoeudsNTAVL <TypeCleTable>::Dupliquer") ;
  	   throw *E ;
   }

	return a ;
}


template <class TypeCleTable> bool	NoeudsNTAVL  <TypeCleTable>:: EstValide (int &h)
{
	int	h1,h2 ;

	if (fg)
   	if (fd)
      {
      	if (fg->EstValide (h1) && fd->EstValide (h2) && (fe == h1-h2))
         {
         	h = 1+((h1>h2)?h1:h2) ;
            return true ;
         }
         else
         	return false ;
      }
      else
      {
      	h = 2 ;
      	if (fg->EstValide (h1) && (h1 == 1) && (fe == 1))
         	return true ;
         else
         	return false ;
      }
   else
   	if (fd)
      {
      	h = 2 ;
      	if (fd->EstValide (h2) && (h2 == 1) && (fe == -1))
         	return true ;
         else
         	return false ;
      }
      else
      	return false ;
}


template <class TypeCleTable> bool			NoeudsNTAVL <TypeCleTable> :: EstDans (const TypeCleTable &c, void *&val)
{
	if (cle == c)
   {
   	val = valeur ;
   	return true ;
   }
   else if (c < cle)
   	if (fg)
      	return fg->EstDans (c,val) ;
      else
      	return false ;
   else
   	if (fd)
      	return fd->EstDans (c,val) ;
      else
      	return false ;
}


template <class TypeCleTable> bool NoeudsNTAVL <TypeCleTable> :: ModifierValeur( const TypeCleTable& c, void*& val )
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.
{
	if (cle == c)
   {
   	valeur=val ;
   	return true ;
   }
   else if (c < cle)
   	if (fg)
      	return fg->ModifierValeur (c,val) ;
      else
      	return false ;
   else
   	if (fd)
      	return fd->ModifierValeur (c,val) ;
      else
      	return false ;
}

////////////////////////////////////////////////////
//
// Implementation de la classe TablesEfficaces
//
////////////////////////////////////////////////////

template <class TypeCleTable> TablesEfficaces <TypeCleTable> :: TablesEfficaces ()
:	a (NULL), nombreElements (0)
{
}


template <class TypeCleTable> TablesEfficaces <TypeCleTable> :: ~TablesEfficaces ()
{
	FautCompter = false ;
	if (a)
   	delete a ;
   nombreElements = 0 ;
   a = NULL ;
}


template <class TypeCleTable> void TablesEfficaces <TypeCleTable> :: Vider ()
// Retire du conteneur toutes les clés qu'il contient. Les pointeurs
// associés aux clés ne sont pas désalloués
{
	if (a)
   	delete a ;
   nombreElements = 0 ;
   a = NULL ;
}


template <class TypeCleTable> bool	TablesEfficaces <TypeCleTable> :: EstVide ()
{
	return a == NULL ;
}


template <class TypeCleTable> bool	TablesEfficaces <TypeCleTable> :: Inserer (const TypeCleTable &c, void *val)
// Insère la clé c et l'info associée si la clé ne figure pas déjà
// dans la table.
// Le résultat est vrai ssi l'insertion a été possible, c'est à dire
// que c n'était pas déjà dans l'arbre et qu'il n'y a pas eu un défaut mémoire.
// Le pointeur est l'adresse d'un quelconque objet que l'on souhaite
// associer à la clé.
{
	if (a)
   {
   	bool	Grandi, Succes ;

      a = a->Inserer (c,val,Grandi,Succes) ;

      if (Succes)
      {
      	nombreElements++ ;
         return true ;
      }
      else
      	return false ;
   }
   else
   {
   	a = new FeuillesAVL <TypeCleTable> (c,val) ;

      nombreElements++ ;
      return true ;
   }
}


template <class TypeCleTable> bool	TablesEfficaces <TypeCleTable>::Supprimer (const TypeCleTable &c, void *&val)
// Supprime la clé c si elle figure dans la table. Dans ce cas, le
// résultat est vrai et le pointeur restitué est celui qui était
// associé à la clé.
{
	if (a)
   {
   	bool	Diminue, Succes ;

   	a = a->Supprimer (c,Diminue,Succes,val) ;

      if (Succes)
      	nombreElements -- ;

      return Succes ;
   }
   else
   {
   	val = NULL ;
   	return false ;
   }
}


template <class TypeCleTable> 
void	TablesEfficaces <TypeCleTable> :: EnTableau (bool OuiPourLesCles, 
																	Tableaux <TypeCleTable> &TC,
																	bool OuiPourLesValeurs,
																	Tableaux <void *> &TP) const
// Initialise le tableau TC avec les clés contenues dans le conteneur
// par ordre croissant et TP avec les pointeurs associés à chaque clé.
// TP [i] est le pointeur associé à la clé TC [i]
// Le précédent contenu des tableaux est perdu.
// TC n'est initialisé que si le premier booléen est vrai, TP que si le 
// deuxième l'est.
{
	if (OuiPourLesCles)
	{
		TC.Changer_Nb_Elements (0) ; // vide sans désallouer

		if (TC.Nb_Elements () < nombreElements)
   		TC.Changer_Nb_Elements (nombreElements) ;
	}

	if (OuiPourLesValeurs)
	{
		TP.Changer_Nb_Elements (0) ; // vide sans désallouer

		if (TP.Nb_Elements () < nombreElements)
   		TP.Changer_Nb_Elements (nombreElements) ;
	}

   if (a)
   	a->RemplirTableau (OuiPourLesCles,TC,OuiPourLesValeurs,TP) ;
}


template <class TypeCleTable> 
int	TablesEfficaces <TypeCleTable> :: NombreElements () const
{
	return nombreElements ;
}


template <class TypeCleTable> 
const TablesEfficaces <TypeCleTable> & TablesEfficaces <TypeCleTable> :: operator = (const TablesEfficaces &o)
// L'affectation ne provoque pas la duplication des valeurs associées
// aux clés : les pointeurs sont ré-utilisés tels quels.
{
	if (this != &o)
   {
   	if (o.a)
      	a = (o.a)->Dupliquer () ;
      else
      	a = NULL ;

      nombreElements = o.nombreElements ;
   }

   return *this ;
}


template <class TypeCleTable> bool	TablesEfficaces <TypeCleTable> :: EstValide ()
{
	int	h ;
	if (a==NULL)
   	return true ;
   else
   	return a->EstValide (h) ;
}


template <class TypeCleTable> bool			TablesEfficaces <TypeCleTable> :: EstDans (const TypeCleTable &c, void *&val)
{
	if (a)
   	return a->EstDans (c,val) ;
   else
   {
   	val = NULL ;
   	return false ;
   }
}


template <class TypeCleTable>
void	TablesEfficaces <TypeCleTable>::Min (TypeCleTable &c, void *&val)
{
	if (a)
   	a->Min (c,val) ;
}


template <class TypeCleTable>
void	TablesEfficaces <TypeCleTable>::Max (TypeCleTable &c, void *&val)
{
	if (a)
   	a->Max (c,val) ;
}


template <class TypeCleTable> bool TablesEfficaces <TypeCleTable> :: ModifierValeur( const TypeCleTable& c, void*& val )
	// Modifier la valeur associée à la clé c. Si la clé est absente, le résultat est faux.
	// Sinon le remplacement est effectué.
	// ATTENTION: l'ancienne valeur n'est pas désallouée. L'appelant doit s'en occuper.
{
	if (a)
   {
      return a->ModifierValeur(c,val) ;
   }
   else
   {
	 return false ;
   }
}



// Matériel pour vérifier les allocations / désallocations

// type (classe) pour décrire une allocation
class UneAllocation
{
	public :
      UneAllocation ()
      : pointeur (NULL)
      {}

      UneAllocation (void	*p)
      : pointeur (p)
      {}

   	void			*pointeur ;

   friend bool operator == (const UneAllocation &a1, const UneAllocation &a2)
   {
   	return a1.pointeur == a2.pointeur ;
   }

   friend bool operator < (const UneAllocation &a1, const UneAllocation &a2)
   {
   	return a1.pointeur < a2.pointeur ;
   }

   friend bool operator > (const UneAllocation &a1, const UneAllocation &a2)
   {
   	return a1.pointeur > a2.pointeur ;
   }

} ;


////////////////////////////////////////////////
//
// ensemble des allocations réalisées
//
////////////////////////////////////////////////

TablesEfficaces <UneAllocation>	LA ;


////////////////////////////////////////////////
//
// Nombre de pointeurs et d'octets actuellement réservés par l'appli (à l'aide de MonMalloc ou des versions
// surchargées de new et new []) et non libérés (avec MonFree ou la version surchargée de delete et delete [])
//
////////////////////////////////////////////////

static long double	NombreOctetsAlloues = 0 ;
static int		NombrePointeursAlloues = 0 ;



////////////////////////////////////////////////
//
// Fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle malloc et augmente la variable contenant
// le nombre d'octets alloués
//
////////////////////////////////////////////////

void *MonMalloc (size_t n)
{
	void		*Pt = malloc (n) ;

	if ( ! FautCompter)
		return Pt ;

	else if (Pt) {
		size_t	*PtQuantite = (size_t *) malloc (sizeof (size_t)) ;
		*PtQuantite = n ;

		UneAllocation Alloc (Pt) ;

		FautCompter = false ;
		LA.Inserer (Alloc,PtQuantite) ;
		FautCompter = true ;

		NombreOctetsAlloues += n ;
		NombrePointeursAlloues ++ ;

		return Pt ;
	}
	else
		return NULL ;
}

////////////////////////////////////////////////
//
// Deuxième fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle calloc et augmente la variable contenant
// le nombre d'octets alloués
//
////////////////////////////////////////////////

void *MonCalloc (size_t Nbr, size_t Taille)
{
	size_t		n = Nbr*Taille ;	// Nb total d'octets alloués
	void		*Pt = calloc (Nbr,Taille) ;

	if ( ! FautCompter)
		return Pt ;

	else if (Pt) {
		size_t	*PtQuantite = (size_t *) malloc (sizeof (size_t)) ;
		*PtQuantite = n ;

		UneAllocation Alloc (Pt) ;

		FautCompter = false ;
		LA.Inserer (Alloc,PtQuantite) ;
		FautCompter = true ;

		NombreOctetsAlloues += n ;
		NombrePointeursAlloues ++ ;

		return Pt ;
	}
	else
		return NULL ;
}


////////////////////////////////////////////////
//
// Fonction de ré-allocation : elle remplace realloc.
//
////////////////////////////////////////////////

void *MonRealloc (void *P, size_t NouvTaille)
{
	if ( ! FautCompter) {
		return realloc (P,NouvTaille) ;
	}

	if (P == NULL)
		return MonMalloc (NouvTaille) ;

	// Recherche du pointeur à ré-allouer dans l'ensemble des allocations
	size_t			*PtQuantite ;
	void			*Pt ;
	UneAllocation	Alloc (P) ;

	FautCompter = false ;
	bool SuppressionReussie = LA.Supprimer (Alloc,Pt) ;
	FautCompter = true ;

	if ( ! SuppressionReussie) {
		// tentative de désallouer un pointeur non alloué
		cout << "tentative de reallouer un pointeur non alloue ou deja desalloue. Tapez une touche." << endl ;
		system ("pause") ;
	}
   else {
		P = realloc (P,NouvTaille) ;
		PtQuantite = (size_t *) Pt ;
		NombreOctetsAlloues -= *PtQuantite ;
		NombreOctetsAlloues += NouvTaille ;

		*PtQuantite = NouvTaille ;

		UneAllocation Alloc (P) ;

		FautCompter = false ;
		LA.Inserer (Alloc,PtQuantite) ;
		FautCompter = true ;
   }

   return P ;
}

////////////////////////////////////////////////
//
// Fonction de libération : elle cherche le pointeur à désallouer dans
// la liste des allocations, le désalloue et diminue la variable contenant
// le nombre d'octets alloués
//
////////////////////////////////////////////////

void MonFree (void *p)
{
	if (p == NULL)
		return ;

	if ( ! FautCompter) {
		free (p) ;
		return ;
	}

	// Recherche du pointeur à désallouer dans l'ensemble des allocations
	size_t			*PtQuantite ;
	void			*Pt ;
	UneAllocation	Alloc (p) ;

	FautCompter = false ;
	bool SuppressionReussie = LA.Supprimer (Alloc,Pt) ;
	FautCompter = true ;

	if ( ! SuppressionReussie) {
		// tentative de désallouer un pointeur non alloué
		cout << "tentative de désallouer un pointeur non alloué. Tapez une touche." << endl ;
		char c ;
		cin >> c ;
	}
   else {
		free (p) ;
		PtQuantite = (size_t *) Pt ;
		NombreOctetsAlloues -= *PtQuantite ;
		NombrePointeursAlloues -- ;
		free (PtQuantite) ;
   }
}

////////////////////////////////////////////////
//
// Restitution du nombre de pointeurs et du nombre d'octets encore alloués pour l'instant
// En fin d'exécution, ces nombres doivent être nuls.
//
////////////////////////////////////////////////

void VerificationMemoire (int *PtNbPointeursNonLiberes, long int *PtNbOctetsNonLiberes)
{
	*PtNbOctetsNonLiberes = NombreOctetsAlloues ;
	*PtNbPointeursNonLiberes = NombrePointeursAlloues ;
}


////////////////////////////////////////////////
//
// Surcharges des opérateurs new et delete
//
////////////////////////////////////////////////

void * operator new (size_t Taille)    // Pour non-tableau
{
	void * p = MonMalloc (Taille) ;
	return p ;
}

void * operator new[](size_t Taille)  // Pour tableaux
{
	void * p = MonMalloc (Taille) ;
	return p ;
}

void operator delete(void *p)     // Pour non-tableaux
{
	MonFree (p) ;
}

void operator delete[](void * p)  // Pour tableaux
{
	MonFree (p) ;
}

