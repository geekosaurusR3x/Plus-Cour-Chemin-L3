#include "Fonction_Minich.h"                                                       
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846
#define EPSANGLES 0.001


void EquationDroite (	double x1, double y1, double x2, double y2,
			double *Pa, double *Pb, double *Pc)
// Calcule l'équation d'une droite passant par les points (x1,y1) et (x2,y2),
// supposés DIFFERENTS !!
// PAS POUR VOUS
{
	*Pb = x1 - x2 ; *Pa = y2 - y1 ; *Pc = -*Pa * x1 - *Pb * y1 ;
}

bool RecouvrementSegmentsAlignes (	double x1, double y1, double x2, double y2,
					double x3, double y3, double x4, double y4)
// Restitue vrai si et seulement si les segments [P1,P2] et [P3,P4], supposés
// alignés, ont un recouvrement (même réduit à un point)
// PAS POUR VOUS
{
	double	dx34 = x4-x3,
   			dy34 = y4-y3,
            dx31 = x1-x3,
            dy31 = y1-y3,
            dx32 = x2-x3,
            dy32 = y2-y3,
            k1,k2 ;	// Abscisses curvilignes de P1 et P2 sur [P3,P4)

	if (fabs (dx34) > fabs (dy34)) {	// Pente faible (< 45 °)
   	k1 = dx31 / dx34 ; k2 = dx32 / dx34 ;
   }
   else {
   	k1 = dy31 / dy34 ; k2 = dy32 / dy34 ;
   }

   if (k2 < k1) {
   	double Temp = k1 ; k1 = k2 ; k2 = Temp ;
   }

   if ((k2 < -0.0001) || (k1 > 1.0001))
   	return false ;	// Le segment [P1,P2] commence au delà de P4 ou termine avant P3
   else
   	return true ;
}

bool ExisteIntersectionSegmentSegment (double xa,double ya,double xb,double yb,
					double xc,double yc,double xd,double yd)
// Restitue vrai ssi les segments ont au moins une intersection
// POUR VOUS
{
	double	dxAB = xb-xa,
   			dyAB = yb-ya,
            dxCD = xd-xc,
            dyCD = yd-yc,
            dxCA = xa-xc,
            dyCA = ya-yc ;

	if (fabs (dxAB*dyCD - dyAB*dxCD) < 0.0001)	// Segments parallèles
   	if (fabs (dxCA*dyCD - dyCA*dxCD) < 0.0001)	// segments alignés
      	return RecouvrementSegmentsAlignes (xa,ya,xb,yb,xc,yc,xd,yd) ;
      else														// segments strictement parallèles
      	return false ;
   else {
   	// Les segments sont sur deux droites sécantes. On vérifie si
      // A et B sont de part et d'autre de (C,D) et si C et D sont de
      // part et d'autre de (A,B)
      double	a1,b1,c1,	// Equation de (A,B)
      			a2,b2,c2 ;	// Equation de (C,D)

      EquationDroite (xa,ya,xb,yb,&a1,&b1,&c1) ;
      EquationDroite (xc,yc,xd,yd,&a2,&b2,&c2) ;

      double	CoteA = a2*xa + b2*ya + c2,
      			CoteB = a2*xb + b2*yb + c2,
               CoteC = a1*xc + b1*yc + c1,
               CoteD = a1*xd + b1*yd + c1 ;

      return (CoteA*CoteB <= 0) && (CoteC*CoteD <= 0) ;
   }
}


double DistancePointPoint (double x1, double y1, double x2, double y2)
// Distance entre deux points
// POUR VOUS
{
	return sqrt ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)) ;
}


double DistancePointSegment (	double x, double y,
				double xa, double ya, double xb, double yb)
// Distance entre un point P(x,y) et un segment [A,B] (A différent de B !!).
// Si P n'est pas du même côté de A que B, c'est dist (A,P)
// Sinon, si P n'est pas du même côté de B que A, c'est dist (B,P)
// sinon, c'est que P se projette dans [A,B] et donc c'est la distance de P
// à (A,B)
// PAS POUR VOUS
{
	double	dxAB = xb-xa,
   			dyAB = yb-ya,
            dxAP = x-xa,
            dyAP = y-ya,
            dxBP = x-xb,
            dyBP = y-yb ;

   if (dxAP*dxAB + dyAP*dyAB <= 0)	// P se projette avant A sur [A,B)
   	return DistancePointPoint (x,y,xa,ya) ;

   else if (dxBP*dxAB + dyBP*dyAB >= 0)	// P se projette après B sur [A,B)
   	return DistancePointPoint (x,y,xb,yb) ;

   else	// P se projette dans [A,B]
   	return fabs (-dxAP*dyAB + dyAP*dxAB) / DistancePointPoint (xa,ya,xb,yb) ;
}

double DistanceSegmentSegment (	double xa, double ya, double xb, double yb,
				double xc, double yc, double xd, double yd)
// Distance entre un segment [A,B] (A différent de B !!) et un segment [C,D]
// (C différent de D !!).
// Si les segments se coupent, c'est 0.
// Sinon c'est le min des distances de chacun des 4 points au segment auquel
// il n'appartient pas
// PAS POUR VOUS
{
	if (ExisteIntersectionSegmentSegment (xa,ya,xb,yb,xc,yc,xd,yd))
   	return 0 ;

   // Cas où les segments ne se coupent pas
	double	Dist = DistancePointSegment (xa,ya,xc,yc,xd,yd),
 				Dist2 = DistancePointSegment (xb,yb,xc,yc,xd,yd) ;

   if (Dist2 < Dist)	Dist = Dist2 ;

   Dist2 = DistancePointSegment (xc,yc,xa,ya,xb,yb) ;
	if (Dist2 < Dist)	Dist = Dist2 ;

   Dist2 = DistancePointSegment (xd,yd,xa,ya,xb,yb) ;
	if (Dist2 < Dist)	Dist = Dist2 ;

   return Dist ;
}

double DistanceSegmentRectangle (double x1, double y1, double x2, double y2,
				double xbg, double ybg, double xhd, double yhd)
// Distance entre un segment [P1,P2] (P1 différent de P2 !!) et un rectangle
// à bords parallèles aux axes et d'aire non nulle (xbg < xhd et ybg < yhd)
// POUR VOUS
{
	double	Dist = DistanceSegmentSegment (x1,y1,x2,y2,xbg,ybg,xhd,ybg),
   			Dist2 = DistanceSegmentSegment (x1,y1,x2,y2,xhd,ybg,xhd,yhd) ;

   if (Dist2 < Dist)	Dist = Dist2 ;

   Dist2 = DistanceSegmentSegment (x1,y1,x2,y2,xhd,yhd,xbg,yhd) ;
   if (Dist2 < Dist)	Dist = Dist2 ;

   Dist2 = DistanceSegmentSegment (x1,y1,x2,y2,xbg,yhd,xbg,ybg) ;
   if (Dist2 < Dist)	Dist = Dist2 ;

   return Dist ;
}

double LongueurArc (    double xc, double yc,
                    double R,
                    double x1, double y1, double x2, double y2)
// Restitue la longueur de l'arc de cercle de centre (xc,yc), de rayon R
// et délimité par les points (x1,y1) et (x2,y2).
// ATTENTION : (x1,y1) et (x2,y2) définissent en réalité deux arcs (un arc
//                    et son complémentaire. C'est la longueur du plus petit des deux
//                    qui est restituée.
{
   // On calcule les positions angulaires de P1 et P2
  double    Angle1 = atan2 (y1-yc,x1-xc),
              Angle2 = atan2 (y2-yc,x2-xc) ;

  // On calcule l'écart angulaire entre P1 et P2
  double    EcartAng = fabs (Angle2 - Angle1) ;

  if (EcartAng > PI)
      EcartAng = 2*PI - EcartAng ;

   // si P1 et P2 sont superposés (mêmes positions angulaires), le résultat
  // est 0
  if (fabs (EcartAng) < EPSANGLES)
      return 0 ;
  else
      return    R*EcartAng ;
}

void TangenteSegmentArc (    double x, double y,    // Point de départ du segment
                                   double xc, double yc, double R,    // Cercle
                          int Quart,    // Numéro du quart (1,2,3,4)
                          int *PtNbPt,    // Nombre de points de tangence
                          double *Pxt1, double *Pyt1,    // point de tangence 1
                          double *Pxt2, double *Pyt2,    // point de tangence 2
                          double *Pangle1,
                          double *Pangle2)
// Vérifie s'il est possible de construire des segments issus de (x,y) et
// tangents à un quart du cercle (xc,yc,R).
// Le quart de cercle souhaité est indiqué par l'entier Quart : 1 si c'est
// celui du premier quadrant, 2 pour celui du deuxième quadrant etc jusque 4.
// *PtNbPt est initialisé au nombre de segments tangents : 0, 1 ou 2. Il peut
// y avoir 2 segments si (x,y) est très proche de l'arc de cercle.
// Les extrémités des segments calculés sont restituées dans (*Pxt1,*Pyt1) et
// (*Pxt2,*Pyt2).
// *Pangle1 et *Pangle2 sont les angles en DEGRES des points de tangence
// par rapport au centre. Ils sont dans [0,360[.
// Quand il y a 2 points de tangence, le premier point restitué est tel que
// son angle soit inférieur à celui du deuxième
{
   // Si (x,y) est dans ou sur le cercle, pas de tangence possible
  double    dx = x - xc,
              dy = y - yc,
           CarreDistPC = dx*dx + dy*dy ;

  if (CarreDistPC <= R*R) {
      *PtNbPt = 0 ; return ;
  }

  // On calcule les 2 extrémités possibles pour un segment issu de (x,y)
  // et tangent au cercle
  double    DistPC = sqrt (CarreDistPC),    // dist de P au centre
              CarreDistPT = CarreDistPC  - R*R,    // carré de la dist de P aux points de tangence
              h1 = ((R*R - CarreDistPT) + CarreDistPC) / (2*DistPC), // dist de C au segment reliant les 2 points tangents
               h2 = sqrt (R*R - h1*h1),    // dist d'un point de tangence à [C,P]
           dxnorm = dx / DistPC,        // coord du vecteur CP normalisé
           dynorm = dy / DistPC ;

  *Pxt1 = xc + h1*dxnorm - h2*dynorm,    // premier point de tangence
  *Pyt1 = yc + h1*dynorm + h2*dxnorm,
  *Pxt2 = xc + h1*dxnorm + h2*dynorm,    // deuxième point de tangence
  *Pyt2 = yc + h1*dynorm - h2*dxnorm ;

  // Rejet des points de tangence qui ne sont pas sur le quart de cercle
  // considéré
  switch (Quart) {
      case 1 : if ((*Pxt2 < xc) || (*Pyt2 < yc))
                       if ((*Pxt1 < xc) || (*Pyt1 < yc))
                         *PtNbPt = 0 ;
                 else
                     *PtNbPt = 1 ;
                 else
                       if ((*Pxt1 < xc) || (*Pyt1 < yc)) {
                         *PtNbPt = 1 ;
                    *Pxt1 = *Pxt2 ; *Pyt1 = *Pyt2 ;
                 }
                 else {
                     *PtNbPt = 2 ;
                 }

                 break ;
      case 2 : if ((*Pxt2 > xc) || (*Pyt2 < yc))
                       if ((*Pxt1 > xc) || (*Pyt1 < yc))
                         *PtNbPt = 0 ;
                 else
                     *PtNbPt = 1 ;
                 else
                       if ((*Pxt1 > xc) || (*Pyt1 < yc)) {
                         *PtNbPt = 1 ;
                    *Pxt1 = *Pxt2 ; *Pyt1 = *Pyt2 ;
                 }
                 else
                     *PtNbPt = 2 ;

                 break ;
      case 3 : if ((*Pxt2 > xc) || (*Pyt2 > yc))
                       if ((*Pxt1 > xc) || (*Pyt1 > yc))
                         *PtNbPt = 0 ;
                 else
                     *PtNbPt = 1 ;
                 else
                       if ((*Pxt1 > xc) || (*Pyt1 > yc)) {
                         *PtNbPt = 1 ;
                    *Pxt1 = *Pxt2 ; *Pyt1 = *Pyt2 ;
                 }
                 else
                     *PtNbPt = 2 ;

                 break ;
      case 4 : if ((*Pxt2 < xc) || (*Pyt2 > yc))
                       if ((*Pxt1 < xc) || (*Pyt1 > yc))
                         *PtNbPt = 0 ;
                 else
                     *PtNbPt = 1 ;
                 else
                       if ((*Pxt1 < xc) || (*Pyt1 > yc)) {
                         *PtNbPt = 1 ;
                    *Pxt1 = *Pxt2 ; *Pyt1 = *Pyt2 ;
                 }
                 else
                     *PtNbPt = 2 ;

                 break ;
     default : abort () ;
  }

  // Calcul des angles des segments allant du centre du cercle aux points
  // de tangence éventuels
  if (*PtNbPt > 0) {
      *Pangle1 = atan2 (*Pyt1-yc,*Pxt1-xc)*180/PI ;
     if (*Pangle1 < 0)
         *Pangle1 += 360 ;

     if (*PtNbPt == 2) {
          *Pangle2 = atan2 (*Pyt2-yc,*Pxt2-xc)*180/PI ;
         if (*Pangle2 < 0)
             *Pangle2 += 360 ;

        if (*Pangle2 < *Pangle1) {
            double Temp = *Pangle2 ; *Pangle2 = *Pangle1 ; *Pangle1 = Temp ;
           Temp = *Pxt1 ; *Pxt1 = *Pxt2 ; *Pxt2 = Temp ;
           Temp = *Pyt1 ; *Pyt1 = *Pyt2 ; *Pyt2 = Temp ;
        }
     }
   }
}

void PointsArriveeSurCoin (double x, double y,    // Point de départ du cercle
                                   double xc, double yc,    // Coin
                          double R,    // rayon du cercle
                          int NumCoin,    // 1 pour le coin haut-droit, 2 pour le haut-gauche, 3 pour le bas-gauche, 4 pour le bas-droit
                          int *PtNbPt,    // Nombre de points d'arrivée
                          double *Pxt1, double *Pyt1,    // point d'arrivée 1
                          double *Pxt2, double *Pyt2,    // point d'arrivée 2
                          double *Pangle1,
                          double *Pangle2)
// Restitue le ou les deux points d'arrivée d'un cercle qui part de (x,y) et
// arrive sur un coin d'obstacle.
// Le coin souhaité est indiqué par l'entier NumCoin (de 1 à 4, voir commentaire plus haut).
// *PtNbPt est initialisé au nombre de points d'arrivée : 0, 1 ou 2. Il peut
// y en avoir 2 segments si (x,y) est très proche du coin.
// Les points calculés sont restituées dans (*Pxt1,*Pyt1) et (*Pxt2,*Pyt2).
// *Pangle1 et *Pangle2 sont les angles en DEGRES de ces points sur le cercle
// centré sur le coin et de rayon R. Ils sont dans [0,360[.
// Quand il y a 2 points restitués, le premier point est tel que
// son angle soit inférieur à celui du deuxième
{
   TangenteSegmentArc (x, y, xc, yc, R, NumCoin,
                              PtNbPt, Pxt1, Pyt1, Pxt2, Pyt2, Pangle1, Pangle2) ;
}