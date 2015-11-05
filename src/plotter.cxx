/*
 * A GTK window that plots points and allows the addition of points
 * by clicking on the window.
 */
#include "plotter.h"

Plotter::Plotter() :
m_frame( "Tour Plot Area"
       , Gtk::ALIGN_CENTER
       , Gtk::ALIGN_CENTER
       , 1.0
       , false /* Ignore child's aspect */)
{

	set_title        ("Shortest Distances Plotter");
	set_border_width (10);

	m_drawing_area.add_events(Gdk::BUTTON_PRESS_MASK);
	
	m_button.signal_clicked().connect(
		sigc::mem_fun(*this, &Plotter::on_button_clicked));

	m_drawing_area.signal_draw().connect(
	    sigc::mem_fun(*this, &Plotter::on_darea_draw));

	m_drawing_area.signal_button_press_event().connect(
	    sigc::mem_fun(*this, &Plotter::on_darea_press));

	m_button.set_label ("Recompute");

	m_frame.set_size_request(400, 400);
	m_separator.set_size_request(-1, 10);

	m_frame.add (m_drawing_area);

	m_box.pack_start(m_frame,     Gtk::PACK_EXPAND_WIDGET);
	m_box.pack_start(m_separator, Gtk::PACK_SHRINK);
	m_box.pack_start(m_button,    Gtk::PACK_SHRINK);

	add (m_box);
	
	show_all_children ();
}

bool Plotter::on_darea_press (const GdkEventButton *evt) {

	Gtk::Allocation allocation = m_drawing_area.get_allocation();
	const int width  = allocation.get_width();
	const int height = allocation.get_height();

	float xscale = max_x / width;
	float yscale = max_y / height;

	Point *new_point = new Point();
	new_point->x = evt->x * xscale;
	new_point->y = evt->y * yscale;

	std::cout << "Area press at coords: " 
		  << new_point->x << " " 
		  << new_point->y << std::endl;

	
	this->points.push_back(new_point);

	queue_draw();

	return true;
}

void Plotter::set_points(std::list<Point*> ps) {
	this->points = ps;

	this->max_x = 0.0;
	this->max_y = 0.0;

	for ( std::list<Point*>::iterator it = this->points.begin()
		; it != this->points.end()
		; ++it) {
		this->max_x = std::max(this->max_x, (float) (*it)->x);
		this->max_y = std::max(this->max_y, (float) (*it)->y);
	}
}

void Plotter::set_edges(std::vector<Edge*> es) {
	this->edges = es;
}

Plotter::~Plotter() {
}

void Plotter::on_button_clicked () {
	std::cout << "Hello World!" << std::endl;
	// Recompute the shortest path heurstic...
	this->edges = 
		compute_shortest_distances_tour_heuristic (this->points);

	queue_draw();
}

bool Plotter::on_darea_draw (const Cairo::RefPtr<Cairo::Context> &cr) {
	std::cout << "Redraw drawing area..." << std::endl;
	
	Gtk::Allocation allocation = m_drawing_area.get_allocation();
	const int width  = allocation.get_width();
	const int height = allocation.get_height();
	
	cr->set_line_width(1.0);
	cr->set_source_rgb(0, 0, 0);
	
	float xscale = width  / max_x;
	float yscale = height / max_y;
	
	for ( std::vector<Edge*>::iterator it = this->edges.begin()
		; it != this->edges.end()
		; ++it) {
		cr->move_to((*it)->p1->x * xscale, (*it)->p1->y * yscale);
		cr->line_to((*it)->p2->x * xscale, (*it)->p2->y * yscale);
	}

	float rect_size = 3.0;
	
	cr->stroke();
	cr->set_source_rgb(0.0, 0.0, 1.0);
	
	for ( std::list <Point*>::iterator it = this->points.begin()
	    ; it != this->points.end()
	    ; ++it) {
			
		cr->rectangle( (*it)->x * xscale - rect_size
		             , (*it)->y * yscale - rect_size
		             , 2 * rect_size
		             , 2 * rect_size);
	}
	cr->stroke();
}