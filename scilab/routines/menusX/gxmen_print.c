/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "men_scilab.h"

#define OK 1
#define CANCEL 2 
#define MEMERR 3

extern PrintDial ScilabPrintD;
static int menu_export (int *colored,int *orient);
static int menu_print (int *colored,int *orient);
/*
 * Gtk version 
 */

#include <stdio.h>
#include <gtk/gtk.h>

int  ExposePrintdialogWindow(int flag,int *colored,int *orientation)
{

  if (flag == 1) 
    return menu_print(colored,orientation);
  else 
    return menu_export(colored,orientation);
}

/*---------------------------------------------------------------
 * data and callbacks for print and export menu  
 *---------------------------------------------------------------*/

typedef enum { pOK, pCANCEL , pDESTROY, RESET } state; 

static void menu_print_ok (GtkButton *button, state * rep) 
{
  *rep = pOK;  gtk_main_quit();
} 

static void menu_print_cancel (GtkButton *button, state * rep) 
{
  *rep = pCANCEL;  gtk_main_quit();
}

static void menu_print_destroy (GtkButton *button, state * rep) 
{
  *rep = pDESTROY;  gtk_main_quit();
}

/* convenience routine */

static void make_menu_items( GtkWidget    *option_menu, 
		      gchar        *name[],
		      guint        nname,
		      guint history,
		      GtkSignalFunc callback) 
{
  guint count = 0; 
  GtkWidget *item,*menu =  gtk_menu_new ();
  for ( count = 0 ; count < nname ; count ++) 
    {
      item = gtk_menu_item_new_with_label (name[count]);
      gtk_signal_connect (GTK_OBJECT (item), "activate",
			  callback,GUINT_TO_POINTER(count));
      gtk_widget_show (item);
      gtk_menu_append (GTK_MENU (menu), item);
    }
  gtk_option_menu_set_menu (GTK_OPTION_MENU (option_menu),menu);
  gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), history );
}

typedef struct _lp_status { 
  int color;         /* color or b&w */
  int portrait;      /* portrait or landscape */
  int lp;            /* printer */
  int format;        /* output format: postscript,xfig,... */
  int in_use;        /* menu already in use */
} lp_status ;

static lp_status current_lp_status = { 0,1,0,0,0}; 

void color_menu_select   ( GtkWidget *item, guint pos ) { current_lp_status.color = pos;} 
void portrait_menu_select( GtkWidget *item, guint pos ) { current_lp_status.portrait = pos;} 
void lp_menu_select      ( GtkWidget *item, guint pos ) { current_lp_status.lp = pos;} 
void format_menu_select  ( GtkWidget *item, guint pos ) { current_lp_status.format = pos;} 

/*---------------------------------------------------------------
 * export_menu 
 *---------------------------------------------------------------*/

static int menu_export (int *colored,int *orient)
{
  static char *color_items_name[]={ "color", "black and white"};
  static int  ncolor_items=2;
  static char *portrait_items_name[]={"landscape", "portrait"};
  static int  nportrait_items=2;

  static GtkWidget *window1 = NULL;
  static GtkWidget *entry;
  static state rep = RESET ;

  GtkWidget *table;
  GtkWidget *printer;
  GtkWidget *type;
  GtkWidget *orientation;
  GtkWidget *colormenu;
  GtkWidget *portraitmenu;
  GtkWidget *okbutton;
  GtkWidget *cancelbutton;
  GtkWidget *formatmenu;
  GtkWidget *filename;
  if ( current_lp_status.in_use == 1) return FALSE ;
  current_lp_status.in_use = 1;
  rep =RESET;

  start_sci_gtk(); /* in case gtk was not initialized */

  /* computing list file type */
  if (SetPrinterList(2)== MEMERR) 
    return(FALSE);

  if ( window1 == NULL ) 
    {
      window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (window1), "Scilab print");

      gtk_signal_connect (GTK_OBJECT (window1), "destroy",
			  GTK_SIGNAL_FUNC(menu_print_destroy),
			  &rep);

      table = gtk_table_new (5, 2, TRUE);
      gtk_widget_ref (table);
      gtk_object_set_data_full (GTK_OBJECT (window1), "table", table, NULL) ;
      gtk_widget_show (table);
      gtk_container_add (GTK_CONTAINER (window1), table);
      gtk_container_set_border_width (GTK_CONTAINER (table), 5);

      printer = gtk_label_new ("Format:");
      gtk_widget_show (printer);
      gtk_table_attach (GTK_TABLE (table), printer, 0, 1, 0, 1,0,0,0,0);

      type = gtk_label_new ("Type:");
      gtk_widget_show (type);
      gtk_table_attach (GTK_TABLE (table), type, 0, 1, 1, 2, 0,0,0,0);

      orientation = gtk_label_new ("Orientation:");
      gtk_widget_show (orientation);
      gtk_table_attach (GTK_TABLE (table), orientation, 0, 1, 2, 3,0,0,0,0);

      filename = gtk_label_new ("Filename:");
      gtk_widget_show (filename);
      gtk_table_attach (GTK_TABLE (table),filename, 0, 1, 3, 4,0,0,0,0);

  
      /* format menu */

      formatmenu = gtk_option_menu_new ();
      gtk_widget_show (formatmenu);
      gtk_table_attach (GTK_TABLE (table), formatmenu, 1, 2, 0, 1,0,0,0,0);
      make_menu_items( formatmenu,ScilabPrintD.PList,ScilabPrintD.ns, current_lp_status.format,
		       GTK_SIGNAL_FUNC(format_menu_select));

      /* colors */

      colormenu = gtk_option_menu_new ();
      gtk_widget_show (colormenu);
      gtk_table_attach (GTK_TABLE (table), colormenu, 1, 2, 1, 2,0,0,0,0);
      make_menu_items( colormenu,color_items_name,ncolor_items, current_lp_status.color,
		       GTK_SIGNAL_FUNC(color_menu_select)) ;

      /* portrait or landscape */
      portraitmenu = gtk_option_menu_new ();
      gtk_widget_show (portraitmenu);
      gtk_table_attach (GTK_TABLE (table), portraitmenu, 1, 2, 2, 3,0,0,0,0);
      make_menu_items( portraitmenu , portrait_items_name,nportrait_items, current_lp_status.portrait,
		       GTK_SIGNAL_FUNC(portrait_menu_select));

      /* filename entry */

      entry = gtk_entry_new() ; 
      gtk_widget_show (entry);
      gtk_table_attach (GTK_TABLE (table), entry, 1, 2, 3, 4,0,0,0,0);
      gtk_widget_show(entry);

      /* ok */ 

      okbutton = gtk_button_new_with_label ("OK");
      gtk_signal_connect (GTK_OBJECT (okbutton), "clicked",
			  GTK_SIGNAL_FUNC (menu_print_ok),
			  &rep);
      gtk_widget_show (okbutton);
      gtk_table_attach (GTK_TABLE (table), okbutton, 0, 1, 4, 5,0,0,0,0);
      
      /* cancel */

      cancelbutton = gtk_button_new_with_label ("Cancel");
      gtk_signal_connect (GTK_OBJECT (cancelbutton), "clicked",
			  GTK_SIGNAL_FUNC (menu_print_cancel),
			  &rep);
      gtk_widget_show (cancelbutton);
      gtk_table_attach (GTK_TABLE (table), cancelbutton, 1, 2, 4, 5,0,0,0,0);

    }

  gtk_widget_show (window1);
  while (1) 
    {
      /* here we only want to quit gtk_main after a selection in 
       * this menu.
       */
      gtk_main();
      if ( rep != RESET ) break;
    }
  
  current_lp_status.in_use = 0;

  switch (rep) {
  case pOK : 
    *colored =  current_lp_status.color + 1;
    *orient=	 current_lp_status.portrait + 1;
    ScilabPrintD.numChoice = current_lp_status.format +1;
    ScilabPrintD.filename  = gtk_editable_get_chars ( GTK_EDITABLE(entry),0,
							GTK_ENTRY(entry)->text_length);
    gtk_widget_hide(window1);
    return(TRUE);
    break;
  case pCANCEL :
    gtk_widget_hide(window1);
    return(FALSE);
    break;
  case pDESTROY :
    window1 = NULL;
    return(FALSE);
    break;
  default :
    break;
  }
  return TRUE;
}

/*---------------------------------------------------------------
 * print_menu 
 *---------------------------------------------------------------*/

static int menu_print (int *colored,int *orient)
{
  static char *color_items_name[]={ "color", "black and white"};
  static int  ncolor_items=2;
  static char *portrait_items_name[]={ "landscape", "portrait"};
  static int  nportrait_items=2;

  static GtkWidget *window1 = NULL;
  static state rep = RESET ;

  GtkWidget *table;
  GtkWidget *printer;
  GtkWidget *type;
  GtkWidget *orientation;
  GtkWidget *colormenu;
  GtkWidget *portraitmenu;
  GtkWidget *okbutton;
  GtkWidget *cancelbutton;
  GtkWidget *lpmenu;

  if ( current_lp_status.in_use == 1) return FALSE;
  current_lp_status.in_use = 1;

  start_sci_gtk(); /* in case gtk was not initialized */
  rep =RESET;
  /* computing list of printers  */
  /* men_print should be changed not to allocate this each time we print */
  if (SetPrinterList(1)== MEMERR) 
    return(FALSE);

  if ( window1 == NULL ) 
    {

      window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (window1), "Scilab print");
      gtk_signal_connect (GTK_OBJECT (window1), "destroy",
			  GTK_SIGNAL_FUNC(menu_print_destroy),
			  &rep);

      table = gtk_table_new (4, 2, TRUE);
      gtk_widget_ref (table);
      gtk_object_set_data_full (GTK_OBJECT (window1), "table", table, NULL) ;
      gtk_widget_show (table);

      gtk_container_add (GTK_CONTAINER (window1), table);
      gtk_container_set_border_width (GTK_CONTAINER (table), 5);

      printer = gtk_label_new ("Printer selection:");
      gtk_widget_show (printer);

      gtk_table_attach (GTK_TABLE (table), printer, 0, 1, 0, 1,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 10, 10);

      type = gtk_label_new ("Type:");
      gtk_widget_show (type);
      gtk_table_attach (GTK_TABLE (table), type, 0, 1, 1, 2,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);

      orientation = gtk_label_new ("Orientation:");
      gtk_widget_show (orientation);
      gtk_table_attach (GTK_TABLE (table), orientation, 0, 1, 2, 3,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);
  
      /* printers  menu */

      lpmenu = gtk_option_menu_new ();
      gtk_widget_show (lpmenu);
      gtk_table_attach (GTK_TABLE (table), lpmenu, 1, 2, 0, 1,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);

      make_menu_items( lpmenu,ScilabPrintD.PList,ScilabPrintD.ns, current_lp_status.lp,
		       GTK_SIGNAL_FUNC(lp_menu_select));

      /* colors */

      colormenu = gtk_option_menu_new ();
      gtk_widget_show (colormenu);
      gtk_table_attach (GTK_TABLE (table), colormenu, 1, 2, 1, 2,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);

      make_menu_items( colormenu,color_items_name,ncolor_items, current_lp_status.color,
		       GTK_SIGNAL_FUNC(color_menu_select)) ;

      /* portrait or landscape */
      portraitmenu = gtk_option_menu_new ();
      gtk_widget_show (portraitmenu);

      gtk_table_attach (GTK_TABLE (table), portraitmenu, 1, 2, 2, 3,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);

      make_menu_items( portraitmenu , portrait_items_name,nportrait_items, current_lp_status.portrait,
		       GTK_SIGNAL_FUNC(portrait_menu_select));

      /* ok */ 

      okbutton = gtk_button_new_with_label ("OK");
      gtk_signal_connect (GTK_OBJECT (okbutton), "clicked",
			  GTK_SIGNAL_FUNC (menu_print_ok),
			  &rep);
      gtk_widget_show (okbutton);

      gtk_table_attach (GTK_TABLE (table), okbutton, 0, 1, 3, 4,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);
      
      /* cancel */
      cancelbutton = gtk_button_new_with_label ("Cancel");
      gtk_signal_connect (GTK_OBJECT (cancelbutton), "clicked",
			  GTK_SIGNAL_FUNC (menu_print_cancel),
			  &rep);
      gtk_widget_show (cancelbutton);

      gtk_table_attach (GTK_TABLE (table), cancelbutton, 1, 2, 3, 4,
			(GtkAttachOptions) (0),
			(GtkAttachOptions) (0), 0, 0);
    }
  gtk_widget_show (window1);
  while (1) 
    {
      /* here we only want to quit gtk_main after a selection in 
       * this menu 
       */
      gtk_main();
      if ( rep != RESET ) break;
    }
  current_lp_status.in_use = 0;

  switch (rep) {
  case pOK : 
    gtk_widget_hide(window1);
    *colored =  current_lp_status.color + 1;
    *orient=	 current_lp_status.portrait + 1;
    ScilabPrintD.numChoice = current_lp_status.lp +1;
    return(TRUE);
    break;
  case pDESTROY :
    window1 = NULL;
    return(FALSE);
  case pCANCEL :
    gtk_widget_hide(window1);
    return(FALSE);
  default :
    return FALSE ;
  }
}



