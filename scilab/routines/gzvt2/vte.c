/*
 * Copyright (C) 2001,2002 Red Hat, Inc.
 *
 * This is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/* 
 * derived from the vteapp.c file of libvte 
 * Adapted for scilab Jean-Philippe Chancelier Enpc 2004 */

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <glib-object.h>
#include <vte/vte.h>

static void
window_title_changed(GtkWidget *widget, gpointer win)
{
  GtkWindow *window;

  g_return_if_fail(VTE_TERMINAL(widget));
  g_return_if_fail(GTK_IS_WINDOW(win));
  g_return_if_fail(VTE_TERMINAL(widget)->window_title != NULL);
  window = GTK_WINDOW(win);
  
  gtk_window_set_title(window, VTE_TERMINAL(widget)->window_title);
}

static void
icon_title_changed(GtkWidget *widget, gpointer win)
{
  GtkWindow *window;

  g_return_if_fail(VTE_TERMINAL(widget));
  g_return_if_fail(GTK_IS_WINDOW(win));
  g_return_if_fail(VTE_TERMINAL(widget)->icon_title != NULL);
  window = GTK_WINDOW(win);

  g_message("Icon title changed to \"%s\".\n",
	    VTE_TERMINAL(widget)->icon_title);
}

static void
char_size_changed(GtkWidget *widget, guint width, guint height, gpointer data)
{
  VteTerminal *terminal;
  GtkWindow *window;
  GdkGeometry geometry;
  int xpad, ypad;

  g_return_if_fail(GTK_IS_WINDOW(data));
  g_return_if_fail(VTE_IS_TERMINAL(widget));

  terminal = VTE_TERMINAL(widget);
  window = GTK_WINDOW(data);

  vte_terminal_get_padding(terminal, &xpad, &ypad);

  geometry.width_inc = terminal->char_width;
  geometry.height_inc = terminal->char_height;
  geometry.base_width = xpad;
  geometry.base_height = ypad;
  geometry.min_width = xpad + terminal->char_width * 2;
  geometry.min_height = ypad + terminal->char_height * 2;

  gtk_window_set_geometry_hints(window, widget, &geometry,
				GDK_HINT_RESIZE_INC |
				GDK_HINT_BASE_SIZE |
				GDK_HINT_MIN_SIZE);
}

static void
deleted_and_quit(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
  gtk_main_quit();
}

static void
destroy_and_quit(GtkWidget *widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
  gtk_main_quit();
}
static void
destroy_and_quit_eof(GtkWidget *widget, gpointer data)
{
  /* g_print("Detected EOF.\n"); */
}
static void
destroy_and_quit_exited(GtkWidget *widget, gpointer data)
{
  /* g_print("Detected child exit.\n"); */
  destroy_and_quit(widget, data);
}

static void
status_line_changed(GtkWidget *widget, gpointer data)
{
  /* 
  g_print("Status = `%s'.\n",
	  vte_terminal_get_status_line(VTE_TERMINAL(widget)));
  */
}

static int
button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  VteTerminal *terminal;
  char *match;
  int tag;
  gint xpad, ypad;
  switch (event->button) {
  case 3:
    terminal = VTE_TERMINAL(widget);
    vte_terminal_get_padding(terminal, &xpad, &ypad);
    match = vte_terminal_match_check(terminal,
				     (event->x - ypad) /
				     terminal->char_width,
				     (event->y - ypad) /
				     terminal->char_height,
				     &tag);
    if (match != NULL) {
      g_print("Matched `%s' (%d).\n", match, tag);
      g_free(match);
      if (GPOINTER_TO_INT(data) != 0) {
	vte_terminal_match_remove(terminal, tag);
      }
    }
    break;
  case 1:
  case 2:
  default:
    break;
  }
  return FALSE;
}

static void
iconify_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_iconify((GTK_WIDGET(data))->window);
    }
  }
}

static void
deiconify_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_deiconify((GTK_WIDGET(data))->window);
    }
  }
}

static void
raise_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_raise((GTK_WIDGET(data))->window);
    }
  }
}

static void
lower_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_lower((GTK_WIDGET(data))->window);
    }
  }
}

static void
maximize_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_maximize((GTK_WIDGET(data))->window);
    }
  }
}

static void
restore_window(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_unmaximize((GTK_WIDGET(data))->window);
    }
  }
}

static void
refresh_window(GtkWidget *widget, gpointer data)
{
  GdkRectangle rect;
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      rect.x = rect.y = 0;
      rect.width = (GTK_WIDGET(data))->allocation.width;
      rect.height = (GTK_WIDGET(data))->allocation.height;
      gdk_window_invalidate_rect((GTK_WIDGET(data))->window,
				 &rect, TRUE);
    }
  }
}

static void
resize_window(GtkWidget *widget, guint width, guint height, gpointer data)
{
  VteTerminal *terminal;
  gint owidth, oheight;
  if ((GTK_IS_WINDOW(data)) && (width >= 2) && (height >= 2)) {
    terminal = VTE_TERMINAL(widget);
    /* Take into account padding and border overhead. */
    gtk_window_get_size(GTK_WINDOW(data), &owidth, &oheight);
    owidth -= terminal->char_width * terminal->column_count;
    oheight -= terminal->char_height * terminal->row_count;
    gtk_window_resize(GTK_WINDOW(data),
		      width + owidth, height + oheight);
  }
}

static void
move_window(GtkWidget *widget, guint x, guint y, gpointer data)
{
  if (GTK_IS_WIDGET(data)) {
    if ((GTK_WIDGET(data))->window) {
      gdk_window_move((GTK_WIDGET(data))->window, x, y);
    }
  }
}

static void
adjust_font_size(GtkWidget *widget, gpointer data, gint howmuch)
{
  VteTerminal *terminal;
  PangoFontDescription *desired;
  gint newsize;
  gint columns, rows, owidth, oheight;

  /* Read the screen dimensions in cells. */
  terminal = VTE_TERMINAL(widget);
  columns = terminal->column_count;
  rows = terminal->row_count;

  /* Take into account padding and border overhead. */
  gtk_window_get_size(GTK_WINDOW(data), &owidth, &oheight);
  owidth -= terminal->char_width * terminal->column_count;
  oheight -= terminal->char_height * terminal->row_count;

  /* Calculate the new font size. */
  desired = pango_font_description_copy(vte_terminal_get_font(terminal));
  newsize = pango_font_description_get_size(desired) / PANGO_SCALE;
  newsize += howmuch;
  pango_font_description_set_size(desired,
				  CLAMP(newsize, 4, 144) * PANGO_SCALE);

  /* Change the font, then resize the window so that we have the same
   * number of rows and columns. */
  vte_terminal_set_font(terminal, desired);
  gtk_window_resize(GTK_WINDOW(data),
		    columns * terminal->char_width + owidth,
		    rows * terminal->char_height + oheight);

  pango_font_description_free(desired);
}

static void
increase_font_size(GtkWidget *widget, gpointer data)
{
  adjust_font_size(widget, data, 1);
}

static void
decrease_font_size(GtkWidget *widget, gpointer data)
{
  adjust_font_size(widget, data, -1);
}

static gboolean
read_and_feed(GIOChannel *source, GIOCondition condition, gpointer data)
{
  char buf[2048];
  gsize size;
  GIOStatus status;
  g_return_val_if_fail(VTE_IS_TERMINAL(data), FALSE);
  status = g_io_channel_read_chars(source, buf, sizeof(buf),
				   &size, NULL);
  if ((status == G_IO_STATUS_NORMAL) && (size > 0)) {
    vte_terminal_feed(VTE_TERMINAL(data), buf, size);
    return TRUE;
  }
  return FALSE;
}

static void
disconnect_watch(GtkWidget *widget, gpointer data)
{
  g_source_remove(GPOINTER_TO_INT(data));
}

static void
clipboard_get(GtkClipboard *clipboard, GtkSelectionData *selection_data,
	      guint info, gpointer owner)
{
  /* No-op. */
  return;
}

static void
take_xconsole_ownership(GtkWidget *widget, gpointer data)
{
  char *name, hostname[255];
  GdkAtom atom;
  GtkClipboard *clipboard;
  GtkTargetEntry targets[] = {
    {"UTF8_STRING", 0, 0},
    {"COMPOUND_TEXT", 0, 0},
    {"TEXT", 0, 0},
    {"STRING", 0, 0},
  };

  memset(hostname, '\0', sizeof(hostname));
  gethostname(hostname, sizeof(hostname) - 1);

  name = g_strdup_printf("MIT_CONSOLE_%s", hostname);
  atom = gdk_atom_intern(name, FALSE);
#if GTK_CHECK_VERSION(2,2,0)
  clipboard = gtk_clipboard_get_for_display(gtk_widget_get_display(widget),
					    atom);
#else
  clipboard = gtk_clipboard_get(atom);
#endif
  g_free(name);

  gtk_clipboard_set_with_owner(clipboard,
			       targets,
			       G_N_ELEMENTS(targets),
			       clipboard_get,
			       (GtkClipboardClearFunc)gtk_main_quit,
			       G_OBJECT(widget));
}

int
main(int argc, char **argv)
{
  char **command_argv;
  int cmdindex =0;
  GtkWidget *window, *hbox, *scrollbar, *widget, *vbox, *socket_button;
  char buf[56];
  char *env_add[] = {NULL, NULL};
  const char *background = NULL;
  gboolean transparent = FALSE, audible = TRUE, blink = TRUE,
    debug = FALSE, geometry = TRUE, dbuffer = TRUE,
    console = FALSE, scroll = FALSE, keep = FALSE,
    icon_title = FALSE;
  long lines = 100;
  const char *message = "Launching interactive shell...\r\n";
  const char *font = NULL;
  const char *terminal = NULL;
  const char *command = NULL;
  const char *working_directory = NULL;
  char **argv2;
  int opt;
  int i, j;
  GList *args = NULL;
  GdkColor fore, back, tint;
  const char *usage = "Usage: %s "
    "[ [-B image] | [-T] ] "
    "[-2] "
    "[-a] "
    "[-b] "
    "[-d] "
    "[-f font] "
    "[-g] "
    "[-h] "
    "[-i] "
    "[-k] "
    "[-n] "
    "[-t terminaltype]"
    "[-e command args]\n";
  back.red = back.green = back.blue = 0xffff;
  fore.red = fore.green = fore.blue = 0x0000;
  tint.red = tint.green = tint.blue = 0;
  tint = back;

  /* Have to do this early. */
  if (getenv("VTE_PROFILE_MEMORY")) {
    if (atol(getenv("VTE_PROFILE_MEMORY")) != 0) {
      g_mem_set_vtable(glib_mem_profiler_table);
    }
  }

  /* Pull out long options for GTK+. */
  for (i = j = 1; i < argc; i++) {
    if (g_ascii_strncasecmp("--", argv[i], 2) == 0) {
      args = g_list_append(args, argv[i]);
      for (j = i; j < argc; j++) {
	argv[j] = argv[j + 1];
      }
      argc--;
      i--;
    }
  }
  argv2 = g_malloc0(sizeof(char*) * (g_list_length(args) + 2));
  argv2[0] = argv[0];
  for (i = 1; i <= g_list_length(args); i++) {
    argv2[i] = (char*) g_list_nth(args, i - 1);
  }
  argv2[i] = NULL;
  g_assert(i < (g_list_length(args) + 2));

  /* Parse some command-line options. */
  while ( cmdindex == 0 && (opt = getopt(argc, argv, "B:T2abe:df:ghkn:st:w:-")) != -1) {
    gboolean bail = FALSE;
    switch (opt) {
    case 'B':
      background = optarg;
      break;
    case 'T':
      transparent = TRUE;
      break;
    case '2':
      dbuffer = !dbuffer;
      break;
    case 'a':
      audible = !audible;
      break;
    case 'b':
      blink = !blink;
      break;
    case 'e':
      command = optarg;
      cmdindex = optind-1;	/* index of argv array to pass to exec */
      break;
    case 'd':
      debug = !debug;
      break;
    case 'f':
      font = optarg;
      break;
    case 'g':
      geometry = !geometry;
      break;
    case 'i':
      icon_title = !icon_title;
      break;
    case 'k':
      keep = !keep;
      break;
    case 'n':
      lines = atol(optarg);
      if (lines == 0) {
	lines = 100;
      }
      break;
    case 's':
      scroll = !scroll;
      break;
    case 't':
      terminal = optarg;
      break;
    case 'w':
      working_directory = optarg;
      break;
    case '-':
      bail = TRUE;
      break;
    case 'h':
    default:
      g_print(usage, argv[0]);
      exit(1);
      break;
    }
    if (bail) {
      break;
    }
  }

  gtk_init(&argc, &argv);
  gdk_window_set_debug_updates(debug);

  /* Create a window to hold the scrolling shell, and hook its
   * delete event to the quit function.. */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_resize_mode(GTK_CONTAINER(window),
				GTK_RESIZE_IMMEDIATE);
  g_signal_connect(G_OBJECT(window), "delete_event",
		   GTK_SIGNAL_FUNC(deleted_and_quit), window);
  gtk_window_set_title (GTK_WINDOW (window), "Scilab");
  gtk_window_set_wmclass (GTK_WINDOW (window), "scilab", "Scilab");

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
  /* create hbox */
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_set_spacing (GTK_BOX (hbox), 2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 2);
  gtk_box_pack_start_defaults(GTK_BOX(vbox),hbox);
  gtk_widget_show (hbox);

  /* Create the terminal widget and add it to the scrolling shell. */
  widget = vte_terminal_new();
  if (!dbuffer) {
    gtk_widget_set_double_buffered(widget, dbuffer);
  }
  gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 0);

  /* Connect to the "char_size_changed" signal to set geometry hints
   * whenever the font used by the terminal is changed. */
  if (geometry) {
    char_size_changed(widget, 0, 0, window);
    g_signal_connect(G_OBJECT(widget), "char-size-changed",
		     G_CALLBACK(char_size_changed), window);
  }

  /* Connect to the "window_title_changed" signal to set the main
   * window's title. */
  g_signal_connect(G_OBJECT(widget), "window-title-changed",
		   G_CALLBACK(window_title_changed), window);
  if (icon_title) {
    g_signal_connect(G_OBJECT(widget), "icon-title-changed",
		     G_CALLBACK(icon_title_changed), window);
  }

  /* Connect to the "eof" signal to quit when the session ends. */
  g_signal_connect(G_OBJECT(widget), "eof",
		   G_CALLBACK(destroy_and_quit_eof), window);
  g_signal_connect(G_OBJECT(widget), "child-exited",
		   G_CALLBACK(destroy_and_quit_exited), window);

  /* Connect to the "status-line-changed" signal. */
  g_signal_connect(G_OBJECT(widget), "status-line-changed",
		   G_CALLBACK(status_line_changed), widget);

  /* Connect to the "button-press" event. */
  g_signal_connect(G_OBJECT(widget), "button-press-event",
		   G_CALLBACK(button_pressed), widget);

  /* Connect to application request signals. */
  g_signal_connect(G_OBJECT(widget), "iconify-window",
		   G_CALLBACK(iconify_window), window);
  g_signal_connect(G_OBJECT(widget), "deiconify-window",
		   G_CALLBACK(deiconify_window), window);
  g_signal_connect(G_OBJECT(widget), "raise-window",
		   G_CALLBACK(raise_window), window);
  g_signal_connect(G_OBJECT(widget), "lower-window",
		   G_CALLBACK(lower_window), window);
  g_signal_connect(G_OBJECT(widget), "maximize-window",
		   G_CALLBACK(maximize_window), window);
  g_signal_connect(G_OBJECT(widget), "restore-window",
		   G_CALLBACK(restore_window), window);
  g_signal_connect(G_OBJECT(widget), "refresh-window",
		   G_CALLBACK(refresh_window), window);
  g_signal_connect(G_OBJECT(widget), "resize-window",
		   G_CALLBACK(resize_window), window);
  g_signal_connect(G_OBJECT(widget), "move-window",
		   G_CALLBACK(move_window), window);

  /* Connect to font tweakage. */
  g_signal_connect(G_OBJECT(widget), "increase-font-size",
		   G_CALLBACK(increase_font_size), window);
  g_signal_connect(G_OBJECT(widget), "decrease-font-size",
		   G_CALLBACK(decrease_font_size), window);

  /* Create the scrollbar for the widget. */
  scrollbar = gtk_vscrollbar_new((VTE_TERMINAL(widget))->adjustment);
  gtk_box_pack_start(GTK_BOX(hbox), scrollbar, FALSE, FALSE, 0);

  /* Set some defaults. */
  vte_terminal_set_audible_bell(VTE_TERMINAL(widget), audible);
  vte_terminal_set_visible_bell(VTE_TERMINAL(widget), !audible);
  vte_terminal_set_cursor_blinks(VTE_TERMINAL(widget), blink);
  vte_terminal_set_scroll_background(VTE_TERMINAL(widget), scroll);
  vte_terminal_set_scroll_on_output(VTE_TERMINAL(widget), FALSE);
  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(widget), TRUE);
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(widget), lines);
  vte_terminal_set_mouse_autohide(VTE_TERMINAL(widget), TRUE);
  if (background != NULL) {
    vte_terminal_set_background_image_file(VTE_TERMINAL(widget),
					   background);
  }
  if (transparent) {
    vte_terminal_set_background_transparent(VTE_TERMINAL(widget),
					    TRUE);
  }
  vte_terminal_set_background_tint_color(VTE_TERMINAL(widget), &tint);
  vte_terminal_set_colors(VTE_TERMINAL(widget), &fore, &back, NULL, 0);
  if (terminal != NULL) {
    vte_terminal_set_emulation(VTE_TERMINAL(widget), terminal);
  }
  
  /* Set the default font. */
  if (font != NULL) 
    {
      vte_terminal_set_font_from_string(VTE_TERMINAL(widget), font);
    }
  else 
    {
      vte_terminal_set_font_from_string(VTE_TERMINAL(widget), "monospace 10");
    }

  gtk_widget_grab_focus (widget);


  /* Go for it! */
  g_object_add_weak_pointer(G_OBJECT(widget), (gpointer*)&widget);
  g_object_add_weak_pointer(G_OBJECT(window), (gpointer*)&window);
  gtk_widget_show_all(window);
  /* need show all before getting the socket id */
  sprintf(buf,"SCIWIN=%ld",GDK_WINDOW_XWINDOW(socket_button->window));
  env_add[0]=buf;
  /* Launch a shell. */
  if ( cmdindex != 0 ) command_argv = &argv[cmdindex];
  vte_terminal_fork_command(VTE_TERMINAL(widget),
			    command, command_argv, env_add,
			    working_directory,
			    FALSE, FALSE, FALSE);
    
  if (command == NULL) {
    vte_terminal_feed_child(VTE_TERMINAL(widget),
			    "pwd\n", -1);
  }
  
  gtk_main();
  g_assert(widget == NULL);
  g_assert(window == NULL);

  if (keep) {
    while (TRUE) {
      sleep(60);
    }
  }
  return 0;
}
