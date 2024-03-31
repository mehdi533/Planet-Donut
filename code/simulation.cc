/**simulation.cc
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "simulation.h"

using namespace std;

//This is one of the 2 functions over 40 lines we are allowed to write

void Simulation::decodage_ligne(string line){
	
	istringstream data(line);
	
	enum Etat_Lecture{NB_GISEMENTS, GISEMENTS, NB_BASES, BASES};
	static int etat(NB_GISEMENTS);
	
	static int nb, nb_lignes_base, i(0), j(0);
	static vector<string> lines_base;
	
	switch(etat){
	case NB_GISEMENTS:
		data >> nb;
		if(nb==0) etat=NB_BASES;
		else etat=GISEMENTS;
		break;
	case GISEMENTS:
		++i;
		gisements.push_back(Gisement(line));
		if(i==nb) etat=NB_BASES;
		break;
	case NB_BASES:
		i=0;
		data >> nb;
		if(nb!=0) etat=BASES;
		break;
	case BASES:
		if(j==0){
			int tmp_nbP, tmp_nbF, tmp_nbT, tmp_nbC;
			double  tmp_ressources, tmp_x, tmp_y;
			
			data>>tmp_x>>tmp_y>>tmp_ressources>>tmp_nbP>>tmp_nbF>>tmp_nbT>>tmp_nbC;
			nb_lignes_base=tmp_nbP+tmp_nbF+tmp_nbT+tmp_nbC+1;
			
		}
		lines_base.push_back(line);
		++j;
		if(j==nb_lignes_base){
			bases.push_back(Base(lines_base,i));
			lines_base.clear();
			++i; // counts the number of bases on record
			j=0; // reinitialises the counter of the number of lines in a base
		}
		if(i==nb){
			intersection_simulation();
			etat=NB_GISEMENTS; // reinitialises the state to be able to reopen file
			i=0;
		}
		break;
	}
}

void Simulation::lecture(char* nom_fichier){
	//when reading a new file, deletes the info of the previous file (if there was any)
	empty_simulation();
	string line;
	ifstream fichier(nom_fichier);
	if(!fichier.fail()){
		while(getline(fichier>>ws, line)){
			if(line[0]=='#') continue;
			decodage_ligne(line);
		}
		if(!error)
			cout <<  message::success();
		else cout << "Please enter an other file\n" << endl;
	}else exit(EXIT_FAILURE);
	//Saves bots connected to each other for display purposes
	update_voisins_bases();
}

void Simulation::write(ofstream& file){
	file << gisements.size() << "\n";
	for(size_t i(0); i<gisements.size(); ++i)
		gisements[i].write(file);
	file << bases.size() << "\n";
	for(size_t i(0); i<bases.size(); ++i)
		bases[i].write(file);
}

void Simulation::intersection_simulation(){
	//Gisements intersection avec des bases
	
	for(size_t i(0); i<gisements.size(); ++i){
		
		for(size_t j(0); j<bases.size(); ++j){
			
			double norme=geomod::norme_min(Vecteur({bases[j].get_centre()},
												   {gisements[i].get_centre()}));
												   
			if(geomod::cercles_intersect(norme, gisements[i].get_rayon(),rayon_base)){
				
				cout << message::base_field_superposition(bases[j].get_centre().x, 
														  bases[j].get_centre().y, 
														  gisements[i].get_centre().x, 
														  gisements[i].get_centre().y);
			    empty_simulation();
			}
		}
	}
	//Gisements intersection avec d'autres gisements
	for(size_t i(0); i<gisements.size(); ++i){
		for(size_t j(0); j<gisements.size(); ++j){
			if(j>i) gisements[i].intersection(gisements[j]);
			if(gisements[i].get_error()){
				empty_simulation();
				error=true;
			}
		}
	}
	//Bases intersection avec d'autres bases
	for(size_t i(0); i<bases.size(); ++i)
		for(size_t j(0); j<bases.size(); ++j){
			if(j!=i) bases[j].intersection(bases[i]);
			if(bases[i].get_error()){
				 empty_simulation();
				 error=true;
			 }
		}	
}

void Simulation::step(){
	
	update_voisins_bases();
	for(size_t i(0); i<bases.size(); ++i){
		if(bases[i].get_ressource()>=finR || bases[i].get_destroyed())
			continue;
		bases[i].maintenance();
		bases[i].creation();
		bases[i].connexion();
		bases[i].update(gisements);
	}
	
	for(size_t i(0); i < bases.size(); ++i){
		if(bases[i].get_ressource()<=0){
			// We chose to do this rather than delete the base in ordrer to be able
			// to keep the same colors for the bases when you reopen the file.
			bases[i].set_destroyed();
		}
	}
	
	//Updating neighbors for display purposes only
	update_voisins_bases();
	
}

void Simulation::update_voisins_bases(){
	
	//resets all the vectors containing neighbors
	for(auto base:bases)
		base.clear_voisins();
	
	for(size_t i(0); i<bases.size(); ++i){
		if(bases[i].get_ressource()<=0)
			continue;
		for(size_t j(0); j<bases.size(); ++j){
			bases[i].update_voisin(bases[j]); //updates neighbours
			bases[i].set_bots_visited();
		}
	}
}

vector<SimData> Simulation::get_bases_data() const{
	
	vector<SimData> tmp;
	
	for(size_t i(0); i<bases.size(); ++i){
		
		if(bases[i].get_destroyed())
			continue;
		tmp.push_back(bases[i].get_data());
		
	}
	return tmp;
	
}

void Simulation::empty_simulation(){
	gisements.clear();
	bases.clear();
}

bool Simulation::is_empty() const{
	if(bases.size()==0 and gisements.size()==0)
		return true;
	return false;
}

void Simulation::draw_planet(bool range, bool links){
	
	for(auto base:bases)
		if(!(base.get_destroyed()))
			base.draw_base(range,links);
		
	for(auto gisement:gisements)
		gisement.draw_gisement();
		
}
