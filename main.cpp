#include <gtkmm.h>
#include <iostream>

#include "src/control/viewport.hpp"

int main(int argc, char **argv)
{
	Gtk::Window *main_window{nullptr};
	Gtk::DrawingArea *draw_area{nullptr};
	control::Viewport *view;

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	//Load the GtkBuilder file and instantiate its widgets:
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file("src/view/interface.glade");
	}
	catch (const Glib::FileError &ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}
	catch (const Glib::MarkupError &ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	}
	catch (const Gtk::BuilderError &ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

	//Get the GtkBuilder-instantiated Dialog:
	refBuilder->get_widget("window_main", main_window);
	if (main_window)
	{
		std::cout << "Main window up" << std::endl;

		refBuilder->get_widget("area_draw", draw_area);

		if (draw_area)
			view = new control::Viewport(*draw_area);

		main_window->show_all();
		app->run(*main_window);
	}

	delete main_window;

	if (view) delete view;

	return 0;
}
