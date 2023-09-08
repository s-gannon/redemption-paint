#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <gtk/gtk.h>


typedef struct image_t{
	GtkWidget* image;
	char file_path[PATH_MAX];
} image_t;
/*
 * the path you put here seems to be relative based on where you run the 
 * build script, so this is relative to the base directory of the project 
 * and not where main.c is OR what directory it gets compiled in (since the 
 * build script goes into /src to compile)
 */
const char* GLADE_LAYOUT_RELATIVE_PATH = "res/layout.glade";

void quit_program(GtkWidget* widget, gpointer ptr){
	gtk_main_quit();
}

void file_open(GtkWidget* widget, gpointer ptr){
	image_t* img = ptr;
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Open file", 
		GTK_WINDOW(NULL),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel", 0,
		"OK", 1,
		NULL
	);
	
	if(gtk_dialog_run(GTK_DIALOG(dialog))){
		strncpy(
			img->file_path, 
			gtk_file_chooser_get_filename(
				GTK_FILE_CHOOSER(dialog)
			),
			PATH_MAX
		);
		g_print("Image file path set to %s\n", img->file_path);
		img->image = gtk_image_new_from_file(img->file_path);
	}

	gtk_widget_destroy(dialog);
}

void file_save_as(GtkWidget* widget, gpointer ptr){
	image_t* img = ptr;

	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Open file", 
		GTK_WINDOW(NULL),
		GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", 0,
		"OK", 1,
		NULL
	);
	
	if(gtk_dialog_run(GTK_DIALOG(dialog))){
		strncpy(
			img->file_path, 
			gtk_file_chooser_get_filename(
				GTK_FILE_CHOOSER(dialog)
			),
			PATH_MAX
		);
		g_print("Image file saved to %s\n", img->file_path);
	}

	gtk_widget_destroy(dialog);
}

void file_save(GtkWidget* widget, gpointer ptr){
	image_t* img = ptr;

	if(img->file_path == NULL){
		file_save_as(widget, ptr);
		return 0;
	}

	//to-do: saving image
}

int main(int argc, char** argv){
	image_t* image = malloc(sizeof(image_t));
	// fuck you realpath
	char* glade_layout_path = realpath(GLADE_LAYOUT_RELATIVE_PATH, NULL);

	gtk_init(&argc, &argv);

	if(!glade_layout_path){	//if glade_layout_path == NULL
		perror("Loading glade layout file!");
		printf(
			"%s\n%s\n%d\n", 
			glade_layout_path, 
			GLADE_LAYOUT_RELATIVE_PATH, 
			errno
		);
		return errno;
	}
	// GTK Widgets
	GtkBuilder *builder = gtk_builder_new_from_file(glade_layout_path);
	GtkWidget *window = (GtkWidget *) gtk_builder_get_object(
		builder, 
		"main_window"
	);
	GtkWidget *file_quit = (GtkWidget *) gtk_builder_get_object(
		builder, 
		"file_quit"
	);
	GtkWidget *file_open = (GtkWidget *) gtk_builder_get_object(
		builder, 
		"file_open"
	);

	// GTK Signals
	g_signal_connect(window, "delete_event", G_CALLBACK(quit_program), NULL);
	g_signal_connect(file_quit, "activate", G_CALLBACK(quit_program), NULL);
	g_signal_connect(file_open, "activate", G_CALLBACK(file_open), image);
	g_signal_connect(file_save, "activate", G_CALLBACK(file_save), image);

	// Showtime and start GTK
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
