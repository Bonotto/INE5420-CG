
#include <gtk/gtk.h>

int main (int argc, char *argv[])
{

	GObject *main_window;
	GtkBuilder *builder;
	
	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();

	gtk_builder_add_from_file (builder, "interface-0.glade", NULL);

	/* Connect signal handlers to the constructed widgets. */
	main_window = gtk_builder_get_object (builder, "main_window");
    
	gtk_widget_show(GTK_WIDGET(main_window));

	gtk_main ();

	return 0;
}