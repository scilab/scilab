/*------------------------------------------------------------------------
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *--------------------------------------------------------------------------*/

#include <string.h> 
#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef aix
#include <sys/select.h>
#endif

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#ifdef WITH_TK
#include "../tksci/tksci.h"
#endif

#include "../graphics/Math.h"
#include "../sun/Sun.h"
#include "All-extern.h"

extern void C2F(diary) _PARAMS((char *str, int *n, int nn));
extern void C2F(zzledt)( char *buffer,int *  buf_size, int * len_line,int * eof, long int dummy1);

/*---------------------------------------------------------
 * Input output functions 
 *---------------------------------------------------------*/

void C2F(xscisncr)(char *str,integer *n,long int dummy)
{
  int i;
  for ( i =0 ; i < *n; i++)  fprintf(stdout,"%c",str[i]);
}

void C2F(xscistring)(char *str, int *n, long int dummy)
{
  int i ;
  for ( i =0 ; i < *n; i++)  fprintf(stdout,"%c",str[i]);
  fprintf(stdout,"\n");
}

void C2F(xscimore)(int *n)
{
  int n1;
  *n=0;
  fprintf(stdout,"%s","[More (y or n ) ?] ");
  n1=Xorgetchar();
  if ( n1 == 110 )  *n=1;
  fprintf(stdout,"\n");
}

/* I/O Function for C routines no test for xscion here */

void Scisncr(char *str)
{
  fprintf(stdout,"%s",str);
}

void Scistring(str)
     char *str;
{
  fprintf(stdout,"%s",str);
}

/* 
 * sciprint(format,arg1,....)
 */

void  sciprint(char *fmt,...) 
{
  integer lstr;
  va_list ap;
  char s_buf[1024];
  va_start(ap,fmt);
  /* next three lines added for diary SS*/
  (void ) vsprintf(s_buf, fmt, ap );
  lstr=strlen(s_buf);
  C2F(diary)(s_buf,&lstr,0L);
  printf("%s",s_buf); 
  va_end(ap);
}

/* 
 * as sciprint but with an added first argument 
 * which is ignored (used in do_printf) 
 */

int  sciprint2(int iv,char *fmt,...) 
{
  int retval,lstr;
  va_list ap;
  char s_buf[1024];
  va_start(ap,fmt);
  /* next three lines added for diary SS*/
  (void ) vsprintf(s_buf, fmt, ap );
  lstr=strlen(s_buf);
  C2F(diary)(s_buf,&lstr,0L);
  retval= vfprintf(stdout, fmt, ap );
  va_end(ap);
  return retval;
}

/************************************************************************ 
 *  This routine is called by the Scilab interpreter in the course of 
 * computation to checks for all events except typed text (but testing 
 * Ctrl )
 * and to deal with them 
 * 
 ************************************************************************/

extern int ctrl_action();

void xevents1()
{
#ifdef WITH_TK
  flushTKEvents();
#endif
  /* XXXXXX */
}

/****************************************
 * main Input function 
 * returns a char and deal with events 
 ****************************************/

/*---------------------------------------------------------------------------
 * Functions to set or to get the scilab status 
 * i.e is it a scilab or a scilab -nw 
 ---------------------------------------------------------------------------*/

static int INXscilab=0;

void SetXsciOn()
{
#ifdef WITH_TK
  inittk();
#endif
  INXscilab=1;
}

/* for Fortran calls */

int C2F(xscion)(int *i)
{
  *i=INXscilab;
  return(0);
}

/*---------------------------------------------------------------------------
 * 
 *  Xorgetchar : function used while in the scilab -nw mode 
 *      by zzledt to get the next typed char (in stdin)
 *      but also checks for X11 events if we are using an X Window 
 *      ( ex a graphic window) with scilab -nw )
 *      
 *      stdin is supposed to be changed 
 *      so as not to wait for <cr> this is done inside zzledt 
 *      (in the following code  the key function is select )
 *---------------------------------------------------------------------------*/

void C2F(zzledt1)( char *buffer,int *  buf_size, int * len_line,int * eof,
			  long int dummy1)
{
  C2F(zzledt)(buffer, buf_size,len_line,eof, dummy1);
}

#define IBSIZE 1024
char sci_input_char_buffer[IBSIZE];
int  sci_input_char_buffer_count = 0;

#define SELECT_DEBUG(x)

/* send string s as if it was typed in scilab window */ 

void write_scilab(s)
    char   *s;
{
  while ( *s != '\0' && sci_input_char_buffer_count < IBSIZE) 
    {
      sci_input_char_buffer[sci_input_char_buffer_count++]= *s;
      s++;
    }
  sci_input_char_buffer[sci_input_char_buffer_count++]='\n';
}

/* XXXXX A terminer */
/* XXXXX remetre aussi ce qu'il faut pour tcl/tk */

int Xorgetchar()
{
  int i;
  static int c_count = -1;
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
    }
  
  for( ; ; ) {
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
    /* XXXX : the two next FD_SET causes select not to wait 
     * and since they do not seam necessary they are commented out  
     */
    /* FD_SET(fd_out,&write_mask);
       FD_SET(fd_err,&write_mask); */

    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;
    while ( gtk_events_pending())
      { 
	gtk_main_iteration(); 
      }
    /* maybe a new string to execute */
    if ( sci_input_char_buffer_count > 0) 
      {
	sci_input_char_buffer_count--;
	return sci_input_char_buffer[++c_count];
      }
    else
      {
	c_count = -1;
      }

    i = select(max_plus1, &select_mask,&write_mask, (fd_set *)NULL,
	       &select_timeout);
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
      return getchar();
      break;
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


/*---------------------------------------------------------------------------
 *  Dealing with X11 Events.
 *  xevents is called by Xorgetchar and also by DispatchEvents in 
 *  routines/system 
 *  xevents must work for scilab and scilab -nw 
 ---------------------------------------------------------------------------*/

int C2F(sxevents)()
{
  /* XXXX check the TK case */ 
  while ( gtk_events_pending())
    gtk_main_iteration(); 
  return(0);
}

/*-------------------------------------------------------
 * winch signal : window size changed 
 *-------------------------------------------------------*/

void sci_winch_signal(int n) 
{
  int rows,cols;
  sci_get_screen_size (&rows,&cols);
  C2F(scilines)(&rows,&cols);
#ifdef DEBUG
  fprintf(stderr,"windows size changed %d %d\r\n",rows,cols);
#endif 
}

