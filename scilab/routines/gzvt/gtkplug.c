/* gtkplug.c */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../graphics/Math.h" 

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


int main(int argc, char *argv[])
{
  GtkWidget *Plug,*Plug_down, *Bouton,*status;
  gtk_init(&argc, &argv);
  fprintf(stdout,"-2->%s",getenv("SCIWIN"));
  Plug = gtk_plug_new(atoi(getenv("SCIWIN")));
  Plug_down = gtk_plug_new(atoi(getenv("SCIINFO")));
  Bouton = gtk_button_new_with_label("Plug");
  gtk_signal_connect(GTK_OBJECT(Bouton), "clicked",
                     (GtkSignalFunc)ClicPlug, NULL);
  gtk_container_add(GTK_CONTAINER(Plug), Bouton);
  gtk_widget_show_all(Plug);


  status  = gtk_statusbar_new ();
  gtk_container_add(GTK_CONTAINER(Plug_down), status);
  gtk_widget_show_all(Plug_down);

  gtk_main();
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


