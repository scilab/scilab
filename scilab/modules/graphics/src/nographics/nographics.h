/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "Scierror.h"

int C2F(gw_graphics)(void);

void loadGraphicModule(void);

BOOL TerminateGraphics(void);

int scig_2dzoom(integer win_num);

int GetDriverId(void);

int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release);

void scig_unzoom(integer win_num);

void set_wait_click(int val);

int ClearClickQueue(int win);

int sciSetViewport( sciPointObj * pObj, int xSize, int ySize );

void sciGetViewport( sciPointObj * pObj, int * xSize, int * ySize );

int get_window_scale( integer i, double * subwin );

double Maxi(double *vect,integer n);

double Mini(double *vect, integer n);

int scig_3drot(integer win_num);

void scig_replay(integer win_num);

void scig_erase(integer win_num);

void scig_loadsg(int win_num, char *filename);

void del_window_scale(integer i);

void scig_savesg(int win_num, char * filename);

void Cscale2default(void);

int scig_toPs(integer win_num, integer colored, char * bufname, char * driver);

void delete_sgwin_entities(int win_num);

Scig_deletegwin_handler get_scig_deletegwin_handler (void);

void set_event_select(int val); 

int C2F(entier2d)( integer * n, double * dx,integer * s );

int C2F(dr)(char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6, integer *x7,double *dx1,double *dx2,double *dx3,double *dx4,integer lx0, integer lx1);

void scig_sel(integer win_num);

void DeleteObjs(integer win_num);

int C2F(dr1)( char x0[],char x1[],integer *x2,integer *x3,integer *x4,integer *x5,integer *x6, integer *x7,double *dx1,double *dx2,double *dx3,double *dx4, integer lx0, integer lx1);

void sciRedrawF(int *num);

int C2F(SetDriver)(void);

void GetDriver1(char *str, integer *v2,integer * v3,integer * v4, integer *v5,integer * v6,integer * v7,double * dv1,double * dv2,double * dv3,double * dv4);

int PushClickQueue (int win,int x,int y,int ibut,int motion,int release);

void sciGetIdFigure (int *vect, int *id, int *flag);

void set_cf_type(int val);
