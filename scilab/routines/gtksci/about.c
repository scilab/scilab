/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "scilab-logo.xpm" 
#include "scilab-gw.xpm" 


void create_scilab_about(void)
{
  static GtkWidget *window = NULL;
  GtkWidget *box1;
  GtkWidget *box2;
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *separator;
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window);

  gtk_window_set_title (GTK_WINDOW (window), "About Scilab");
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_realize(window);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  pixmap = gdk_pixmap_create_from_xpm_d (window->window, &mask, NULL,
					 scilab_logo_xpm);
  pixmapwid = gtk_pixmap_new (pixmap, mask);
  gdk_pixmap_unref (pixmap);
  gdk_pixmap_unref (mask);

  /* XXXX : A garder ds un coin */ 


  pixmap = gdk_pixmap_create_from_xpm_d (window->window, &mask, NULL,
					 scilab_gw_xpm);
  gdk_window_set_icon (window->window, NULL,pixmap,pixmap);



  gtk_container_add (GTK_CONTAINER (box1), pixmapwid);

  label = gtk_label_new ("Scilab\n Copyright (C) 1989-2001 Inria/Enpc\nhttp://www-rocq.inria.fr/scilab");
  gtk_box_pack_start (GTK_BOX (box1), label , FALSE, TRUE, 5);

  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);

  box2 = gtk_hbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
  gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);

  button = gtk_button_new_with_label ("close");
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC(gtk_widget_destroy),
			     GTK_OBJECT (window));
  gtk_box_pack_start (GTK_BOX (box2), button, FALSE,FALSE, 0);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button);
  gtk_widget_show_all (window);

}

