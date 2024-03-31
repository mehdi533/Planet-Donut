/**graphic.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <iostream>
#include <gtkmm.h>

enum Couleur{ROUGE,VERT,BLEU,JAUNE,MAGENTA,CYAN,NOIR,GRIS,VIOLET,BLANC};

namespace graphic{
	
	void set_couleur(Couleur);
	
	void circle(double,double,double,Couleur);
	void full_circle(double,double,double,Couleur);
	void lines(double,double,double,double,Couleur);
	void arc(double,double,double,double,double,double,Couleur);
	
}

#endif // GRAPHIC_H
