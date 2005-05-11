/*------------------------------------------------------------------------
 *    Scilab Gtk menus 
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "men_scilab.h"

extern SciMess ScilabMessage;

typedef enum { pOK, pCANCEL , RESET } state; 

int ExposeMessageWindow(void);
int ExposeMessageWindow1(void);

#include <gtk/gtk.h>

static GtkWidget *window = NULL; 

#if GTK_MAJOR_VERSION == 1 
  
char *sci_convert_to_utf8(char *str, int *alloc)
{ 
  /* to be checked for gtk */
  gchar *str_utf8 = str;
  *alloc = FALSE;
  return str_utf8;
}

char *sci_convert_from_utf8(char *str, int *alloc,char *code)
{ 
  /* to be checked for gtk */
  gchar *str_utf8 = str;
  *alloc = FALSE;
  return str_utf8;
}


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

#endif 

#if GTK_MAJOR_VERSION == 2 

/*  
 * message with just an OK button 
 */  

#define DEBUG_STR(x) 
/* #define DEBUG_STR(x) sciprint(x) */


/* convert str to utf8 
 * str can be utf8, locale or assumed to be ISO-8859-15
 */
  
char *sci_convert_to_utf8(char *str, int *alloc)
{ 
  gchar *str_utf8 = str;
  *alloc = FALSE;
  if ( g_utf8_validate(str,-1,NULL) == TRUE ) 
    {
      DEBUG_STR("xname: str is utf8\r\n");
    }
  else
    {
      if (g_get_charset (NULL)) 
	{
	  DEBUG_STR("xname: non utf8 string and your locale is UTF8\r\n");
	  DEBUG_STR("       assuming that your string is ISO-8859-15\r\n");
	  str_utf8 = g_convert (str, -1,"UTF8","ISO-8859-15", NULL, NULL, NULL);
	  if ( str_utf8 != NULL) 
	    {
	      *alloc = TRUE; 
	    }
	  else 
	    {
	      DEBUG_STR("xname: convertion to UTF-8 failed\r\n");
	      str_utf8 = str;
	    }
	}
      else 
	{
	  str_utf8 =g_locale_to_utf8 (str, -1, NULL, NULL, NULL);
	  DEBUG_STR("xname: from locale to UTF8\r\n");
	  if ( str_utf8 != NULL)
	    {
	      *alloc = TRUE; 
	    }
	  else 
	    {
	      DEBUG_STR("xname: convertion to UTF-8 failed\r\n");
	      str_utf8 = str;
	    }
	}
    }
  return str_utf8;
}


/* if str is utf8 converts str to locale or ISO-8859-15 if (code == NULL)
 *                             to code if ( code != NULL)
 */

char *sci_convert_from_utf8(char *str, int *alloc,char *code)
{ 
  gchar *str_res = str;
  *alloc = FALSE;
  if ( g_utf8_validate(str,-1,NULL) == TRUE ) 
    {
      if ( code == NULL) 
	{
	  if (g_get_charset (NULL)) 
	    {
	      DEBUG_STR("xname: utf8 string and your locale is UTF8\r\n");
	      DEBUG_STR("       assuming conversion to ISO-8859-15\r\n");
	      str_res = g_convert (str, -1,"ISO-8859-15","UTF8", NULL, NULL, NULL);
	      if ( str_res != NULL) 
		{
		  *alloc = TRUE; 
		}
	      else 
		{
		  DEBUG_STR("xname: convertion to UTF-8 failed\r\n");
		  str_res = str;
		}
	    }
	  else 
	    {
	      str_res =g_locale_from_utf8 (str, -1, NULL, NULL, NULL);
	      DEBUG_STR("xname: from locale to UTF8\r\n");
	      if ( str_res != NULL)
		{
		  *alloc = TRUE; 
		}
	      else 
		{
		  DEBUG_STR("xname: convertion to UTF-8 failed\r\n");
		  str_res = str;
		}
	    }
	}
      else
	{
	  str_res = g_convert (str, -1,code,"UTF8", NULL, NULL, NULL);
	  if ( str_res != NULL) 
	    {
	      *alloc = TRUE; 
	    }
	  else 
	    {
	      DEBUG_STR("xname: convertion to UTF-8 failed\r\n");
	      str_res = str;
	    }
	}
    }

  return str_res;
}





int ExposeMessageWindow1(void)
{
  int alloc; 
  GtkWidget *dialog;
  char *msg_utf8 = sci_convert_to_utf8(ScilabMessage.string,&alloc);

  dialog = gtk_message_dialog_new (GTK_WINDOW (window),
				   GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_INFO,
				   GTK_BUTTONS_OK,
				   msg_utf8);
  g_signal_connect (dialog, "response",  G_CALLBACK (gtk_widget_destroy),  NULL);
  gtk_widget_show (dialog);
  if ( alloc == TRUE) g_free (msg_utf8);
  return 1;
}


int ExposeMessageWindow(void)
{
  int alloc =FALSE;
  GtkWidget *dialog;
  GtkWidget *hbox;
  GtkWidget *stock;
  GtkWidget *label;
  gint response;
  char *ok_mess, *cancel_mess;
  char *msg_utf8 = sci_convert_to_utf8(ScilabMessage.string,&alloc);

  ok_mess = ScilabMessage.pButName[0]; 
  if ( strcasecmp(ok_mess,"Ok")==0 ) ok_mess = GTK_STOCK_OK; 
  
  switch ( ScilabMessage.nb ) 
    {
    case 0: return 1 ; break;
    case 1 : 
      dialog = gtk_dialog_new_with_buttons ("Scilab Dialog", GTK_WINDOW (window),
					    GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
					    ok_mess, GTK_RESPONSE_OK,
					    NULL);
      break;
    case 2:
    default: 
      cancel_mess = ScilabMessage.pButName[1]; 
      if ( strcasecmp(cancel_mess,"Cancel")==0 ) cancel_mess = GTK_STOCK_CANCEL; 
      dialog = gtk_dialog_new_with_buttons ("Scilab Dialog",
					    GTK_WINDOW (window),
					    GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
					    ok_mess, GTK_RESPONSE_OK,
					    cancel_mess,  GTK_RESPONSE_CANCEL,
					    NULL);
      break;
    }

  hbox = gtk_hbox_new (FALSE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
  
  if ( ScilabMessage.nb >= 2) 
    stock = gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
  else
    stock = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start (GTK_BOX (hbox), stock, FALSE, FALSE, 0);
  gtk_widget_show (stock);
  label = gtk_label_new (msg_utf8);
  gtk_box_pack_start (GTK_BOX (hbox),label, TRUE, TRUE, 0);
  gtk_widget_show (label);
  response = gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  if ( alloc == TRUE) g_free (msg_utf8);
  if (response == GTK_RESPONSE_OK)
    return 1; 
  else 
    return 2; 
}

#endif
