/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "../../includes/bcg.h"
#include "../../includes/ObjectStructure.h"
#include "../../includes/clipping.h"
#include "../../includes/Events.h"
#include "../../includes/WindowList.h"
#include "machine.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_graphics)()
{
	Scierror(999,"Graphics interface not installed.\r\n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void loadGraphicModule( void )
{

}
/*-----------------------------------------------------------------------------------*/ 
BOOL TerminateGraphics(void)
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
int getWinsMaxId( void )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int scig_2dzoom(integer win_num)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
struct BCG * getWindowXgcNumber( integer i ) 
{
	 return NULL ;
}
/*-----------------------------------------------------------------------------------*/ 
void getWins( integer * Num, integer * Ids, integer * flag ) 
{

}
/*-----------------------------------------------------------------------------------*/ 
int GetDriverId( void ) 
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
WindowList * getScilabWindowList( void )
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void scig_unzoom(integer win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
void set_wait_click(int val)
{

}
/*-----------------------------------------------------------------------------------*/ 
int ClearClickQueue(int win)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int sciSetViewport( sciPointObj * pObj, int xSize, int ySize )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void sciGetViewport( sciPointObj * pObj, int * xSize, int * ySize )
{

}
/*-----------------------------------------------------------------------------------*/ 
int get_window_scale( integer i, double * subwin )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
double Maxi(double *vect,integer n)
{
	return 0.0;
}
/*-----------------------------------------------------------------------------------*/ 
double Mini(double *vect, integer n)
{
	return 0.0;
}
/*-----------------------------------------------------------------------------------*/ 
struct BCG * addWindowItem( void ) 
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
int removeWindowItem( struct BCG * window )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int scig_3drot(integer win_num)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void scig_replay(integer win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
void scig_erase(integer win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
void scig_loadsg(int win_num, char *filename) 
{

}
/*-----------------------------------------------------------------------------------*/ 
struct BCG * getFirstWindow( void )
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL isWindowListEmpty( void )
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
void del_window_scale( integer i ) 
{

}
/*-----------------------------------------------------------------------------------*/ 
void scig_savesg( int win_num, char * filename ) 
{

}
/*-----------------------------------------------------------------------------------*/ 
void Cscale2default( void )
{

}
/*-----------------------------------------------------------------------------------*/ 
void C2F(clipPolyLine)( integer       n     , 
					   integer     * vx    , 
					   integer     * vy    , 
					   integer       closed,
					   SClipRegion * clipping )
{

}
/*-----------------------------------------------------------------------------------*/ 
int scig_toPs( integer win_num, integer colored, char * bufname, char * driver )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void delete_sgwin_entities(int win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
Scig_deletegwin_handler get_scig_deletegwin_handler ( void )
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
void set_event_select(int val)
{

}
/*-----------------------------------------------------------------------------------*/ 
int C2F(entier2d)( integer * n, double * dx,integer * s )
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int C2F(dr)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
			integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
			integer lx0, integer lx1) 
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void scig_sel(integer win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
void DeleteObjs(integer win_num)
{

}
/*-----------------------------------------------------------------------------------*/ 
int C2F(dr1)( char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,
			 integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,
			 integer lx0, integer lx1) 
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void sciRedrawF(int *num)
{

}
/*-----------------------------------------------------------------------------------*/ 
 int C2F(SetDriver)()
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
 void GetDriver1(char *str, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6,
	 integer * v7,double * dv1,double * dv2,double * dv3,double * dv4) 
{

}
/*-----------------------------------------------------------------------------------*/ 
int PushClickQueue (int win,int x,int y,int ibut,int motion,int release)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void sciGetIdFigure (int *vect, int *id, int *flag)
{

}
/*-----------------------------------------------------------------------------------*/ 
void set_cf_type( int val ) 
{

}
/*-----------------------------------------------------------------------------------*/ 
