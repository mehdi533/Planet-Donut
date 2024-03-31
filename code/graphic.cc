#include <iostream>
#include <gtkmm/drawingarea.h>

#include "graphic.h"
#include "graphic_gui.h"
#include "constantes.h"

namespace{
	const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
}

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}

void graphic::set_couleur(Couleur couleur){
	switch(couleur){
	case ROUGE:
		(*ptcr)->set_source_rgb(.8,0,0);
		break;
	case BLEU:
		(*ptcr)->set_source_rgb(0,.8,0);
		break;
	case VERT:
		(*ptcr)->set_source_rgb(0,0,.8);
		break;
	case JAUNE:
		(*ptcr)->set_source_rgb(.8,.8,0);
		break;
	case MAGENTA:
		(*ptcr)->set_source_rgb(.8,0,.8);
		break;
	case CYAN:
		(*ptcr)->set_source_rgb(0,.8,.8);
		break;
	case NOIR:
		(*ptcr)->set_source_rgb(0,0,0);
		break;
	case GRIS:
		(*ptcr)->set_source_rgb(.5,.5,.5);
		break;
	case VIOLET:
		(*ptcr)->set_source_rgb(.5,0,.5);
		break;
	case BLANC:
		(*ptcr)->set_source_rgb(1,1,1);
		break;
	}
}


void graphic::full_circle(double x, double y, double r, Couleur color){
	
	graphic::set_couleur(color);
	
	(*ptcr)->set_line_width(10.0);
	
	(*ptcr)->arc(x,y,r,0,2*M_PI);
	(*ptcr)->fill();
	(*ptcr)->stroke();
	
}

void graphic::circle(double x, double y, double r, Couleur color){
	
	graphic::set_couleur(color);
	
	(*ptcr)->set_line_width(2.0);
	
	(*ptcr)->arc(x,y,r,0,2*M_PI);
	(*ptcr)->stroke();
	
}

void graphic::lines(double x1, double y1, double x2, double y2, Couleur color){
	
	graphic::set_couleur(color);
	
	(*ptcr)->set_line_width(3.0);
	(*ptcr)->move_to(x1,y1);
	
	// cuts the lines if they are reapeated so that it matches the planet's limits
	
	if(x2>dim_max || x2<-dim_max || y2>dim_max || y2<-dim_max){ 
		
		if(x2>dim_max) 
			(*ptcr)->line_to(dim_max,y2);
		if(x2<-dim_max) 
			(*ptcr)->line_to(-dim_max,y2);
		if(y2>dim_max) 
			(*ptcr)->line_to(x2,dim_max);
		if(y2<-dim_max) 
			(*ptcr)->line_to(x2,-dim_max);
		
	}else 
		(*ptcr)->line_to(x2,y2);
	
	(*ptcr)->stroke();
	
}

void graphic::arc(double x, double y, double r, double angle1, double angle2, 
				  double width, Couleur color){
					  
	graphic::set_couleur(color);
	
	(*ptcr)->set_line_width(width);
	
	(*ptcr)->arc(x,y,r,angle1,angle2);
	(*ptcr)->stroke();
}
