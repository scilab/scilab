/* C code produced by gperf version 2.7.1 (19981006 egcs) */
/* Command-line: gperf -C -t -k 2,3,4,$ Xcall.gperf  *//*------------------------------------------------------------------------
   *  Graphic library for 2D and 3D plotting 
   *  Copyright (C) 1998-2001 Chancelier Jean-Philippe
   * jpc@cermics.enpc.fr 
   --------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#ifdef MAC
#include "../machine.h"
#include "periMac.h"
#else 
#ifdef WIN32
#include "Math.h"
#include "periWin.h"
#else
#include "Math.h"
#include "periX11.h"
#endif
#endif 

#include "periPos.h"
#include "periFig.h"
#include "periGif.h"

/* should be removed when scilab will be full ansi C */

#ifndef __STDC__ 
#define const 
#endif 


/*---------------------------------------------------------------
 * The basic graphic driver is X11 
 *    The name is X11 due to historical reasons 
 *    but according to architecture X11 can be an Xwindow driver 
 *    a driver for Macintosh 
 *    or a Windows driver 
 * Other drivers are Postscript Fig ( xfig ) and Rec ( Rec= X11 + Recording capabilities) 
 *    xfig is only meaningfull when using Unix machine 
 * ----------------------------------------------------------------*/

static void C2F(all)();

static void C2F(vide)(char *v1,integer * v2,integer * v3,integer * v4,integer * v5,integer * v6,
		      integer *v7,double * dv1,double * dv2,double * dv3,double * dv4) {}

typedef void (*func)();
typedef void (function)() ;
struct funreplace { char *name; func action[4]; };

#define TOTAL_KEYWORDS 40
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 141
/* maximum key range = 138, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142,  35, 142,   5,
       15,   0,  25,  33,  20,   0, 142,   5,  10,  40,
       42,  30,   0, 142,   0,   0,   0,  15, 142,  10,
       10,   0, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
      142, 142, 142, 142, 142, 142
    };
  return len + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct funreplace *
xcall_in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  static const struct funreplace wordlist[] =
    {
      {""}, {""}, {""}, {""},
      {"xset",    {C2F(MissileGCset),C2F(scilabgcsetPos),C2F(scilabgcsetXfig),C2F(scilabgcsetGif)}},
      {""},
      {"xsetdr",    {C2F(SetDriver),C2F(SetDriver),C2F(SetDriver),C2F(SetDriver)}},
      {"xreplay",    {Tape_Replay,Tape_Replay,Tape_Replay,Tape_Replay}},
      {""}, {""},
      {"xrect",    {C2F(drawrectangle),C2F(drawrectanglePos),C2F(drawrectangleXfig),C2F(drawrectangleGif)}},
      {"xrects",    {C2F(drawrectangles),C2F(drawrectanglesPos),C2F(drawrectanglesXfig),C2F(drawrectanglesGif)}},
      {""}, {""},
      {"xreplaysc",    {Tape_ReplayNewScale,Tape_ReplayNewScale,Tape_ReplayNewScale,Tape_ReplayNewScale}},
      {""}, {""},
      {"xselect",    {C2F(xselgraphic),C2F(xselgraphicPos),C2F(xselgraphicXfig),C2F(xselgraphicGif)}},
      {"xstringl",    {C2F(boundingbox),C2F(boundingboxPos),C2F(boundingboxXfig),C2F(boundingboxGif),}},
      {""}, {""},
      {"xclear",    {C2F(clearwindow),C2F(clearwindowPos),C2F(clearwindowXfig),C2F(clearwindowGif)}},
      {""}, {""},
      {"xclickany",    {C2F(xclick_any),C2F(xclick_anyPos),C2F(xclick_anyXfig),C2F(xclick_anyGif)}},
      {""},
      {"xclick",    {C2F(xclick),C2F(xclickPos),C2F(xclickXfig),C2F(xclickGif)}},
      {""}, {""},
      {"xreplaysh",    {Tape_Replay_Show,Tape_Replay_Show,Tape_Replay_Show,Tape_Replay_Show}},
      {""},
      {"xfrect",    {C2F(fillrectangle),C2F(fillrectanglePos),C2F(fillrectangleXfig),C2F(fillrectangleGif)}},
      {""}, {""}, {""}, {""}, {""},
      {"xget",    {C2F(MissileGCget),C2F(scilabgcgetPos),C2F(scilabgcgetXfig),C2F(scilabgcgetGif)}},
      {"xsegs",    {C2F(drawsegments),C2F(drawsegmentsPos),C2F(drawsegmentsXfig),C2F(drawsegmentsGif)}},
      {"xgetdr",    {GetDriver1,GetDriver1,GetDriver1,GetDriver1}},
      {"xstring",    {C2F(displaystring),C2F(displaystringPos),C2F(displaystringXfig),C2F(displaystringGif)}},
      {"xstart",    {CleanPlots,CleanPlots,CleanPlots,CleanPlots}},
      {"xgetmouse",    {C2F(xgetmouse),C2F(xgetmousePos),C2F(xgetmouseXfig),C2F(xgetmouseGif)}},
      {""},
      {"xreplayna",    {Tape_ReplayNewAngle,Tape_ReplayNewAngle,Tape_ReplayNewAngle,Tape_ReplayNewAngle}},
      {"xarcs",    {C2F(drawarcs),C2F(drawarcsPos),C2F(drawarcsXfig),C2F(drawarcsGif)}},
      {"xpolys",    {C2F(drawpolylines),C2F(drawpolylinesPos),C2F(drawpolylinesXfig),C2F(drawpolylinesGif)}},
      {"xinit",    {C2F(initgraphic),C2F(initgraphicPos),C2F(initgraphicXfig),C2F(initgraphicGif)}},
      {""},
      {"xarc",    {C2F(drawarc),C2F(drawarcPos),C2F(drawarcXfig),C2F(drawarcGif)}},
      {"xaxis",    {C2F(drawaxis),C2F(drawaxisPos),C2F(drawaxisXfig),C2F(drawaxisGif)}},
      {"xarrow",    {C2F(drawarrows),C2F(drawarrowsPos),C2F(drawarrowsXfig),C2F(drawarrowsGif)}},
      {""}, {""}, {""},
      {"xclea",    {C2F(cleararea),C2F(clearareaPos),C2F(clearareaXfig),C2F(clearareaGif)}},
      {"xpause",    {C2F(xpause),C2F(xpausePos),C2F(xpauseXfig),C2F(xpauseGif)}},
      {""},
      {"xlines",    {C2F(drawpolyline),C2F(drawpolylinePos),C2F(drawpolylineXfig),C2F(drawpolylineGif)}},
      {"xliness",    {C2F(fillpolylines),C2F(fillpolylinesPos),C2F(fillpolylinesXfig),C2F(fillpolylinesGif)}},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"xfarcs",    {C2F(fillarcs),C2F(fillarcsPos),C2F(fillarcsXfig),C2F(fillarcsGif)}},
      {""}, {""}, {""},
      {"xfarc",    {C2F(fillarc),C2F(fillarcPos),C2F(fillarcXfig),C2F(fillarcGif)}},
      {"xlfont",    {C2F(loadfamily),C2F(loadfamilyPos),C2F(loadfamilyXfig),C2F(loadfamilyGif)}},
      {""}, {""}, {""},
      {"xarea",    {C2F(fillpolyline),C2F(fillpolylinePos),C2F(fillpolylineXfig),C2F(fillpolylineGif)}},
      {"xend",    {C2F(xend),C2F(xendPos),C2F(xendXfig),C2F(xendGif)}},
      {""}, {""}, {""}, {""},
      {"xmarks",    {C2F(drawpolymark),C2F(drawpolymarkPos),C2F(drawpolymarkXfig),C2F(drawpolymarkGif)}},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
      {"xgfont",    {C2F(queryfamily),C2F(queryfamilyPos),C2F(queryfamilyXfig),C2F(queryfamilyGif)}},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"xinfo",    {C2F(xinfo),C2F(vide),C2F(vide),C2F(vide)}},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"xname",    {C2F(setpopupname),C2F(vide),C2F(vide),C2F(vide)}},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"xnum",    {C2F(displaynumbers),C2F(displaynumbersPos),C2F(displaynumbersXfig),C2F(displaynumbersGif)}}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

static char DriverName[]= "Rec";
static int  DriverId = 0;

void C2F(SetDriver)(char *x0, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6,
		    integer * v7,double * dv1,double * dv2,double * dv3,double * dv4)
{
  switch (x0[0])
    {
    case 'I':
      /** special driver for windows : used when hdc is fixed elsewhere */
      strcpy(DriverName,"Int"); /* internal : for Win32 */
      DriverId = 0;
      break;
    case 'G':
      if (x0[1] == 'I') {
          strcpy(DriverName,"GIF");
	  DriverId = 3;
          break;
      }
    case 'X':
    case 'W':
      strcpy(DriverName,"X11");
      DriverId = 0;
      break;
    case 'P'  :
      if (x0[1] == 'P') {
	strcpy(DriverName,"PPM");
	DriverId = 3;
	break;
      }
      else {
 	strcpy(DriverName,"Pos");
 	DriverId = 1;
      }
      break;
    case 'F'  :
      strcpy(DriverName,"Fig");
      DriverId = 2;
      break;
    case 'R'  :
      strcpy(DriverName,"Rec");
      DriverId = 0;
      break;
    default:
      Scistring("\n Wrong driver name I'll use X11");
      strcpy(DriverName,"X11");
      DriverId = 0;
      break;
    }
}

void GetDriver1(char *str, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6,
		    integer * v7,double * dv1,double * dv2,double * dv3,double * dv4)
{
  strcpy(str,DriverName);
}

char GetDriver() {return(DriverName[0]);}

int GetDriverId() { return DriverId;}

/* The following function can be called by fortran so we 
   use the Sun C-fortran interface conventions 
   dr has 2 last extra parametres to deal with the size of
   x0 and x1 */

int C2F(dr)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
	    integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
	    integer lx0, integer lx1)
{ 
#ifdef WIN32
  if ( DriverId == 0 && DriverName[0] != 'I' ) 
    {
      SetWinhdc();
      C2F(all)(x0,x1,x2,x3,x4,x5,x6,x7,dx1,dx2,dx3,dx4);
      ReleaseWinHdc();
    }
  else 
    {
      C2F(all)(x0,x1,x2,x3,x4,x5,x6,x7,dx1,dx2,dx3,dx4);
    }
#else
      C2F(all)(x0,x1,x2,x3,x4,x5,x6,x7,dx1,dx2,dx3,dx4);
#endif
  return 0;
}

static void C2F(all)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,
		     integer *x6,
		     integer *x7,double *dx1,double *dx2,double *dx3,double *dx4)
{ 
  const struct funreplace *fr;
  fr=xcall_in_word_set(x0,strlen(x0));
  if ( fr != NULL ) 
    {
      (*(fr->action[DriverId]))(x1,x2,x3,x4,x5,x6,x7,dx1,dx2,dx3,dx4);
    }
  else 
    {
      sciprint("\n Unknow X operator <%s>\r\n",x0);
    }
}

int C2F(foo_inttest)(int *x1)
{
  sciprint("Value  of x1 = <%d>\r\n",*x1);
  return(0);
}

/*----------------------------------END---------------------------*/



