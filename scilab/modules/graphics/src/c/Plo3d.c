/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <math.h>
#include <stdio.h>
#include "math_graphics.h"
#include "PloEch.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "Xcall1.h"
#include "Plo3d.h"
#include "axesScale.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "Format.h"
#include "periScreen.h"
#include "CurrentObjectsManagement.h"

#include "MALLOC.h" /* MALLOC */

void wininfo  __PARAMS((char *fmt,...)); 

extern double C2F(dsort)();
extern char GetDriver(void);
extern int Check3DPlots(char *, integer *);
extern int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag);

/** PGEOX and PGEOY are like GEOX or GEOY in PloEch.h but we keep values in xx1 and yy1 for finite check **/
#define PGEOX(x1,y1,z1) inint(xx1= Cscale.Wscx1*(TRX(x1,y1,z1)-Cscale.frect[0]) +Cscale.Wxofset1);
#define PGEOY(x1,y1,z1) inint(yy1= Cscale.Wscy1*(-TRY(x1,y1,z1)+Cscale.frect[3])+Cscale.Wyofset1);

static void dbox __PARAMS((void));

void UpNext(integer ind1, integer *ind2, integer *ind3) ;
void DownNext(integer ind1, integer *ind2, integer *ind3) ;
void C2F(TDdrawaxis)(double size, double FPval, double LPval, integer *nax, integer *FPoint, integer *LPoint, integer *Ticsdir);
void TDAxis(integer flag, double FPval, double LPval, integer *nax, integer *FPoint, integer *LPoint, integer *Ticsdir) ;
int I3dRotation(void) ;

/*-------------------------------------------------------------------
 * box3d 
 *-------------------------------------------------------------------*/

int C2F(box3d)(double *xbox, double *ybox, double *zbox)
{  
 
  static integer InsideU[4],InsideD[4],flag[]={1,1,3};
  static integer ixbox[4],iybox[4], n=2, m=1;
  char * legends = NULL;
  sciPointObj * psubwin = NULL;
  sciSubWindow * ppsubwin =  NULL;
  
  char * legx = NULL;
  char * legy = NULL;
  char * legz = NULL;
  /** Calcule l' Enveloppe Convexe de la boite **/
  /** ainsi que les triedres caches ou non **/



  psubwin = sciGetCurrentSubWin();
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  legx = getStrMatElement( sciGetText(ppsubwin->mon_x_label), 0, 0 ) ;
  legy = getStrMatElement( sciGetText(ppsubwin->mon_y_label), 0, 0 ) ;
  legz = getStrMatElement( sciGetText(ppsubwin->mon_z_label), 0, 0 ) ;

  if ((legends = MALLOC ((strlen(legx)+
    strlen(legy)+
    strlen(legz)+
    7)*sizeof (char))) == NULL)
    sciprint("box3d : No more Place to store legends (3D labels)\n");

  if(legx == NULL)
    strcpy(legends,"");
  else
    strcpy(legends,legx);

  strcat(legends,"@"); 

  if(legy == NULL)
    strcat(legends,"");
  else
    strcat(legends,legy);

  strcat(legends,"@"); 

  if(legz == NULL)
    strcat(legends,"");
  else
    strcat(legends,legz);

  Convex_Box(xbox,ybox,InsideU,InsideD,legends,flag,Cscale.bbox1);
  FREE(legends); legends = NULL;


  C2F (dr) ("xset","thickness",&m,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);    
  if (zbox[InsideU[0]] > zbox[InsideD[0]])
    DrawAxis(xbox,ybox,InsideU,2);
  else 
    DrawAxis(xbox,ybox,InsideD,2);    
  if (zbox[InsideU[0]] > zbox[InsideD[0]])
    DrawAxis(xbox,ybox,InsideD,2);
  else 
    DrawAxis(xbox,ybox,InsideU,2);

  ixbox[0]=XScale(xbox[0]);ixbox[1]=XScale(xbox[2]);
  ixbox[2]=XScale(xbox[1]);ixbox[3]=XScale(xbox[3]);
  iybox[0]=YScale(ybox[0]); iybox[1]=YScale(ybox[2]);
  iybox[2]=YScale(ybox[1]); iybox[3]=YScale(ybox[3]);
  C2F(dr)("xpolys","v",ixbox,iybox,&n,&m,&n,PI0,PD0,PD0,PD0,PD0,0L,0L);  
  C2F(dr)("xpolys","v",ixbox+2,iybox+2,&n,&m,&n,PI0,PD0,PD0,PD0,PD0,0L,0L); 



  return(0);
}


/*-------------------------------------------------------------------
 * functions for 3D scales 
 *-------------------------------------------------------------------*/

/* 
 * if flag==1,2,3  m and bbox and Cscale are  recomputed  
 * if flag==0      we only change m without changing scales 
 */

void SetEch3d1(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha, integer flag)
{
  double xmmin = 0 ;
  double ymmax = 0 ;
  double xmmax = 0 ;
  double ymmin = 0 ;
  double FRect[4],WRect[4],ARect[4];
  integer ib;
  static integer aaint[]={2,10,2,10};
  int verbose=0,wdim[2],narg_;
  char logf_[2];
  double R,xo,yo,zo,dx,dy,dz,hx,hy,hx1,hy1,Teta,Alpha;
  integer wmax = 0, hmax = 0 ;
  static double cost=0.5,sint=0.5,cosa=0.5,sina=0.5;
  Teta=*teta;
  Alpha=*alpha;
  
  Cscale.alpha = Alpha;
  Cscale.theta = Teta;
  cost=cos((Teta)*M_PI/180.0);
  sint=sin((Teta)*M_PI/180.0);
  cosa=cos((Alpha)*M_PI/180.0);
  sina=sin((Alpha)*M_PI/180.0);
  Cscale.m[0][0]= -sint    ;    Cscale.m[0][1]= cost      ;    Cscale.m[0][2]= 0;
  Cscale.m[1][0]= -cost*cosa;   Cscale.m[1][1]= -sint*cosa;    Cscale.m[1][2]= sina;
  Cscale.m[2][0]=  cost*sina;   Cscale.m[2][1]= sint*sina;     Cscale.m[2][2]= cosa;
  /* Coordonn\'ees apr\`es transformation g\'eometrique de la
   * boite qui entoure le plot3d                            
   * le plan de projection est defini par x et y            
   */
  for (ib=0;ib<6 ;ib++) 
    { 
      if (flag==0) 
	bbox[ib]=Cscale.bbox1[ib];
      else 
	Cscale.bbox1[ib]=bbox[ib];
    }
  xbox[0]=TRX(bbox[0],bbox[2],bbox[4]);
  ybox[0]=TRY(bbox[0],bbox[2],bbox[4]);
  zbox[0]=TRZ(bbox[0],bbox[2],bbox[4]);
  xbox[1]=TRX(bbox[0],bbox[3],bbox[4]);
  ybox[1]=TRY(bbox[0],bbox[3],bbox[4]);
  zbox[1]=TRZ(bbox[0],bbox[3],bbox[4]);
  xbox[2]=TRX(bbox[1],bbox[3],bbox[4]);
  ybox[2]=TRY(bbox[1],bbox[3],bbox[4]);
  zbox[2]=TRZ(bbox[1],bbox[3],bbox[4]);
  xbox[3]=TRX(bbox[1],bbox[2],bbox[4]);
  ybox[3]=TRY(bbox[1],bbox[2],bbox[4]);
  zbox[3]=TRZ(bbox[1],bbox[2],bbox[4]);
  xbox[4]=TRX(bbox[0],bbox[2],bbox[5]);
  ybox[4]=TRY(bbox[0],bbox[2],bbox[5]);
  zbox[4]=TRZ(bbox[0],bbox[2],bbox[5]);
  xbox[5]=TRX(bbox[0],bbox[3],bbox[5]);
  ybox[5]=TRY(bbox[0],bbox[3],bbox[5]);
  zbox[5]=TRZ(bbox[0],bbox[3],bbox[5]);
  xbox[6]=TRX(bbox[1],bbox[3],bbox[5]);
  ybox[6]=TRY(bbox[1],bbox[3],bbox[5]);
  zbox[6]=TRZ(bbox[1],bbox[3],bbox[5]);
  xbox[7]=TRX(bbox[1],bbox[2],bbox[5]);
  ybox[7]=TRY(bbox[1],bbox[2],bbox[5]);
  zbox[7]=TRZ(bbox[1],bbox[2],bbox[5]);
  /** Calcul des echelles en fonction de la taille du dessin **/
  if ( flag == 1 || flag == 3 )
    {
      xmmin=  (double) Mini(xbox,8L);xmmax= (double) Maxi(xbox,8L);
      ymmax=  (double) - Mini(ybox,8L);
      ymmin=  (double) - Maxi(ybox,8L);
    }
  /* code added by es: isoview scaling */
  if ( flag == 2 || flag == 3 )
    {
      /* get current window size */
      C2F(dr)("xget","wdim",&verbose,wdim,&narg_, PI0,PI0,PI0,
              PD0,PD0,PD0,PD0,0L,0L);
      getscale2d(WRect,FRect,logf_,ARect);
      wmax=linint((double)wdim[0] * WRect[2]);
      hmax=linint((double)wdim[1] * WRect[3]); 
    }
  if ( flag == 2 )
    {
      /* radius and center of the sphere circumscribing the box */
      dx=bbox[1]-bbox[0]; dy=bbox[3]-bbox[2]; dz=bbox[5]-bbox[4];
      R= (double) sqrt(dx*dx + dy*dy + dz*dz)/2;
      xo= (double) (xbox[0]+xbox[6])/2 ;
      yo= (double) (ybox[0]+ybox[6])/2 ;
      zo= (double) (zbox[0]+zbox[6])/2 ;
      xmmin=  (double) xo - R ;
      xmmax=  (double) xo + R ;
      ymmax=  (double) -yo + R ;
      ymmin=  (double) -yo - R ;
    }
  if (flag==2 || flag==3)
    {
      /* adjust limits (code adapted from Plo2d.c & Stephane's patch) */
      hx=xmmax-xmmin;
      hy=ymmax-ymmin;
      if ( hx/(double)wmax  < hy/(double)hmax ) 
        {
          hx1=wmax*hy/hmax;
          xmmin=xmmin-(hx1-hx)/2.0;
          xmmax=xmmax+(hx1-hx)/2.0;
        }
      else 
        {
          hy1=hmax*hx/wmax;
          ymmin=ymmin-(hy1-hy)/2.0;
          ymmax=ymmax+(hy1-hy)/2.0;
        }
    }
  if (flag !=0 )
    {
      FRect[0]=xmmin;FRect[1]= -ymmax;FRect[2]=xmmax;FRect[3]= -ymmin;
      set_scale("tftttf",NULL,FRect,aaint,"nn",NULL);
      Cscale.metric3d=flag; /* the metric mode is stored into the
                             * List of Scales */
    }
  /* end of code added by es */
}

/* renvoit les indices des points voisins de ind1 sur la face haute 
de la boite  */

void UpNext(integer ind1, integer *ind2, integer *ind3)
{
  *ind2 = ind1+1;
  *ind3 = ind1-1;
  if (*ind2 == 8) *ind2 = 4;
  if (*ind3 == 3) *ind3 = 7;
}

void DownNext(integer ind1, integer *ind2, integer *ind3)
{
  *ind2 = ind1+1;
  *ind3 = ind1-1;
  if (*ind2 == 4) *ind2 = 0;
  if (*ind3 == -1) *ind3 = 3;
}

/*---------------------------------------------------------------------
 *Trace l'enveloppe convexe de la boite contenant le dessin 
 * et renvoit dans InsideU et InsideD les indices des points dans xbox et ybox
 * qui sont sur les 2 tri\`edres a l'interieur de l'enveloppe convexe
 *---------------------------------------------------------------------*/

void Convex_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox)
{
  double xmaxi;
  integer ixbox[8],iybox[8];
  integer xind[8];
  integer ind2,ind3,ind;
  integer p,n,dvect[1],dash[6];
  integer verbose=0,narg_,pat;
  integer i,j;
  /** dans xbox[8] se trouve l'abscisse des points successifs   **/
  /** de la boite qui continent la surface                      **/
  /** on stocke dans xind[8] les indices des points de la boite **/
  /** qui sont sur l'enveloppe convexe en partant du pointeger en haut **/
  /** a droite et en tournant ds le sens trigonometrique           **/
  /** par exemple avec : **/
  /*      4 ----- 5        */
  /*       /    /|         */
  /*     7----6  |         */
  /*      | 0 | / 1        */
  /*     3----- 2          */
  /** on doit trouver xind={5,4,7,3,2,1}; **/
  /** on en profite pour stocker aussi les points des triedres **/

  xmaxi=((double) Maxi(xbox,8L));
  ind= -1;
  for (i =0 ; i < 8 ; i++)
    {
      MaxiInd(xbox,8L,&ind,xmaxi);
      if ( ind > 3)
	{
	  xind[0]=ind;
	  break;
	}
    }
  if (ind < 0 || ind > 8) 
    {
      sciprint("xind out of bounds");
      xind[0]=0;
    }
  UpNext(xind[0],&ind2,&ind3);
  if (ybox[ind2] > ybox[ind3]) 
    {
      xind[1]=ind2;InsideU[0]=ind3;
    }
  else 
    {
      xind[1]=ind3;InsideU[0]=ind2;
    }
  UpNext(ind2,&ind2,&ind3); InsideU[1]=xind[0];
  InsideU[2]=ind2; InsideU[3]=InsideU[0]-4;
  xind[2]=ind2;
  /* le pointeger en bas qui correspond */
  xind[3]=ind2-4;
  DownNext(xind[3],&ind2,&ind3);
  if (ybox[ind2] < ybox[ind3]) 
    {
      xind[4]=ind2;InsideD[0]=ind3;
    }
  else  
    {
      xind[4]=ind3;InsideD[0]=ind2;
    }
  DownNext(ind2,&ind2,&ind3);
  InsideD[1]=xind[3];
  InsideD[2]=ind2;
  InsideD[3]=InsideD[0]+4;
  xind[5]=ind2;
  for (i=0; i < 6 ; i++)
    {
      ixbox[i]=XScale(xbox[xind[i]]);
      iybox[i]=YScale(ybox[xind[i]]);
    }
  ixbox[6]=ixbox[0];iybox[6]=iybox[0];
  p=7,n=1;
  /**DJ.Abdemouche 2003**/

  dvect[0]=2;       
  /** On trace l'enveloppe cvxe **/
  C2F(dr)("xget","line style",&verbose,dash,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(j=1,&j),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset","thickness",(j=1,&j),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);     /**DJ.Abdemouche 2003**/	   
  if (flag[2]>=3){
    C2F(dr)("xpolys","v",ixbox,iybox,dvect,&n,&p
	    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  C2F(dr)("xget","pattern",&verbose,&pat,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",dvect,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  if (flag[2]>=3)
    AxesStrings(flag[2],ixbox,iybox,xind,legend,bbox);
  C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",dash,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}

void C2F(TDdrawaxis)(double size, double FPval, double LPval, integer *nax, integer *FPoint, integer *LPoint, integer *Ticsdir)
{ 
  integer i;
  double dx,dy,ticsx,ticsy;
  dx= ((double) LPoint[0]-FPoint[0])/((double)nax[1]*nax[0]);
  dy= ((double) LPoint[1]-FPoint[1])/((double)nax[1]*nax[0]);
  if ( Ticsdir[0] == 0 && Ticsdir[1] == 0) 
  {
    ticsx= ticsy = 0;
  }
  else 
  {
    ticsx= ( Ticsdir[0])/
      sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1]);
    ticsy= ( Ticsdir[1])/
      sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1]);
  }
  for (i=0; i <= nax[1]*nax[0];i++)
  {       
    integer siz=2,x[2],y[2],iflag=0,style=0;
    x[0] =linint(FPoint[0]+ ((double)i)*dx );
    y[0] =linint(FPoint[1]+ ((double)i)*dy );
    x[1] =linint(x[0]+ ticsx*size);
    y[1] =linint(y[0]+ ticsy*size);
    C2F(dr)("xsegs","v",x,y,&siz,&style,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
}


void TDAxis(integer flag, double FPval, double LPval, integer *nax, integer *FPoint, integer *LPoint, integer *Ticsdir)
{
  char fornum[100];
  integer i,barlength;
  double xp, dx,dy,ticsx,ticsy,size;
  integer verbose=0,narg_,xz[2];
  C2F(dr)("xget","wdim",&verbose,xz,&narg_, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  size = xz[0]>=xz[1] ? xz[1]/50.0 : xz[0]/50.0; 
  C2F(TDdrawaxis)(size,FPval,LPval,nax,FPoint,LPoint,Ticsdir) ;
  ChoixFormatE(fornum,Min(FPval,LPval),Max(LPval,FPval),
    Abs((LPval-FPval))/nax[1]);
  xp= FPval;
  barlength=inint(1.2*size);
  dx= ((double) LPoint[0]-FPoint[0])/((double)nax[1]);
  dy= ((double) LPoint[1]-FPoint[1])/((double)nax[1]);
  if ( Ticsdir[0] == 0 && Ticsdir[1] == 0) 
  {
    ticsx= ticsy = 0;
  }
  else 
  {
    ticsx= barlength*( Ticsdir[0])/
      sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1]);
    ticsy= barlength*( Ticsdir[1])/
      sqrt((double) Ticsdir[0]*Ticsdir[0]+Ticsdir[1]*Ticsdir[1]);
  }
  for (i=0; i <= nax[1];i++)
  { double angle=0.0;
  integer flag1=0;
  integer xx=0,yy=0, posi[2],rect[4];
  char foo[100];/*** JPC : must be cleared properly **/
  double lp;
  lp = xp + i*(LPval-FPval)/((double)nax[1]);
  sprintf(foo,fornum,lp);   
  C2F(dr)("xstringl",foo,&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  posi[0]=inint(FPoint[0]+ i*dx + 2*ticsx );
  posi[1]=inint(FPoint[1]+ i*dy + 2*ticsy +rect[3]/2 );
  switch ( flag)
  {
  case 1: posi[0] -= rect[2];
    /** pour separer ;e 1er arg de l'axe des z de l'axe voisin **/
    if ( i== nax[1]) posi[1] -= rect[3]/2;
    break;
  case 2: posi[0] -= rect[2];break;
  }
  C2F(dr)("xstring",foo,&(posi[0]),&(posi[1]),PI0,&flag1,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
  } 
}


/** rajoute des symboles x,y,z : sur les axes     **/
/** et une graduation sur les axes **/
/** (ixbox,iybox) : Coordonnees des points de l'envelloppe cvxe en pixel **/
/** xind : indices des points de l'enveloppe cvxe ds xbox et ybox **/

void AxesStrings(integer axflag, integer *ixbox, integer *iybox, integer *xind, char *legend, double *bbox)
{
  integer verbose=0,narg_,xz[2];
  integer iof;
  char *loc = NULL;
  /*   char * buff = NULL; */
  char * legx = NULL;
  char * legy = NULL;
  char * legz = NULL;
  integer rect[4],flag=0,x,y;
  double ang=0.0;

  loc=(char *) MALLOC( (strlen(legend)+1)*sizeof(char));
  if ( loc == NULL)
    {
      sciprint("AxesString : No more Place to store Legends\n");
      return;
    }
  
  strcpy(loc,legend);
  /*   legx=strtok_r(loc,"@",&buff); */
  /*   legy=strtok_r(NULL,"@",&buff); */
  /*   legz=strtok_r(NULL,"@",&buff); */
 
  legx=strtok(loc,"@");
  legy=NULL ;strtok(NULL,"@");
  legz=NULL ;strtok(NULL,"@");

  /** le cot\'e gauche ( c'est tjrs un axe des Z **/
  C2F(dr)("xget","wdim",&verbose,xz,&narg_, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  iof = (xz[0]+xz[1])/50;

  x = (integer) (ixbox[2]-(xz[0]+xz[1])/20) ;
  y = (integer) (0.5*iybox[3]+0.5*iybox[2]);
  /*** le z scaling ***/
  if ( axflag>=4)
    {
      double fx,fy,fz,lx,ly,lz;
      integer LPoint[2],FPoint[2],Ticsdir[2],xnax[2];
      xnax[0]=5;xnax[1]=2;
      FPoint[0]=ixbox[2];FPoint[1]=iybox[2];
      LPoint[0]=ixbox[3];LPoint[1]=iybox[3];
      Ticsdir[0]= -1;
      Ticsdir[1]=0;
      BBoxToval(&fx,&fy,&fz,xind[2],bbox);
      BBoxToval(&lx,&ly,&lz,xind[3],bbox);
      TDAxis(1L,fz,lz,xnax,FPoint,LPoint,Ticsdir);
    }
  if (legz != NULL)
    {
      C2F(dr)("xstringl",legz,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstring",legz,(x=x - rect[2],&x),&y,PI0,&flag
	      ,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
    }
  /** le cot\^e en bas \`a gauche **/
  
  x=inint((ixbox[3]+ixbox[4])/2.0 -iof);
  y=inint((1/3.0)*iybox[3]+(2/3.0)*iybox[4]+(xz[0]+xz[1])/30);

  if ( xind[3]+xind[4] == 3)
    {
      if ( axflag>=4)
	{
	  double fx,fy,fz,lx,ly,lz;
	  integer LPoint[2],FPoint[2],Ticsdir[2],xnax[2];
	  xnax[0]=5;xnax[1]=2;
	  FPoint[0]=ixbox[3];FPoint[1]=iybox[3];
	  LPoint[0]=ixbox[4];LPoint[1]=iybox[4];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);
	  BBoxToval(&lx,&ly,&lz,xind[4],bbox);
	  TDAxis(2L,fx,lx,xnax,FPoint,LPoint,Ticsdir);
	}
      if (legx != NULL)
	{

	  C2F(dr)("xstringl",legx,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstring",legx,(x=x-rect[2],&x),&y,PI0,&flag
		  ,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	}
    }
  else 
    {
      if ( axflag>=4)
	{
	  double fx,fy,fz,lx,ly,lz;
	  integer LPoint[2],FPoint[2],Ticsdir[2],xnax[2];
	  xnax[0]=5;xnax[1]=2;
	  FPoint[0]=ixbox[3];FPoint[1]=iybox[3];
	  LPoint[0]=ixbox[4];LPoint[1]=iybox[4];
	  Ticsdir[0]=ixbox[4]-ixbox[5];
	  Ticsdir[1]=iybox[4]-iybox[5];
	  BBoxToval(&fx,&fy,&fz,xind[3],bbox);
	  BBoxToval(&lx,&ly,&lz,xind[4],bbox);
	  TDAxis(2L,fy,ly,xnax,FPoint,LPoint,Ticsdir);
	}
      if (legy != NULL)
	{

	  C2F(dr)("xstringl",legy,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xstring",legy,(x=x-rect[2],&x),&y,PI0,&flag
		  ,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	}
    }
  /** le cot\'e en bas a droite **/
  
  x=inint((ixbox[4]+ixbox[5])/2+(xz[0]+xz[1])/30);
  y=inint(((2/3.0)*iybox[4]+(1/3.0)*iybox[5])+iof);

  if ( xind[4]+xind[5] == 3)
  {
      if ( axflag>=4)
	{
	  double fx,fy,fz,lx,ly,lz;
	  integer LPoint[2],FPoint[2],Ticsdir[2],xnax[2];
	  xnax[0]=5;xnax[1]=2;
	  FPoint[0]=ixbox[4];FPoint[1]=iybox[4];
	  LPoint[0]=ixbox[5];LPoint[1]=iybox[5];
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  BBoxToval(&lx,&ly,&lz,xind[5],bbox);
	  TDAxis(3L,fx,lx,xnax,FPoint,LPoint,Ticsdir); 
	}
      if (legx != NULL) 
	{
	  C2F(dr)("xstring",legx,&x,&y,PI0,&flag,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	}
    }
  else 
    {
      if ( axflag>=4)
	{
	  double fx,fy,fz,lx,ly,lz;
	  integer LPoint[2],FPoint[2],Ticsdir[2],xnax[2];
	  xnax[0]=5;xnax[1]=2;
	  FPoint[0]=ixbox[4];FPoint[1]=iybox[4];
	  LPoint[0]=ixbox[5];LPoint[1]=iybox[5];
	  Ticsdir[0]=ixbox[4]-ixbox[3];
	  Ticsdir[1]=iybox[4]-iybox[3];
	  BBoxToval(&fx,&fy,&fz,xind[4],bbox);
	  BBoxToval(&lx,&ly,&lz,xind[5],bbox);
	  TDAxis(3L,fy,ly,xnax,FPoint,LPoint,Ticsdir); 
	}
      if (legy != NULL) 
	{
	  C2F(dr)("xstring",legy,&x,&y,PI0,&flag,PI0,PI0,&ang,PD0,PD0,PD0,0L,0L);
	}
    }  
  FREE(loc);
}


/** Returns the [x,y,z] values of a pointeger given its xbox or ybox indices **/

void BBoxToval(double *x, double *y, double *z, integer ind, double bbox[6] )
{
  switch ( ind)
    {
    case 0:	*x=bbox[0],*y=bbox[2],*z=bbox[4];break;
    case 1:	*x=bbox[0],*y=bbox[3],*z=bbox[4];break;
    case 2:	*x=bbox[1],*y=bbox[3],*z=bbox[4];break;
    case 3:	*x=bbox[1],*y=bbox[2],*z=bbox[4];break;
    case 4:	*x=bbox[0],*y=bbox[2],*z=bbox[5];break;
    case 5:	*x=bbox[0],*y=bbox[3],*z=bbox[5];break;
    case 6:	*x=bbox[1],*y=bbox[3],*z=bbox[5];break;
    case 7:	*x=bbox[1],*y=bbox[2],*z=bbox[5];break;
    }
}

/*-------------------------------------
 *  interactive rotation of a 3d plot 
 *--------------------------------------*/

/** Changement interactif de 3d **/
static double theta,alpha;
/* return 1 if figure has been close during rotation */
int I3dRotation(void)
{
  integer pixmode,alumode,verbose=0,narg_,ww;
  double xx,yy;
  double theta0,alpha0;
  static int one=1;

  static integer modes[]={1,0};/* for xgemouse only get mouse mouvement*/ 
  sciSons *psonstmp;
  sciPointObj * psubwin = NULL ;
  sciPointObj * tmpsubwin =  NULL ;
  sciPointObj * pold = NULL ;
  integer xr, yr;


  integer ibutton,in,iwait=0,istr=0;
  integer verbose_=0,ww_;
  double x0,yy0,x,y,xl,yl ;

  C2F(dr1)("xget","window",&verbose,&ww,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /**DJ.Abdemouche 2003**/
  C2F(dr)("xget","pixmap",&verbose,&pixmode,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","alufunction",&verbose,&alumode,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


#ifdef _MSC_VER
  SetWinhdc();
  SciMouseCapture();  
#endif
  C2F(dr)("xclick","one",&ibutton,&xr,&yr,&iwait,&istr,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef _MSC_VER
  ReleaseWinHdc();
  SciMouseRelease();
#endif
  theta=Cscale.theta ;
  alpha=Cscale.alpha ;
  theta0=theta;
  alpha0=alpha;

  ibutton=-1;
  tmpsubwin = sciGetCurrentSubWin() ;   
  if ( pFIGURE_FEATURE(sciGetCurrentFigure())->rotstyle == 0)    
  {
    psubwin = CheckClickedSubwin(xr,yr);
    /**DJ.Abdemouche 2003**/
    if( psubwin != NULL )
    {
      sciSubWindow * ppSubWin = pSUBWIN_FEATURE(psubwin) ;
      sciSetSelectedSubWin (psubwin);

      theta0 = ppSubWin-> theta; 
      alpha0 = ppSubWin-> alpha;
      ppSubWin->is3d = TRUE;
      Cscale.metric3d = (long)(ppSubWin->axes.flag[1]+1)/2; 

      /* Modif. HERE F.Leray 24.05.04 : we take advantage of update_specification_bounds and update_3dbounds previous call */
      /* brect variable should not exist any more.*/
      Cscale.bbox1[0] = ppSubWin->FRect[0]; 
      Cscale.bbox1[1] = ppSubWin->FRect[2];
      Cscale.bbox1[2] = ppSubWin->FRect[1];
      Cscale.bbox1[3] = ppSubWin->FRect[3];
      Cscale.bbox1[4] = ppSubWin->FRect[4];
      Cscale.bbox1[5] = ppSubWin->FRect[5];

      if( ppSubWin->cube_scaling )
      {
        Cscale.bbox1[0] =  0.; 
        Cscale.bbox1[1] =  1.;
        Cscale.bbox1[2] =  0.;
        Cscale.bbox1[3] =  1.;
        Cscale.bbox1[4] =  0.;
        Cscale.bbox1[5] =  1.;
      }
    }
    else
    {
      wininfo("No 3d object selected");
      return 0;
    }
  }
  else
  {                  
    psonstmp = sciGetLastSons (sciGetCurrentFigure());  
    while (psonstmp != (sciSons *) NULL)
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN) 
        break;   
      psonstmp = psonstmp->pnext;
    } 
    sciSetSelectedSubWin (psonstmp->pointobj);
  } 
  xx=1.0/Abs(Cscale.frect[0]-Cscale.frect[2]);
  yy=1.0/Abs(Cscale.frect[1]-Cscale.frect[3]);

  C2F(echelle2d)(&x0,&yy0,&xr,&yr,&one,&one,"i2f",3L);
  x0=(x0-Cscale.frect[0])*xx;
  yy0=(yy0-Cscale.frect[1])*yy;
  x=x0;y=yy0;

#ifdef _MSC_VER
  SetWinhdc();
  SciMouseCapture();
#endif

  if ( pixmode == 0 ) C2F(dr1)("xset","alufunction",(in=6,&in),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  while ( ibutton == -1 ) 
  {
    /* dessin d'un rectangle */
    theta= ((int)(theta0 - 180.0*(x-x0)) % 360);
    alpha= ((int)(alpha0 + 180.0*(y-yy0)) % 360);
    wininfo("alpha=%.1f,theta=%.1f",alpha,theta); 
    if ( pixmode == 1) C2F(dr1)("xset","wwpc",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    dbox();
    if ( pixmode == 1) C2F(dr1)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr1)("xgetmouse","one",&ibutton,&iwait,PI0,PI0,modes,PI0,&xl, &yl,PD0,PD0,0L,0L);
    if (ibutton==-100) return 1;/* window has been closed */
    /* effacement du rectangle */
    dbox();
    xx=1.0/Abs(Cscale.frect[0]-Cscale.frect[2]);
    yy=1.0/Abs(Cscale.frect[1]-Cscale.frect[3]);
    x=(xl-Cscale.frect[0])*xx;
    y=(yl-Cscale.frect[1])*yy;
  }
  if ( pixmode == 0) C2F(dr1)("xset","alufunction",(in=3,&in),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xget","window",&verbose_,&ww_,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#ifdef _MSC_VER
  ReleaseWinHdc();
  SciMouseRelease();
#endif

  if (pFIGURE_FEATURE((sciPointObj *)sciGetCurrentFigure())->rotstyle == 0){
    pold = sciGetCurrentSubWin();
    sciSetSelectedSubWin(psubwin);
    Obj_RedrawNewAngle(psubwin,theta,alpha);
    sciSetSelectedSubWin(pold);
  }
  else
  { 
    psonstmp = sciGetSons (sciGetCurrentFigure());  
    while (psonstmp != (sciSons *) NULL)	
    {  
      if(sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN){
        pold = sciGetCurrentSubWin();
        sciSetSelectedSubWin(psonstmp->pointobj);
        Obj_RedrawNewAngle(psonstmp->pointobj,theta,alpha);
        sciSetSelectedSubWin(pold);
      }
      psonstmp = psonstmp->pnext;
    } 
  }
  sciRedrawFigure(); 
  wininfo("alpha=%.1f,theta=%.1f",alpha,theta); 
  sciSetSelectedSubWin (tmpsubwin);
  return 0;
}

/*
 * Win32, warning when using xor mode
 * colors are changed and black is turned to white
 * so we must use an other pattern than the black one
 * inside dbox
 */

static void dbox(void)
{
  double xbox[8],ybox[8],zbox[8];
#ifdef _MSC_VER
  integer verbose_=0,pat,pat1=3,narg_,un=1;
  C2F(dr)("xget","pattern",&verbose_,&pat,&narg_,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","pattern",&pat1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "line style",&un,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif/**DJ.Abdemouche 2003**/
  SetEch3d1(xbox,ybox,zbox,Cscale.bbox1,&theta,&alpha,Cscale.metric3d);
  C2F(box3d)(xbox,ybox,zbox);
#ifdef _MSC_VER
  C2F(dr)("xset","pattern",&pat,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
#endif
}

/*---------------------------------------------------------------------------------
 *This function sorts the vertices such that the color value is in decreasing order
 *---------------------------------------------------------------------------------*/

int  triangleSort(integer *polyxin, integer *polyyin, integer *fillin, integer *polyx, integer *polyy, integer *fill)
{ 
  integer tmp,k;
  for (k=0;k<3;k++) {polyx[k]=polyxin[k]; polyy[k]=polyyin[k]; fill[k]=Abs(fillin[k]);}
      
  if (fill[0]<fill[1]) {  
    tmp=fill[0]; fill[0]=fill[1]; fill[1]=tmp;
    tmp=polyx[0]; polyx[0]=polyx[1]; polyx[1]=tmp;
    tmp=polyy[0]; polyy[0]=polyy[1]; polyy[1]=tmp;
  }
  if (fill[0]<fill[2]) {  
    tmp=fill[0]; fill[0]=fill[2]; fill[2]=tmp;
    tmp=polyx[0]; polyx[0]=polyx[2]; polyx[2]=tmp;
    tmp=polyy[0]; polyy[0]=polyy[2]; polyy[2]=tmp;
  }
  if (fill[1]<fill[2]) {  
    tmp=fill[1]; fill[1]=fill[2]; fill[2]=tmp;
    tmp=polyx[1]; polyx[1]=polyx[2]; polyx[2]=tmp;
    tmp=polyy[1]; polyy[1]=polyy[2]; polyy[2]=tmp;
  }
  return 0;
}


/*-----------------------------------------------------------------------
 * This is the main shading function. When the polygone has 4 vertices, it
 * is splitted in two triangles and shade() is recursively called twice.
 * Author : mottelet 2000 
 * XXXX: remplacer les malloc par graphic_alloc pour uniformiser avec les autres 
 *       routines 
 *-----------------------------------------------------------------------*/

int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag)
{
  integer px[5],py[5],fil[4],is[3],ie[3],n[3];
  integer npoly=1,k,col,cols,psize,i,s,e;
  integer polyxs[4],polyys[4],fills[4],*x[3],*y[3];
  double dx,dy;

  if (polysize == 3) { /* The triangle case */
 
    triangleSort(polyx,polyy,fill,polyxs,polyys,fills);
  
    is[0]=0; ie[0]=1;
    is[1]=1; ie[1]=2;
    is[2]=0; ie[2]=2;
     
    /* Computation of coordinates of elementary polygons for each side */
     
    for(i=0;i<3;i++) {

      s=is[i];
      e=ie[i];
      n[i]=fills[s]-fills[e];

      if (n[i]) {
	
	x[i]=(integer *)MALLOC((n[i]+2)*sizeof(integer));
	y[i]=(integer *)MALLOC((n[i]+2)*sizeof(integer)); 
	if (x[i]==NULL || y[i]==NULL) {
	  sciprint("shade : MALLOC No more Place\n");
	  return 0;
	}
		
	dx=((double)(polyxs[e]-polyxs[s]))/(double)n[i];
	dy=((double)(polyys[e]-polyys[s]))/(double)n[i];

	x[i][0]=polyxs[s];
	y[i][0]=polyys[s];
	   
	for(k=0;k<n[i];k++) {
	  x[i][k+1]=linint((double)polyxs[s] + (0.5+k)*dx);
	  y[i][k+1]=linint((double)polyys[s] + (0.5+k)*dy);
	}
	   
	x[i][n[i]+1]=polyxs[e];
	y[i][n[i]+1]=polyys[e];
      }
    }
     
    /* Fill the whole triangle with color fill[1] if all colors are equal */
         
    if (!n[0] && !n[1]) {

      psize=3;
      col=fills[0];
      C2F(dr)("xliness","str",polyxs,polyys,(cols=-Abs(col),&cols),&npoly,&psize ,PI0,PD0,PD0,PD0,PD0,0L,0L);
      /*          return(0);*/
    }
     
    if (n[0]) {
      psize=4;
      col=fills[0];  
      for(i=0;i<=n[0];i++) {
	px[0]=x[2][i]; px[1]=x[0][i]; px[2]=x[0][i+1]; px[3]=x[2][i+1];
	py[0]=y[2][i]; py[1]=y[0][i]; py[2]=y[0][i+1]; py[3]=y[2][i+1];
	C2F(dr)("xliness","str",px,py,(cols=-Abs(col),&cols),&npoly,&psize ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	col--;
      }
      FREE(x[0]);
      FREE(y[0]);
    }
     
    if (n[1]) {
      psize=4;
      col=fills[1];
      for(i=0;i<=n[1];i++) {
	px[0]=x[2][n[0]+i]; px[1]=x[1][i]; px[2]=x[1][i+1]; px[3]=x[2][n[0]+i+1];
	py[0]=y[2][n[0]+i]; py[1]=y[1][i]; py[2]=y[1][i+1]; py[3]=y[2][n[0]+i+1];
	C2F(dr)("xliness","str",px,py,(cols=-Abs(col),&cols),&npoly,&psize ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	col--;
      }
      FREE(x[1]);
      FREE(y[1]);  
    }

    if (n[2]) {
      FREE(x[2]);
      FREE(y[2]);
    }

  }
   
  else { /* The 4 vertices case  */
     
    px[0]=polyx[0]; px[1]=polyx[1]; px[2]=polyx[2];
    py[0]=polyy[0]; py[1]=polyy[1]; py[2]=polyy[2];
    fil[0]=fill[0]; fil[1]=fill[1]; fil[2]=fill[2];
    scilab_shade(px,py,fil,3,-1);

    px[0]=polyx[0]; px[1]=polyx[2]; px[2]=polyx[3];
    py[0]=polyy[0]; py[1]=polyy[2]; py[2]=polyy[3];
    fil[0]=fill[0]; fil[1]=fill[2]; fil[2]=fill[3];
    scilab_shade(px,py,fil,3,-1);
  }
   
  /* If flag>0 then the contour is drawn */
   
  if (flag > 0) { 
    fil[0]=0;
    polysize+=1;
    C2F (dr) ("xliness", "str", polyx, polyy, fil, &npoly,
	      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  }
  return 0;
}     


