/*-----------------------------------------------------
 * XXXXXX experience 
 * zzledt with readline 
 *-----------------------------------------------------*/

#include <string.h> 
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#ifdef aix
#include <sys/select.h>
#endif

#include <readline/readline.h>

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#ifdef WITH_TK
#include "../tclsci/TCL_Global.h"
#endif

#include "../graphics/Math.h"

extern char sci_input_char_buffer[];
extern int  sci_input_char_buffer_count;

static int stop;
static char * full_line = NULL;
static int sci_readline(void);
static void process_line(char *line);
static char Sci_Prompt[10];


void C2F(zzledt)(char *buffer,int *  buf_size, int * len_line,int * eof,
		 long int dummy1)
{
  rl_callback_handler_install(Sci_Prompt, process_line);
  stop = 1;
  set_echo_mode(TRUE);
  set_is_reading(TRUE);
  sci_readline();
  /* now full_line contains the line */
  strcpy(buffer,full_line);
  *len_line = strlen(full_line);
  *eof = FALSE;
  set_is_reading(FALSE);
  if ( stop == 2 )
    free(full_line);
}

static int sci_readline()
{
  int i;
  static int GtkXsocket,fd_in,fd_out,fd_err;
  static int first = 0,max_plus1;
  fd_set select_mask,write_mask;
  static struct timeval select_timeout;
  if ( first == 0) 
    {
      first++;
      GtkXsocket = ConnectionNumber(GDK_DISPLAY());
      fd_in  = fileno(stdin) ;
      fd_out = fileno(stdout);
      fd_err = fileno(stderr);
      max_plus1 = Max(fd_in,GtkXsocket);      
      max_plus1 = Max(fd_out,max_plus1);
      max_plus1 = Max(fd_err,max_plus1) + 1;
      select_timeout.tv_sec = 50;
      select_timeout.tv_usec = 0;
    }
  
  while (stop == 1)  {
    /* always flush writes before waiting */
    gdk_flush();
    fflush(stdout); 
    fflush(stderr);
    /* Update the masks and, unless X events are already in the queue,
     * wait for I/O to be possible. 
     */
    FD_ZERO(&select_mask);
    FD_SET(fd_in , &select_mask);
    FD_SET(GtkXsocket, &select_mask);
    FD_ZERO(&write_mask);
    /* 
     * XXXX : the two next FD_SET causes select not to wait 
     * and since they do not seam necessary they are commented out 
     */
    /* FD_SET(fd_out,&write_mask);
       FD_SET(fd_err,&write_mask); */

    while ( gtk_events_pending())
      { 
	gtk_main_iteration(); 
      }
    /* maybe a new sentence to execute is already available */
    if ( sci_input_char_buffer_count > 0) 
      {
	full_line  = sci_input_char_buffer;
	sci_input_char_buffer_count = 0 ; 
	stop = 3; 
	return ; 
      }
    /* select */
    i = select(max_plus1, &select_mask,&write_mask, (fd_set *)NULL,
	       & select_timeout); 
    if (i < 0) {
      if (errno != EINTR)
	{ 
#ifdef DEBUG
	  fprintf(stderr,"error in select\n");
#endif
	  exit(0);
	  continue;
	} 
    }
    /* if there's something to output */
    if ( FD_ISSET(fd_out,&write_mask)) { 
      fflush(stdout); 
    }
    if ( FD_ISSET(fd_err,&write_mask)) { 
      fflush(stderr); 
    }

    /* if there's something to read */
    if ( FD_ISSET(fd_in,&select_mask )) { 
      rl_callback_read_char();
    } 
    if ( FD_ISSET(GtkXsocket,&select_mask)) { 
      /* if there are X events in our queue, it
       * counts as being readable 
       */
      while ( gtk_events_pending()) /*  ||(select_mask & gtk_mask) */
	{ 
	  gtk_main_iteration(); 
	}
    }
  }
}

void process_line(char *line)
{
  if ( line != NULL ) 
    { 
      if (line[0] != '\0' ) add_history(line);
      full_line = line ;
      rl_callback_handler_install("", process_line);
      stop = 2;
    }
}


void C2F(setprlev)(pause)
     int *pause;
{
  if ( *pause == 0 ) 
    sprintf(Sci_Prompt,"-->");
  else if ( *pause > 0 )
    sprintf(Sci_Prompt,"-%d->",*pause);
  else
    sprintf(Sci_Prompt,">>");
}

