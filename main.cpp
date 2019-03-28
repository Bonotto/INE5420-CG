#include <gtkmm.h>
#include <iostream>

#include "src/control/main_control.hpp"

int main(int argc, char **argv)
{
	Gtk::Window *main_window{nullptr};
	control::MainControl *main_control{nullptr};

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	auto refBuilder = Gtk::Builder::create();

	refBuilder->get_widget("window_main", main_window);
	if (main_window)
	{
		main_window->set_default_size(800, 600);
		main_window->show_all();

		main_control = new control::MainControl(refBuilder);

		app->run(*main_window);
	}

	delete main_control;
	delete main_window;

	return 0;
}
