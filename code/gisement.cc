/**gisement.cc
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "gisement.h"

using namespace std;

Gisement::Gisement(string line){
	istringstream data(line);
	
	data >> centre.x >> centre.y >> rayon >> capacite;
	
	gisementR = 1000*pow(rayon/rayon_min, 2);
}

Gisement::~Gisement(){}

void Gisement::write(ofstream &file){
	file << centre.x << " " << centre.y << " " << rayon << " " << capacite << "\n";
}

void Gisement::intersection(Gisement const& g){
	double norme=geomod::norme_min(Vecteur(g.get_centre(), centre));
	if(geomod::cercles_intersect(norme,g.get_rayon(),rayon)){
		cout << message::field_superposition(g.get_centre().x, g.get_centre().y, 
											 centre.x, centre.y);
		error=true; 
	}
}

void Gisement::draw_gisement(){
	geomod::draw_circle({centre, rayon}, color,true);
	if(capacite<=0)
		geomod::draw_arc({centre,rayon-rayon/4},0,2*M_PI,25,BLANC);
	else geomod::draw_arc({centre,rayon-rayon/4},0,2*M_PI*(1-capacite/gisementR),
						   25,BLANC);
}

bool Gisement::update_gisement(){
	if(capacite>=deltaR){
		capacite-=deltaR;
		return true;
	}else
		capacite=0;
	return false;
}

//================================ getter functions =================================//

Point Gisement::get_centre() const{
	return centre;
}

double Gisement::get_rayon() const{
	return rayon;
}

double Gisement::get_capacite() const{
	return capacite;
}

bool Gisement::get_error() const{
	return error;
}
