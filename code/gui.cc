#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include "gui.h"
#include "graphic.h"
#include "graphic_gui.h"
#include "geomod.h"
#include "constantes.h"

using namespace std;

// ces lignes encadrées par l'option de compilation sont nécessaires pour GTKmm 3.

#define GTK_COMPATIBILITY_MODE

#ifdef GTK_COMPATIBILITY_MODE
namespace Gtk
{
  template<class T, class... T_Args>
  auto make_managed(T_Args&&... args) -> T*
  {
    return manage(new T(std::forward<T_Args>(args)...));
  }
}
#endif

// ==============================================================================
// cette constante et le modèle de struct qui suit simulent les données que pourrait
// fournir le module simulation en réponse à une demande de tree_view_update()

namespace {
	Simulation planetdonut;
	bool show_link(true), show_range(false);
}

Gui::Gui(unsigned nb_arg, char* nom_fichier):
	m_Box(Gtk::ORIENTATION_VERTICAL, 20),  m_Top_Box(Gtk::ORIENTATION_HORIZONTAL),
	m_Bottom_Box(Gtk::ORIENTATION_VERTICAL, 10), m_Right_Box(),
	m_Left_Box(Gtk::ORIENTATION_VERTICAL, 10),
	m_General_Box(Gtk::ORIENTATION_VERTICAL, 10),
	m_Toggle_Box(Gtk::ORIENTATION_VERTICAL, 10), m_Button_Exit("exit"),
	m_Button_Open("open"), m_Button_Save("save"), m_Button_Start("start"),
	m_Button_Step("step"), m_Button_TLink("toggle link"),
	m_Button_TRange("toggle range"), m_Frame_General("General"),
	m_Frame_Toggle("Toggle display"){

	set_title("Planet Donut - DEMO");
	add(m_Box);

	init_Buttons_Box();
	init_Right_Box();
	init_Bottom_Box();

	signal_Box();

	m_Top_Box.pack_start(m_Left_Box, false, false);
	m_Top_Box.pack_start(m_Right_Box, true, true);

	m_Box.pack_start(m_Top_Box, true, true);
	m_Box.pack_start(m_Bottom_Box, true, true);

	show_all_children();

	if(nb_arg==2)
		open_file(nom_fichier);
}

void Gui::open_file(char* nom_fichier){
	planetdonut.lecture(nom_fichier);
	tree_view_update();
}

void Gui::init_Buttons_Box(){

	//buttons to box
	m_General_Box.pack_start(m_Button_Exit, true, true);
	m_General_Box.pack_start(m_Button_Open, true, true);
	m_General_Box.pack_start(m_Button_Save, true, true);
	m_General_Box.pack_start(m_Button_Start, true, true);
	m_General_Box.pack_start(m_Button_Step, true, true);
	//Box to frame
	m_Frame_General.add(m_General_Box);

	//buttons to box
	m_Toggle_Box.pack_start(m_Button_TLink, true, true);
	m_Button_TLink.set_active();
	m_Toggle_Box.pack_start(m_Button_TRange, true, true);
	//Box to frame
	m_Frame_Toggle.add(m_Toggle_Box);

	//Frame to main box
	m_Left_Box.pack_start(m_Frame_General, false, false);
	m_Left_Box.pack_start(m_Frame_Toggle, false, false);

	m_Left_Box.set_size_request(40,100);
}

void Gui::init_Right_Box(){
	m_Right_Box.pack_start(area,true,true);
	m_Right_Box.set_size_request(400,400);
}

void Gui::init_Bottom_Box(){

	m_Bottom_Box.pack_start(_scrolled_window, false, true);
	_scrolled_window.set_size_request(-1, 200);
	_scrolled_window.add(_tree_view);

	_scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC,
							  Gtk::PolicyType::POLICY_AUTOMATIC);
	_scrolled_window.set_hexpand();

	_tree_view.append_column("nbP", _columns._col_nbP);
	_tree_view.append_column("nbF", _columns._col_nbF);
	_tree_view.append_column("nbT", _columns._col_nbT);
	_tree_view.append_column("nbC", _columns._col_nbC);
	_tree_view.append_column_numeric("Amount resource", _columns._col_resource, "%.2f");

	auto cell = Gtk::make_managed<Gtk::CellRendererProgress>();
	int cols_count = _tree_view.append_column("Mission completeness", *cell);

	auto progress_col = _tree_view.get_column(cols_count - 1);
	if(progress_col)
		progress_col->add_attribute(cell->property_value(),
									_columns._col_resource_percentage);

	tree_view_update();

}

void Gui::signal_Box(){
	m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_exit_clicked));
	m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_open_clicked));
	m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_save_clicked));
	m_Button_Start.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_start_clicked));
	m_Button_Step.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_step_clicked));
	m_Button_TLink.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_tlink_clicked));
	m_Button_TRange.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui::on_button_trange_clicked));
}

void Gui::on_button_exit_clicked(){
	cout << "Exit clicked" << endl;
	exit(0);
}

void Gui::on_button_open_clicked(){

	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result){
		case(Gtk::RESPONSE_OK):{
			cout << "Open clicked." << endl;
			string filename = dialog.get_filename();
			cout << "File selected: " << filename << endl;
			open_file(const_cast<char*>(filename.c_str()));
			nb_simulation = 0;
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			cout << "Cancel clicked." << endl;
			break;
		}
		default:{
			cout << "Unexpected button clicked." << endl;
		}
	}
}

void Gui::on_button_save_clicked(){

	Gtk::FileChooserDialog dialog("Save a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result){
		case(Gtk::RESPONSE_OK):{
			cout << "Save clicked." << endl;

			string filename = dialog.get_filename();
			cout << "File created: " << filename << endl;

			ofstream file(filename);
			planetdonut.write(file);

			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			cout << "Cancel clicked." << endl;
			break;
		}
		default:{
			cout << "Unexpected button clicked." << endl;
		}
	}
}

void Gui::on_button_step_clicked(){
	if(m_Button_Start.get_label() != "start") return;
	if(planetdonut.is_empty()){
		cout << "Planet Donut is empty !" << endl;
		return;
	}
	update();
}

void Gui::on_button_start_clicked(){

	enum Etat_Start{START, STOP};
	static int etat(START);

	switch(etat){
	case START:
		if(planetdonut.is_empty()){
			cout << "Planet Donut is empty !" << endl;
			break;
		}
		started = !started;
		m_Button_Start.set_label("stop");
		etat=STOP;
		Glib::signal_timeout().connect(sigc::mem_fun(*this, &Gui::on_timeout),15);
		//slows down the process of updates to ensure that on_draw is called
		break;
	case STOP:
		started = !started;
		m_Button_Start.set_label("start");
		etat=START;
		break;
	}
}

void Gui::on_button_tlink_clicked(){
	show_link = m_Button_TLink.get_active();
	area.refresh();
}

void Gui::on_button_trange_clicked(){
	show_range = m_Button_TRange.get_active();
	area.refresh();
}

void Gui::update(){
	planetdonut.step();
	cout << "Mise à jour de la simulation numéro : " << ++nb_simulation << endl;
	tree_view_update();
	area.refresh();
}

bool Gui::on_timeout(){
	if(!started)
		return false;
	update();
	return true;
}

void Gui::tree_view_update(){

	Glib::RefPtr<Gtk::ListStore> ref_tree_model = Gtk::ListStore::create(_columns);
	_tree_view.set_model(ref_tree_model);

	if(!planetdonut.is_empty())
	{
		vector<SimData> data = planetdonut.get_bases_data();
		for(size_t i = 0 ; i < data.size() ; ++i)
		{
			auto row = *(ref_tree_model->append());
			row[_columns._col_nbP] = data[i].nbP;
			row[_columns._col_nbF] = data[i].nbF;
			row[_columns._col_nbT] = data[i].nbT;
			row[_columns._col_nbC] = data[i].nbC;
			row[_columns._col_resource] = data[i].ressource;
			int tmp = int(100*data[i].ressource/finR);
			if(data[i].ressource>=finR) tmp=100;
			row[_columns._col_resource_percentage] = tmp;
			area.refresh();
		}

	}

	area.refresh();
}

bool Gui::on_key_press_event(GdkEventKey* key_event){

	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 's':
				on_button_start_clicked();
				break;
			case '1':
				on_button_step_clicked();
				break;
		}
	}
	return Gtk::Window::on_key_press_event(key_event);
}

Gui::~Gui(){}

MyArea::MyArea(){}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){

	adjust_frame();

	orthographic_projection(cr);

    graphic_set_context(cr);

	planetdonut.draw_planet(show_range,show_link);

	//Draws the square that delimitates the planet.
	cr->set_line_width(15.0);
	cr->set_source_rgb(.5,.5,.5);
	cr->move_to(-1000,1000);
	cr->line_to(1000,1000);
	cr->line_to(1000,-1000);
	cr->line_to(-1000,-1000);
	cr->line_to(-1000,1000);
	cr->stroke();

	return true;
}

void MyArea::orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr){
	// déplace l'origine au centre de la fenêtre
	cr->translate(frame.width/2, frame.height/2);

	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// et inverse la direction de l'axe Y.
	cr->scale(frame.width/(frame.xMax - frame.xMin),
             -frame.height/(frame.yMax - frame.yMin));

	// décalage au centre du cadrage
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

void MyArea::refresh(){
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(),
						      get_allocation().get_height());

		win->invalidate_rect(r,false);
	}

}

void MyArea::adjust_frame(){

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	frame.width  = width;
	frame.height = height;

	// Preventing distorsion by adjusting the frame (cadrage)
	// to have the same proportion as the graphical area

    // use the reference framing as a guide for preventing distortion
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > frame_ref.asp){
		// keep yMax and yMin. Adjust xMax and xMin
	    frame.yMax = frame_ref.yMax ;
	    frame.yMin = frame_ref.yMin ;

	    double delta(frame_ref.xMax - frame_ref.xMin);
	    double mid((frame_ref.xMax + frame_ref.xMin)/2);
        // the new frame is centered on the mid-point along X
	    frame.xMax = mid + 0.5*(new_aspect_ratio/frame_ref.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/frame_ref.asp)*delta ;
    }
    else{ 
		// keep xMax and xMin. Adjust yMax and yMin
	    frame.xMax = frame_ref.xMax ;
	    frame.xMin = frame_ref.xMin ;

 	    double delta(frame_ref.yMax - frame_ref.yMin);
	    double mid((frame_ref.yMax + frame_ref.yMin)/2);
        // the new frame is centered on the mid-point along Y
	    frame.yMax = mid + 0.5*(frame_ref.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(frame_ref.asp/new_aspect_ratio)*delta ;
    }
}

MyArea::~MyArea(){}
