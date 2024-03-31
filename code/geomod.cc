/**geomod.cc
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#include <iostream>
#include <cmath>
#include <vector>

#include "geomod.h"

using namespace std;

//========================== vecteur class ==========================================//

Vecteur::Vecteur(Point a, Point b):point1(a), point2(b){
	norme= distance();
}

Vecteur::Vecteur(Point a, Point b, double n):point1(a), point2(b), norme(n){}

Vecteur::Vecteur():point1({0,0}), point2({0,0}), norme(0){}

double Vecteur::distance(){
	
	double coordonnee_x=(point2.x - point1.x);
	double coordonnee_y=(point2.y - point1.y);
	
	return sqrt(pow(coordonnee_x, 2) + pow(coordonnee_y, 2));
}

Point Vecteur::get_point1() const{
	return point1;
}

Point Vecteur::get_point2() const{
	return point2;
}

double Vecteur::get_norme() const{
	return norme;
}

void Vecteur::set_norme(double x){
	norme=x;
}

void Vecteur::set_point1(Point a){
	point1=a; 
}

void Vecteur::set_point2(Point a){
	point2=a;
}


const bool Point::operator==(const Point& p){
	if(x==p.x and y==p.y)
		return true;
	return false;
}

//=============================== geomod ============================================//

Point geomod::normalisation(Point init_point){
    Point point_mod;
    
    if (init_point.x > dim_max)
		point_mod.x = init_point.x - 2*dim_max;
    else if (init_point.x < -dim_max)
		point_mod.x = init_point.x + 2*dim_max;
    else
		point_mod.x = init_point.x;
    
    if (init_point.y > dim_max)
		point_mod.y = init_point.y - 2*dim_max;
    else if (init_point.y < -dim_max)
		point_mod.y = init_point.y + 2*dim_max;
    else 
		point_mod.y = init_point.y;
    
    return point_mod;    
}

Vecteur geomod::normalisation(Vecteur v){
	Vecteur equival;
	Point point_equi;
	double norme_min(v.get_norme());
	v.set_norme(v.distance());

	for (int K_x = -1; K_x < 2; K_x++)
	{
		for (int K_y = -1; K_y < 2; K_y++)
		{
			equival.set_point1(v.get_point1());
			equival.set_point2({v.get_point2().x + 2 * K_x * dim_max, 
								v.get_point2().y + 2 * K_y * dim_max});
			equival.set_norme(equival.distance());

			if (equival.get_norme() < norme_min) 
			{
				norme_min=equival.get_norme();
				point_equi = {v.get_point2().x + 2 * K_x * dim_max,
							  v.get_point2().y + 2 * K_y * dim_max};
			}
		}
	}
	return Vecteur(v.get_point1(),point_equi, norme_min);
}

vector<Point> geomod::rebouclement(Cercle cercle){
	
	vector<Point> points_mod;
	//searches for all the points where we should redraw a circle
	if(cercle.centre.x-cercle.rayon<-dim_max)
		points_mod.push_back({cercle.centre.x+2*dim_max, cercle.centre.y});
		
	if(cercle.centre.x+cercle.rayon>dim_max)
		points_mod.push_back({cercle.centre.x-2*dim_max, cercle.centre.y});
		
	if(cercle.centre.y-cercle.rayon<-dim_max)
		points_mod.push_back({cercle.centre.x, cercle.centre.y+2*dim_max});
		
	if(cercle.centre.y+cercle.rayon>dim_max)
		points_mod.push_back({cercle.centre.x, cercle.centre.y-2*dim_max});
		
	if(cercle.centre.x-cercle.rayon<-dim_max&&cercle.centre.y-cercle.rayon<-dim_max)
		points_mod.push_back({cercle.centre.x+2*dim_max, cercle.centre.y+2*dim_max});
		
	if(cercle.centre.x+cercle.rayon>dim_max&&cercle.centre.y+cercle.rayon>dim_max)
		points_mod.push_back({cercle.centre.x-2*dim_max, cercle.centre.y-2*dim_max});
		
	if(cercle.centre.x-cercle.rayon<-dim_max&&cercle.centre.y+cercle.rayon>dim_max)
		points_mod.push_back({cercle.centre.x+2*dim_max, cercle.centre.y-2*dim_max});
		
	if(cercle.centre.x+cercle.rayon>dim_max&&cercle.centre.y-cercle.rayon<-dim_max)
		points_mod.push_back({cercle.centre.x-2*dim_max, cercle.centre.y+2*dim_max});
	
	return points_mod;
	
}

double geomod::norme_min(Vecteur v){
	return (normalisation(v).get_norme());
}

bool geomod::appartient_cercle(double r, double n){
	if(n<(r-dim_max*pow(10.0,-5)))
		return true;
	else return false;		
}

bool geomod::equal_zero(double norme){
	
    if(abs(norme) < dim_max*pow(10.0,-5))
		return true;
    else return false;
}

bool geomod::cercles_intersect(double n, double r1, double r2){
	
	if(n<(r1+r2-dim_max*pow(10.0,-5)))
		return true;
	else
		return false;
}

bool geomod::cercle_hors_champ(Cercle cercle){
	
	if(cercle.centre.x-cercle.rayon<-dim_max)
		return true;
		
	if(cercle.centre.x+cercle.rayon>dim_max)
		return true;
		
	if(cercle.centre.y-cercle.rayon<-dim_max)
		return true;
		
	if(cercle.centre.y+cercle.rayon>dim_max)
		return true;
		
	return false;
}

//====================== drawing functions of geomod ================================//

void geomod::draw_circle(Cercle circle, Couleur color, bool full){
	
	if(full){
		graphic::full_circle(circle.centre.x, circle.centre.y, circle.rayon, color);
			
		if(geomod::cercle_hors_champ(circle)){
			
			vector<Point> out_of_pic(geomod::rebouclement(circle));
				
			for(size_t j(0); j<out_of_pic.size(); ++j)
				graphic::full_circle(out_of_pic[j].x, out_of_pic[j].y, circle.rayon, 
									 color);
				
		}
	}else{
		graphic::circle(circle.centre.x, circle.centre.y, circle.rayon, color);
			
		if(geomod::cercle_hors_champ(circle)){
			
			vector<Point> out_of_pic(geomod::rebouclement(circle));
				
			for(size_t j(0); j<out_of_pic.size(); ++j)
				graphic::circle(out_of_pic[j].x, out_of_pic[j].y, circle.rayon, color);
		}
		
	}
				
}

void geomod::draw_arc(Cercle circle, double angle1, double angle2, double width,
					  Couleur color){
						  				  
	graphic::arc(circle.centre.x, circle.centre.y, circle.rayon, angle1, angle2,
				 width, color);
			
	if(geomod::cercle_hors_champ({circle.centre,(8/7)*circle.rayon})){
			
		vector<Point> out_of_pic(geomod::rebouclement(circle));
				
		for(size_t i(0); i<out_of_pic.size(); ++i)
			graphic::arc(out_of_pic[i].x, out_of_pic[i].y, circle.rayon, angle1, 
					     angle2, width, color);
								
	}
}

void geomod::draw_lines(Point p1, Point p2, Couleur color, double d_max){
	
	//Since there are always two different options to draw lines on a planet, we check
	//if the one we are drawing corresponds to what we wanted.

	if(sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)) > d_max){
			
		Vecteur tmp1 = geomod::normalisation(Vecteur(p1,p2));
		graphic::lines(p1.x, p1.y, tmp1.get_point2().x,tmp1.get_point2().y,color);
							   
		Vecteur tmp2 = geomod::normalisation(Vecteur(p2,p1));
		graphic::lines(p2.x, p2.y, tmp2.get_point2().x, tmp2.get_point2().y,color);
						   
	}else graphic::lines(p1.x, p1.y, p2.x, p2.y, color);
	
}
