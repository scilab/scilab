/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "math_graphics.h"
#include "Graphics.h" 
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "Xcall1.h"

extern int xinitxend_flag;

/********************************************************
 * the functions in this file are called from 
 * callback ( see jpc_SGraph.c ) for the XWindow version 
 * Nov 1998 : we must be sure that during the evaluation of 
 *            scig_xxx an other function scig_yyy won't be 
 *            run. This is possible since during the execution of 
 *            one scig_xxx function a sciprint can be performed 
 *            and it will lead to an event check which can 
 *            produce a call to an other scig_yyy function 
 *            flag scig_buzy  is used to check for that 
 *            
 ********************************************************/
extern int sciSwitchWindow  __PARAMS((int *winnum));/* NG */
extern void sciGetIdFigure __PARAMS((int *vect, int *id, int *iflag));/* NG */
#if !defined(_MSC_VER)
extern int WithBackingStore();
#endif
static int scig_buzy = 0;

/********************************************************
 * A handler which can be dynamically set to custiomize 
 * action of scig_xxx functions 
 ********************************************************/

int scig_handler_none(int win_num) {return win_num;};

Scig_handler scig_handler = scig_handler_none;

Scig_handler set_scig_handler(Scig_handler f)
{
  Scig_handler old = scig_handler;
  scig_handler = f;
  return old;
}

void reset_scig_handler(void)
{
  scig_handler = scig_handler_none;
}

/********************************************************
 * Basic Replay : redraw recorded graphics 
 ********************************************************/

void scig_replay(integer win_num)
{
  /* Modification Allan CORNET Mai 2004 */
  integer verb=0,cur,pix,na,backing;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pixmap",&verb,&pix,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

#if defined(_MSC_VER)
  backing = 0;
#else
  backing = WithBackingStore();
#endif
  if (backing) 
    {
      C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
    }
  else 
    {
      if (pix == 0) 
	{
	  if ( (GetDriver()) != 'R') C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	  if (version_flag() == 0)
	    {
	      sciRedrawFigure();
	      C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
	    }
	  else C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else
	{
	  C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
	}
    }

  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}


/********************************************************
 * Basic Replay : expose graphics i.e 
 * if we have a pixmap we can perform a wshow 
 * else we perform a sgig_replay 
 ********************************************************/

void scig_expose(integer win_num)
{
  integer verb=0,cur,pix,na,backing;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pixmap",&verb,&pix,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

#if defined(_MSC_VER)
  backing = 0;
#else
  backing = WithBackingStore();
#endif
  if (backing) 
    {
      /* only used whith X11 were pixmap mode can be used for backing store 
       * we are here in a case where the pixmap is used for backing store 
       */
      C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
    }
  else 
    {
      if (pix == 0) 
	{
	  if ( (GetDriver()) != 'R') 
	    C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	  if (version_flag() == 0)
	    {
	      sciRedrawFigure();
	      C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
	    }
	  else 
	    C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      else
	{
	  C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
	}
    }
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}

/********************************************************
 * Redraw graphic window win_num  after resizing 
 ********************************************************/

void scig_resize(integer win_num)
{
  integer verb=0,cur,na,pix,backing;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R') 
    C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","pixmap",&verb,&pix,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  CPixmapResize1();
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
#if defined(_MSC_VER)
  backing = 0;
#else
  backing = WithBackingStore();
#endif
  if (version_flag() == 0) {
      sciRedrawFigure();
/*       if (pix==1) C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);     */
      if (backing && pix!=1 ) C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
    }
  else 
    C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}

/********************************************************
 * Just resize a pixmap (win95 only)
 ********************************************************/

void scig_resize_pixmap(integer win_num)
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(SetDriver)("Int",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  CPixmapResize1();
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}


/********************************************************
 * clear window 
 ********************************************************/

void  scig_erase(integer win_num)
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R') 
    C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (version_flag() == 0) sciXbasc(); /*NG */
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xstart","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}



/*******************************************************
 * 2D Zoom 
 ******************************************************/

int scig_2dzoom(integer win_num)
{
  char name[4];
  int ret;

  if ( scig_buzy  == 1 ) return 0; ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /* if ( (GetDriver()) !='R'&&version_flag !=0) */ /* F.Leray 03.03.04*/
  if ( (GetDriver()) !='R'&& version_flag() !=0)
    {
      wininfo("Zoom works only with the Rec driver");
      return 0;
    }
  else 
    {
      integer verb=0,cur,na;
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      ret=zoom();
      if (cur != win_num)
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
  return ret;
}


/*******************************************************
 * Unzoom function 
 ******************************************************/

void   scig_unzoom(integer win_num)
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /* if ( (GetDriver()) !='R'&&version_flag !=0) */ /* F.Leray 03.03.04*/
  if ( (GetDriver()) !='R'&& version_flag() !=0)
    {
      wininfo("UnZoom works only with the Rec driver ");
    }
  else 
    {
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      unzoom();
      if (cur != win_num)
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
}

/*******************************************************
 * 3d rotation function 
 ******************************************************/

int scig_3drot(integer win_num)
{
  integer verb=0,cur,na,ret;
  char name[4];
  if ( scig_buzy  == 1 ) return 0;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /*  if ( (GetDriver()) !='R'&&version_flag !=0) */ /* F.Leray 03.03.04 */
  if ( (GetDriver()) !='R'&& version_flag() !=0)
    {
      wininfo("Rot3D works only with the Rec driver");
      return 0;
    }
  else 
    {
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      ret=I3dRotation();
      if (cur != win_num)
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
  return ret;
}

/********************************************************
 * graphic Window selection 
 ********************************************************/

void scig_sel(integer win_num)
{
  char c ;
  int v=1;
  if ((c=GetDriver())=='R' || c == 'X' || c == 'W')
    {
      C2F(dr)("xset","window",&win_num,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if (version_flag() == 0) sciSwitchWindow(&win_num);
    }
}

/********************************************************
 * graphic Window raise 
 ********************************************************/

void scig_raise(integer win_num)
{
 
  char c ;
  int cur,n,na,verb=0,iflag=0;

  if (version_flag() == 0) /* NG */
    { 
      sciGetIdFigure (PI0,&n,&iflag);
      if (n>0)
	{
	  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  if (win_num != cur)
	    {
	      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      sciSwitchWindow(&win_num);
	      C2F(dr)("xselect","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  else
	    {
	      C2F(dr)("xselect","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	}
      else
	{ 
	  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  sciSwitchWindow(&win_num);
	}
    }
  else 
    {
      if ((c=GetDriver())=='R' || c == 'X' || c == 'W')
	{
	  C2F(getwins)(&n,PI0 ,&iflag);
	  if (n>0) /* at least on figure exists, preserve the current one*/
	    {
	      C2F (dr)("xget", "window",&verb,&cur,&n,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xselect","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  else
	    {
	      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	}
    }
}


/********************************************************
 * Reload a saved graphic
 ********************************************************/

void scig_loadsg(int win_num, char *filename)
{
  integer verb=0,cur,na;
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(xloadplots)(filename,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}

