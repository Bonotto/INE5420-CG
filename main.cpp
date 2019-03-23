#include <gtkmm.h>
#include <iostream>

#include "src/control/main_control.hpp"

int main(int argc, char **argv)
{
	Gtk::Window *main_window{nullptr};
	control::MainControl *main_control{nullptr};

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
		main_window->set_default_size(800, 600);
		main_window->show_all();
		main_window->show_all();

		main_control = new control::MainControl(refBuilder);

		app->run(*main_window);
	}

	delete main_control;
	delete main_window;

	return 0;
}
