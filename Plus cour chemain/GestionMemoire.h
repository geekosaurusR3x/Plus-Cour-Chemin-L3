//---------------------------------------------------------------------------
#ifndef GestionMemoireH
#define GestionMemoireH

#include <stdlib.h>

// Versions surchargées des 2 opérateurs new
void * operator new (size_t Taille) ;	// Pour non-tableau
void * operator new[] (size_t Taille) ;	// Pour tableaux

// Versions surchargées des 2 opérateurs free
void operator delete (void *p);     // Pour non-tableaux
void operator delete[] (void *p);  // Pour tableaux

// Fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle malloc et augmente la variable contenant
// le nombre d'octets alloués
extern void *MonMalloc (size_t n) ;

// Deuxième fonction d'allocation : elle enregistre la demande d'allocation
// dans l'ensemble, appelle calloc et augmente la variable contenant
// le nombre d'octets alloués
extern void *MonCalloc (size_t Nbr, size_t Taille) ;

// Fonction de ré-allocation : elle remplace realloc.
void *MonRealloc (void *P, size_t NouvTaille) ;

// Fonction de libération : elle cherche le pointeur à désallouer dans
// la liste des allocations, le désalloue et diminue la variable contenant
// le nombre d'octets alloués
extern void MonFree (void *p) ;


// Restitution du nombre de pointeurs et du nombre d'octets encore alloués pour l'instant.
// En fin d'exécution, ce nombre doit être nul.
extern void VerificationMemoire (int *PtNbPointeursNonLiberes, long int *PtNbOctetsNonLiberes) ;

#endif
