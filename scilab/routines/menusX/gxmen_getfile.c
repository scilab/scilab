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

extern int set_cmap __PARAMS((Window w)); 
extern void DisplayInit __PARAMS((char *string,Display **dpy,Widget *toplevel));
extern int ok_prep  __PARAMS((char *filemask,char *dirname,char *title,int flag,int *err));
extern int ok_end  __PARAMS((void));
extern int w_init  __PARAMS((  Widget w));
extern int set_temp_wf_cursor __PARAMS((Cursor cursor));
extern void Rescan    __PARAMS( (Widget widget, XEvent*	event,
				String* params,    Cardinal* num_params));
extern int reset_wf_cursor  __PARAMS((void));
extern int create_file_panel  __PARAMS((  Widget w, char * description));
extern int getMenuBut0  __PARAMS((  Widget *w));

extern void C2F(cvstr)();

/*--------------------------------------------------------------
 * Gtk version 
 *--------------------------------------------------------------*/


/*
 * GtkFileSelection
 */

typedef enum { OK, CANCEL , RESET } state; 

static void file_selection_ok (GtkWidget  *w,  state *rep)
{
  *rep = OK;
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
  static state rep;
  GtkWidget *window;
  rep =RESET ;
  window = gtk_file_selection_new (title);

  gtk_file_selection_set_filename (GTK_FILE_SELECTION (window),dirname);

  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(file_selection_cancel),
		      &rep);

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (window)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC(file_selection_ok),
		      &rep);
  
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (window)->cancel_button),
		      "clicked", GTK_SIGNAL_FUNC(file_selection_cancel),
		      &rep);
  gtk_widget_show (window);
  while (1) 
    {
      gtk_main();
      if ( rep != RESET ) break;
    }
  if ( rep == CANCEL ) 
    {
      gtk_widget_destroy(window);
      return(FALSE);
    }
  else
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
      gtk_widget_destroy(window);
      return TRUE;
    }
}
