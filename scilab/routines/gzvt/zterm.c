/*  zterm.c - Zed's Virtual Terminal
 *  Copyright (C) 1998  Michael Zucchi
 *
 *  A simple terminal program, based on ZTerm.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* needed for getopt under 'gcc -ansi -pedantic' on GNU/Linux */
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkkeysyms.h>

#include <zvt/zvtterm.h>

#define FONT "-misc-fixed-medium-r-normal--12-200-75-75-c-100-iso8859-1"

extern char      **environ;		
static char      **env;
static char      **env_copy;
static GtkWidget  *window = NULL;

static void
child_died_event (ZvtTerm *term)
{
  gtk_exit(0);
}

static void
title_changed_event (ZvtTerm *term, VTTITLE_TYPE type, char *newtitle)
{
  switch(type) {
  case VTTITLE_WINDOW:
  case VTTITLE_WINDOWICON:
    gtk_window_set_title (GTK_WINDOW (window), newtitle);
    break;
  default:
    break;
  }
}

static int
button_press_event (ZvtTerm *term, GdkEventButton *e)
{
  int x,y;
  GdkModifierType mask;
  char *data, *str;

  gdk_window_get_pointer(GTK_WIDGET(term)->window, &x, &y, &mask);

  str = zvt_term_match_check(term, x/term->charwidth, y/term->charheight,(void *)&data);
  if (str) {
    gtk_signal_emit_stop_by_name (GTK_OBJECT (term), "button_press_event");
    printf("User clicked on type '%s' -> '%s'\n", data, str);
    zvt_term_writechild(term, str, strlen(str));
    return TRUE;
  }
  return FALSE;
}

static void
set_hints (GtkWidget *widget)
{
  ZvtTerm *term;
  GdkGeometry hints;
  GtkWidget *app;

  g_assert (widget != NULL);
  term = ZVT_TERM (widget);

  app = gtk_widget_get_toplevel(widget);
  g_assert (app != NULL);

#define PADDING 2
  hints.base_width = (GTK_WIDGET (term)->style->klass->xthickness * 2) + PADDING;
  hints.base_height =  (GTK_WIDGET (term)->style->klass->ythickness * 2);

  hints.width_inc = term->charwidth;
  hints.height_inc = term->charheight;
  hints.min_width = hints.base_width + hints.width_inc;
  hints.min_height = hints.base_height + hints.height_inc;

  gtk_window_set_geometry_hints(GTK_WINDOW(app),
				GTK_WIDGET(term),
				&hints,
				GDK_HINT_RESIZE_INC|GDK_HINT_MIN_SIZE|GDK_HINT_BASE_SIZE);
}

/*
 *  main routine
 *  Does setup, initialises windows, forks child.
 */

gint main (gint argc, gchar *argv[])
{
  static char buf[128],buf1[128];
  int i, c, cmdindex, scrollbacklines, login_shell;
  char **p, *fontname=FONT;
  struct passwd *pw;
  GtkWidget *term, *hbox, *scrollbar,*vbox,
    *socket_button,*socket_button_down,*button;
  enum { RIGHT, LEFT } scrollpos = LEFT;

  login_shell = 0;
  cmdindex = 0;
  scrollbacklines = 1000;

  /* set up terminal environment */
  env = environ;

  for (p = env; *p; p++);
  i = p - env;
  env_copy = (char **) g_malloc (sizeof (char *) * (i + 5));
  for (i = 0, p = env; *p; p++) {
    if (strncmp (*p, "TERM=", 5) == 0)
      env_copy [i++] = "TERM=xterm";
    else if ((strncmp (*p, "COLUMNS=", 8) == 0) ||
	     (strncmp (*p, "LINES=", 6) == 0))
      continue;
    else
      env_copy [i++] = *p;
  }
  env_copy [i++] = "COLORTERM=zterm";
  
  gtk_init(&argc, &argv);

  /* process arguments */
  while ( (cmdindex==0) && (c=getopt(argc, argv, "le:s:rhf:")) != EOF ) {
    switch(c) {
    case 'e':
      cmdindex = optind-1;	/* index of argv array to pass to exec */
      break;
      
    case 's':
      scrollbacklines = atoi(optarg);
      break;
      
    case 'l':
      login_shell = 1;
      break;
      
    case 'r':
      scrollpos = RIGHT;
      break;
      
    case 'f':
      fontname = optarg;
      break;

    case '?':
    case 'h':
    default:
      fprintf(stderr, "Usage: zterm [-sNN] [-l] [-r] [-f fontname] [-e command args]\n");
      exit(1);
      break;
    }
  }

  /* Create widgets and set options */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Scilab");
  gtk_window_set_wmclass (GTK_WINDOW (window), "scilab", "Scilab");
  gtk_widget_realize (window);

  /* create vbox */
  
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_box_set_spacing (GTK_BOX (vbox), 2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 2);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  /* création du socket qui contiendra le plug du programme gtkplug */

  socket_button = gtk_socket_new();
  gtk_box_pack_start(GTK_BOX(vbox), socket_button,FALSE,TRUE,0);
  gtk_widget_show(socket_button);
  sprintf(buf,"SCIWIN=%ld",GDK_WINDOW_XWINDOW(socket_button->window));
  env_copy [i++] = buf;
  env_copy [i] = NULL;

  /* create hbox */
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_set_spacing (GTK_BOX (hbox), 2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 2);
  gtk_box_pack_start_defaults(GTK_BOX(vbox),hbox);
  gtk_widget_show (hbox);

  /* create terminal */
  term = zvt_term_new_with_size(80,24);
  /* zvt_term_set_font_name(ZVT_TERM (term), fontname); */
  /* zvt_term_set_blink (ZVT_TERM (term), TRUE);
   * zvt_term_set_bell (ZVT_TERM (term), TRUE);
   */
  zvt_term_set_scrollback(ZVT_TERM (term), scrollbacklines);
  zvt_term_set_scroll_on_keystroke (ZVT_TERM (term), TRUE);
  zvt_term_set_scroll_on_output (ZVT_TERM (term), FALSE);
  zvt_term_set_background (ZVT_TERM (term), NULL, 0, 0);

  /* 
  zvt_term_set_wordclass (ZVT_TERM (term), "-A-Za-z0-9/_:.,?+%=");
  zvt_term_match_add (ZVT_TERM (term), "http://[^ ]*[^\\.]", VTATTR_UNDERLINE|(1<<VTATTR_FORECOLOURB)|(2<<VTATTR_BACKCOLOURB), "http");
  zvt_term_match_add (ZVT_TERM (term), "ftp://[^ ]*[^\\.]", VTATTR_BOLD, "ftp");
  zvt_term_match_add (ZVT_TERM (term), "file://[^ ]*[^\\.]", VTATTR_REVERSE, "file");
  zvt_term_match_add (ZVT_TERM (term), "[0-9\\.]*", VTATTR_BOLD, "ip #");
  */

  gtk_signal_connect (
      GTK_OBJECT (term),
      "button_press_event",
      GTK_SIGNAL_FUNC (button_press_event),
      NULL);
  
  gtk_signal_connect (
      GTK_OBJECT (term),
      "child_died",
      GTK_SIGNAL_FUNC (child_died_event),
      NULL);

  gtk_signal_connect (
      GTK_OBJECT (term),
      "destroy",
      GTK_SIGNAL_FUNC (child_died_event),
      NULL);

  gtk_signal_connect (
      GTK_OBJECT (term),
      "title_changed",
      GTK_SIGNAL_FUNC (title_changed_event),
      NULL);
  
  gtk_signal_connect_after (
      GTK_OBJECT (term),
      "realize",
      GTK_SIGNAL_FUNC (set_hints),
      term);

  gtk_widget_show (term);

  /* scrollbar */
  scrollbar = 
    gtk_vscrollbar_new (GTK_ADJUSTMENT (ZVT_TERM (term)->adjustment));
  GTK_WIDGET_UNSET_FLAGS (scrollbar, GTK_CAN_FOCUS);
  if (scrollpos == LEFT) {
    gtk_box_pack_start (GTK_BOX (hbox), scrollbar, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), term, 1, 1, 0);
  } else {
    gtk_box_pack_start (GTK_BOX (hbox), term, 1, 1, 0);
    gtk_box_pack_start (GTK_BOX (hbox), scrollbar, FALSE, TRUE, 0);
  }
  gtk_widget_show (scrollbar);

  /* socket for the bottom widget */ 

  socket_button_down = gtk_socket_new();
  gtk_box_pack_start(GTK_BOX(vbox), socket_button_down,FALSE,TRUE,0);
  gtk_widget_show(socket_button_down);
  sprintf(buf1,"SCIINFO=%ld",GDK_WINDOW_XWINDOW(socket_button_down->window));
  env_copy [i++] = buf1;
  env_copy [i] = NULL;


  /* show them all! */
  gtk_widget_show (window);

  /* fork the shell/program */

  switch (zvt_term_forkpty(ZVT_TERM (term), ZVT_TERM_DO_UTMP_LOG | ZVT_TERM_DO_WTMP_LOG | ZVT_TERM_DO_LASTLOG)) {
  case -1:
    perror("ERROR: unable to fork:");
    exit(1);
    break;
    
  case 0:
    if (cmdindex) {
      environ = env_copy;
      execvp(argv[cmdindex], &argv[cmdindex]);
    } else {
      GString *shell, *name;
      
      /* get shell from passwd */
      pw = getpwuid(getuid());
      if (pw) {
	shell = g_string_new(pw->pw_shell);
	if (login_shell) {
	  name = g_string_new("-");
	} else {
	  name = g_string_new("");
	}
	      
	g_string_append(name, strrchr(pw->pw_shell, '/'));
      } else {
	shell = g_string_new("/bin/sh");
	if (login_shell) {
	  name = g_string_new("-sh");
	} else {
	  name = g_string_new("sh");
	}
      }
      
      execle (shell->str, name->str, NULL, env_copy);
      perror ("Could not exec\n");
      _exit (127);
    }
    perror("ERROR: Cannot exec command:");
    exit(1);


  default:
    break;
  }
  
  /* main loop */
  gtk_main ();
  gtk_exit(0);
  return 0;
}


