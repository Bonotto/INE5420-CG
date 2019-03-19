#include <gtkmm.h>
#include <iostream>

Gtk::Window* main_window = nullptr;

static
void on_button_clicked()
{
	std::cout << "HELLO" << std::endl;
}

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  //Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("interface.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated Dialog:
  refBuilder->get_widget("main_window", main_window);
  if(main_window)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = nullptr;
    refBuilder->get_widget("up_button", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(on_button_clicked) );
    }

    app->run(*main_window);
  }

  delete main_window;

  return 0;
}