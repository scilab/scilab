/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "men_scilab.h"

extern SciMess ScilabMessage;

typedef enum { pOK, pCANCEL , RESET } state; 

extern void ShellFormCreate(char *, Widget *, Widget *, Display **);
int ExposeMessageWindow(void);
int ExposeMessageWindow1(void);

#include <gtk/gtk.h>

static GtkWidget *window = NULL; 

static void sci_message_ok(GtkWidget *widget,
			  int *answer)
{
  gtk_widget_destroy(window); 
  *answer = pOK;
  gtk_main_quit();
}

static void sci_message_cancel(GtkWidget *widget,
			      int *answer)
{
  gtk_widget_destroy(window); 
  *answer = pCANCEL;
  gtk_main_quit();
}

int ExposeMessageWindow1(void)
{ 
  return  ExposeMessageWindow();
}


int ExposeMessageWindow(void)
{
  GtkWidget *box1;
  GtkWidget *box2;
  GtkWidget *button;
  GtkWidget *separator;
  GtkWidget *scrolled_window;
  GtkWidget *label;
  static int answer  = RESET ;

  start_sci_gtk(); /* in case gtk was not initialized */
  /* do not accept a reenter mode */ 
  if ( window != NULL) return FALSE ; 

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window, "Scilab message");
  gtk_widget_set_usize (window, 300,300);
  gtk_window_set_policy (GTK_WINDOW(window), TRUE, TRUE, FALSE);

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(sci_message_cancel),
		      &answer);

  gtk_window_set_title (GTK_WINDOW (window), "Scilab message");
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);
  gtk_widget_show (box1);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (box1), scrolled_window, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);
  gtk_widget_show (scrolled_window);

  label = gtk_label_new (ScilabMessage.string);
  gtk_widget_show (label);
  gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW (scrolled_window), label);

  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);

  box2 = gtk_hbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
  gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);
  gtk_widget_show (box2);

  button = gtk_button_new_with_label (ScilabMessage.pButName[0]);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(sci_message_ok),
		      &answer);

  gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  if ( ScilabMessage.nb == 2) 
    {
      button = gtk_button_new_with_label (ScilabMessage.pButName[1]);
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
			  GTK_SIGNAL_FUNC(sci_message_cancel),
			  &answer);
      gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
      gtk_widget_show (button);
    }

  gtk_widget_show (window);


  while (1) 
    {
      /* here we only want to quit gtk_main after a selection in 
       */
      gtk_main();
      if ( answer != RESET ) break;
    }
  window = NULL;
  return (answer == pOK) ? 1 : 2;
}

