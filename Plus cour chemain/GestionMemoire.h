//---------------------------------------------------------------------------
#ifndef GestionMemoireH
#define GestionMemoireH

#include <stdlib.h>

// Versions surcharg�es des 2 op�rateurs new
void * operator new (size_t Taille) ;	// Pour non-tableau
void * operator new[] (size_t Taille) ;	// Pour tableaux

// Versions surcharg�es des 2 op�rateurs free
void operator delete (void *p);     // Pour non-tableaux
void operator delete[] (void *p);  // Pour tableaux

// Fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle malloc et augmente la variable contenant
// le nombre d'octets allou�s
extern void *MonMalloc (size_t n) ;

// Deuxi�me fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle calloc et augmente la variable contenant
// le nombre d'octets allou�s
extern void *MonCalloc (size_t Nbr, size_t Taille) ;

// Fonction de r�-allocation : elle remplace realloc.
void *MonRealloc (void *P, size_t NouvTaille) ;

// Fonction de lib�ration : elle cherche le pointeur � d�sallouer dans
// la liste des allocations, le d�salloue et diminue la variable contenant
// le nombre d'octets allou�s
extern void MonFree (void *p) ;


// Restitution du nombre de pointeurs et du nombre d'octets encore allou�s pour l'instant.
// En fin d'ex�cution, ce nombre doit �tre nul.
extern void VerificationMemoire (int *PtNbPointeursNonLiberes, long int *PtNbOctetsNonLiberes) ;

#endif
