/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "men_scilab.h"

int MatrixDialogWindow(void);

extern MADialog MAD;

/*
 * Gtk version 
 */

#include <stdio.h>
#include <gtk/gtk.h>

static void table_menu_to_utf8(char **table,int n);

/*---------------------------------------------------------------
 * data and callbacks for print and export menu  
 *---------------------------------------------------------------*/

typedef enum { pOK, pCANCEL , RESET } state; 

static void sci_matdialog_ok (GtkButton       *button, state * rep) 
{
  *rep = pOK;  gtk_main_quit();
} 

static void sci_matdialog_cancel (GtkButton       *button, state * rep) 
{
  *rep = pCANCEL;  gtk_main_quit();
}

/*---------------------------------------------------------------
 * export_menu 
 *---------------------------------------------------------------*/

int MatrixDialogWindow(void)
{
  int i,j;
  static GtkWidget *window = NULL;
  static GtkWidget **entries,** h_labels, **v_labels;
  static state rep = RESET ;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *vbox; 
  GtkWidget *separator; 
  GtkWidget *hbbox; 

  start_sci_gtk(); /* in case gtk was not initialized */

  rep =RESET;
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window),"Scilab matrix dialog");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  gtk_window_set_wmclass (GTK_WINDOW (window), "mat_dialog", "Scilab");

  /* XXXXX attention il faut aussi un gtk_main_quit */
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window);

  gtk_container_set_border_width (GTK_CONTAINER (window), 0);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);

  gtk_widget_show (vbox);

  table_menu_to_utf8(&MAD.labels,1);
  label = gtk_label_new (MAD.labels);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  /* Allocation of table of widgets */
  entries=(GtkWidget **)MALLOC(  (MAD.nc*MAD.nl)*sizeof(GtkWidget *));
  h_labels=(GtkWidget **)MALLOC( (MAD.nc)*sizeof(GtkWidget*));
  v_labels=(GtkWidget **)MALLOC( (MAD.nl)*sizeof(GtkWidget*));
  if ( entries == NULL || h_labels == NULL || v_labels == NULL)
    {
      /* ierr=1; XXXX */
      return(FALSE);
    } 
  
  /* convert to utf8 */

  table_menu_to_utf8(MAD.VDesc,MAD.nl);
  table_menu_to_utf8(MAD.HDesc,MAD.nc);
  table_menu_to_utf8(MAD.data,MAD.nl*MAD.nc);


  /* XXXX faire un label a viewport */

  table = gtk_table_new (MAD.nl+1,MAD.nc+1, FALSE);
  gtk_widget_show (table);
  gtk_container_set_border_width (GTK_CONTAINER (table), 5);
  gtk_box_pack_start (GTK_BOX (vbox), table ,TRUE,TRUE,0);

  /* The first row : a set of labels */

  for (j=0 ; j < MAD.nl ; j++)
    {
      GtkWidget *label;
      v_labels[j] = label = gtk_label_new (MAD.VDesc[j]);
      gtk_widget_show (label);
      gtk_table_attach (GTK_TABLE (table),label,0,1,j+1,j+2,0,0,0,0);
    }
  
  /* The other rows */
  
  for (i=0 ; i< MAD.nc ; i++) 
    {
      /* first a label */ 
      GtkWidget *label;
      h_labels[i]= label = gtk_label_new (MAD.HDesc[i]);
      gtk_widget_show (label);
      gtk_table_attach (GTK_TABLE (table),label,i+1,i+2,0,1,0,0,0,0);
      for (j=0 ; j<MAD.nl ; j++)
	{
	  GtkWidget *entry;
	  entries[j + i*(MAD.nl)] = entry =  gtk_entry_new() ;
	  gtk_entry_set_text (GTK_ENTRY(entry),MAD.data[j+i*(MAD.nl)]);
	  gtk_widget_show (entry);
	  gtk_table_attach (GTK_TABLE (table),entry,i+1,i+2,j+1,j+2,0,0,0,0);
	}
    }
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);

  /* ok and cancel buttons at the bottom */

  hbbox = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (vbox), hbbox, TRUE, TRUE, 2);
  gtk_widget_show (hbbox);

#ifdef USE_GNOME
  button = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
#else
  button = gtk_button_new_with_label ("OK");
#endif
  gtk_container_add (GTK_CONTAINER (hbbox), button);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(sci_matdialog_ok),
		      &rep);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

#ifdef USE_GNOME
  button = gnome_stock_button (GNOME_STOCK_BUTTON_CANCEL);
#else
  button = gtk_button_new_with_label ("Cancel");
#endif
  gtk_container_add (GTK_CONTAINER (hbbox), button);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(sci_matdialog_cancel),
		      &rep);
  gtk_widget_show (button);

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
      for (i=0; i < MAD.nc*MAD.nl  ; i++) {
	char * text = gtk_editable_get_chars(GTK_EDITABLE(entries[i]),0,
					     GTK_ENTRY(entries[i])->text_length);
	if ( text == NULL) {/* XXXX ierr=1;*/ return FALSE;}
	FREE(MAD.data[i]);
	MAD.data[i] =  text ;
      }
      FREE(entries);
      FREE(h_labels);
      FREE(v_labels);
      gtk_widget_destroy(window);
      return(TRUE);
    }
  else
    {
      FREE(entries);
      FREE(h_labels);
      FREE(v_labels);
      gtk_widget_destroy(window);
      return(FALSE);
    }
}


extern char *sci_convert_to_utf8(char *str, int *alloc);

/* note that since the strings were allocated 
 * if we reallocate them or not they have to be freed 
 * at the end in all cases thus alloc is not used. 
 */

static void table_menu_to_utf8(char **table,int n)
{
  int alloc,i;
  for (i = 0; i < n ; i++)
    table[i] = sci_convert_to_utf8(table[i],&alloc);
}
