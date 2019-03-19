
#include <gtk/gtk.h>
#include <cairo.h>
#include <iostream>

static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
//   cairo_set_source_surface (cr, surface, 0, 0);
//   cairo_paint (cr);

	std::cout << "a" << std::endl;

  return FALSE;
}

int main (int argc, char *argv[])
{

	GObject *main_window;
	GObject *draw_area;
	GtkBuilder *builder;

	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();

	gtk_builder_add_from_file (builder, "interface-1.glade", NULL);

	/* Connect signal handlers to the constructed widgets. */
	main_window = gtk_builder_get_object (builder, "main_window");
	draw_area = gtk_builder_get_object (builder, "viewport");

	g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	// g_signal_connect (draw_area, "draw", G_CALLBACK (draw_cb), NULL);

	gtk_widget_show(GTK_WIDGET(main_window));

	gtk_main ();

	return 0;
}
