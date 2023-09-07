#include <gtk/gtk.h>

const char GLADE_LAYOUT[] = "./../res/glade_template.glade";

int main(int argc, char** argv){
	gtk_init(&argc, &argv);
	
	GtkBuilder *builder = gtk_builder_new_from_file(GLADE_LAYOUT);
	
	GtkWidget *window = (GtkWidget *) gtk_builder_get_object(
		builder, 
		"window1"
	);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
