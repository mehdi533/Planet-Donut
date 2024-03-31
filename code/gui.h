/**gui.h
 *GEDIK Eren 324414
 *ABDALLAHI Mehdi 324341**/
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <iostream>
#include <gtkmm.h>

#include "simulation.h"
#include "constantes.h"
#include "graphic.h"
//#include "geomod.h"

struct Frame // window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio
	int height;  // window height
	int width;   // window width
};

class MyArea : public Gtk::DrawingArea{
public:
    MyArea();
    virtual ~MyArea();
    
    void refresh();
    
private:

	void adjust_frame();
	// initialized once ; serve as a reference after
    Frame frame_ref = {-1000,1000,-1000,1000,1,2000,2000};
	Frame frame;
	
	void orthographic_projection(const Cairo::RefPtr<Cairo::Context>&);
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>&) override;
    
};

class Gui: public Gtk::Window{
public:
	Gui(unsigned,char*);
	~Gui();
private:

	//Child widgets for box
	Gtk::Box m_Box, m_Top_Box, m_Bottom_Box, m_Right_Box, m_Left_Box;
	Gtk::Box m_General_Box, m_Toggle_Box;
	Gtk::Button m_Button_Exit,m_Button_Open,m_Button_Save,m_Button_Start,m_Button_Step;
	Gtk::ToggleButton m_Button_TLink, m_Button_TRange;
	Gtk::Frame m_Frame_General, m_Frame_Toggle;

	bool started = false;
	unsigned nb_simulation = 0;

	MyArea area;

	class Model_columns : public Gtk::TreeModel::ColumnRecord
	{
	public:
		Model_columns()
		{
			add(_col_id);
			add(_col_nbP);
			add(_col_nbF);
			add(_col_nbT);
			add(_col_nbC);
			add(_col_resource);
			add(_col_resource_percentage);
		}

		Gtk::TreeModelColumn<int> _col_id;
		Gtk::TreeModelColumn<int> _col_nbP;
		Gtk::TreeModelColumn<int> _col_nbF;
		Gtk::TreeModelColumn<int> _col_nbT;
		Gtk::TreeModelColumn<int> _col_nbC;
		Gtk::TreeModelColumn<double> _col_resource;
		Gtk::TreeModelColumn<int> _col_resource_percentage;
	};

	Model_columns _columns;
	Gtk::ScrolledWindow _scrolled_window;
	Gtk::TreeView _tree_view;
	
	std::vector<SimData> report; 
	
	void tree_view_update();

	//Signal handlers:
	void on_button_exit_clicked();
	void on_button_open_clicked();
	void on_button_save_clicked();
	void on_button_step_clicked();
	void on_button_start_clicked();
	void on_button_tlink_clicked();
	void on_button_trange_clicked();
	void signal_Box();
	
	//Box initialiers
	void init_Buttons_Box();
	void init_Right_Box();
	void init_Bottom_Box();


	void open_file(char* nom_fichier);
	
	void update();
	
	bool on_timeout();
	
	bool on_key_press_event(GdkEventKey * key_event);

};

#endif // GUI_H
