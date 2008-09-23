/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*--------------------------------------------------------------------------*/
#include "nographics.h"
#include "WindowList.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_graphics(void)
{
	Scierror(999,_("Scilab Graphics module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
void loadGraphicModule( void )
{

}
/*--------------------------------------------------------------------------*/
BOOL TerminateGraphics(void)
{
	return TRUE;
}
/*--------------------------------------------------------------------------*/
int scig_2dzoom(int win_num)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
int GetDriverId( void )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void scig_unzoom(int win_num)
{

}
/*--------------------------------------------------------------------------*/
void set_wait_click(int val)
{

}
/*--------------------------------------------------------------------------*/
int ClearClickQueue(int win)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
int sciSetViewport( sciPointObj * pObj, int xSize, int ySize )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void sciGetViewport( sciPointObj * pObj, int * xSize, int * ySize )
{

}
/*--------------------------------------------------------------------------*/
int get_window_scale( int i, double * subwin )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
double Maxi(double *vect,int n)
{
	return 0.0;
}
/*--------------------------------------------------------------------------*/
double Mini(double *vect, int n)
{
	return 0.0;
}
/*--------------------------------------------------------------------------*/
int scig_3drot(int win_num)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void scig_replay(int win_num)
{

}
/*--------------------------------------------------------------------------*/
void scig_erase(int win_num)
{

}
/*--------------------------------------------------------------------------*/
void scig_loadsg(int win_num, char *filename)
{

}
/*--------------------------------------------------------------------------*/
void del_window_scale( int i )
{

}
/*--------------------------------------------------------------------------*/
void scig_savesg( int win_num, char * filename )
{

}
/*--------------------------------------------------------------------------*/
void Cscale2default( void )
{

}
/*--------------------------------------------------------------------------*/
int scig_toPs( int win_num, int colored, char * bufname, char * driver )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void delete_sgwin_entities(int win_num)
{

}
/*--------------------------------------------------------------------------*/
Scig_deletegwin_handler get_scig_deletegwin_handler ( void )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void set_event_select(int val)
{

}
/*--------------------------------------------------------------------------*/
int C2F(entier2d)( int * n, double * dx,int * s )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void scig_sel(int win_num)
{

}
/*--------------------------------------------------------------------------*/
void DeleteObjs(int win_num)
{

}
/*--------------------------------------------------------------------------*/
void sciRedrawF(int *num)
{

}
/*--------------------------------------------------------------------------*/
 int C2F(SetDriver)()
{
	return 0;
}
/*--------------------------------------------------------------------------*/
 void GetDriver1(char *str, int *v2,int * v3,int * v4, int *v5,int * v6,
	 int * v7,double * dv1,double * dv2,double * dv3,double * dv4)
{

}
/*--------------------------------------------------------------------------*/
int PushClickQueue (int win,int x,int y,int ibut,int motion,int release)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL sciHasFigures( void )
{
  return FALSE;
}
/*--------------------------------------------------------------------------*/
char **getDictionaryGetProperties(int *sizearray)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **getDictionarySetProperties(int *sizearray)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/

