/**base.cc
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <vector>

#include "base.h"

using namespace std;

Base::Base(vector<string> lines,int c){
	
	istringstream file_data (lines[0]);
	
	file_data >> centre.x >> centre.y >> data.ressource >> data.nbP >> data.nbF >> 
			     data.nbT >> data.nbC;
			     
	for(unsigned i(0); i<data.nbP; ++i) 
		robots.push_back(new Prospection(lines[i+1], centre));
		
	for(unsigned i(data.nbP); i<(data.nbP+data.nbF); ++i){
		robots.push_back(new Forage(lines[i+1], centre));
		if(!(robots[i]->get_but()==centre))
			forage_has_visited.push_back(robots[i]);
	}
		
	for(unsigned i(data.nbP+data.nbF); i<(data.nbP+data.nbF+data.nbT); ++i) 
		robots.push_back(new Transport(lines[i+1], centre));
		
	bool comm_centre(false);
	
	for(unsigned i(data.nbP+data.nbF+data.nbT);i<(data.nbP+data.nbF+data.nbT+data.nbC);++i){
		
		robots.push_back(new Communication(lines[i+1], centre));
		
		network.push_back(robots[i]->get_but());
		if(geomod::equal_zero(geomod::norme_min(Vecteur(robots[i]->get_emplacement(),
														centre)))){
			if(!comm_centre)
				robots[i]->set_objectif(centre);
			comm_centre=true;
		}
	}
	check_errors(comm_centre);
	set_couleur_base(c);
	set_parcours_com();
	set_parcours_prosp();
}

void Base::check_errors(bool comm_centre){
	
	if(!comm_centre){
		cout << message::missing_robot_communication(centre.x, centre.y);
		error=true;
	}
	
	vector<int> visited;
	for(size_t i(0); i<robots.size(); ++i)
		visited.push_back(robots[i]->get_uid());
	sort(visited.begin(), visited.end());
	for(size_t i(1); i<visited.size(); ++i){
		if(visited[i]==visited[i-1]){
			cout << message::identical_robot_uid(visited[i]);
			error=true;
		}
	}
	
	if(!data.nbC){
		cout << message::missing_robot_communication(centre.x, centre.y);
		error=true;
	}
}

void Base::set_couleur_base(unsigned i){
	if(i%6==0)	couleur=ROUGE;
	if(i%6==1)	couleur=BLEU;
	if(i%6==2)	couleur=VERT;
	if(i%6==3)	couleur=JAUNE;
	if(i%6==4)	couleur=MAGENTA;
	if(i%6==5)	couleur=CYAN;
}

Base::~Base(){
	robots.clear();
}

void Base::set_destroyed(){
	robots.clear();
	connecte.clear();
	data={0,0,0,0,0};
	centre={0,0};
	is_destroyed=true;
}

void Base::write(ofstream& file){
	file << centre.x << " " << centre.y << " " << data.ressource << " " << data.nbP 
		 << " " << data.nbF << " " << data.nbT << " " << data.nbC << "\n";
	
	vector<Robot*> prosp;
	vector<Robot*> forage;
	vector<Robot*> transp;
	vector<Robot*> com;
	
	for(auto robot:robots){
		if(robot->is_prosp_bot())
			prosp.push_back(robot);
		if(robot->is_forage_bot())
			forage.push_back(robot);
		if(robot->is_transp_bot())
			transp.push_back(robot);
		if(robot->is_comm_bot())
			com.push_back(robot);
	} 
	
	for(auto robot:prosp){
		robot->write(file);
		file << "\n";
	}
	for(auto robot:forage){
		robot->write(file);
		file << "\n";
	}
	for(auto robot:transp){
		robot->write(file);
		file << "\n";
	}
	for(auto robot:com){
		robot->write(file);
		file << "\n";
	}
}


void Base::intersection(Base const& b){
	double norme=geomod::norme_min(Vecteur(b.get_centre(), centre));
	if(geomod::cercles_intersect(norme,rayon_base,rayon_base)){
		cout << message::base_superposition(centre.x, centre.y, b.get_centre().x, 
											b.get_centre().y);
		error=true;
	}
}

void Base::draw_base(bool range, bool links){
	
	geomod::draw_circle({centre, 50*rayon_base}, couleur, false);
	
	for(auto robots:robots)
		robots->draw_robot(range,links,couleur);
		
}

//=============================== getter functions ==================================//

unsigned Base::get_couleur_base() const{
	return couleur;
}

Point Base::get_centre() const{
	return centre;
}

double Base::get_ressource() const{
	return data.ressource;
}

SimData Base::get_data() const{
	return data;
}

bool Base::get_destroyed() const{
	return is_destroyed;
}

bool Base::get_error() const{
	return error;
}

//============================= functions for simulation ============================//

void Base::update_voisin(Base base){
	for(size_t i(0); i<robots.size(); ++i){
		for(size_t j(0); j<base.robots.size(); ++j){
			if(robots[i]==base.robots[j])
				continue;
			double distance;
			distance = geomod::norme_min(Vecteur(robots[i]->get_emplacement(), 
											     base.robots[j]->get_emplacement()));
			if(distance <= rayon_comm){
				robots[i]->add_voisins(base.robots[j]);
			}
		
		}
	}
}

void Base::clear_voisins(){
	for(auto robot:robots)
		robot->clear_voisins();
}

void Base::maintenance(){
	//Procedes to the maintenance of bots if they are in the middle of the base
	for(size_t i(0); i<robots.size(); ++i){
		double distance_centre=(geomod::norme_min(Vecteur(robots[i]->get_emplacement(), 
														  centre)));
		if(geomod::equal_zero(distance_centre) && robots[i]->get_dp()>0) 
			data.ressource-=robots[i]->maintenance();
	}
	if(data.ressource>finR)
		data.ressource=finR;
}

void Base::set_bots_visited(){
	//sets the boolean we use for the connexion algorithm back to false.
	for(auto bot:robots)
		bot->set_visited_false();
}

void Base::connexion(){
	connecte.clear();
	for(auto robot:robots)
		if(robot->is_comm_bot() && robot->get_emplacement()==centre){
			connecte.push_back(robot);
			robot->rec_DFS(connecte); 
		}
}

bool Base::is_connecte(Robot* bot){
	for(auto connect:connecte)
		if(connect==bot)
			return true;
	return false;
}

void Base::creation(){
	unsigned count(0);
	
	if(data.nbP==0)
		create_prosp(count);
		
	if(data.nbF==0)
		create_forage(count);
		
	if(data.nbT==0)
		create_transp(count);
	
	for(size_t i(0); i<connecte.size(); ++i)
		if(connecte[i]->found_gisement())
			if(not_visited(visited_gisement,connecte[i]->get_emplacement_gisement())){
				++nbG_found;
				visited_gisement.push_back(connecte[i]->get_emplacement_gisement());
				for(auto robot:robots)
					if(connecte[i]==robot)
						connecte[i]->suppr_found();
			}
	
	while(data.nbF<nbG_found && data.ressource>cost_forage && count<max_robots)
		create_forage(count);
	
	if(data.ressource>3*cost_transp && count<max_robots && data.nbT<5*nbG_found && 
	   data.nbT<25)
		create_transp(count);
	
	if((count<=1 || nbG_found==0) && data.ressource>cost_prosp && count<max_robots 
	   && data.nbP<10)
		create_prosp(count);	
	
	while(count<max_robots && data.ressource>cost_com && data.nbC<network.size())
		create_com(count);
}

//=========================== functions to create bots ==============================//

void Base::create_prosp(unsigned& count){
	int uid(new_uid());
	robots.push_back(new Prospection(uid,centre));
	data.ressource-=cost_prosp;
	data.nbP+=1;
	set_parcours_prosp();
	++count;
}
void Base::create_forage(unsigned& count){
	int uid(new_uid());
	robots.push_back(new Forage(uid,centre));
	data.ressource-=cost_forage;
	data.nbF+=1;
	++count;
}
void Base::create_com(unsigned& count){
	int uid(new_uid());
	robots.push_back(new Communication(uid,centre, network[data.nbC]));
	data.ressource-=cost_com;
	data.nbC+=1;
	++count;
}
void Base::create_transp(unsigned& count){
	int uid(new_uid());
	robots.push_back(new Transport(uid,centre));
	data.ressource-=cost_transp;
	data.nbT+=1;
	++count;
}	

int Base::new_uid(){
	for(size_t i(0); i<=robots.size(); ++i)
		if(!uid_already_used(i)) return i;
	return (robots.size()+1);
}

bool Base::uid_already_used(unsigned uid) const{
	for(size_t i(0); i<robots.size(); ++i)
		if(uid==robots[i]->get_uid())
			return true;
	return false;
}

//============ sets the goal of the prospection and communications bots =============//

void Base::set_parcours_com(){
	int multiplicateur_x(1);
	int multiplicateur_y(0);
	
	vector<Point> tmp_network;
	 
	for(size_t i(0); i<48; ++i){
		if(multiplicateur_x==7){
			multiplicateur_x=0;
			++multiplicateur_y;
		}
		tmp_network.push_back(geomod::normalisation({centre.x + 
											       (2*dim_max)/7 * multiplicateur_x,
											        centre.y + 
											       (2*dim_max)/7 * multiplicateur_y}));
		++multiplicateur_x;
	}
		
	for(size_t i(0); i<tmp_network.size(); ++i)
		for(size_t j(0); j<network.size(); ++j)
			if(geomod::equal_zero(geomod::norme_min(Vecteur(network[j],
															tmp_network[i]))))
				tmp_network.erase(tmp_network.begin()+i);
	
	for(size_t i(0); i<tmp_network.size(); ++i)
		network.push_back(tmp_network[i]);
		
}

void Base::set_parcours_prosp(){
	int multiplicateur_x(0);
	
	vector<double> goal;
	
	for(size_t i(0); i<data.nbP; ++i){
		goal.push_back((2*dim_max/data.nbP*multiplicateur_x)-dim_max);
		++multiplicateur_x;
	}
	
	vector<int> visited;
	
	double min(dim_max);
	int chosen_one(-1);
		
	for(size_t i(0); i<goal.size(); ++i){
		min=dim_max;
		chosen_one=-1;
		for(size_t j(0); j<robots.size(); ++j){
			if(robots[j]->is_prosp_bot())
				if(geomod::norme_min(Vecteur({robots[j]->get_emplacement().x,0},
									 {goal[i],0})) < min && not_visited(visited,j)){
					min=geomod::norme_min(Vecteur({robots[j]->get_emplacement().x,0},
												  {goal[i],0}));
					chosen_one = j;
				}
		}
		if(chosen_one!=-1 && robots[chosen_one]->is_prosp_bot()){
			robots[chosen_one]->set_objectif({goal[i],
											robots[chosen_one]->get_emplacement().y});
			visited.push_back(chosen_one);
		}
	}

}

//============these two functions are called every time we update the planet=========//
//==================they set the goal of forage and transport bots ==================//

void Base::set_parcours_forage(vector<Gisement> gisements){
	vector<Robot*> forage;
	vector<int> gisements_nb;
	
	for(auto bot:forage_has_visited)
		for(size_t i(0); i<gisements.size(); ++i)
			if(gisements[i].get_centre()==bot->get_but())
				visited_forage.push_back(i);
				
	for(auto robot:robots)
		if(robot->is_forage_bot() && is_connecte(robot))
			forage.push_back(robot);

	for(auto nb_g:visited_gisement)
		for(size_t i(0); i<gisements.size(); ++i)
			if(gisements[i].get_centre()==nb_g && not_visited(gisements_nb,i))
				gisements_nb.push_back(i);
				
	double ressource_max(-1);
	int chosen_one(-1);
	
	for(auto nb:gisements_nb)
		if(gisements[nb].get_capacite()>ressource_max && 
		   not_visited(visited_forage,nb))
			chosen_one = nb;
	for(auto forage:forage){
		int nb=forage->get_gisement_goal(gisements);
		if(nb==-1 && chosen_one<=gisements.size() && chosen_one !=-1 && 
		   not_visited(visited_forage,chosen_one)){
			forage->set_objectif(gisements[chosen_one].get_centre());
			visited_forage.push_back(chosen_one);
		}
	}
}

//This is the second function we are allowed to write with more than 40 lines.
void Base::set_parcours_transp(vector<Gisement> gisements){
	
	vector<Robot*> transp_connecte;
	
	for(auto robot:robots)
		if(robot->is_transp_bot() && is_connecte(robot))
			transp_connecte.push_back(robot);
	
	vector<Point> transp_location;
	
	for(auto robot:robots)
		if(robot->is_forage_bot() && (is_connecte(robot))){
			int tmp_1 = robot->get_gisement_goal(gisements);
			int tmp_2 = intersect_gisement(gisements,robot->get_emplacement());
			if(tmp_1 != -1 && gisements[tmp_1].get_capacite()>0){
				transp_location.push_back(gisements[tmp_1].get_centre());
				continue;
			}
			if(tmp_2 != -1 && gisements[tmp_2].get_capacite()>0){
				transp_location.push_back(gisements[tmp_2].get_centre());
				continue;
			}
	   }
	
	int most_interesting_one(-1);
	double max(0);
	
	if(transp_location.size()<transp_connecte.size()){
		for(size_t i(0); i<transp_location.size(); ++i){
			if((gisements[intersect_gisement(gisements, 
				transp_location[i])]).get_capacite()>max){
					
				int tmp = intersect_gisement(gisements,transp_location[i]);
				
				max= gisements[tmp].get_capacite();
				most_interesting_one=i;
			}
		}
	}
	
	if(transp_location.size()>=transp_connecte.size())
		for(size_t i(0); i<transp_connecte.size(); ++i)
			if(transp_connecte[i]->get_but()==centre)
				transp_connecte[i]->set_objectif(transp_location[i]);
				
	if(transp_location.size()<transp_connecte.size()){
		for(size_t i(0); i<transp_location.size(); ++i)
			if(transp_connecte[i]->get_but()==centre)
				transp_connecte[i]->set_objectif(transp_location[i]);
		if(most_interesting_one!=-1)
			for(auto transp:transp_connecte)
				if(transp->get_but()==centre)
					transp->set_objectif(transp_location[most_interesting_one]);
	}
	
}

//================ functions useful to set the goals for the robots =================//

bool Base::not_visited(vector<int> visited,int j){
	for(auto visited:visited)
		if(visited==j)
			return false;
	return true;
}

bool Base::not_visited(vector<Point> visited,Point j){
	for(auto visited:visited)
		if(visited==j)
			return false;
	return true;
}

int Base::intersect_gisement(vector<Gisement> gisements, Point pt){
	for(size_t i(0); i<gisements.size(); ++i)
		if(geomod::appartient_cercle(gisements[i].get_rayon(), 
		   geomod::norme_min(Vecteur(pt, gisements[i].get_centre()))))
		   return i;
	return -1;
}

//===================== function for the update of the simulation ===================//

void Base::update(vector<Gisement>& gisements){
	
	if(gisements.size()>0){
		set_parcours_forage(gisements);
		set_parcours_transp(gisements);
	}
	
	for(size_t i(0); i<robots.size(); ++i){
		if(is_connecte(robots[i]))
			robots[i]->remote(gisements);
		else robots[i]->autonome(gisements);
	}
	
}
