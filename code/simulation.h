/**simulation.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "gisement.h"
#include "base.h"
#include "geomod.h"
#include "message.h"
#include "constantes.h"

class Simulation{
public:

	void lecture(char* nom_fichier);
	void write(std::ofstream&);
	
	bool is_empty() const;
	
	void step();
	void draw_planet(bool,bool);
	std::vector<SimData> get_bases_data() const;
	
private:
	
	std::vector<Gisement> gisements;
	std::vector<Base> bases;
	
	bool error = false;
	
	void empty_simulation();
	void intersection_simulation();
	void update_voisins_bases();
	
	void decodage_ligne(std::string line);
	
};

#endif // SIMULATION_H
