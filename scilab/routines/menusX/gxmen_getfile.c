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
 * Il faut aussi expanser les dirname ? SCI HOME etc....
 */

int GetFileWindow(char *filemask,char **file,char *dirname,
		  int flag,int *ierr,char *title)
{
 return  sci_get_file_window(filemask,file,dirname,flag,0,ierr,title);
}
    

int  sci_get_file_window(char *filemask,char **file,char *dirname,
			 int flag,int action,int *ierr,char *title)
{
  static int last_choice = 0;
  GList *cbitems = NULL;
  GtkWidget *combo;
  static int n_actions = 4 ;
  static char *actions[]={ "getf","exec","load","chdir",NULL };
  guint signals[3];
  static state rep;
  GtkWidget *window;
  rep =RESET ;

  start_sci_gtk(); /* in case gtk was not initialized */

  window = gtk_file_selection_new (title);

  if ( strcmp(dirname,".") == 0) 
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (window),"./");
  else 
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (window),dirname);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);

  if ( action == 1 ) 
    {
      int j;
      for (j = 0; j < n_actions  ; ++j) 
	cbitems = g_list_append(cbitems, actions[j]);
      combo =  gtk_combo_new ();
      gtk_combo_set_popdown_strings (GTK_COMBO (combo), cbitems);
      gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(combo)->entry),
			  actions[last_choice]);
      gtk_entry_set_editable(GTK_ENTRY (GTK_COMBO(combo)->entry),FALSE);
      
      gtk_box_pack_start (GTK_BOX (GTK_FILE_SELECTION (window)->action_area), 
			  combo, FALSE, FALSE, 0);
      gtk_widget_show (combo);
    }

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
      int action_length=0;
      char *loc = gtk_file_selection_get_filename(GTK_FILE_SELECTION(window));
      if ( action == 1 ) 
	{
	  int j;
	  gchar *entry_text;
	  entry_text = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo)->entry));
	  for (j = 0; j < n_actions ; ++j) 
	    { 
	      if ( strcmp(entry_text, actions[j]) == 0) 
		{
		  last_choice = j ;
		  action_length = strlen(actions[j])+3;
		  break;
		}
	    }
	}
      /* Attention la taille doit correspondre à "%s('%s');" */
      if (( *file = (char *) MALLOC((strlen(loc)+6+action_length)*sizeof(char))) == NULL) 
	{
	  Scistring("Malloc : running out of memory");
	  *ierr = 1;
	}
      else 
	{ 
	  if ( action == 1) 
	    sprintf(*file,"%s('%s');",actions[last_choice],loc);
	  else 
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
