/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "men_scilab.h"

extern void C2F(cvstr)();

/*--------------------------------------------------------------
 * Gtk version for scilab file selection 
 * GtkFileSelection
 *--------------------------------------------------------------*/

typedef enum { OK, CANCEL , DESTROY, RESET } state; 

static void file_selection_ok (GtkWidget  *w,  state *rep)
{
  *rep = OK;
  gtk_main_quit();
}

static void file_selection_destroy (GtkWidget  *w,  state *rep)
{
  *rep = DESTROY;
  gtk_main_quit();
}

static void file_selection_cancel (GtkWidget *w,  state *rep)
{
  *rep = CANCEL;
  gtk_main_quit();
}

/* XXXX reste a rajouter un bouton home et un bouton SCI 
 * Il faut aussi expanser les dirname 
 */

int GetFileWindow(char *filemask,char **file,char *dirname,int flag,int *ierr,char *title)
{
  guint signals[3];
  static state rep;
  GtkWidget *window;
  rep =RESET ;
  window = gtk_file_selection_new (title);

  if ( strcmp(dirname,".") == 0) 
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (window),"");
  else 
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (window),dirname);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  signals[0]=gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(file_selection_destroy),
		      &rep);

  signals[1]=gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (window)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC(file_selection_ok),
		      &rep);
  
  signals[2]=gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (window)->cancel_button),
		      "clicked", GTK_SIGNAL_FUNC(file_selection_cancel),
		      &rep);
  gtk_widget_show (window);
  while (1) 
    {
      gtk_main();
      /* want to quit the gtk_main only when this getfile is achieved 
       */
      if ( rep != RESET ) break;
    }
  if ( rep == OK ) 
    {
      char *loc = gtk_file_selection_get_filename(GTK_FILE_SELECTION(window));
      if (( *file = (char *) MALLOC((strlen(loc)+1)*sizeof(char))) == NULL) 
	{
	  Scistring("Malloc : running out of memory");
	  *ierr = 1;
	}
      else 
	{ 
	  strcpy(*file,loc);
	  *ierr=0;
	}
    }
  /* since here we are no more in a gtk_main we must disconnect signals 
   * before destroying widget window 
   */
  if ( rep != DESTROY ) 
    {
      gtk_signal_disconnect(GTK_OBJECT(window),signals[0]);
      gtk_signal_disconnect(GTK_OBJECT (GTK_FILE_SELECTION (window)->ok_button),signals[1]);
      gtk_signal_disconnect(GTK_OBJECT (GTK_FILE_SELECTION (window)->cancel_button),signals[2]);
      gtk_widget_destroy(window);
    }
  return (rep == OK) ? TRUE : FALSE ; 
}
