/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "men_scilab.h"

#ifdef USE_GNOME
#include <gnome.h>
#endif
#include <gtk/gtk.h>

extern void ShellFormCreate();
extern void C2F(cvstr)();
int DialogWindow();

extern char *dialog_str ;
extern SciDialog ScilabDialog;

static void sci_dialog_ok(GtkWidget *widget,
			  int *answer)
{
  *answer = TRUE;
  gtk_main_quit();
}

static void sci_dialog_cancel(GtkWidget *widget,
			      int *answer)
{
  *answer = FALSE;
  gtk_main_quit();
}


int  DialogWindow()
{
  GtkWidget *window = NULL;
  GtkWidget *vbox;
  GtkWidget *hbbox;
  GtkWidget *button;
  GtkWidget *separator;
  GtkWidget *scrolled_window;
  GtkWidget *text;
  GtkWidget *label;
  GdkFont *font;
  static int answer ;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window),"Scilab dialog");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  gtk_window_set_wmclass (GTK_WINDOW (window), "mdialog", "Scilab");

  /* XXXXX attention il faut aussi un gtk_main_quit */
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window);

  gtk_container_set_border_width (GTK_CONTAINER (window), 0);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
  gtk_widget_show (vbox);

  label = gtk_label_new (ScilabDialog.description);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);
  gtk_widget_show (scrolled_window);
  
  text = gtk_text_new (NULL, NULL);
  gtk_text_set_editable (GTK_TEXT (text), TRUE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), text);
  gtk_widget_grab_focus (text);
  gtk_widget_show (text);

  gtk_text_freeze (GTK_TEXT (text));
  font = gdk_font_load ("-adobe-courier-medium-r-normal--*-120-*-*-*-*-*-*");
  gtk_text_insert (GTK_TEXT (text), font, NULL, NULL, 
		  ScilabDialog.init , -1);
  gdk_font_unref (font);
  gtk_text_thaw (GTK_TEXT (text));

  /* separator */

  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);

  /* ok and cancel buttons at the bottom */

  hbbox = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (vbox), hbbox, TRUE, TRUE, 2);
  gtk_widget_show (hbbox);

  if ( strcmp(ScilabDialog.pButName[0],"OK")==0)
    {
#ifdef USE_GNOME
      button = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
#else
      button = gtk_button_new_with_label ("OK");
#endif
    }
  else 
    button = gtk_button_new_with_label (ScilabDialog.pButName[0]);
  gtk_container_add (GTK_CONTAINER (hbbox), button);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(sci_dialog_ok),
		      &answer);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  if ( strcmp(ScilabDialog.pButName[1],"Cancel")==0)
    {
#ifdef USE_GNOME
      button = gnome_stock_button (GNOME_STOCK_BUTTON_CANCEL);
#else
      button = gtk_button_new_with_label ("Cancel");
#endif
    }
  else 
    button = gtk_button_new_with_label (ScilabDialog.pButName[1]);
  gtk_container_add (GTK_CONTAINER (hbbox), button);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(sci_dialog_cancel),
		      &answer);
  gtk_widget_show (button);

  gtk_widget_show (window);
  gtk_main();
  if ( answer == TRUE ) 
    { 
      /* OK activated : XXXXX attention est-ce que dialog est freed plus loin */ 
      dialog_str = gtk_editable_get_chars ( GTK_EDITABLE(text),0,
					    gtk_text_get_length(GTK_TEXT(text)));
      if ( dialog_str == NULL ) answer = FALSE;
      else 
	{
	  int ind = strlen(dialog_str) - 1 ;
	  if ( dialog_str[ind] == '\n') dialog_str[ind] = '\0' ;
	}
    }
  gtk_widget_destroy(window);
  return answer ;
}

