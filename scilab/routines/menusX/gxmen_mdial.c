/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

extern MDialog SciMDialog;        /** used to stored the mdialog data **/

extern void ShellFormCreate();
int mDialogWindow();

/*
 * Gtk version 
 */

#include <stdio.h>
#include <gtk/gtk.h>


/*---------------------------------------------------------------
 * data and callbacks for print and export menu  
 *---------------------------------------------------------------*/

typedef enum { pOK, pCANCEL , RESET } state; 

static void menu_print_ok (GtkButton       *button, state * rep) 
{
  *rep = pOK;  gtk_main_quit();
} 

static void menu_print_cancel (GtkButton       *button, state * rep) 
{
  *rep = pCANCEL;  gtk_main_quit();
}

/*---------------------------------------------------------------
 * export_menu 
 *---------------------------------------------------------------*/

int mDialogWindow()
{
  int i;
  static GtkWidget *window1 = NULL;
  static GtkWidget **entries; 
  
  static state rep = RESET ;
  
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *okbutton;
  GtkWidget *cancelbutton;

  rep =RESET;
  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "Scilab mdialog");

  if (( entries = MALLOC( SciMDialog.nv*sizeof(GtkWidget *))) == NULL) 
    {
      SciMDialog.ierr=1;
      return(FALSE);
    }

  /* faire un label a viewport */

  table = gtk_table_new (SciMDialog.nv+1, 2, TRUE);
  gtk_widget_show (table);
  gtk_container_add (GTK_CONTAINER (window1), table);
  gtk_container_set_border_width (GTK_CONTAINER (table), 5);
  
  for ( i = 0 ; i < SciMDialog.nv ; i++) 
    {
      label = gtk_label_new (SciMDialog.pszTitle[i]);
      gtk_widget_show (label);
      gtk_table_attach (GTK_TABLE (table),label,0,1,i,i+1,0,0,0,0);
      entries[i] = gtk_entry_new() ; 
      gtk_entry_set_text (GTK_ENTRY(entries[i]),  SciMDialog.pszName[i]);
      gtk_widget_show (entries[i]);
      gtk_table_attach (GTK_TABLE (table), entries[i],1,2,i,i+1,0,0,0,0);
    }

  /* ok */ 

  okbutton = gtk_button_new_with_label ("OK");
  gtk_signal_connect (GTK_OBJECT (okbutton), "clicked",
		      GTK_SIGNAL_FUNC (menu_print_ok),
		      &rep);
  gtk_widget_show (okbutton);
  gtk_table_attach (GTK_TABLE (table), okbutton, 0, 1,
		    SciMDialog.nv,SciMDialog.nv+1,0,0,0,0);
  /* cancel */
  cancelbutton = gtk_button_new_with_label ("Cancel");
  gtk_signal_connect (GTK_OBJECT (cancelbutton), "clicked",
		      GTK_SIGNAL_FUNC (menu_print_cancel),
		      &rep);
  gtk_widget_show (cancelbutton);
  gtk_table_attach (GTK_TABLE (table), cancelbutton, 1, 2,
		    SciMDialog.nv,SciMDialog.nv+1,0,0,0,0);

  gtk_widget_show (window1);

  while (1) 
    {
      /* here we only want to quit gtk_main after a selection in 
       * this menu XXXXX attention rajouter un test sur destroy 
       */
      gtk_main();
      if ( rep != RESET ) break;
    }

  if ( rep == pOK ) 
    {
      for (i=0; i < SciMDialog.nv  ; i++) {
	char * text = gtk_editable_get_chars(GTK_EDITABLE(entries[i]),0,
					     GTK_ENTRY(entries[i])->text_length);
	if ( text == NULL) { SciMDialog.ierr=1; return FALSE;}
	FREE(SciMDialog.pszName[i]);
	SciMDialog.pszName[i] = text ;
      }
      FREE(entries);
      gtk_widget_destroy(window1);
      return(TRUE);
    }
  else
    {
      FREE(entries);
      gtk_widget_destroy(window1);
      return(FALSE);
    }
}
