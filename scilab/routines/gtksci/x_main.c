/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 * mainsci.f directly call this function 
 * thus this is the real main for scilab 
 *----------------------------------------------------------*/

#include <pwd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <gtk/gtk.h>

#include "../version.h"
#include "../machine.h"
#include "../graphics/Math.h"
#include "All-extern.h"

char *ProgramName = NULL;

#if 0 
static void Syntax  (char *badOption);  
static void Help  (void);  
static void create_scilab_status(void);
#endif 

static void set_sci_env (void);

/*---------------------------------------------------------- 
 * mainsci.f directly call this function 
 * thus this is the real main for scilab 
 * Copyright Inria/Enpc 
 *----------------------------------------------------------*/

#define MIN_STACKSIZE 180000

static int  no_startup_flag=0;
static int  memory = MIN_STACKSIZE;
static int  no_window = 0;
static char * initial_script = NULL;
static int  initial_script_type = 0; /* 0 means filename 1 means code */

#ifdef WITH_TK
  /*extern void initTCLTK(void);
  extern void flushTKEvents(void);*/
#endif

/* extern int C2F(initcom)(int *,int*); */
extern int scilab_main (int argc,char **argv,char *pname,int no_window,int no_startup, char *display);
extern int C2F(nofpex)(void);
extern int C2F(scigetarg)(int *,char *,long int l);
extern int C2F(sciiargc)(void);
extern void C2F(settmpdir)(void);
extern char *get_sci_data_strings(int n);

/* menus.c */
extern void create_plugged_main_menu() ;
extern GtkWidget *create_main_menu( GtkWidget  *window);
extern void settexmacs();

static char ** create_argv(int *argc);
static void strip_blank(char *source);
static void nsp_create_gtk_toplevel(gint argc, gchar *argv[]);


static gtk_menu_force_ref(int x) 
{  
  if (x) gtk_menu_popdown(NULL);
}


/* global var */

int  sci_show_banner=1;

void C2F(realmain)()
{
  int ierr, argc,i,x=0;
  static int ini=-1;
  char startup[128];
  char **argv, *display = NULL;
  /* to be sure that gtkmenu.c patched  (for gtk < 1.3.5 ) 
   * is loaded 
   */
  gtk_menu_force_ref(x);
  /* floating point exceptions */
  C2F(nofpex)(); 
  /* create argv */
  if (( argv = create_argv(&argc))== NULL) 
    exit(1);
  ProgramName = argv[0];
  /* scanning options */
  for ( i=0 ; i < argc ; i++) 
    {

      if ( strcmp(argv[i],"-nw") == 0) { no_window = 1; } 
      else if ( strcmp(argv[i],"-nwni") == 0) { no_window = 1; } 
      else if ( strcmp(argv[i],"-display") == 0) { display = argv[++i];} 
      else if ( strcmp(argv[i],"-ns") == 0) { no_startup_flag = 1; }
      else if ( strcmp(argv[i],"-nb") == 0) { sci_show_banner = 0; }
      else if (strcmp(argv[i],"-mem") == 0) { memory = Max(atoi(argv[++i]),MIN_STACKSIZE );} 
      else if (strcmp(argv[i],"-f") == 0) { initial_script = argv[++i];} 
      else if ( strcmp(argv[i],"-e") == 0) 
	{
	  initial_script = argv[++i];
	  initial_script_type = 1;
	} 
      else if ( strcmp(argv[i],"-pipes") == 0) 
	{
	  /* old stuff used by geci 
	  int p1,p2;
  	  p1 = atoi(argv[++i]);
	  p2 = atoi(argv[++i]); 
	  C2F(initcom)(&p1, &p2); */
	}
      else if ( strcmp(argv[i],"--texmacs") == 0)  { no_window = 1;settexmacs();}
    }
  
  /* provide a default SCI  */
  set_sci_env();

  /* create temp directory */
  C2F(settmpdir)();

  #ifdef WITH_TK
    /*initTCLTK();
    flushTKEvents();*/
  #endif
  
  if ( no_window == 0 ) 
    {
      char *shmid= getenv("SHMID");
      /* Not Always initialise gtk */
      gtk_init(&argc,&argv);
      /* we are in window mode */
      if ( shmid != NULL )
	{
	  /* we build a toplevel widget and 
	   * zterm will be inserted through socket/plug mechanism 
	   */
	  nsp_create_gtk_toplevel(argc,argv);
	}
      else 
	{
	  /* we just create a menu which will be inserted 
	   * in the calling zterm through socket/plug mechanism 
	   */
	  create_plugged_main_menu() ;
	}
      /* create a status bar */ 
      /* XXX en attente est-ce utile ? 
	 create_scilab_status();
      */
      SetXsciOn();
    }
  /* signals */
  signal(SIGINT,sci_clear_and_exit);
  signal(SIGBUS,sci_clear_and_exit);
  signal(SIGSEGV,sci_clear_and_exit);
  signal(SIGQUIT,sci_clear_and_exit);
  signal(SIGHUP,sci_clear_and_exit);
  signal(SIGUSR1,sci_usr1_signal);
  signal(SIGWINCH, sci_winch_signal);

  /* initialize scilab interp  */
  C2F(inisci)(&ini, &memory, &ierr);
  /* try to get terminal size 
   */
  sci_winch_signal(0);
  {
    /* sci_winch_signal(0) does not work at this point 
     * thus we give default values
     */ 
    int r=22,c=60;
    C2F(scilines)(&r,&c);
  }

  if (ierr > 0) return ;
  /*  execute startup 
   *  and enter main loop 
   */

  if ( no_startup_flag == 0) 
    {
      /* execute a startup */
      strcpy(startup,get_sci_data_strings(1));
      strcat(startup,";quit");
      C2F(scirun)(startup,strlen(startup));
    }
  /* now fill startup with the initial_script if necessary */
  if ( initial_script != NULL ) 
    {
      switch ( initial_script_type ) 
	{
	case 0 : 
	  sprintf(startup,"exec('%s',-1)",initial_script);
	  break;
	case 1 : 
	  sprintf(startup,"%s;",initial_script);
	  break;
	}
    }
  else 
    strcpy(startup," ");
  /* message */  
  /* scilab_status_show("Scilab (C) Inria/Enpc"); */ 
  /* execute the initial script and enter scilab */ 

  /* 
  C2F(sciwin)();
  test3DA();
  gtk_main();
  */

  C2F(scirun)(startup,strlen(startup));
  /* cleaning */
  C2F(sciquit)();
}

/* This is to be used for starting gtk when 
 * scilab has been called with scilab -nw 
 * and a menu or graphic window is activated 
 */

void start_sci_gtk() {
  int i;
  C2F(xscion)(&i); 
  if ( i== 0 && GetBasic() == 1) 
    {
      int argc;
      char **argv; 
      if (( argv = create_argv(&argc))== NULL) 
	exit(1);
      /* initialise gtk */
      gtk_init(&argc,&argv);
      SetNotBasic();
    }
}

/* utility */

#define BSIZE 128 

static char ** create_argv(int *argc)
{
  int i;
  char **argv;
  *argc = C2F(sciiargc)() + 1;
  if ( ( argv = malloc((*argc)*sizeof(char *))) == NULL) return NULL;
  for ( i=0 ; i < *argc ; i++) 
    {
      char buf[BSIZE];
      C2F(scigetarg)(&i,buf,BSIZE);
      buf[BSIZE-1]='\0';
      strip_blank(buf);
      argv[i] = malloc((strlen(buf)+1)*sizeof(char));
      if ( argv[i] == NULL) return NULL;
      strcpy(argv[i],buf);
#ifdef DEBUG
      fprintf(stderr,"arg[%d] %s\n",i,argv[i]);
#endif 
    }
  return argv;
}

/* utility */

static void strip_blank(char *source)
{
  char *p;
  p = source;
  /* look for end of string */
  while(*p != '\0') p++;
  while(p != source) {
    p--;
    if(*p != ' ') break;
    *p = '\0';
  }
}

/*-------------------------------------------------------
 * Exit function called by some 
 * X11 functions 
 * call sciquit which call clearexit
 *-------------------------------------------------------*/

int C2F(sciquit)()
{
  int status = 0;
  if ( no_startup_flag == 0) 
    {
      char *quit_script =  get_sci_data_strings(5);
      C2F(scirun)(quit_script,strlen(quit_script));
    }
  C2F(clearexit)(&status);
  return 0;
} 


void sci_clear_and_exit(int n)
{
  C2F(sciquit)();
}

int C2F(clearexit)(n)
     int *n;
{
  /** clean tmpfiles **/
  C2F(tmpdirc)();
  /** clean ieee **/
#ifdef sun 
#ifndef SYSV
#include <sys/ieeefp.h>
  {
    char *mode, **out, *in;
    ieee_flags("clearall","exeption","all", &out);
  }
#endif 
#endif 
  /* really exit */
  exit(*n);
  return(0);
}

/*-------------------------------------------------------
 * usr1 signal : used to transmit a Control C to 
 * scilab 
 *-------------------------------------------------------*/

void sci_usr1_signal(int n) 
{
  controlC_handler(n);
}



/*-------------------------------------------------------
 * Ctrl-Z : stops the current computation 
 *          or the current interface call 
 *-------------------------------------------------------*/

void  sci_sig_tstp(int n)
{
  Scierror(999,"SIGSTP: aborting current computation\r\n");
}

/*-------------------------------------------------------
 * Utility function to try to hide system differences from
 * everybody who used to call killpg() 
 *-------------------------------------------------------*/

int kill_process_group(pid, sig)
    int pid;
    int sig;
{
    return kill (-pid, sig);
}

/*-------------------------------------------------------
 * Syntax 
 *-------------------------------------------------------*/

#if 0

static struct _options {
  char *opt;
  char *desc;
} options[] = {
{ "-help",                 "print out this message" },
{ "-ns",                   "no startup mode " },
{ "-nw",                   "no window mode " },
{ "-display displayname",  "X server to contact" },
{ "-name string",          "client instance, icon, and title strings" },
{ "-xrm resourcestring",   "additional resource specifications" },
{ "-tm string",            "terminal mode keywords and characters" },
{ NULL, NULL }};

static void Syntax (badOption)
    char *badOption;
{
  struct _options *opt;
  int col;

  fprintf (stderr, "%s:  bad command line option \"%s\"\r\n\n",
	   ProgramName, badOption);

  fprintf (stderr, "usage:  %s", ProgramName);
  col = 8 + strlen(ProgramName);
  for (opt = options; opt->opt; opt++) {
    int len = 3 + strlen(opt->opt);	 /* space [ string ] */
    if (col + len > 79) {
      fprintf (stderr, "\r\n   ");  /* 3 spaces */
      col = 3;
    }
    fprintf (stderr, " [%s]", opt->opt);
    col += len;
  }

  fprintf (stderr, "\r\n\nType %s -help for a full description.\r\n\n",
	   ProgramName);
  exit (1);
}

#endif 
/*-------------------------------------------------------
 * Help utility function 
 *-------------------------------------------------------*/

#if 0 

static char *message[] = {
  "Options that start with a plus sign (+) restore the default.",
  NULL
};

static void Help ()
{
  struct _options *opt;
  char **cpp;

  fprintf (stderr, "usage:\n        %s [-options ...] \n\n",
	   ProgramName);
  fprintf (stderr, "where options include:\n");
  for (opt = options; opt->opt; opt++) {
    fprintf (stderr, "    %-28s %s\n", opt->opt, opt->desc);
  }
  putc ('\n', stderr);
  for (cpp = message; *cpp; cpp++) {
    fputs (*cpp, stderr);
    putc ('\n', stderr);
  }
  putc ('\n', stderr);
  exit (0);
}

#endif 

/*-------------------------------------------------------
 * color status 
 *-------------------------------------------------------*/

static int screencolor = 1 ; /* default screen color status */

/* return the current screencolor */

void getcolordef(integer *screenc)
{
  *screenc= screencolor;
}

void setcolordef( int screenc)
{
  screencolor = screenc;
}

/*-------------------------------------------------------
 * try to build SCI and MANCHAPTERS if not provided 
 *-------------------------------------------------------*/

static char *sci_env;

extern int C2F(scigetcwd)( char **path, int *lpath, int *err);

static void set_sci_env ()
{
  char *p1; 
  if ((p1 = getenv ("SCI")) == (char *) 0)
    {
      sci_env = malloc((strlen(ProgramName)+1+4)*sizeof(char));
      if ( sci_env != NULL) 
	{
	  int i;
	  sprintf (sci_env, "SCI=%s",ProgramName);
	  /* removing the trailing /bin/scilex  */
	  for ( i = strlen(sci_env) ; i >= 0 ; i-- ) 
	    {
	      if ( sci_env[i]== '/' ) 
		{
		  if ( i >= 4 ) sci_env[i-4]= '\0';
		  else { free(sci_env) ; return ;}
		  break;
		}
	    }
	  if ( strcmp(sci_env,"SCI")==0 )  
	    {
	      /* special case when ProgramName = bin/scilex */
	      char *cwd;
	      int  lcwd,err;
	      C2F(scigetcwd)(&cwd,&lcwd,&err);
	      if ( err == 0 ) 
		{
		  free(sci_env);
		  sci_env = malloc((lcwd+1+4)*sizeof(char));
		  if ( sci_env != NULL) 
		    {
		      strcpy(sci_env,"SCI=");
		      strcat(sci_env,cwd);
		      putenv(sci_env);
		    }
		  else 
		    putenv("SCI=./");
		}
	      else 
		{
		  putenv("SCI=./");
		}
	    }
	  else 
	    {
	      putenv(sci_env);
	    }
	}
    }    
    
	#if (defined __GNUC__  )
		putenv ("COMPILER=gcc");
	#else
		putenv ("COMPILER=cc or another");
	#endif

    
}

/*-------------------------------------------------------
 * status bar 
 *-------------------------------------------------------*/

#if 0 

static GtkWidget *status = NULL; 

static void create_scilab_status()
{
  GtkWidget *Plug; 
  char * plug_info = getenv("SCIINFO");
  if ( plug_info == NULL) return ; 

  Plug = gtk_plug_new(atoi(getenv("SCIINFO")));
  status  = gtk_statusbar_new ();
  gtk_container_add(GTK_CONTAINER(Plug), status);
  gtk_widget_show_all(Plug);
}

void scilab_status_show(char * message)
{
  gtk_statusbar_pop (GTK_STATUSBAR(status), 1);
  gtk_statusbar_push(GTK_STATUSBAR(status), 1, message);
}

#endif 

/*
 * scilab toplevel widget when zterm widget is plugged 
 */

#include <gdk/gdkx.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkkeysyms.h>

/**
 * we use  shared memory to send back to the calling process 
 * the id of a socket button 
 */ 

#include <sys/ipc.h>
#include <sys/shm.h>

static char *get_shared() 
{
  int shmid;
  char *shm;
  char *s= getenv("SHMID");
  
  if ( s == NULL) 
    {
      perror("cannot getenv SHMID");
      exit(1);
    }
  shmid = atoi(s);

  /*
   * Now we attach the segment to our data space.
   */
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }
  return shm;
}

/*
 *  main routine
 *  Does setup, initialises windows, forks child.
 */

static void nsp_create_gtk_toplevel(gint argc, gchar *argv[])
{
  GtkWidget  *window = NULL;
  guint32 *xid; 
  char * shm = get_shared() ;
  GtkWidget *vbox,*menubar, *socket_button;

  gtk_init(&argc, &argv);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Scilab");
  gtk_window_set_wmclass (GTK_WINDOW (window), "scilab", "Scilab");

#if GTK_MAJOR_VERSION == 2 
  gtk_widget_set_size_request (window,600,400);
#else 
  gtk_widget_set_usize (window,600,400);
#endif 

  /* create vbox */
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_box_set_spacing (GTK_BOX (vbox), 2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  /* create the menu bar */

  menubar= create_main_menu( window);
  gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,TRUE,0);

  /* a socket in which I will redirect interaction */ 
  socket_button = gtk_socket_new();
  /* gtk_widget_set_usize(socket_button,500,300);*/
  gtk_box_pack_start(GTK_BOX(vbox), socket_button,TRUE,TRUE,0);

  /* show them all! */
  gtk_widget_show_all(window);
  gtk_widget_grab_focus(socket_button);
  /* I transmit the socket Id via shared memory  */ 
  xid = (guint32 *) (shm+1); 
  *xid = GDK_WINDOW_XWINDOW(socket_button->window); 
  *shm = '*' ; /* just to tell that there's something to read */
} 


/*-----------------------------------------------------------
 * should be moved elsewhere 
 * I ignore what this function is supposed to do 
 *-----------------------------------------------------------*/

void ShowWindowFunction (char *fname)
{
  
}

/*-----------------------------------------------------------
 * a new function ...
 *-----------------------------------------------------------*/

int GetScreenProperty(char *prop, char *value)
{ 
  /* be sure a gtk_init is started */

#if GTK_MAJOR_VERSION == 1 
  /* sciprint("GetScreenProperty not implemented in gtk 1\r\n"); */
  return -1;
#else 
  start_sci_gtk();
  GdkScreen *screen =  gdk_screen_get_default();
  
  if(!strcmp(prop,"screensize_px"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)1,(float)1,
	      (float) gdk_screen_get_width(screen),
	      (float) gdk_screen_get_height(screen));
    }
  else if(!strcmp(prop,"screensize_mm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float) gdk_screen_get_width_mm(screen),
	      (float) gdk_screen_get_height_mm (screen));
    }
  else if(!strcmp(prop,"screensize_cm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float) gdk_screen_get_width_mm(screen)/10.0,
	      (float) gdk_screen_get_height_mm (screen)/10.0);
    }
  else if(!strcmp(prop,"screensize_in"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float)  gdk_screen_get_width_mm(screen)/25.4,
	      (float)  gdk_screen_get_height_mm (screen)/25.4);
    }
	      else if(!strcmp(prop,"screensize_pt"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float)  gdk_screen_get_width_mm(screen)/25.4*72.0,
	      (float)  gdk_screen_get_height_mm (screen)/25.4*72.0);
    }
  else if(!strcmp(prop,"screensize_norm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,(float)1,(float)1);
    }
  else
    {
       return -1;
    }
  return 0;
#endif
}

/* Scilab get the DPI (root properties) */
/* F.Leray 08.03.05 */
/* return the x/y DPI */
int GetScreenDPI(int *ixres, int *iyres)
{
  /* be sure a gtk_init is started */
  
#if GTK_MAJOR_VERSION == 1 
  /* These are default values, corresponding to a 17 inches 
     flat panel with 1280x1024 resolution. */
  *ixres = 96;
  *iyres = 96;
  return 0;
#else 
  start_sci_gtk();
  GdkScreen *screen =  gdk_screen_get_default();
  double xres, yres;
  
  /*
   * there are 2.54 centimeters to an inch; so there are 25.4 millimeters.
   *
   *     dpi = N pixels / (M millimeters / (25.4 millimeters / 1 inch))
   *         = N pixels / (M inch / 25.4)
   *         = N * 25.4 pixels / M inch
   */
  
  xres = ((double) gdk_screen_get_width(screen) * 25.4) / 
	  ((double) gdk_screen_get_width_mm(screen));
  yres = ((double) gdk_screen_get_height(screen) * 25.4) / 
	  ((double) gdk_screen_get_height_mm(screen));
  
  *ixres = (int) (xres + 0.5);
  *iyres = (int) (yres + 0.5);
  
  return 0;
#endif
}


/* no comments .... should be elsewhere */

#include "../stack-c.h"

int XSaveNative _PARAMS((char *fname))
{
  sciprint("Not yet for Linux\n");
  LhsVar(1)=0;
  return 0;
}
