/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * The GIMP Help Browser
 * Copyright (C) 1999-2002 Sven Neumann <sven@gimp.org>
 *                         Michael Natterer <mitch@gimp.org>
 *
 * Some code & ideas stolen from the GNOME help browser.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* #include "config.h" */

#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <libgtkhtml/gtkhtml.h>

#include "../graphics/Math.h"
#include "All-extern.h"

#include "queue-gtk2.h"
#include "uri.h"

/*  defines  */

#define _(x) x 

#ifdef __EMX__
#define chdir _chdir2
#endif

enum
{
  BUTTON_HOME,
  BUTTON_INDEX,
  BUTTON_BACK,
  BUTTON_FORWARD
};

typedef struct
{
  const gchar *title;
  const gchar *ref;
  gint         count;
} HistoryItem;


static gchar *scilab_help_contents= NULL;
static gchar *scilab_help_index= NULL;


static const gchar *eek_png_tag = "<h1>Eeek!</h1>";

static gchar       *gimp_help_root = NULL;

static GList       *history = NULL;
static Queue       *queue;
static gchar       *current_ref;

static GtkWidget   *html;
static GtkWidget   *back_button;
static GtkWidget   *forward_button;
static GtkWidget   *combo;

static GtkWidget *window = NULL; /* jpc */

static GtkTargetEntry help_dnd_target_table[] =
{
  { "_NETSCAPE_URL", 0, 0 },
};


static gboolean temp_proc_installed = FALSE;

/*  forward declaration  */

static void     load_page   (const gchar  *ref,
                             gboolean      add_to_queue);
static void     request_url (HtmlDocument *doc,
                             const gchar  *url,
                             HtmlStream   *stream,
                             gpointer      data);
static gboolean io_handler  (GIOChannel   *io, 
                             GIOCondition  condition, 
                             gpointer      data);


/* Taken from glib/gconvert.c:
 * Test of haystack has the needle prefix, comparing case
 * insensitive. haystack may be UTF-8, but needle must
 * contain only ascii.
 */
static gboolean
has_case_prefix (const gchar *haystack, const gchar *needle)
{
  const gchar *h = haystack;
  const gchar *n = needle;

  while (*n && *h && g_ascii_tolower (*n) == g_ascii_tolower (*h))
    {
      n++;
      h++;
    }
  
  return (*n == '\0');
}

static void
close_callback (GtkWidget *widget,
		gpointer   user_data)
{
  gtk_widget_destroy(window); 
  window=NULL;
}

static void
update_toolbar (void)
{
  if (back_button)
    gtk_widget_set_sensitive (back_button, queue_has_prev (queue));
  if (forward_button)
    gtk_widget_set_sensitive (forward_button, queue_has_next (queue));
}

static void
button_callback (GtkWidget *widget,
                 gpointer   data)
{
  const gchar *ref;

  switch (GPOINTER_TO_INT (data))
    {
    case BUTTON_HOME:
      if ( scilab_help_contents)  load_page (scilab_help_index, TRUE);
      break;

    case BUTTON_INDEX:
      if ( scilab_help_index) load_page (scilab_help_contents, TRUE);
      break;

    case BUTTON_BACK:
      if (!(ref = queue_prev (queue)))
        return;
      load_page (ref, FALSE);
      queue_move_prev (queue);
      break;

    case BUTTON_FORWARD:
      if (!(ref = queue_next (queue)))
        return;
      load_page (ref, FALSE);
      queue_move_next (queue);
      break;

    default:
      return;
    }

  update_toolbar ();
}

static void 
entry_changed_callback (GtkWidget *widget,
			gpointer   data)
{
  GList       *list;
  HistoryItem *item;
  const gchar *entry_text;
  gchar       *compare_text;
  gboolean     found = FALSE;

  entry_text = gtk_entry_get_text (GTK_ENTRY (widget));

  for (list = history; list && !found; list = list->next)
    {
      item = (HistoryItem *) list->data;

      if (item->count)
        {
          compare_text = g_strdup_printf ("%s <%i>",
                                          item->title, item->count + 1);
        }
      else
        {
          compare_text = (gchar *) item->title;
        }

      if (strcmp (compare_text, entry_text) == 0)
	{
	  load_page (item->ref, TRUE);
	  found = TRUE;
	}

      if (item->count)
        {
          g_free (compare_text);
        }
    }
}

static void
history_add (const gchar *ref,
	     const gchar *title)
{
  GList       *list;
  GList       *found = NULL;
  HistoryItem *item;
  GList       *combo_list = NULL;
  gint         title_found_count = 0;

  for (list = history; list && !found; list = list->next)
    {
      item = (HistoryItem *) list->data;

      if (strcmp (item->title, title) == 0)
	{
	  if (strcmp (item->ref, ref) != 0)
	    {
	      title_found_count++;
	      continue;
	    }

	  found = list;
        }
    }

  if (found)
    {
      item = (HistoryItem *) found->data;
      history = g_list_remove_link (history, found);
    }
  else
    {
      item = g_new (HistoryItem, 1);
      item->ref   = g_strdup (ref);
      item->title = g_strdup (title);
      item->count = title_found_count;
    }

  history = g_list_prepend (history, item);

  for (list = history; list; list = list->next)
    {
      gchar* combo_title;

      item = (HistoryItem *) list->data;

      if (item->count)
	combo_title = g_strdup_printf ("%s <%i>",
				       item->title,
				       item->count + 1);
      else
	combo_title = g_strdup (item->title);

      combo_list = g_list_prepend (combo_list, combo_title);
    }

  combo_list = g_list_reverse (combo_list);

  g_signal_handlers_block_by_func (GTK_COMBO (combo)->entry,
                                   entry_changed_callback, combo);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo), combo_list);
  g_signal_handlers_unblock_by_func (GTK_COMBO (combo)->entry,
                                     entry_changed_callback, combo);

  for (list = combo_list; list; list = list->next)
    g_free (list->data);

  g_list_free (combo_list);
}

static void
title_changed (HtmlDocument *doc,
               const gchar  *new_title,
               gpointer      data)
{
  gchar *title;

  if (!new_title)
    new_title = (_("<Untitled>"));
      
  title = g_strstrip (g_strdup (new_title));

  history_add (current_ref, title);

  g_signal_handlers_block_by_func (GTK_COMBO (combo)->entry,
                                   entry_changed_callback, combo);
  gtk_entry_set_text (GTK_ENTRY (GTK_COMBO (combo)->entry), title);
  g_signal_handlers_unblock_by_func (GTK_COMBO (combo)->entry,
                                     entry_changed_callback, combo);

  g_free (title);
}


static void
load_page (const gchar *ref,	  
	   gboolean     add_to_queue)
{
  HtmlDocument *doc = HTML_VIEW (html)->document;
  gchar        *abs;
  gchar        *new_ref;
  gchar        *anchor;

  g_return_if_fail (ref != NULL);

  abs = uri_to_abs (ref, current_ref);

  g_return_if_fail (abs != NULL);

  anchor = strchr (ref, '#');
  if (anchor && anchor[0] && anchor[1])
    {
      new_ref = g_strconcat (abs, anchor, NULL);
      anchor += 1;
    }
  else
    {
      new_ref = g_strdup (abs);
      anchor = NULL;
    }

  if (strcmp (current_ref, abs))
    {
      if (!has_case_prefix (abs, "file:/"))
        {
          /* load_remote_page (ref); XXXX */
          return;
        }

      html_document_clear (doc);
      html_document_open_stream (doc, "text/html");
      gtk_adjustment_set_value (gtk_layout_get_vadjustment (GTK_LAYOUT (html)),
                                0);
      
      request_url (doc, abs, doc->current_stream, NULL);

    }

  if (anchor)
    {
      fprintf(stdout,"jump to anchor\n");
      html_view_jump_to_anchor (HTML_VIEW (html), anchor);
    }

  g_free (current_ref);
  current_ref = new_ref;

  if (add_to_queue) 
    queue_add (queue, new_ref);
  
  update_toolbar ();
}

static void
link_clicked (HtmlDocument *doc,
              const gchar  *url,
              gpointer      data)
{
  load_page (url, TRUE);
}

static void
request_url (HtmlDocument *doc,
             const gchar  *url,
             HtmlStream   *stream,
             gpointer      data)
{
  gchar *abs;
  gchar *filename;

  g_return_if_fail (url != NULL);
  g_return_if_fail (stream != NULL);

  abs = uri_to_abs (url, current_ref);
  if (!abs)
    return;

  filename = g_filename_from_uri (abs, NULL, NULL);

  if (filename)
    {
      gint fd;

      fd = open (filename, O_RDONLY);

      if (fd == -1)
        {
          gchar *name;
          gchar *msg;

          name = g_filename_to_utf8 (filename, -1, NULL, NULL, NULL);

          msg = g_strdup_printf
            ("<html>"
             "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"
             "<head><title>%s</title></head>"
             "<body bgcolor=\"white\">"
             "<div align=\"center\">"
             "<div>%s</div>"
             "<h3>%s</h3>"
             "<tt>%s</tt>"
             "</div>"
             "<br /><br />"
             "<div align=\"justify\">%s</div>"
             "</body>"
             "</html>",
             _("Document Not Found"),
             eek_png_tag,
             _("Could not locate help documentation"),
             name,
             _("The requested document could not be found in your Scilab help "
               "path as shown above. This means that the topic has not yet "
               "been written or your installation is not complete. Ensure "
               "that your installation is complete before reporting this "
               "error as a bug."));
          
          html_document_write_stream (doc, msg, strlen (msg));

          g_free (msg);
          g_free (name);
        }
      else
        {
          GIOChannel *io = g_io_channel_unix_new (fd);

          g_io_channel_set_close_on_unref (io, TRUE);
          g_io_channel_set_encoding (io, NULL, NULL);

          g_io_add_watch (io, G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL, 
                          io_handler, stream);
        }

      g_free (filename);
    }

  g_free (abs);
}

static gboolean
io_handler (GIOChannel   *io,
            GIOCondition  condition, 
            gpointer      data)
{
  HtmlStream *stream;
  gchar       buffer[8192];
  guint       bytes;

  stream = (HtmlStream *) data;

  if (condition & G_IO_IN) 
    {
      if (g_io_channel_read_chars (io, buffer, sizeof (buffer),
                                   &bytes, NULL) != G_IO_STATUS_ERROR
          && bytes > 0)
        {
          html_stream_write (stream, buffer, bytes);
        }
      else
	{
          html_stream_close (stream);
          g_io_channel_unref (io);

	  return FALSE;
	}

      if (condition & G_IO_HUP) 
        {
          while (g_io_channel_read_chars (io, buffer, sizeof (buffer),
                                          &bytes, NULL) != G_IO_STATUS_ERROR
                 && bytes > 0)
            {
              html_stream_write (stream, buffer, bytes);
            }
        }
    }

  if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL)) 
    {
      html_stream_close (stream);
      g_io_channel_unref (io);

      return FALSE;
    }

  return TRUE;
}

static void
drag_begin (GtkWidget      *widget,
            GdkDragContext *context,
            gpointer        data)
{
  gtk_drag_set_icon_stock (context, GTK_STOCK_JUMP_TO, -8, -8);
}

static void
drag_data_get (GtkWidget        *widget, 
               GdkDragContext   *context,
               GtkSelectionData *selection_data,
               guint             info,
               guint             time,
               gpointer          data)
{
  if (! current_ref)
    return;

  gtk_selection_data_set (selection_data,
                          selection_data->target,
                          8, 
                          current_ref, 
                          strlen (current_ref));
}


static void
open_browser_dialog (const gchar *help_path,
		     const gchar *locale,
		     const gchar *help_file)
{
  /* GtkWidget *window; */
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *bbox;
  GtkWidget *scroll;
  GtkWidget *button;
  GtkWidget *drag_source;
  GtkWidget *image;
  gchar     *eek_png_path;
  static int first =0;

  start_sci_gtk(); /* in case gtk was not initialized */

  if ( window != NULL) 
    {
      return ;
    }

  /* initialize scilab_help_path */ 
  if ( first == 0 ) 
    {
      scilab_help_contents= g_build_filename(help_path,locale,"contents.htm", NULL); 
      scilab_help_index= g_build_filename(help_path,locale,"index.htm", NULL); 
      first++;
    }

  eek_png_path = g_build_filename (gimp_help_root, "images", "eek.png", NULL);

  if (g_file_test (eek_png_path, G_FILE_TEST_EXISTS))
    eek_png_tag = g_strdup_printf ("<img src=\"%s\">", eek_png_path);

  g_free (eek_png_path);

  /*  the dialog window  */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (close_callback),
                    NULL);
  gtk_window_set_wmclass (GTK_WINDOW (window), "helpbrowser", "Scilab");
  gtk_window_set_title (GTK_WINDOW (window), _("Scilab Help Browser"));

  /* gimp_help_connect (window, gimp_standard_help_func, "dialogs/help.html"); */

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  /*  buttons  */
  bbox = gtk_hbutton_box_new ();
  gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);
  gtk_box_pack_start (GTK_BOX (vbox), bbox, FALSE, FALSE, 0);
  gtk_widget_show (bbox);

  button = gtk_button_new_from_stock (GTK_STOCK_HOME);
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_show (button);
  g_signal_connect (button, "clicked",
                    G_CALLBACK (button_callback),
                    GINT_TO_POINTER (BUTTON_HOME));

  button = gtk_button_new_from_stock (GTK_STOCK_INDEX);
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_show (button);
  g_signal_connect (button, "clicked",
                    G_CALLBACK (button_callback),
                    GINT_TO_POINTER (BUTTON_INDEX));

  back_button = button = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_set_sensitive (GTK_WIDGET (button), FALSE);
  g_signal_connect (button, "clicked",
                    G_CALLBACK (button_callback),
                    GINT_TO_POINTER (BUTTON_BACK));
  gtk_widget_show (button);

  forward_button = button = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_set_sensitive (GTK_WIDGET (button), FALSE);
  g_signal_connect (button, "clicked",
                    G_CALLBACK (button_callback),
                    GINT_TO_POINTER (BUTTON_FORWARD));
  gtk_widget_show (button);

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  /*  the drag source  */
  drag_source = gtk_event_box_new ();
  gtk_box_pack_start (GTK_BOX (hbox), drag_source, FALSE, FALSE, 4);
  gtk_widget_show (drag_source);

  gtk_drag_source_set (GTK_WIDGET (drag_source),
                       GDK_BUTTON1_MASK,
                       help_dnd_target_table,
                       G_N_ELEMENTS (help_dnd_target_table), 
                       GDK_ACTION_MOVE | GDK_ACTION_COPY);
  g_signal_connect (drag_source, "drag_begin",
                    G_CALLBACK (drag_begin),
                    NULL);
  g_signal_connect (drag_source, "drag_data_get",
                    G_CALLBACK (drag_data_get),
                    NULL);
  
  image = gtk_image_new_from_stock (GTK_STOCK_JUMP_TO, GTK_ICON_SIZE_BUTTON);
  gtk_container_add (GTK_CONTAINER (drag_source), image);
  gtk_widget_show (image);
  
  /*  the title combo  */
  combo = gtk_combo_new ();
  gtk_widget_set_size_request (GTK_WIDGET (combo), 360, -1);
  gtk_combo_set_use_arrows (GTK_COMBO (combo), TRUE);
  g_object_set (GTK_COMBO (combo)->entry, "editable", FALSE, NULL); 
  gtk_box_pack_start (GTK_BOX (hbox), combo, TRUE, TRUE, 0);
  gtk_widget_show (combo);

  g_signal_connect (GTK_COMBO (combo)->entry, "changed",
                    G_CALLBACK (entry_changed_callback), 
                    combo);


  /*  HTML view  */
  html  = html_view_new ();
  queue = queue_new ();

  gtk_widget_set_size_request (GTK_WIDGET (html), -1, 240);

  scroll = 
    gtk_scrolled_window_new (gtk_layout_get_hadjustment (GTK_LAYOUT (html)),
                             gtk_layout_get_vadjustment (GTK_LAYOUT (html)));
  
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (vbox), scroll, TRUE, TRUE, 0);
  gtk_widget_show (scroll);

  gtk_container_add (GTK_CONTAINER (scroll), html);
  gtk_widget_show (html);
  
  html_view_set_document (HTML_VIEW (html), html_document_new ());

  g_signal_connect (HTML_VIEW (html)->document, "title_changed",
                    G_CALLBACK (title_changed),
                    NULL);
  g_signal_connect (HTML_VIEW (html)->document, "link_clicked",
                    G_CALLBACK (link_clicked),
                    NULL);
  g_signal_connect (HTML_VIEW (html)->document, "request_url",
                    G_CALLBACK (request_url),
                    NULL);

  gtk_widget_show (window);

  current_ref = g_strconcat ("file://", help_path, "/", locale, "/", NULL);
  load_page (help_file, TRUE);
}



/*------------------------------------------------------
 * mandir = SCI+'man'  
 * locale = "eng" or "fr" 
 * help_file = null or absolute file name 
 * returns 0 on success and 1 if index.html not found 
 *------------------------------------------------------*/

int Sci_Help(char *mandir,char *locale,char *help_file) 
{
  if ( window == NULL) 
    open_browser_dialog (mandir,locale,help_file);
  else if ( help_file != NULL)
    load_page ( help_file, TRUE);
  return 0;
}


static void write_scilab_example(char *example)
{
  char *pos = example, *tmpdir;
  FILE *fd;
  gchar *fname,*instr; 

  if (( tmpdir=getenv("TMPDIR")) == NULL) 
    {
      sciprint("TMPDIR not set \r\n");
      return;
    }
  
  fname = g_strconcat (tmpdir, G_DIR_SEPARATOR_S, "example.sce",NULL);
  if ( fname == NULL) return ;

  if ((fd = fopen(fname,"w"))==NULL) return ;

  while ( 1 ) 
    {
      while ( *pos != '&' && *pos != '\0' ) 
	{
	  putc(*pos++,fd);
	}
      if ( *pos == '&') 
	{
	  if ( strncmp(pos,"&#10;",5) ==0) 
	    {
	      putc('\n',fd);

	      pos = pos +5;
	    }
	  else if ( strncmp(pos,"&quot;",6) ==0) 
	    {
	      putc('"',fd);
	      pos = pos +6;
	    }
	  else if ( strncmp(pos,"&amp;",5) ==0) 
	    {
	      putc('&',fd);
	      pos = pos +5;
	    }
	  else 
	    {
	      putc(*pos++,fd);
	    }
	}
      else if ( *pos == '\0') 
	{
	  break;
	}
    }

  fclose(fd);
  
  instr = g_strconcat("exec('",fname,"',7);",NULL);
  write_scilab(instr);
  g_free(instr);
  g_free(fname);

}

/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * The GIMP Help Browser - URI functions
 * Copyright (C) 2001  Jacob Schroeder  <jacob@convergence.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* 
#include <string.h>
#include <glib.h>
*/
#include "uri.h"

/*  #define URI_DEBUG 1  */

typedef enum
{
  URI_UNKNOWN,
  URI_ABSURI,
  URI_NETPATH,
  URI_ABSPATH,
  URI_RELPATH,
  URI_QUERY,
  URI_EMPTY,
  URI_FRAGMENT,
  URI_INVALID
} UriType;


static UriType
uri_get_type (const gchar *uri)
{
  gchar        c;
  const gchar *cptr;
  UriType      type = URI_UNKNOWN;
  
  if (!uri)
    return type;

  cptr = uri;
  c = *cptr++;

  if (g_ascii_isalpha (c))
    {
      type = URI_RELPATH;  /* assume relative path */
      
      while ((c = *cptr++))
        {
          if (g_ascii_isalnum (c) || c == '+' || c == '-' || c == '.')
            continue;
          
          if (c == ':')
            {
              /* it was a scheme */
              type = URI_ABSURI;
            }
          break;
        }
    }
  else
    {
      switch (c)
        {
        case '/':
          if (*cptr == '/')
            {
              cptr++;
              type = URI_NETPATH;
            }
          else
            {
              type = URI_ABSPATH;
            }
          break;
        case '?':
          type = URI_QUERY;
          break;
        case '#':
          type = URI_FRAGMENT;
          break;
        case '\0':
          type = URI_EMPTY;
          break;
        default:
          type = URI_RELPATH;
          break;
        }
    }

#ifdef URI_DEBUG
  g_print ("uri_get_type (\"%s\") -> ", uri);
  switch (type)
    {
    case URI_UNKNOWN:  g_print ("unknown");  break;
    case URI_ABSURI:   g_print ("absuri");   break;
    case URI_NETPATH:  g_print ("netpath");  break;
    case URI_ABSPATH:  g_print ("abspath");  break;
    case URI_RELPATH:  g_print ("relpath");  break;
    case URI_QUERY:    g_print ("query");    break;
    case URI_EMPTY:    g_print ("empty");    break;
    case URI_FRAGMENT: g_print ("fragment"); break;
    case URI_INVALID:  g_print ("invalid");  break;
    }
  g_print ("\n");
#endif

  return type;
}

gchar *
uri_to_abs (const gchar *uri,
            const gchar *base_uri)
{
  gchar        c;
  const gchar *cptr;
  gchar       *retval    = NULL;
  UriType      uri_type  = URI_UNKNOWN;
  UriType      base_type = URI_UNKNOWN;
  
  gint base_cnt    =  0;  /* no of chars to be copied from base URI  */
  gint uri_cnt     =  0;  /* no of chars to be copied from URI       */
  gint sep_cnt     =  0;  /* no of chars to be inserted between them */
  gint path_offset = -1;
  
  const gchar *sep_str = ""; /* string to insert between base and uri */
  const gchar *part;
  const gchar *last_segment = NULL;
  
#ifdef URI_DEBUG
  g_print ("uri_to_abs (\"%s\", \"%s\")\n", uri, base_uri);
#endif

  /* this function does not use the algorithm that is being proposed
   * in RFC 2396. Instead it analyses the first characters of each
   * URI to determine its kind (abs, net, path, ...).
   * After that it locates the missing parts in the base URI and then
   * concats everything into a newly allocated string.
   */
  
  /* determine the kind of the URIs */
  uri_type = uri_get_type (uri);
  
  if (uri_type != URI_ABSURI)
    {
      base_type = uri_get_type (base_uri);
    
      if (base_type != URI_ABSURI)
        {
          g_warning ("base uri is not absolute: '%s'\n", base_uri);
          return NULL;
        }
    }

  /* find missing parts in base URI */
  switch (uri_type)
    {
    case URI_ABSURI:
      /* base uri not needed */
      break;

    case URI_QUERY:
      /* ??? last segment? */
      uri_type = URI_RELPATH;
    case URI_NETPATH:  /* base scheme */
    case URI_ABSPATH:  /* base scheme and authority */
    case URI_RELPATH:  /* base scheme, authority and path */
      cptr = base_uri;

      /* skip scheme */
      while ((c = *cptr++) && c != ':')
        ; /* nada */
      
      base_cnt = cptr - base_uri; /* incl : */

      if (*cptr != '/')
        {
          /* completion not possible */
          return NULL;
        }

      if (uri_type == URI_NETPATH)
        break;
      
      /* skip authority */
      if (cptr[0] == '/' && cptr[1] == '/')
        {
          part = cptr;
          cptr += 2;

          while ((c = *cptr++) && c != '/' && c != '?' && c != '#')
            ; /* nada */

          cptr--;
          base_cnt += cptr - part;
        }

      path_offset = base_cnt; 

      if (uri_type == URI_ABSPATH)
        break;
      
      /* skip path */
      if (*cptr != '/')
        {
          sep_cnt = 1;
          sep_str = "/";
          break;
        }

      part = cptr;
      
      g_assert (*cptr == '/');
      
      while ((c = *cptr++) && c != '?' && c != '#')
        {
          if (c == '/')
            last_segment = cptr - 1;
        };
      
      g_assert (last_segment);
      
      cptr = last_segment;
      
      while ((c = *uri) && c == '.' && cptr > part)
        {
          gint shift_segment = 0;

          c = uri[1];
          
          if (c == '.' )
            {
              c = uri[2];
              shift_segment = 1;
            }

          if (c == '/')
            {
              uri += 2;
            } 
          else if (c == 0 || c == '?' || c == '#')
            {
              uri += 1;
            } 
          else 
            {
              break;
            }

          g_assert (*cptr == '/');
          
          if (shift_segment)
            {
              uri += 1;
              while (cptr > part && *--cptr != '/')
                ; /* nada */
            }
        }
      
      base_cnt += cptr - part + 1;
      break;
      
    case URI_EMPTY:
    case URI_FRAGMENT:
      /* use whole base uri */
      base_cnt = strlen (base_uri);
      break;

    case URI_UNKNOWN:
    case URI_INVALID:
      return NULL;
    }
  
  /* do not include fragment part from the URI reference */
  for (cptr = uri; (c = *cptr) && c != '#'; cptr++)
    ; /* nada */

  uri_cnt = cptr - uri;
  
  /* allocate string and copy characters */
  
  retval = g_new (gchar, base_cnt + sep_cnt + uri_cnt + 1);
  
  if (base_cnt)
    strncpy (retval, base_uri, base_cnt);

  if (sep_cnt)
    strncpy (retval + base_cnt, sep_str, sep_cnt);

  if (uri_cnt)
    strncpy (retval + base_cnt + sep_cnt, uri, uri_cnt);

  retval[base_cnt + sep_cnt + uri_cnt] = '\0';

#ifdef URI_DEBUG
  g_print ("  ->  \"%s\"\n", retval);
#endif

  return retval;
}

#if 0
RFC 2396                   URI Generic Syntax                August 1998


A. Collected BNF for URI

      URI-reference = [ absoluteURI | relativeURI ] [ "#" fragment ]
      absoluteURI   = scheme ":" ( hier_part | opaque_part )
      relativeURI   = ( net_path | abs_path | rel_path ) [ "?" query ]

      hier_part     = ( net_path | abs_path ) [ "?" query ]
      opaque_part   = uric_no_slash *uric

      uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |
                      "&" | "=" | "+" | "$" | ","

      net_path      = "//" authority [ abs_path ]
      abs_path      = "/"  path_segments
      rel_path      = rel_segment [ abs_path ]

      rel_segment   = 1*( unreserved | escaped |
                          ";" | "@" | "&" | "=" | "+" | "$" | "," )

      scheme        = alpha *( alpha | digit | "+" | "-" | "." )

      authority     = server | reg_name

      reg_name      = 1*( unreserved | escaped | "$" | "," |
                          ";" | ":" | "@" | "&" | "=" | "+" )

      server        = [ [ userinfo "@" ] hostport ]
      userinfo      = *( unreserved | escaped |
                         ";" | ":" | "&" | "=" | "+" | "$" | "," )

      hostport      = host [ ":" port ]
      host          = hostname | IPv4address
      hostname      = *( domainlabel "." ) toplabel [ "." ]
      domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
      toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
      IPv4address   = 1*digit "." 1*digit "." 1*digit "." 1*digit
      port          = *digit

      path          = [ abs_path | opaque_part ]
      path_segments = segment *( "/" segment )
      segment       = *pchar *( ";" param )
      param         = *pchar
      pchar         = unreserved | escaped |
                      ":" | "@" | "&" | "=" | "+" | "$" | ","

      query         = *uric

      fragment      = *uric

      uric          = reserved | unreserved | escaped
      reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
                      "$" | ","
      unreserved    = alphanum | mark
      mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
                      "(" | ")"

      escaped       = "%" hex hex
      hex           = digit | "A" | "B" | "C" | "D" | "E" | "F" |
                              "a" | "b" | "c" | "d" | "e" | "f"

      alphanum      = alpha | digit
      alpha         = lowalpha | upalpha

      lowalpha = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
                 "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
                 "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
      upalpha  = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
                 "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
                 "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
      digit    = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
                 "8" | "9"

#endif
