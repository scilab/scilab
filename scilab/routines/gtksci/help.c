/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 * Gtk widgets for Scilab help 
 *--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "../version.h"
#include "../sun/h_help.h"
#include "../graphics/Math.h"


static void  initHelpWidgets(GtkWidget *);
static void changeHelpList(int i) ;
static void my_gtk_list_change(GtkWidget * list,char ** help,int nhelp);
static void SciApropos(char *str);
static void create_text (GtkWidget *container,char *path, char *fname) ;
static GtkNotebookPage * notebook_search_label(GtkNotebook *notebook,
					       char *name);
static void my_gtk_list_change(GtkWidget * list,char ** help,int nhelp);
static void create_page (GtkNotebook *notebook,char *path, char *name) ;
static void create_init_page (GtkNotebook *notebook,char *name);
static void create_notebook (char *pathname,char *name);
/*------------------------------------------------------------
 * Help Popup 
 *------------------------------------------------------------*/

static GtkWidget *notebook_window = NULL; /* help widow */ 

/*------------------------------------------------------------
 * activate help on topic Topic 
 *------------------------------------------------------------*/

void SciCallHelp(helpPath,Topic)
     char *Topic;
     char *helpPath;
{
  static char format1[]= "%s/%s.cat";
  char *buf = (char *) MALLOC((strlen(helpPath)+strlen(Topic)+6) * (sizeof(char)));
  if (buf == NULL){ sciprint("Running out of memory, I cannot activate help\n");return;}
  sprintf(buf,format1,helpPath, Topic);
  create_notebook(buf,Topic);
  FREE(buf);
}

/*------------------------------------------------------------
 * Creates the help popup window or add a new page 
 * The help popup is a notebook 
 * with pages for each selected manual page 
 * and a notebook page which contains selection lists 
 *------------------------------------------------------------*/

GdkColor blue_color =  { 0, 0x0000, 0x0000, 0xFFFF }; 

static char * book_open_xpm[] = {
"16 16 4 1",
"       c None s None",
".      c black",
"X      c #808080",
"o      c white",
"                ",
"  ..            ",
" .Xo.    ...    ",
" .Xoo. ..oo.    ",
" .Xooo.Xooo...  ",
" .Xooo.oooo.X.  ",
" .Xooo.Xooo.X.  ",
" .Xooo.oooo.X.  ",
" .Xooo.Xooo.X.  ",
" .Xooo.oooo.X.  ",
"  .Xoo.Xoo..X.  ",
"   .Xo.o..ooX.  ",
"    .X..XXXXX.  ",
"    ..X.......  ",
"     ..         ",
"                "};

static char * book_closed_xpm[] = {
"16 16 6 1",
"       c None s None",
".      c black",
"X      c red",
"o      c yellow",
"O      c #808080",
"#      c white",
"                ",
"       ..       ",
"     ..XX.      ",
"   ..XXXXX.     ",
" ..XXXXXXXX.    ",
".ooXXXXXXXXX.   ",
"..ooXXXXXXXXX.  ",
".X.ooXXXXXXXXX. ",
".XX.ooXXXXXX..  ",
" .XX.ooXXX..#O  ",
"  .XX.oo..##OO. ",
"   .XX..##OO..  ",
"    .X.#OO..    ",
"     ..O..      ",
"      ..        ",
"                "};


/*
 * GtkNotebook
 */

GdkPixmap *book_open;
GdkPixmap *book_closed;
GdkBitmap *book_open_mask;
GdkBitmap *book_closed_mask;
GtkWidget *sample_notebook;

/* callback activated when a page is selected */ 

static void
page_switch (GtkWidget *widget, GtkNotebookPage *page, gint page_num)
{
  GtkNotebookPage *oldpage;
  GtkWidget *pixwid;
  oldpage = GTK_NOTEBOOK (widget)->cur_page;
  if (page == oldpage)
    return;
  pixwid = ((GtkBoxChild*)
	    (GTK_BOX (page->tab_label)->children->data))->widget;
  gtk_pixmap_set (GTK_PIXMAP (pixwid), book_open, book_open_mask);
  pixwid = ((GtkBoxChild*)
	    (GTK_BOX (page->menu_label)->children->data))->widget;
  gtk_pixmap_set (GTK_PIXMAP (pixwid), book_open, book_open_mask);

  if (oldpage)
    {
      pixwid = ((GtkBoxChild*)
		(GTK_BOX (oldpage->tab_label)->children->data))->widget;
      gtk_pixmap_set (GTK_PIXMAP (pixwid), book_closed, book_closed_mask);
      pixwid = ((GtkBoxChild*)
		(GTK_BOX (oldpage->menu_label)->children->data))->widget;
      gtk_pixmap_set (GTK_PIXMAP (pixwid), book_closed, book_closed_mask);
    }
}

/* destroy callback  */ 

static void notebook_destroyed (GtkWidget *widget, GtkWidget **window)
{
  gtk_widget_destroy (*window);
  *window = NULL;
}

/* notebook creation */ 

void create_notebook (char *pathname,char *name)
{
  GtkWidget *box1;
  GtkWidget *button;
  GtkWidget *separator;
  GdkColor *transparent = NULL;
  GtkWidget *cbox;

  if ( notebook_window != NULL ) 
    {
      /* just create a new_page and select it as the current page 
       */
      GtkNotebookPage *page;
      if ((page = notebook_search_label(GTK_NOTEBOOK (sample_notebook),name))
	  == NULL)
	create_page(GTK_NOTEBOOK (sample_notebook),pathname,name);
      else 
	{
	  int p = gtk_notebook_page_num (GTK_NOTEBOOK (sample_notebook),
					    page->child);
	  gtk_notebook_set_page(GTK_NOTEBOOK (sample_notebook),p);
	}
      return ;
    }

  /* toplevel window */ 

  notebook_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (notebook_window, "ScilabHelp");
  gtk_window_set_title (GTK_WINDOW (notebook_window), "Scilab Help");
  gtk_container_set_border_width (GTK_CONTAINER (notebook_window), 0);
  gtk_widget_set_usize (notebook_window, 600, 500);
  gtk_window_set_policy (GTK_WINDOW(notebook_window), TRUE, TRUE, FALSE);

  gtk_signal_connect (GTK_OBJECT (notebook_window), "destroy",
		      GTK_SIGNAL_FUNC(notebook_destroyed),
		      &notebook_window);
  
  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (notebook_window), box1);

  /* a notebook */ 

  sample_notebook = gtk_notebook_new ();
  gtk_signal_connect (GTK_OBJECT (sample_notebook), "switch_page",
		      GTK_SIGNAL_FUNC (page_switch), NULL);

  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (sample_notebook), GTK_POS_TOP);
  gtk_box_pack_start (GTK_BOX (box1), sample_notebook, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (sample_notebook), 10);

  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (sample_notebook), TRUE);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (sample_notebook), TRUE);
 
  gtk_widget_realize (sample_notebook);
  book_open = gdk_pixmap_create_from_xpm_d (sample_notebook->window,
					    &book_open_mask, 
					    transparent, 
					    book_open_xpm);
  book_closed = gdk_pixmap_create_from_xpm_d (sample_notebook->window,
					      &book_closed_mask,
					      transparent, 
					      book_closed_xpm);

  /* create the first page  */ 

  create_init_page(GTK_NOTEBOOK (sample_notebook),"Helps");

  /* create the selected page i.e manual page for name */ 

  create_page(GTK_NOTEBOOK (sample_notebook),pathname,name);


  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
  
  cbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box1), cbox, FALSE, TRUE, 0);

  button = gtk_button_new_with_label ("close");
  gtk_container_set_border_width (GTK_CONTAINER (button), 5);
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
				 GTK_SIGNAL_FUNC (gtk_widget_destroy),
				 GTK_OBJECT (notebook_window));
  gtk_box_pack_start (GTK_BOX (cbox), button, FALSE, FALSE, 0);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default (button);
  gtk_widget_show_all (notebook_window);
}


/*------------------------------------------------------------
 * Creates the main page of the notebook 
 * the page with the manual lists 
 *------------------------------------------------------------*/

static  GtkWidget *help_list ,*help_list1;

/* callback when a manual item is selected */

static void item_selected(GtkWidget *widget, int nitem)
{
  HelpActivate(nitem);
}

/* callback for achapter selction  */

static void chapter_selected (GtkWidget *widget, int nitem) 
{
  changeHelpList(nitem + 1);
  sciprint("selected %d\r\n",nitem);
}

/* callback for a key pressed in the apropos entry */ 

static void sci_apropos_key_pressed (GtkEntry *entry,
				     GdkEventKey *event,void *v)
{
  char *entry_text = gtk_entry_get_text(entry);
  if ( strlen(entry_text) >=2 ) 
    {
#ifdef DEBUG
      fprintf(stderr,"[%s]\n",entry_text);
#endif
      SciApropos(entry_text);
    }
}


static void create_init_page (GtkNotebook *notebook,char *name)
{
  int page;
  GtkWidget *child = NULL;
  GtkWidget *label;
  GtkWidget *label_box;
  GtkWidget *menu_box;
  GtkWidget *pixwid;
  child  = gtk_vbox_new (FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (child), 10);

  initHelpWidgets(child);
  gtk_widget_show_all (child);

  label_box = gtk_hbox_new (FALSE, 0);
  pixwid = gtk_pixmap_new (book_closed, book_closed_mask);
  gtk_box_pack_start (GTK_BOX (label_box), pixwid, FALSE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixwid), 3, 1);
  label = gtk_label_new (name);
  gtk_box_pack_start (GTK_BOX (label_box), label, FALSE, TRUE, 0);
  gtk_widget_show_all (label_box);
  menu_box = gtk_hbox_new (FALSE, 0);
  pixwid = gtk_pixmap_new (book_closed, book_closed_mask);
  gtk_box_pack_start (GTK_BOX (menu_box), pixwid, FALSE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixwid), 3, 1);
  label = gtk_label_new (name);
  gtk_box_pack_start (GTK_BOX (menu_box), label, FALSE, TRUE, 0);
  gtk_widget_show_all (menu_box);
  gtk_notebook_append_page_menu (notebook, child, label_box, menu_box);
  page = gtk_notebook_page_num (notebook,child);
  gtk_notebook_set_page(notebook,page);
}


static void initHelpWidgets(GtkWidget *container)
{
  int i;
  GtkWidget *cbox;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *scrolled_win;
  GtkWidget *separator;
  GtkWidget *entry; 
  char            buf[64];

  vbox = container ; 

  sprintf(buf, "This is help for %s", VERSION);
  label = gtk_label_new (buf) ;
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

  /* A scrolled windows which contains topics for a selected chapter 
   * or for an apropos string  in a gtk_list named help_list 
   */ 
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 1);
  gtk_widget_set_usize (scrolled_win,200, 100);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);

  help_list = gtk_list_new ();
  gtk_list_set_selection_mode (GTK_LIST (help_list), GTK_SELECTION_SINGLE);
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW (scrolled_win), help_list);
  gtk_container_set_focus_vadjustment
    (GTK_CONTAINER (help_list),
     gtk_scrolled_window_get_vadjustment
     (GTK_SCROLLED_WINDOW (scrolled_win)));
  gtk_container_set_focus_hadjustment
    (GTK_CONTAINER (help_list),
     gtk_scrolled_window_get_hadjustment
     (GTK_SCROLLED_WINDOW (scrolled_win)));

  separator = gtk_hseparator_new ();

  /* A scrolled windows which contains the chapter list 
   * in a gtk_list named help_list1 
   */ 

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 1);
  gtk_widget_set_usize (scrolled_win, 200,100);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);

  help_list1 = gtk_list_new ();
  gtk_list_set_selection_mode (GTK_LIST (help_list1), GTK_SELECTION_SINGLE);
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW (scrolled_win), help_list1);
  gtk_container_set_focus_vadjustment
    (GTK_CONTAINER (help_list1),
     gtk_scrolled_window_get_vadjustment
     (GTK_SCROLLED_WINDOW (scrolled_win)));
  gtk_container_set_focus_hadjustment
    (GTK_CONTAINER (help_list1),
     gtk_scrolled_window_get_hadjustment
     (GTK_SCROLLED_WINDOW (scrolled_win)));

  for (i = 0; i < nInfo ; i++) 
    {
      GtkWidget *item;
      item = gtk_list_item_new_with_label(helpInfo[i]);
      gtk_widget_show(item);
      gtk_signal_connect (GTK_OBJECT (item), "select", 
			  GTK_SIGNAL_FUNC(chapter_selected ),
			  GINT_TO_POINTER(i)); 
      gtk_container_add (GTK_CONTAINER (help_list1), item);
    }

  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 0);

  cbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), cbox, FALSE, TRUE, 0);

  label = gtk_label_new("Apropos : ");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (cbox), label, FALSE, TRUE, 0);

  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), "");
  gtk_editable_select_region (GTK_EDITABLE (entry), 0, 5);
  gtk_box_pack_start (GTK_BOX (cbox), entry, TRUE, TRUE, 0);

  gtk_signal_connect (GTK_OBJECT (entry), "key_press_event",
		      (GtkSignalFunc) sci_apropos_key_pressed, NULL);

}

/* meme chose mais appel r'eduit pour appel a partir de C */

void help_info(message,str1,str2)
     char *message,*str1,*str2;
{
  /* XXXXXXX 
     char buf[56];
     Arg args[1];
     Cardinal n = 0;
     sprintf(buf,"%s %s %s",message,str1,str2);
     XtSetArg(args[n], XtNlabel,buf);n++;
     XtSetValues(aproposInfo, args, n);
  */
}

/* query  XXXXXXXXXX 

static void queryAproposAction(w,event,params,num_params) 
     Widget w; XEvent *event; String *params; Cardinal *num_params;
{
  char           *apropos;
  if ((apropos = getWidgetString(aproposText)) == NULL || *apropos == '\0') 
    {
      help_info("Apropos string is empty ","","");
      return;
    } 
  else 
    {
      SciApropos(apropos);
    }
}

*/


/*-----------------------------------
 * Changes the current help list to 
 * chapter i
 -----------------------------------*/

static void changeHelpList(int i) 
{
  setHelpTopicInfo(i);
  if (nTopicInfo > 0)
    my_gtk_list_change(help_list,helpTopicInfo, nTopicInfo);
}

/*------------------------------------
 * Scilab apropos function 
 *------------------------------------*/

static void SciApropos(char *str)
{
  int status = AP.nTopic;
  if ( SetAproposTopics(str) == 1) return ; /** memory allocation **/
  if ( AP.nTopic == 0) 
    {
      if ( status != 0) 
	{
	  /** the previous valid list was an apropos one we clear it **/
	  changeHelpList(1);
	  help_info("No Info on topic ",str,", back to chapter one");
	}
      else
	help_info("No Info on topic ", str,"");
      return;
    }
  help_info("","","");
  my_gtk_list_change(help_list, AP.HelpTopic, AP.nTopic);
}

/*------------------------------------------------------
 * create a new notebook page for a Scilab manual page 
 *------------------------------------------------------*/


static void create_page (GtkNotebook *notebook,char *path, char *name) 
{
  int page;
  GtkWidget *child = NULL;
  GtkWidget *label;
  GtkWidget *label_box;
  GtkWidget *menu_box;
  GtkWidget *pixwid;
  child  = gtk_vbox_new (FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (child), 10);

  create_text(child,path,name);

  gtk_widget_show_all (child);

  label_box = gtk_hbox_new (FALSE, 0);
  pixwid = gtk_pixmap_new (book_closed, book_closed_mask);
  gtk_box_pack_start (GTK_BOX (label_box), pixwid, FALSE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixwid), 3, 1);
  label = gtk_label_new (name);
  gtk_box_pack_start (GTK_BOX (label_box), label, FALSE, TRUE, 0);
  gtk_widget_show_all (label_box);
  menu_box = gtk_hbox_new (FALSE, 0);
  pixwid = gtk_pixmap_new (book_closed, book_closed_mask);
  gtk_box_pack_start (GTK_BOX (menu_box), pixwid, FALSE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixwid), 3, 1);
  label = gtk_label_new (name);
  gtk_box_pack_start (GTK_BOX (menu_box), label, FALSE, TRUE, 0);
  gtk_widget_show_all (menu_box);
  gtk_notebook_append_page_menu (notebook, child, label_box, menu_box);
  page = gtk_notebook_page_num (notebook,child);
  gtk_notebook_set_page(notebook,page);
}


static void delete_page (GtkButton   *button, int i)
{
  int cp = gtk_notebook_get_current_page (GTK_NOTEBOOK (sample_notebook));
  gtk_notebook_remove_page (GTK_NOTEBOOK (sample_notebook),cp);
}


static void select_main_page(GtkButton   *button, int i)
{
  int p;
  GtkNotebookPage *page = notebook_search_label(GTK_NOTEBOOK (sample_notebook),
						"Helps");
  if ( page == NULL ) return ;
  p = gtk_notebook_page_num (GTK_NOTEBOOK (sample_notebook),
			     page->child);
  gtk_notebook_set_page(GTK_NOTEBOOK (sample_notebook),p);
}

/*---------------------------------------------------------
 * text widget for the manual of function fname 
 *---------------------------------------------------------*/

static void create_text (GtkWidget *container,char *path, char *fname) 
{
  GtkWidget *cbox;
  GtkWidget *box1;
  GtkWidget *button;
  GtkWidget *scrolled_window;
  GtkWidget *text;
  GdkFont *font;

  FILE *infile;

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (container), box1);
  gtk_widget_show (box1);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (box1), scrolled_window, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);
  gtk_widget_show (scrolled_window);

  text = gtk_text_new (NULL, NULL);
  gtk_text_set_editable (GTK_TEXT (text), FALSE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), text);
  gtk_widget_grab_focus (text);
  gtk_widget_show (text);
  gtk_text_freeze (GTK_TEXT (text));
  
  infile = fopen(path, "r");
      
  font = gdk_font_load ("-adobe-courier-medium-r-normal--*-120-*-*-*-*-*-*");

  if (infile)
    {
      int i,c; 
#define MAXCHAR 1024 
      char buffer[1024];
      while (1) 
	{
	  i=0;
	  while (1) {
	    c = buffer[i]= fgetc(infile);
	    if ( buffer[i]== '\n' || buffer[i] == EOF || i == MAXCHAR -1 ) 
	      {
		if ( strncmp(buffer,"NAME",4) ==0 
		     || strncmp(buffer,"DESCRIPTION",11) ==0 
		     || strncmp(buffer,"CALLING SEQUENCE",16) ==0 
		     || strncmp(buffer,"PARAMETERS",9) ==0 
		     || strncmp(buffer,"DESCRIPTION",11) ==0 
		     || strncmp(buffer,"EXAMPLE",7) ==0 
		     || strncmp(buffer,"SEE ALSO",8) ==0 
		     || strncmp(buffer,"AUTHOR",6) ==0 
		     ) 
		  gtk_text_insert (GTK_TEXT (text), font,&blue_color,
				   NULL, buffer,i+1 );
		else 
		  gtk_text_insert (GTK_TEXT (text), font, NULL,
				   NULL, buffer,i+1 );
		break;
	      }
	    i++;
	  }
	  if (c == EOF) break;
	}
      fclose (infile);
    }
  
  /* The Text widget will reference count the font, so we
   * unreference it here
   */
  gdk_font_unref (font);
  gtk_text_thaw (GTK_TEXT (text));
  /* 
  gtk_adjustment_set_value( gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW ( scrolled_window)),0);
  */
  cbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box1), cbox, FALSE, TRUE, 0);
  
  button = gtk_button_new_with_label ("Close");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC(delete_page),
			     NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (cbox), button, FALSE,FALSE, 0);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Help menu");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC(select_main_page),
			     NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (cbox), button, FALSE,FALSE, 0);
  gtk_widget_show (button);

}



/*---------------------------------------------------------------
 * Utility function: changes a gtk_list 
 *---------------------------------------------------------------*/

static void my_gtk_list_change(GtkWidget * list,char ** help,int nhelp)
{
  int i;
  GList *l,*l1;
  /* sciprint("Je change la liste des help %d \r\n",nhelp); */ 
  /* we first remove the previous entries */
  l = l1 = gtk_container_children (GTK_CONTAINER (list));
  while ( l != NULL) 
    {
      gtk_container_remove (GTK_CONTAINER (list), l->data);
      l = l->next;
    }
  g_list_free(l1);
  /* now we fill with new entries */
  for (i = 0; i < nhelp ; i++) 
    {
      GtkWidget *item;
      char *info =  strchr(help[i],'@');
      if ( info != NULL) *info = '\0';
      item = gtk_list_item_new_with_label(help[i]);
      gtk_widget_show(item);
      if ( info != NULL) *info = '@';
      gtk_signal_connect (GTK_OBJECT (item), "select", 
			  GTK_SIGNAL_FUNC(item_selected ),
			  GINT_TO_POINTER(i)); 
      gtk_container_add (GTK_CONTAINER (list), item);
    }
}

/*---------------------------------------------------------------
 * Utility function: search a page named name in the notebook 
 *---------------------------------------------------------------*/

static GtkNotebookPage * notebook_search_label(GtkNotebook *notebook,
					       char *name)
{
  GtkWidget *label;
  GtkNotebookPage *page;
  GList *children;
  for (children = notebook->children; children; children = children->next)
    {
      char *str;
      page = children->data;
      label =  ((GtkBoxChild*)
		(GTK_BOX (page->tab_label)->children->next->data))->widget;
      gtk_label_get(GTK_LABEL(label),&str);
#ifdef DEBUG
      fprintf(stderr,"label %s \n",str);
#endif 
      if ( strcmp(str,name) == 0 ) 
	{
#ifdef DEBUG
	  fprintf(stderr,"j'ai trouve un truc \n");
#endif 
	  return page;
	}
    }
#ifdef DEBUG
  fprintf(stderr,"j'ai pas trouve un truc \n");
#endif 
  return NULL;
}  

