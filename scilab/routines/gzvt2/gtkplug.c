/* gtkplug.c */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <gdk/gdkx.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkkeysyms.h>

#include "nsp/Math.h" 

/* Cette fonction est appelée lorsque l'utilisateur clique sur le bouton "plug" */

void ClicPlug(GtkWidget *bouton)
{
  g_print("Le bouton plug (dans le programme gtkplug) a été pressé.\n");
}

extern Display	*gdk_display;

int Xorgetchar()
{
  static int GtkXsocket,fd_in,fd_out,fd_err,in_mask,out_mask,gtk_mask, first = 0,max_plus1;
  int select_mask,write_mask;
  register int i;
  static struct timeval select_timeout;
  if ( first == 0) 
    {
      first++;
      GtkXsocket = ConnectionNumber(gdk_display);
      fd_in  = fileno(stdin) ;
      fd_out = fileno(stdout);
      fd_err = fileno(stderr);
      in_mask = 1 << fd_in;
      gtk_mask = 1 << GtkXsocket;
      out_mask = (1 << fd_out ) | ( 1 << fd_err);
      max_plus1 = Max(fd_in,GtkXsocket);      
      max_plus1 = Max(fd_out,max_plus1);
      max_plus1 = Max(fd_err,max_plus1) + 1;
    }

  /* XXXXXX   if ( BasicScilab) return(getchar()); */ /* -nw */

  for( ; ; ) {
    /* always flush writes before waiting */
    gdk_flush();
    /* Update the masks and, unless X events are already in the queue,
     * wait for I/O to be possible. 
     */
    select_mask = in_mask | gtk_mask;
    write_mask =  out_mask;
    select_timeout.tv_sec = 10;
    select_timeout.tv_usec = 0;
    while ( gtk_events_pending())
      { 
	gtk_main_iteration(); 
      } 
    i = select(max_plus1, (fd_set *)&select_mask,(fd_set *) &write_mask, (fd_set *)NULL,
	       &select_timeout);
    if (i < 0) {
      exit(0);
      continue;
    }
    /* if there's something to output */
    if (write_mask) {  fflush(stdout); fflush(stderr); }
    /* if there's something to read */
    if (select_mask & in_mask ) { return(getchar()); break; }
    /* if there are X events in our queue, it
     * counts as being readable 
     */
    while ( gtk_events_pending()) /*  ||(select_mask & gtk_mask) */
      { 
	gtk_main_iteration(); 
      } 
  }
}



/* Obligatory basic callback */
static void print_hello( GtkWidget *w,
                         gpointer   data )
{
  g_message ("Hello, World!\n");
}

/* For the check button */
static void print_toggle(gpointer   callback_data,
                         guint      callback_action,
                         GtkWidget *menu_item)
{
   g_message ("Check button state - %d\n",
              GTK_CHECK_MENU_ITEM(menu_item)->active);
}

/* For the radio buttons */
static void print_selected(gpointer   callback_data,
                           guint      callback_action,
                           GtkWidget *menu_item)
{
   if(GTK_CHECK_MENU_ITEM(menu_item)->active)
     g_message("Radio button %d selected\n", callback_action);
}

/* Our menu, an array of GtkItemFactoryEntry structures that defines each menu item */

static GtkItemFactoryEntry menu_items_o[] = {
  { "/_File",         NULL,         NULL,           0, "<Branch>" },
  { "/File/_New",     "<control>N", print_hello,    0, "<Item>" },
  { "/File/_Open",    "<control>O", print_hello,    0, "<Item>" },
  { "/File/_Save",    "<control>S", print_hello,    0, "<Item>" },
  { "/File/Save _As", NULL,         NULL,           0, "<Item>" },
  { "/File/sep1",     NULL,         NULL,           0, "<Separator>" },
  { "/File/Quit",     "<control>Q", gtk_main_quit,  0, "<Item>" },
  { "/_Options",      NULL,         NULL,           0, "<Branch>" },
  { "/Options/tear",  NULL,         NULL,           0, "<Tearoff>" },
  { "/Options/Check", NULL,         print_toggle,   1, "<CheckItem>" },
  { "/Options/sep",   NULL,         NULL,           0, "<Separator>" },
  { "/Options/Rad1",  NULL,         print_selected, 1, "<RadioItem>" },
  { "/Options/Rad2",  NULL,         print_selected, 2, "/Options/Rad1" },
  { "/Options/Rad3",  NULL,         print_selected, 3, "/Options/Rad1" },
  { "/_Help",         NULL,         NULL,           0, "<LastBranch>" },
  { "/_Help/About",   NULL,         NULL,           0, "<Item>" },
};


static GtkItemFactoryEntry menu_items[] = {
  { "/_File",         NULL,         NULL,           0, "<Branch>" },
  { "/File/_New",     NULL,         print_hello,    0, "<Item>" },
  { "/File/_Open",    NULL, print_hello,    0, "<Item>" },
  { "/File/_Save",    NULL, print_hello,    0, "<Item>" },
  { "/File/Save _As", NULL,         NULL,           0, "<Item>" },
  { "/File/sep1",     NULL,         NULL,           0, "<Separator>" },
  { "/File/Quit",     NULL, gtk_main_quit,  0, "<Item>" },
  { "/_Options",      NULL,         NULL,           0, "<Branch>" },
  { "/Options/tear",  NULL,         NULL,           0, "<Tearoff>" },
  { "/Options/Check", NULL,         print_toggle,   1, "<CheckItem>" },
  { "/Options/sep",   NULL,         NULL,           0, "<Separator>" },
  { "/Options/Rad1",  NULL,         print_selected, 1, "<RadioItem>" },
  { "/Options/Rad2",  NULL,         print_selected, 2, "/Options/Rad1" },
  { "/Options/Rad3",  NULL,         print_selected, 3, "/Options/Rad1" },
  { "/_Help",         NULL,         NULL,           0, "<LastBranch>" },
  { "/_Help/About",   NULL,         NULL,           0, "<Item>" },
};

static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

/* Returns a menubar widget made from the above menu */

GtkWidget *get_menubar_menu( GtkWidget  *window)
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group=NULL;

  /* Make an accelerator group (shortcut keys) */
  if ( window != NULL)  accel_group = gtk_accel_group_new ();

  /* Make an ItemFactory (that makes a menubar) */
  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",
                                       accel_group);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  /* Attach the new accelerator group to the window. */
  if (window != NULL)   gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

  /* Finally, return the actual menu bar created by the item factory. */
  return gtk_item_factory_get_widget (item_factory, "<main>");
}


int main(int argc, char *argv[])
{
  GtkWidget *Plug,*Plug_down, *Bouton,*status,*menubar;
  gtk_init(&argc, &argv);
  fprintf(stdout,"-2->%s",getenv("SCIWIN"));
  Plug = gtk_plug_new(atoi(getenv("SCIWIN")));
  Plug_down = gtk_plug_new(atoi(getenv("SCIINFO")));
  Bouton = gtk_button_new_with_label("Plug");
  gtk_signal_connect(GTK_OBJECT(Bouton), "clicked",
                     (GtkSignalFunc)ClicPlug, NULL);
  gtk_container_add(GTK_CONTAINER(Plug_down), Bouton);
  gtk_widget_show_all(Plug_down);

  menubar = get_menubar_menu (NULL);
  gtk_container_add(GTK_CONTAINER(Plug),menubar);
  gtk_widget_show_all(Plug);
  /*
  status  = gtk_statusbar_new ();
  gtk_container_add(GTK_CONTAINER(Plug_down), status);
  gtk_widget_show_all(Plug_down);

  */


  while (1) 
    {
      char buf[256];
      int i=0;
      while ((buf[i] = Xorgetchar()) != '\n' ) i++ ; 
      buf[i] = '\0';
      fprintf(stdout,"-->%s\n",buf);
    }

  return 0;
}


