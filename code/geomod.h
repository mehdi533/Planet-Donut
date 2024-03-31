/**geomod.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef GEOMOD_H
#define GEOMOD_H

#include <vector>

#include "constantes.h"
#include "graphic.h"

struct Point
{
    double x;
    double y;
    const bool operator==(const Point&);
};

struct Cercle
{
	Point centre;
	double rayon;
};

class Vecteur{
private:
	Point point1;
	Point point2;
	double norme;
public:
	Vecteur(Point a, Point b);
	Vecteur();
	Vecteur(Point a, Point b, double n);
	
	Vecteur set_vecteur(double max_);
	
	double distance();
	
	Point get_point1() const;
	Point get_point2() const;
	double get_norme() const;
	
	void set_norme(double x);
	void set_point1(Point a);
	void set_point2(Point a);

};

namespace geomod{

	Vecteur normalisation(Vecteur);
	Point normalisation(Point);
	std::vector<Point> rebouclement(Cercle);
	double norme_min(Vecteur);
	
	bool appartient_cercle(double r, double n);
	bool equal_zero(double norme);
	bool cercles_intersect(double n, double r1, double r2);
	bool cercle_hors_champ(Cercle);
	
	void draw_circle(Cercle,Couleur,bool);
	void draw_arc(Cercle,double,double,double,Couleur);
	void draw_lines(Point,Point,Couleur,double);
	
}

#endif // GEOMOD_H
