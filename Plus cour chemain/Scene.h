#ifndef _SCENE_
#define _SCENE_

typedef struct point {
	double x, y;
}SPoint, *Point;

typedef struct obstacle{
	struct point *PointBas, *PointHaut;
}SObstacle, *Obstacle;

typedef struct scene {
	struct point *PointBas, *PointHaut;
	double Rayon;
	struct point *Depart, *Arrivee;
	unsigned int NbObstacles;
	Obstacle *tab_obstacle;
}SScene, *Scene;

Point NewPoint(double, double);

double Get_x(Point);

double Get_y(Point);

Obstacle NewObstacle_Scene(Point, Point);

Point Get_point(Obstacle, bool); //booléen pour le "choix" du point à recuperer : 0 pour PointBas et 1 pour PointHaut

Scene NewScene(double, double, double, double, double);

Obstacle Get_obstacle(Scene, int);

double Get_rayon(Scene);

Point Get_depart(Scene);

Point Get_arrivee(Scene);

int Get_nbobstacles(Scene);

#endif _SCENE_