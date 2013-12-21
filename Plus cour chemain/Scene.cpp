#include <stdlib.h>
#include "Scene.h"

Point NewPoint(double x, double y)
{
	Point temp = (Point)malloc(sizeof(SPoint));
	temp -> x = x;
	temp -> y = y;
	return temp;
}

double Get_x(Point point)
{
	return point->x;
}

double Get_y(Point point)
{
	return point->y;
}
	
Obstacle NewObstacle_Scene(Point pointbas, Point pointhaut)
{
	Obstacle temp = (Obstacle)malloc(sizeof(SObstacle));
	temp->PointBas = pointbas;
	temp->PointHaut = pointhaut;
	return temp;
}

Point Get_point(Obstacle obstacle, bool pos)
{
	Point temp;
	if (pos == false)
		temp = obstacle->PointBas;
	else 
		temp = obstacle->PointHaut;
	return temp;
}

Scene NewScene(double rayon, double xbas, double ybas, double xhaut, double yhaut)
{
	Scene temp = (Scene)malloc(sizeof(SScene));
	temp->PointBas->x = xbas;
	temp->PointBas->y = ybas;
	temp->PointHaut->x = xhaut;
	temp->PointHaut->y = yhaut;
	temp->Rayon = rayon;
	return temp;
}

Obstacle Get_obstacle(Scene scene, int numobstacle)
{
	return scene->tab_obstacle[numobstacle];
}
	

double Get_rayon(Scene scene)
{
	return scene->Rayon;
}

Point Get_depart(Scene scene)
{
	return scene->Depart;
}

Point Get_arrivee(Scene scene)
{
	return scene->Arrivee;
}

int Get_nbobstacles(Scene scene)
{
	return scene->NbObstacles;
}




