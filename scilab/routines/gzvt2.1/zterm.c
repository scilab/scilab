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
#include <libzvt/libzvt.h>

/* for shared memory */
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 */

#define FONT "-misc-fixed-medium-r-normal--12-200-75-75-c-100-iso8859-1"

extern char      **environ;		
static char      **env;
static char      **env_copy;

static int
button_press_event (ZvtTerm *term, GdkEventButton *e)
{
  int x,y;
  GdkModifierType mask;
  char *data, *str;
  gdk_window_get_pointer(GTK_WIDGET(term)->window, &x, &y, &mask);

  str = zvt_term_match_check(term, x/term->charwidth, y/term->charheight,(void *)&data);
  if (str) {
    g_signal_stop_emission_by_name (term, "button_press_event");
    /* printf("User clicked on type '%s' -> '%s'\n", data, str); */
    zvt_term_writechild(term, str, strlen(str));
    return TRUE;
  }
  return FALSE;
}

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
    break;
  default:
    break;
  }
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
  hints.base_width = (GTK_WIDGET (term)->style->xthickness * 2) + PADDING;
  hints.base_height = (GTK_WIDGET (term)->style->ythickness * 2);
  hints.width_inc = term->charwidth;
  hints.height_inc = term->charheight;
  hints.min_width = hints.base_width + hints.width_inc;
  hints.min_height = hints.base_height + hints.height_inc;

  gtk_window_set_geometry_hints(GTK_WINDOW(app),
				GTK_WIDGET(term),
				&hints,
				GDK_HINT_RESIZE_INC|GDK_HINT_MIN_SIZE|GDK_HINT_BASE_SIZE);
}

char *create_shared(int *shmid) 
{
  char *shm; 
  /*
   * Create the segment.
   */
  if (( *shmid = shmget(IPC_PRIVATE,1,IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
  }
  /*
   * Now we attach the segment to our data space.
   */
  if ((shm = shmat(*shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }
  /* initialize with ' ' */
  *shm = ' ';
  return shm;
}


int main(gint argc, gchar *argv[])
{
  int shmid;
  char *shm = create_shared(&shmid) ; 
  char shmid_env[64];
  guint32 xid=0;
  int i, c, cmdindex, scrollbacklines, login_shell;
  char **p, *fontname=FONT;
  struct passwd *pw;
  GtkWidget *term, *hbox, *scrollbar, *window; 

  enum { RIGHT, LEFT } scrollpos = LEFT;

  login_shell = 0;
  cmdindex = 0;
  scrollbacklines = 1000;

  gtk_init (&argc, &argv);

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
  sprintf(shmid_env,"SHMID=%d",shmid);
  env_copy [i++] = shmid_env; 

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

  /* create terminal */
  term = zvt_term_new_with_size(60,24);
  zvt_term_set_scrollback(ZVT_TERM (term), scrollbacklines);
  zvt_term_set_scroll_on_keystroke (ZVT_TERM (term), TRUE);
  zvt_term_set_scroll_on_output (ZVT_TERM (term), FALSE);
  zvt_term_set_background (ZVT_TERM (term), NULL, 0, 0);

  g_signal_connect (G_OBJECT (term), "button_press_event", 
		    G_CALLBACK  (button_press_event), NULL);

  g_signal_connect (G_OBJECT (term),"child_died",
		    G_CALLBACK (child_died_event), NULL);

  g_signal_connect (G_OBJECT (term),"destroy",
		    G_CALLBACK(child_died_event), NULL);

  g_signal_connect ( G_OBJECT (term), "title_changed",
		     G_CALLBACK (title_changed_event),  NULL);
  
  g_signal_connect_after ( G_OBJECT (term), "realize",
			   G_CALLBACK(set_hints), term);

  gtk_widget_set_usize (term,ZVT_TERM(term)->charwidth*60, 
			ZVT_TERM(term)->charheight*24);
  gtk_widget_show (term);

  /* hbox */

  hbox = gtk_hbox_new (FALSE, 0);

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

  /*
   * Finally, we wait until the other process 
   * has writen the socket Id for us in the shared memory.
   * *shm == '*' indicates that information is available. 
   */

  while (*shm != '*')
    { 
      sleep(1);
    }
  xid = * ((guint32 *) (shm+1) );
  window = gtk_plug_new (xid);
  
  /* here we could detach the shared memory segment XXX */

  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_container_add (GTK_CONTAINER (window), hbox);
  gtk_widget_show_all (window);
  zvt_term_set_size (ZVT_TERM (term),80,24);
  gtk_widget_grab_focus (term);
  gtk_main() ;
  gtk_exit(0);
  return 0;
}




