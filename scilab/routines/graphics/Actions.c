/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include "Math.h"

/********************************************************
 * the functions in this file are called from 
 * callback ( see jpc_SGraph.c ) for the XWindow version 
 * Nov 1998 : we must be sure that during the evaluation of 
 *            scig_xxx an other function scig_yyy won't be 
 *            run. Thi sis possible since during the execution of 
 *            one scig_xxx function a sciprint can be performed 
 *            and it will lead to an event check which can 
 *            produce a call to an other scig_yyy function 
 *            flag scig_buzy  is used to check for that 
 *            
 ********************************************************/

static int scig_buzy = 0;

/********************************************************
 * A handler which can be dynamically set to custiomize 
 * action of scig_xxx functions 
 ********************************************************/


int scig_handler_none(win_num)int win_num; {return win_num;};

Scig_handler scig_handler = scig_handler_none;

Scig_handler set_scig_handler(f) 
     Scig_handler f;
{
  Scig_handler old = scig_handler;
  scig_handler = f;
  return old;
}

void reset_scig_handler() 
{
  scig_handler = scig_handler_none;
}


/********************************************************
 * Basic Replay : redraw recorded graphics 
 ********************************************************/

void scig_replay(win_num)
     integer win_num;
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) != 'R') 
    C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
  /* XXXX scig_handler(win_num); */
  C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy=0;
}


/********************************************************
 * Basic Replay : expose graphics i.e 
 * if we have a pixmap we can perform a wshow 
 * else we perform a sgig_replay 
 ********************************************************/

void scig_expose(win_num)
     integer win_num;
{
  integer verb=0,cur,pix,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","pixmap",&verb,&pix,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  if ( pix == 0) 
    {
      if ( (GetDriver()) != 'R') 
	C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
      /* XXXX scig_handler(win_num); */
      C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  else
    {
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
}

/********************************************************
 * Redraw graphic window win_num  after resizing 
 ********************************************************/

void scig_resize(win_num)
     integer win_num;
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
  CPixmapResize1();
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
  /* XXXX scig_handler(win_num); */
  C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}

/********************************************************
 * Just resize a pixmap (win95 only)
 ********************************************************/

void scig_resize_pixmap(win_num)
     integer win_num;
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

void  scig_erase(win_num)
     integer win_num;
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
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xstart","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  scig_buzy = 0;
}

/********************************************************
 * send recorded graphics to file bufname in ``driver'' syntax ( Pos or Fig )
 * win_num : the number of the window,
 * colored : 1 if color is wanted
 * bufname : string the name of the file 
 * driver : driver for code generation 
 ********************************************************/

void scig_tops(win_num,colored,bufname,driver)
     integer win_num,colored;
     char *bufname,*driver;
{
  char name[4];
  integer zero=0,un=1;
  integer verb=0,cur,na,screenc;
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",driver,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xinit",bufname,&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if (colored==1) 
    C2F(dr)("xset","use color",&un,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  else
    C2F(dr)("xset","use color",&zero,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  getcolordef(&screenc);
  /** we set the default screen color to the value of colored 
    because we don't want that recorded events such as xset("default") could 
    change the color status .
    and we set the UseColorFlag to 1 not to replay xset("use color",..) events 
    **/
  setcolordef(colored);
  UseColorFlag(1);
  /* XXXX scig_handler(win_num); */
  C2F(dr)("xreplay","v",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /** back to default values **/
  UseColorFlag(0);
  setcolordef(screenc);
  C2F(dr)("xend","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /* to force a reset in the graphic scales */
  SwitchWindow(&cur);
  scig_buzy = 0;
}

int C2F(xg2psofig)(fname,len,iwin,color,driver,l1,l2)
     char *fname,*driver;
     integer *len,*iwin,*color;
     long int l1,l2;
{
  int sc;
  if ( *color == -1 ) 
    getcolordef(&sc);
  else 
    sc= *color;
  scig_tops(*iwin,sc,fname,driver);
  return(0);
}

/*******************************************************
 * 2D Zoom 
 ******************************************************/

void scig_2dzoom(win_num)
     integer win_num;
{
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R') 
    {
      wininfo("Zoom works only with the Rec driver");
    }
  else 
    {
      integer verb=0,cur,na;
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      zoom();
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
}


/*******************************************************
 * Unzoom function 
 ******************************************************/

void   scig_unzoom(win_num)
     integer win_num;
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R') 
    {
      wininfo("UnZoom works only with the Rec driver ");
    }
  else 
    {
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      unzoom();
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
}

/*******************************************************
 * 3d rotation function 
 ******************************************************/

void scig_3drot(win_num)
     integer win_num;
{
  integer verb=0,cur,na;
  char name[4];
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  GetDriver1(name,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( (GetDriver()) !='R') 
    {
      wininfo("Rot3D works only with the Rec driver");
    }
  else 
    {
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      I3dRotation();
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xsetdr",name, PI0, PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  scig_buzy = 0;
}
/********************************************************
 * graphic Window selection 
 ********************************************************/

void scig_sel(win_num)
     integer win_num;
{
  C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}


/********************************************************
 * Reload a saved graphic
 ********************************************************/

void scig_loadsg(win_num,filename)
     int win_num;
     char *filename;
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

