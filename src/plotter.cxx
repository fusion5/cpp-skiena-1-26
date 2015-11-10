/*
 * A GTK window that plots points and allows the addition of points
 * by clicking on the window.
 */
#include "plotter.h"

// #define PLOTTER_TRACE

Plotter::Plotter() :
frame( "Tour diagram: click to add a point"
       , Gtk::ALIGN_CENTER
       , Gtk::ALIGN_CENTER
       , 1.0
       , false /* Ignore child's aspect */)
{

	set_title        ("Shortest Distances Heuristic Plotter");
	set_border_width (10);

	drawing_area.add_events(Gdk::BUTTON_PRESS_MASK);
	
	button_closest_pairs.signal_clicked().connect(
		sigc::mem_fun(*this, &Plotter::on_button_pairs_clicked));

	button_closest_next.signal_clicked().connect(
		sigc::mem_fun(*this, &Plotter::on_button_closest_clicked));

	button_reset.signal_clicked().connect(
	    sigc::mem_fun(*this, &Plotter::on_button_reset_clicked));
	
	drawing_area.signal_draw().connect(
	    sigc::mem_fun(*this, &Plotter::on_darea_draw));

	drawing_area.signal_button_press_event().connect(
	    sigc::mem_fun(*this, &Plotter::on_darea_press));

	button_closest_next.set_label  ("Closest Next");
	button_closest_pairs.set_label ("Closest Pairs");
	button_reset.set_label ("Reset");
	
	frame.set_size_request(400, 400);
	separator.set_size_request(-1, 10);
	// button_separator1.set_size_request(10, -1);

	frame.add (drawing_area);

	button_box.set_layout(Gtk::BUTTONBOX_EXPAND);
	button_box.add(button_closest_pairs);	
	button_box.add(button_closest_next);	
	button_box.add(button_reset);
	
	box.pack_start(frame, 						Gtk::PACK_EXPAND_WIDGET);
	box.pack_start(separator,	 				Gtk::PACK_SHRINK);
	box.pack_start(button_box,					Gtk::PACK_SHRINK);
		
	add (box);
	
	show_all_children ();
}

bool Plotter::on_darea_press (const GdkEventButton *evt) {

	Gtk::Allocation allocation = drawing_area.get_allocation();
	const int width  = allocation.get_width();
	const int height = allocation.get_height();

	float xscale = max_x / width;
	float yscale = max_y / height;

	Point *new_point = new Point(evt->x * xscale, evt->y * yscale);

#ifdef PLOTTER_TRACE
	std::cout << "Area press at coords: " 
		  << new_point->x << " " 
		  << new_point->y << std::endl;
#endif
	
	this->points.push_back(new_point);

	queue_draw();

	return true;
}

void Plotter::set_points(std::list<Point*> ps) {
	this->points = ps;

	// If there are no initial points, we use 100 width and 100 height as 
	// defaults (this means we'll have a grid of 100x100 because Point x and y 
	// are integers).
	this->max_x = 100.0;
	this->max_y = 100.0;

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

void Plotter::on_button_reset_clicked() {
	this->edges.clear();
	this->points.clear();
	queue_draw();
}

void Plotter::on_button_pairs_clicked () {
	this->edges = compute_closest_pairs_tour_heuristic (this->points);
	queue_draw();
}

void Plotter::on_button_closest_clicked () {
	this->edges = compute_shortest_distances_tour_heuristic (this->points);
	queue_draw();
}
bool Plotter::on_darea_draw (const Cairo::RefPtr<Cairo::Context> &cr) {

#ifdef PLOTTER_TRACE
	// std::cout << "Redraw drawing area..." << std::endl;
#endif
	
	Gtk::Allocation allocation = drawing_area.get_allocation();
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
	    ; ++it) 
		cr->rectangle( (*it)->x * xscale - rect_size
		             , (*it)->y * yscale - rect_size
		             , 2 * rect_size
		             , 2 * rect_size);
	
	cr->stroke();
}
