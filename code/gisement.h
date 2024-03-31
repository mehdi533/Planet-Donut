/**gisement.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED

#include <iostream>
#include <string>

#include "geomod.h"
#include "constantes.h"
#include "message.h"

class Gisement{
public:
	Gisement(std::string);
	~Gisement();
	
	void write(std::ofstream&);
	
	void draw_gisement();
	
	void intersection(Gisement const&);
	
	Point  get_centre() const;
	double get_rayon() const;
	double get_capacite() const;
	bool   get_error() const;
	
	bool update_gisement();
	
private:
	
	// We use this boolean to know if there was any error in the construction 
	// of planetdonut
	bool error = false;
	
	Point centre;
	double rayon, capacite, gisementR;
	Couleur color = NOIR;
	
};

#endif // GISEMENT_H
