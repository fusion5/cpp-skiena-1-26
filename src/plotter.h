#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/aspectframe.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/separator.h>
#include <cairomm/context.h>

#include <vector>
#include <iostream>

#include "data.h"
#include "algorithms.h"

class Plotter : public Gtk::Window {
	public:
		Plotter();
		virtual ~Plotter();
		void set_edges  (std::vector<Edge*> es);
		void set_points (std::list<Point*> ps);
	protected:
		void on_button_clicked();
		void on_reset_clicked ();
		virtual bool on_darea_draw(const Cairo::RefPtr<Cairo::Context> &cr);
		virtual bool on_darea_press(const GdkEventButton *evt);
		Gtk::Button			button_recompute;
		Gtk::Button			button_reset;
		
		Gtk::AspectFrame	frame;
		Gtk::DrawingArea	drawing_area;
		Gtk::VBox			box;
		Gtk::HButtonBox		button_box;
		// Gtk::VSeparator	    button_separator1;
		Gtk::Separator		separator;
		
	private:
		std::list<Point*> points;
		std::vector<Edge*> edges;
		float max_x;
		float max_y;

		// From window to model coordinates and back
		// Point* scale_transform   (Point *p);
		// Point* descale_transform (Point *p)
};
