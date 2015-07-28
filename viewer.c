#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <cairo.h>

#include "socket-client.h"
#include "viewer.h"

cairo_surface_t *image = NULL;
GtkWidget *window;
GtkWidget *darea;

static gboolean on_draw_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data) {      
	//~ printf("on_draw_event called\n");
	cairo_t *cr;
	cr = gdk_cairo_create(widget->window);

	int width, height;
	gtk_window_get_size(GTK_WINDOW(window), &width, &height);

	double scale, origin_x, origin_y = 0;
	if (width < height) {
		scale = (double)width / cairo_image_surface_get_width(image);
		origin_y = (height-width)/2.0/scale;
	} else {
		scale = (double)height / cairo_image_surface_get_height(image);
		origin_x = (width-height)/2.0/scale;
	}

	//~ printf("scale %f\n",scale);
	//~ printf("origin_x %f\n",origin_x);
	//~ printf("origin_y %f\n",origin_y);
	//~ printf("size %d, %d\n",width, height);
	//~ printf("image size %d, %d\n",cairo_image_surface_get_width(image), cairo_image_surface_get_height(image));
	
	if (image != NULL) {
		//~ printf("cairo status: %s\n", cairo_status_to_string(cairo_surface_status(image)));
		//~ cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
		cairo_scale(cr, scale, scale);
		cairo_set_source_surface(cr, image, origin_x, origin_y);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_FAST);
  		cairo_paint(cr);
	} else {
		//~ printf("no image data to show\n");
		//~ cairo_set_source_rgb(cr, 0, 0, 0); 
		//~ cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, 16);
		cairo_move_to(cr, 20, 100);
		cairo_show_text(cr, "no image data");  
	}
	cairo_destroy(cr);

	return TRUE;
}

static gboolean on_destroy_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data) {
	//~ gtk_main_quit ();

    cairo_surface_destroy(image);
	gtk_widget_destroy(window);    	
	return TRUE;
}


int viewer_init(int *argc, char **argv[]) {
	// initialize gtk
	gtk_init(argc, argv);

	// create window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy_event), NULL);
    
	// create drawing area
	GtkWidget *darea;
	darea = gtk_drawing_area_new();
	// add area to window
	gtk_container_add(GTK_CONTAINER(window), darea);	
	// connect events - "draw" for gtk3
	g_signal_connect(G_OBJECT(darea), "expose-event", G_CALLBACK(on_draw_event), NULL); 

	// show window
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400); 
	gtk_window_set_title(GTK_WINDOW(window), "ADNS frame grabber");  	
	gtk_widget_show_all(window);

	return EXIT_SUCCESS;
}

void viewer_update() {
	//~ gtk_main();
	while (gtk_events_pending())
		gtk_main_iteration();
}

int viewer_set_image(const char *img, int width, int height) {
	// if window is destroyed return
	if (!window->window) return EXIT_FAILURE;

	// setup image
	if (image != NULL) {
	    cairo_surface_destroy(image);
	}
	
	// get valid stride for cairo
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, width);
	
	// reorganize memory - convert 6bit grayscale to RGB24 stored in 32bit
	static uint32_t *frame;
	frame = malloc(height*stride); 
	int i;
	for (i=0;i<(height*width);i++) {
		// 6bit -> 8bit grayscale
		uint8_t p = 4*img[i];
		frame[i] = 0 | ((p << 24) | (p << 16) | (p << 8) | p);
	}		
	
    image = cairo_image_surface_create_for_data((unsigned char*)frame, CAIRO_FORMAT_RGB24, width, height, stride);
		
	// force redraw
	gtk_widget_queue_draw(window);
	return EXIT_SUCCESS;
}
