/**robot.cc
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>
#include <vector>

#include "robot.h"

using namespace std;

Robot::Robot(int id, Point centre, Point goal):uid(id), emplacement(centre), 
											   but(goal), base_centre(centre){}

Robot::Robot(int id, Point centre):uid(id), emplacement(centre), but(centre), 	
								   base_centre(centre){}
											   
Robot::Robot(){}

Prospection::Prospection(string line, Point centre){
	istringstream data(line);
	data >> uid >> dp >> emplacement.x >> emplacement.y >> but.x >> 
	but.y >> boolalpha >> atteint >> boolalpha >> retour >> boolalpha >> found;
	if(found) 
		data>>emplacement_gisement.x>>emplacement_gisement.y>>rayong>>capaciteg;
	base_centre=centre;
}

Prospection::Prospection(int id, Point centre):Robot(id,centre), found(false){}
											   
Forage::Forage(string line, Point centre){
	istringstream data(line);
	data >> uid >> dp >> emplacement.x >> emplacement.y >> but.x >> but.y >> boolalpha 
	>> atteint;
	base_centre=centre;
}

Forage::Forage(int id, Point centre):Robot(id,centre){}

Transport::Transport(string line, Point centre){
	istringstream data(line);
	data >> uid >> dp >> emplacement.x >> emplacement.y >> but.x >> but.y >> boolalpha 
	>> atteint >> boolalpha >> retour;
	base_centre=centre;
}

Transport::Transport(int id, Point centre):Robot(id,centre){}

Communication::Communication(string line, Point centre){
	istringstream data(line);
	data >> uid >> dp >> emplacement.x >> emplacement.y >> but.x >> but.y >> 
	boolalpha >> atteint;
	base_centre=centre;
}

Communication::Communication(int id, Point centre, Point goal): Robot(id,centre,goal){}

//============================== getters and setters ===============================//

Point Robot::get_but() const{
	return but;
}

unsigned Robot::get_uid() const{
	return uid;
}
Point Robot::get_emplacement() const{
	return emplacement;
}

double Robot::get_dp() const{
	return dp;
}

bool Robot::is_prosp_bot() const{
	return false;
}

bool Prospection::is_prosp_bot() const{
	return true;
}

bool Robot::is_forage_bot() const{
	return false;
}

bool Forage::is_forage_bot() const{
	return true;
}

bool Robot::is_transp_bot() const{
	return false;
}

bool Transport::is_transp_bot() const{
	return true;
}

bool Robot::is_comm_bot() const{
	return false;
}

bool Communication::is_comm_bot() const{
	return true;
}

bool Robot::found_gisement() const{
	return false;
}

bool Prospection::found_gisement() const{
	return found;
} 

Point Robot::get_emplacement_gisement() const{
	return base_centre;
}

Point Prospection::get_emplacement_gisement() const{
	return emplacement_gisement;
}

int Robot::get_gisement_goal(vector<Gisement> gisements) const{
	return -1;
}

int Forage::get_gisement_goal(vector<Gisement> gisements) const{
	for(size_t i(0); i<gisements.size(); ++i)
		if(gisements[i].get_centre()==but)
			return i;
	return -1;
}

int Robot::get_gisement(vector<Gisement> gisements) const{
	return -1;
}

int Prospection::get_gisement(vector<Gisement> gisements) const{
	for(size_t i(0); i<gisements.size(); ++i)
		if(gisements[i].get_centre()==emplacement_gisement)
			return i;
	return -1;
}

void Robot::set_objectif(Point pt){
	atteint=false;
	but=pt;
}

void Robot::set_visited_false(){
	visited=false;
}

void Prospection::set_retour(){
	
	double distance_a_base = geomod::norme_min(Vecteur(emplacement,base_centre));
	
	if(distance_a_base+deltaD > maxD_prosp-dp){
		retour = true;
		but = emplacement;
		atteint=false;
	}
}

void Robot::suppr_found(){}

void Prospection::suppr_found(){
	found = false;
}

//=============================== draw robots =======================================//

void Robot::draw_robot(bool range,bool links, Couleur color){
	geomod::draw_circle({emplacement, 25*display_rayon_robot}, color, true);
	
	if(links)
		for(auto voisins:voisins)
			geomod::draw_lines(emplacement,voisins->emplacement, VIOLET, rayon_comm);
	if(range) 
		geomod::draw_circle({emplacement,rayon_comm}, GRIS, false);
}

void Communication::draw_robot(bool range, bool links, Couleur color){
	Robot::draw_robot(range,links,color);
	
	//draws the wifi sign for the communication bots
	geomod::draw_arc({emplacement, 15}, M_PI/4, 3*M_PI/4, 3.00, color);
	geomod::draw_arc({emplacement, 25}, M_PI/4, 3*M_PI/4, 3.00, color);
	geomod::draw_arc({emplacement, 35}, M_PI/4, 3*M_PI/4, 3.00, color);
}

//============================= add or remove neighbours ============================//

void Robot::add_voisins(Robot* new_voisin){
	bool is_new(true);
	for(auto voisin:voisins)
		if(new_voisin==voisin)
			is_new = false;
			
	if(is_new)
		voisins.push_back(new_voisin);
}

void Robot::clear_voisins(){
	voisins.clear();
}

void Robot::rec_DFS(vector<Robot*>& connecte){
	
	visited=true;
	
	for(unsigned i(0); i<voisins.size() ; ++i){
		if(!(voisins[i]->visited)){
			connecte.push_back(voisins[i]);
			voisins[i]->rec_DFS(connecte);
		}
	}
	
}
//===================== maintenance functions for all bots ==========================//

double Robot::maintenance(){
	double cout_maintenance=cost_repair*dp;
	dp=0;
	return cout_maintenance;
}

double Prospection::maintenance(){
	retour = false;
	return Robot::maintenance();
}

double Transport::maintenance(){
	double cout_maintenance(0);
	
	cout_maintenance=Robot::maintenance();
	
	if(atteint){
		cout_maintenance-=deltaR;
		atteint=false;
	}else but = base_centre;
	
	
	retour = false;
	return cout_maintenance;
}

//===================================================================================//
	
void Robot::direction(Point pt){
	
	Vecteur vect = geomod::normalisation(Vecteur(emplacement,pt));
	
	if(vect.get_norme()==Vecteur(emplacement,pt).distance())
		vect=Vecteur(emplacement,pt);
	
	Point pos_to_goal = {vect.get_point2().x-emplacement.x,
						 vect.get_point2().y-emplacement.y};
	
	if(vect.get_norme()<=deltaD){
		emplacement=pt;
		return;
	}
	
	double angle = atan2(pos_to_goal.y,pos_to_goal.x);
	
	Point travel_dir = {cos(angle), sin(angle)};
	
	emplacement.x += deltaD*travel_dir.x;
	emplacement.y += deltaD*travel_dir.y;
	
	emplacement = geomod::normalisation(emplacement);
	
	dp+=deltaD;
}

void Prospection::test_intersect_gisement(vector<Gisement> gisements){
	for(size_t i(0); i<gisements.size(); ++i){
		if(geomod::appartient_cercle(gisements[i].get_rayon(), 
		   geomod::norme_min(Vecteur(emplacement, gisements[i].get_centre())))){
			found=true;
			emplacement_gisement=gisements[i].get_centre();
			rayong=gisements[i].get_rayon();
			capaciteg=gisements[i].get_capacite();
		}
	}
}

bool Transport::is_arrived(vector<Gisement>& gisements){
	for(size_t i(0); i<gisements.size(); ++i)
		if(geomod::appartient_cercle(gisements[i].get_rayon(), 
			   geomod::norme_min(Vecteur(but, gisements[i].get_centre()))))
			if(geomod::appartient_cercle(gisements[i].get_rayon(), 
			   geomod::norme_min(Vecteur(emplacement, gisements[i].get_centre())))){
				if(gisements[i].update_gisement())
					atteint = true;
				return true;
			}
	
	return false;
}

//=================== Remote and autonomous for all bots ============================//

//both of these functions should not be used, they are virtual.
void Robot::remote(vector<Gisement>& gisements){}
void Robot::autonome(vector<Gisement>& gisements){}

void Prospection::remote(vector<Gisement>& gisements){

	if(!retour)
		set_retour();
		
	if(retour){
		direction(base_centre);
		test_intersect_gisement(gisements);
		return;
	}
	
	if(atteint){
		atteint=false;
		but = geomod::normalisation({but.x+rayon_min/4,but.y+dim_max/2});
		direction(but);
	}else direction(but);
	
	if(emplacement==but)
		atteint=true;
	
	test_intersect_gisement(gisements);
	
}
void Prospection::autonome(vector<Gisement>& gisements){
			
	if(found){
		direction(base_centre);
		return;
	}
	
	if(!retour)
		set_retour();
	
	if(retour){
		direction(base_centre);
		test_intersect_gisement(gisements);
		return;
	}
	
	if(atteint){
		atteint=false;
		but = geomod::normalisation({but.x+rayon_min/2, but.y+dim_max});
		direction(but);
	}else direction(but);
	
	if(emplacement==but)
		atteint=true;
	
	test_intersect_gisement(gisements);
}

void Forage::remote(vector<Gisement>& gisements){
	if(atteint)
		return;
	
	direction(but);
	
	for(auto gisement:gisements)
		if(gisement.get_centre()==but)
			if(geomod::appartient_cercle(gisement.get_rayon(),geomod::norme_min
					  (Vecteur(emplacement, but))))
				atteint = true;
	
}

void Forage::autonome(vector<Gisement>& gisements){
	if(atteint)
		return;
	
	direction(but);
		
	for(auto gisement:gisements)
		if(gisement.get_centre()==but)
			if(geomod::appartient_cercle(gisement.get_rayon(),geomod::norme_min
					  (Vecteur(emplacement, but))))
				atteint = true;
	
}

void Transport::remote(vector<Gisement>& gisements){
	if(retour){
		direction(base_centre);
		return;
	}
	direction(but);
	if(is_arrived(gisements))
		retour=true;
}

void Transport::autonome(vector<Gisement>& gisements){	
	if(retour){
		direction(base_centre);
		return;
	}
	direction(but);
	if(is_arrived(gisements))
		retour=true;
}

void Communication::remote(vector<Gisement>& gisements){
	if(atteint)
		return;
	direction(but);
	
	if(emplacement==but)
		atteint=true;
}

void Communication::autonome(vector<Gisement>& gisements){
	if(atteint) 
		return;
	direction(but);
	
	if(emplacement==but)
		atteint=true;
}

//===================================================================================//

void Robot::write(ofstream& file){
	file << uid << " " << dp << " " << emplacement.x << " " << emplacement.y << " " <<
	but.x << " " << but.y << " " << boolalpha << atteint;
}

void Prospection::write(ofstream& file){
	Robot::write(file);
	file << " " << boolalpha << retour << " " << boolalpha << found;
	if(found) 
		file << " " << emplacement_gisement.x << " " << emplacement_gisement.y << " " 
		<< rayong << " " << capaciteg;
}

void Transport::write(ofstream& file){
	Robot::write(file);
	file << " " << boolalpha << retour;
}
