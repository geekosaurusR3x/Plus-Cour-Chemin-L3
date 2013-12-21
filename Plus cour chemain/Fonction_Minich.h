void EquationDroite (	double x1, double y1, double x2, double y2,
			double *Pa, double *Pb, double *Pc);

bool RecouvrementSegmentsAlignes (	double x1, double y1, double x2, double y2,
					double x3, double y3, double x4, double y4);

bool ExisteIntersectionSegmentSegment (double xa,double ya,double xb,double yb,
					double xc,double yc,double xd,double yd);


double DistancePointPoint (double x1, double y1, double x2, double y2);

double DistanceSegmentSegment (	double xa, double ya, double xb, double yb,
				double xc, double yc, double xd, double yd);

double DistanceSegmentRectangle (double x1, double y1, double x2, double y2,
				double xbg, double ybg, double xhd, double yhd);

double LongueurArc (    double xc, double yc,
                    double R,
                    double x1, double y1, double x2, double y2);

void TangenteSegmentArc (    double x, double y,    // Point de départ du segment
                                   double xc, double yc, double R,    // Cercle
                          int Quart,    // Numéro du quart (1,2,3,4)
                          int *PtNbPt,    // Nombre de points de tangence
                          double *Pxt1, double *Pyt1,    // point de tangence 1
                          double *Pxt2, double *Pyt2,    // point de tangence 2
                          double *Pangle1,
                          double *Pangle2);

void PointsArriveeSurCoin (double x, double y,    // Point de départ du cercle
                                   double xc, double yc,    // Coin
                          double R,    // rayon du cercle
                          int NumCoin,    // 1 pour le coin haut-droit, 2 pour le haut-gauche, 3 pour le bas-gauche, 4 pour le bas-droit
                          int *PtNbPt,    // Nombre de points d'arrivée
                          double *Pxt1, double *Pyt1,    // point d'arrivée 1
                          double *Pxt2, double *Pyt2,    // point d'arrivée 2
                          double *Pangle1,
                          double *Pangle2);