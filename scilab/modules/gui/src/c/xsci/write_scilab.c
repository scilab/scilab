/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre Ledru  */
/* @author Others  */
/*-----------------------------------------------------------------------------------*/
#include <string.h>

#include "x_ptyxP.h"
#include "x_data.h"
#include "machine.h"
#include "All-extern-x.h"
#include "All-extern.h"
#include "xscion.h"
#include "TclEvents.h" /* flushTKEvents() */

#include "dynamic_menus.h" /* ismenu() */
#include "sciprint.h"

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
#include "inter.h"
#include "checkevts.h"
#include "scilabmode.h"
#include "write_scilab.h"
#include "sxevents.h"

static int posinbuff=0;

static void str_to_xterm_nw ( register char * string, int nbytes)
{
  posinbuff=0;
  if (nbytes>MAXBUFFSIZE-1)
    {
      Scierror(1020,"%s\n", "The menu name is too long to be stored");
      return;
    }
  sprintf(buffstring,string);
  buffstring[nbytes]='\n';
  lenbuffstring=nbytes+1;
  Click_menu(1);
}

void write_scilab(char *s)
{
  int  i;
  C2F(xscion)(&i);
  if (i==1)
    str_to_xterm(s,strlen(s));
  else
    str_to_xterm_nw(s,strlen(s));
  /*      	printf(" asynchronous actions are not supported with -nw option\n");*/

}
