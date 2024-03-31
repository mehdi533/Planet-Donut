/**base.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "robot.h"
#include "gisement.h"
#include "message.h"
#include "geomod.h"

struct SimData {
	unsigned nbP;
	unsigned nbF;
	unsigned nbT;
	unsigned nbC;
	double ressource;
};

class Base{
public:
	Base(std::vector<std::string>,int);
	~Base();
	void write(std::ofstream&);
	
	void draw_base(bool,bool);
	
	Point    get_centre() const;
	double   get_ressource() const;
	unsigned get_couleur_base() const;
	SimData  get_data() const;
	bool  	 get_error() const;
	bool     get_destroyed() const;
	
	void     set_destroyed();
	
	void intersection(Base const&);
	
	void connexion();
	void set_bots_visited();
	
	void update_voisin(Base);
	void clear_voisins();
	
	void maintenance();
	void creation();
	void update(std::vector<Gisement>&);
	
	void empty();

private:
	
	// We use this boolean to know if there was any error in the construction 
	// of planetdonut
	bool error = false;
	
	Point centre;
	SimData data;
	
	std::vector<Robot*> robots;
	
	Couleur couleur;
	void set_couleur_base(unsigned);
	
	bool is_destroyed = false;
	
	void check_errors(bool);
	//Contains every robot connected to the base.
	std::vector<Robot*> connecte;
	bool is_connecte(Robot*);
	
	//contains the goal of every communication bot.
	std::vector<Point> network;
	void set_network();
	
	unsigned nbG_found = 0;
	std::vector<Point> visited_gisement;
	std::vector<Robot*> forage_has_visited;
	std::vector<int> visited_forage;
	
	void set_parcours_com();
	void set_parcours_prosp();
	void set_parcours_transp(std::vector<Gisement>);
	void set_parcours_forage(std::vector<Gisement>);
	
	int intersect_gisement(std::vector<Gisement>,Point);
	
	void create_prosp(unsigned&);
	void create_forage(unsigned&);
	void create_com(unsigned&);
	void create_transp(unsigned&);
	
	bool uid_already_used(unsigned) const;
	int  new_uid();
	
	bool not_autonome(Robot*) const;
	bool not_remote(Robot*) const;
	
	bool not_visited(std::vector<int>,int);
	bool not_visited(std::vector<Point>,Point);
	
};
	
#endif // BASE_H
