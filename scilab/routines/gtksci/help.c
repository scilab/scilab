/* Copyright (C) 1998 Chancelier Jean-Philippe */

#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

#include "version.h"
#include "../sun/h_help.h"
#include "../graphics/Math.h"

void  popupHelpPanel();

static int  isPoppedUp = FALSE;
static void initHelpWidgets();
static void changeHelpList(int i) ;
static void my_gtk_list_change(GtkWidget * list,char ** help,int nhelp);
static void changeHelpList(int i) ;

/*------------------------------------------------------------
 * Help Popup 
 *------------------------------------------------------------*/

static GtkWidget *window = NULL; /* help widow */ 
static int using_menu_help =0;


void popupHelpPanel()
{
  if (isPoppedUp) 
    {
      gtk_widget_show(window);
      return;
    }
  if ( window  == NULL) 
    {
      if ( Help_Init() == 1) 
	{
	  sciprint("can't use man\r\n");
	  return;
	}
      initHelpWidgets();
      isPoppedUp = TRUE;
      using_menu_help =1;
    }
  else 
    {
      isPoppedUp = TRUE;
      using_menu_help = 1;
    }
}

int help_popped_status()
{
  return(using_menu_help);
}

/*------------------------------------------------------------
 * Creates the help popup panel 
 *------------------------------------------------------------*/

static  GtkWidget *help_list,*help_list1;
static  const gchar * item_data_key ="position";

static void help_done(GtkWidget *widget, GtkWidget *window)
{
  gtk_widget_hide(window);
  isPoppedUp = FALSE;
}

static void item_selected(GtkWidget *widget, int nitem)
{
  /* sciprint("selected %d\r\n",nitem); */ 
  HelpActivate(nitem);
}

static void info_selected (GtkWidget *widget, int nitem) 
{
  changeHelpList(nitem + 1);
  sciprint("selected %d\r\n",nitem);
}

static void help_menu_select ( GtkWidget *item, guint pos ) 
{ 
  sciprint("selected %d\r\n",pos);
}

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
      /* 
	 gtk_widget_grab_focus(item);
	 gtk_object_set_data(GTK_OBJECT(item),
	 list_item_data_key,
	 "ListItem with integrated Label");
      */
      gtk_menu_append (GTK_MENU (menu), item);
    }
  gtk_option_menu_set_menu (GTK_OPTION_MENU (option_menu),menu);
  gtk_option_menu_set_history (GTK_OPTION_MENU (option_menu), history );
}



static void initHelpWidgets()
{
  int i;
  GtkWidget *cbox;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *scrolled_win;
  GtkWidget *button;
  GtkWidget *separator;
  GtkWidget *help_menu;

  char            buf[64];

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  /* To be done */

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window);
  
  gtk_window_set_title (GTK_WINDOW (window), "Scilab Help");
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  sprintf(buf, "This is help for %s", VERSION);
  label = gtk_label_new (buf) ;
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

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

  my_gtk_list_change(help_list, helpTopicInfo, nTopicInfo);
  
  separator = gtk_hseparator_new ();

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

  /* MyXawListChange(list1, helpInfo, nInfo, 0, True); */
  for (i = 0; i < nInfo ; i++) 
    {
      GtkWidget *item;
      item = gtk_list_item_new_with_label(helpInfo[i]);
      gtk_widget_show(item);
      gtk_signal_connect (GTK_OBJECT (item), "select", 
			  GTK_SIGNAL_FUNC(info_selected ),
			  GINT_TO_POINTER(i)); 
      gtk_container_add (GTK_CONTAINER (help_list1), item);
    }

  help_menu = gtk_option_menu_new ();
  gtk_widget_show (help_menu);
  gtk_box_pack_start (GTK_BOX (vbox), help_menu, TRUE, TRUE, 0);
  make_menu_items( help_menu,helpInfo,nInfo,0,GTK_SIGNAL_FUNC(help_menu_select));


  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 0);
  
  cbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), cbox, FALSE, TRUE, 0);
  
  button = gtk_button_new_with_label ("Done");
  gtk_container_set_border_width (GTK_CONTAINER (button), 10);
  gtk_box_pack_start (GTK_BOX (cbox), button, TRUE, TRUE, 0);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC(help_done),
		      window);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  /* gtk_widget_grab_default (button); */
  gtk_widget_show_all (window);
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
 ------------------------------------*/

static void SciApropos(char *str)
{
  int status = AP.nTopic;
  if ( SetAproposTopics(str) == 1) return ; /** memory allocation **/
  if (AP.nTopic == 0) 
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
