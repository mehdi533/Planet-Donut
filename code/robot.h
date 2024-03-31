/**robot.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "constantes.h"
#include "gisement.h"
#include "geomod.h"

class Robot{
public:
	Robot(int, Point, Point);
	Robot(int, Point);
	Robot();
	
	virtual void write(std::ofstream&);
	
	unsigned    get_uid() const;
	Point  get_emplacement() const;
	Point  get_but() const;
	double get_dp() const;
	
	void   set_objectif(Point);
	virtual Point get_emplacement_gisement() const;

	virtual bool is_comm_bot() const;
	virtual bool is_forage_bot() const;
	virtual bool is_prosp_bot() const;
	virtual bool is_transp_bot() const;
	
	void set_visited_false();
	
	void rec_DFS(std::vector<Robot*>&);
	
	void add_voisins(Robot*);
	void clear_voisins();
	
	virtual double maintenance();
	
	virtual void draw_robot(bool,bool,Couleur);
	
	virtual void remote(std::vector<Gisement>&);
	virtual void autonome(std::vector<Gisement>&);
	
	virtual bool found_gisement() const;
	virtual int  get_gisement(std::vector<Gisement>) const;
	virtual int  get_gisement_goal(std::vector<Gisement>) const;
	
	virtual void suppr_found();
	
protected:
	int uid;
	Point emplacement, but;
	Point base_centre;
	double angle;
	double dp = 0;
	bool atteint = false;
	
	std::vector <Robot*> voisins;
	
	void direction(Point);
	
private:
	bool visited = false;
};

class Prospection: public Robot{
public:
	Prospection(std::string,Point);
	Prospection(int, Point);
	
	double maintenance() override;
	
	void remote(std::vector<Gisement>&) override;
	void autonome(std::vector<Gisement>&) override;
	void write(std::ofstream&) override;
	
	bool  is_prosp_bot() const override;
	Point get_emplacement_gisement() const override;
	bool  found_gisement() const override;
	int   get_gisement(std::vector<Gisement>) const override;
	
	void suppr_found() override;
	
private:
	void set_retour();
	void test_intersect_gisement(std::vector<Gisement>);
	void deplacement_prospection();
	Point emplacement_gisement;
	double rayong, capaciteg;
	bool retour = false, found = false;
};

class Forage: public Robot{
public:
	Forage(std::string,Point);
	Forage(int,Point);
	
	void remote(std::vector<Gisement>&) override;
	void autonome(std::vector<Gisement>&) override;
	
	bool is_forage_bot() const override;
	int  get_gisement_goal(std::vector<Gisement>) const override;
};

class Transport: public Robot{
public:
	Transport(std::string, Point);
	Transport(int, Point);
	
	double maintenance() override;
	
	bool is_transp_bot() const override;
	
	void remote(std::vector<Gisement>&) override;
	void autonome(std::vector<Gisement>&) override;
	
	void write(std::ofstream&) override;
	
private:
	bool is_arrived(std::vector<Gisement>&);
	bool retour = false;
};

class Communication: public Robot{
public:
	Communication(std::string,Point);
	Communication(int,Point,Point);
	
	void draw_robot(bool,bool,Couleur) override;
	
	void remote(std::vector<Gisement>&) override;
	void autonome(std::vector<Gisement>&) override;
	
	bool is_comm_bot() const override;
};
#endif // ROBOT_H
