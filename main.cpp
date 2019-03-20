#include <gtkmm.h>
#include <iostream>

#include "./src/vision/Viewport.hpp"

int main(int argc, char **argv)
{
	Gtk::Window *main_window{nullptr};
	Gtk::DrawingArea *draw_area{nullptr};

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	//Load the GtkBuilder file and instantiate its widgets:
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file("interface.glade");
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
	refBuilder->get_widget("main_window", main_window);
	if (main_window)
	{
		refBuilder->get_widget("draw_area", draw_area);

		if (!draw_area)
			Vision::Viewport(*draw_area);

		app->run(*main_window);
	}

	delete main_window;

	return 0;
}
