/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
#include "men_scilab.h"

extern MDialog SciMDialog;        /** used to stored the mdialog data **/

/* int mDialogWindow();
 * Gtk version 
 */

#include <stdio.h>
#include <gtk/gtk.h>

/*---------------------------------------------------------------
 * data and callbacks for print and export menu  
 *---------------------------------------------------------------*/

typedef enum { pOK, pCANCEL , RESET } state; 

static void sci_mdialog_ok (GtkButton       *button, state * rep) 
{
  *rep = pOK;  gtk_main_quit();
} 

static void sci_mdialog_cancel (GtkButton       *button, state * rep) 
{
  *rep = pCANCEL;  gtk_main_quit();
}

/*---------------------------------------------------------------
 * export_menu 
 *---------------------------------------------------------------*/

int mDialogWindow(void)
{
  int use_scrolled=0;
  int i;
  guint signals[3];
  static GtkWidget *window = NULL;
  static GtkWidget **entries; 
  
  static state rep = RESET ;


  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *button_ok;
  GtkWidget *button_cancel;
  GtkWidget *vbox;
  GtkWidget *hbbox;
  GtkWidget *scrolled_win;

  start_sci_gtk(); /* in case gtk was not initialized */

  rep =RESET;
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Scilab mdialog");

  gtk_window_set_title   (GTK_WINDOW (window),"Scilab dialog");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  gtk_window_set_wmclass  (GTK_WINDOW (window), "mdialog", "Scilab");

  signals[0]=gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(sci_mdialog_cancel),
		      &rep);

  gtk_container_set_border_width (GTK_CONTAINER (window), 0);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
  gtk_widget_show (vbox);

  if (( entries = MALLOC( SciMDialog.nv*sizeof(GtkWidget *))) == NULL) 
    {
      SciMDialog.ierr=1;
      return(FALSE);
    }

  /* label widget description of the mdialog */
  label = gtk_label_new ( SciMDialog.labels);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  /* table widget  of the mdialog */

  if (  SciMDialog.nv > 15 ) use_scrolled = 1;

  if ( use_scrolled ) {
    scrolled_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 1);
    gtk_widget_set_usize (scrolled_win,300,300);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				    GTK_POLICY_AUTOMATIC,
				    GTK_POLICY_AUTOMATIC);
  }

  table = gtk_table_new (SciMDialog.nv, 2, TRUE);
  gtk_widget_show (table);

  if ( use_scrolled == 1) 
    {
      gtk_scrolled_window_add_with_viewport
	(GTK_SCROLLED_WINDOW (scrolled_win), table);
      gtk_widget_show(scrolled_win);  
    }
  else 
    gtk_box_pack_start (GTK_BOX (vbox), table , TRUE, TRUE , 0);

  gtk_container_set_border_width (GTK_CONTAINER (table), 5);
  
  for ( i = 0 ; i < SciMDialog.nv ; i++) 
    {
      label = gtk_label_new (SciMDialog.pszTitle[i]);
      gtk_widget_show (label);
      gtk_table_attach (GTK_TABLE (table),label,0,1,i,i+1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
			0,0);
      entries[i] = gtk_entry_new() ; 
      gtk_entry_set_text (GTK_ENTRY(entries[i]),  SciMDialog.pszName[i]);
      gtk_widget_show (entries[i]);
      gtk_table_attach (GTK_TABLE (table), entries[i],1,2,i,i+1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
			0,0);
    }

  /* ok */ 

  hbbox = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (vbox), hbbox, FALSE, FALSE , 2);
  gtk_widget_show (hbbox);

  button_ok = gtk_button_new_with_label ("OK");
  gtk_container_add (GTK_CONTAINER (hbbox), button_ok);

  signals[1]=gtk_signal_connect (GTK_OBJECT (button_ok), "clicked",
		      GTK_SIGNAL_FUNC (sci_mdialog_ok),
		      &rep);

  GTK_WIDGET_SET_FLAGS (button_ok, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button_ok);
  gtk_widget_show (button_ok);

  /* cancel */

  button_cancel = gtk_button_new_with_label ("Cancel");
  gtk_container_add (GTK_CONTAINER (hbbox), button_cancel);
  signals[2]=gtk_signal_connect (GTK_OBJECT (button_cancel), "clicked",
		      GTK_SIGNAL_FUNC (sci_mdialog_cancel),
		      &rep);
  GTK_WIDGET_SET_FLAGS (button_cancel, GTK_CAN_DEFAULT);
  gtk_widget_show (button_cancel);

  gtk_widget_show (window);

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
    }
  FREE(entries);
  gtk_signal_disconnect(GTK_OBJECT (window),signals[0]);
  gtk_signal_disconnect(GTK_OBJECT (button_ok),signals[1]);
  gtk_signal_disconnect(GTK_OBJECT (button_cancel),signals[2]);
  gtk_widget_destroy(window);
  return (rep == pOK) ? TRUE : FALSE  ;
}

