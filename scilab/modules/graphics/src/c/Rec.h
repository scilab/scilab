/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifndef SCI_REC
#define SCI_REC 

/*---------------------------------------------------------------------
 * xcall1 
 *---------------------------------------------------------------------------*/

struct xcall1_rec {char *fname,*string;
		   integer *x1,*x2,*x3,*x4,*x5,*x6;
		   double *dx1,*dx2,*dx3,*dx4;
		   integer n1,n2,n3,n4,n5,n6,ndx1,ndx2,ndx3,ndx4;
		 } ;

/*---------------------------------------------------------------------
 * scales 
 *---------------------------------------------------------------------------*/


struct scale_rec { char *name; double *Wrect,*Frect,*Frect_kp; char logflag[2]; } ;

struct nscale_rec { char *name; char *flag;double *Wrect,*Frect,*Arect,*Frect_kp; char logflag[2]; } ;

  
/*---------------------------------------------------------------------
 * plot2d 
 *---------------------------------------------------------------------------*/

struct plot2d_rec {char *name,*xf;
		   double *x,*y,*brect;
		   integer   n1,n2,*style,*aint;
		   char *legend,*strflag;
		   char *strflag_kp;
		   integer *aint_kp;
		   double *brect_kp;
		 } ;

/*---------------------------------------------------------------------
 * axis 
 *---------------------------------------------------------------------------*/

struct sciaxis_rec {
  char *name;
  char pos,xy_type,logflag;
  int nx,ny,subtics,fontsize,textcolor,ticscolor,f_l,seg_flag;
  double *x,*y;  
  char **str;
  char *format; 
} ;
  
/*---------------------------------------------------------------------
 * grid 
 *---------------------------------------------------------------------------*/

struct xgrid_rec { char *name;
		   integer style ;
		 } ;


/*---------------------------------------------------------------------
 * param3d 
 *---------------------------------------------------------------------------*/

struct param3d_rec {char *name;
		    double *x,*y,*z,*bbox;
		    integer   n,*flag;
		    double teta,alpha;
		    char  *legend;
		 } ;

struct param3d1_rec {char *name;
		    double *x,*y,*z,*bbox;
		    integer   n,m,iflag,*colors,*flag;
		    double teta,alpha;
		    char  *legend;
		 } ;

/*---------------------------------------------------------------------
 * plot3d 
 *---------------------------------------------------------------------------*/

struct plot3d_rec {char *name;
		   double *x,*y,*z,*bbox;
		   integer   p,q,*flag;
		   double teta,alpha;
		   char  *legend;
		 } ;


/*---------------------------------------------------------------------
 * fac3d 
 *---------------------------------------------------------------------------*/

struct fac3d_rec {char *name;
		   double *x,*y,*z,*bbox;
		   integer   p,q,*flag,*cvect;
		   double teta,alpha;
		   char  *legend;
		 } ;

/*---------------------------------------------------------------------
 * fec 
 *---------------------------------------------------------------------------*/


struct fec_rec {char *name;
                double *x,*y,*triangles,*func;
                integer   Nnode,Ntr;
                double *brect, *zminmax;      /* zminmax added by bruno */
                integer  *aaint, *colminmax, *colout;  /* colminmax, colout added by bruno */
                int with_mesh;            /* added by bruno */
		char  *legend,*strflag;
		char *strflag_kp;
		double *brect_kp;
		integer *aaint_kp;
	      } ;

/*---------------------------------------------------------------------
 * contour 
 *---------------------------------------------------------------------------*/
/** general **/
struct contour_rec {char *name;
		    double *x,*y,*z,*zz,zlev;
		    integer   n1,n2,nz,flagnz;
		    double *bbox;
		    double teta,alpha;
                    integer *flag;
		    char  *legend;
		 } ;

/** version 2d **/

struct contour2d_rec {char *name;
  double *x,*y,*z,*zz;
  integer   n1,n2,nz,flagnz;
  double *brect;
  integer *style,*aint;
  char *legend,*strflag;
  char *strflag_kp;
  integer *aint_kp;
  double *brect_kp;
} ;



/*---------------------------------------------------------------------
 * xgray 
 *---------------------------------------------------------------------------*/

struct gray_rec {char *name,*strflag;
		 double *x,*y,*z,*brect;
		 integer   n1,n2,*aaint;
		 integer *aaint_kp;
		 double *brect_kp;
		 char *strflag_kp;
		 } ;

struct gray_rec_2 {char *name;
		 double *z,*xrect;
		 integer   n1,n2;
		 } ;

/*---------------------------------------------------------------------
 * xchamp  
 *---------------------------------------------------------------------------*/

struct champ_rec {char *name;
		  double *x,*y,*fx,*fy,*vrect,arfact;
		  integer   n1,n2;
		  char *strflag;
		  char *strflag_kp;
		  double *vrect_kp;
} ;

/*---------------------------------------------------------------------
 * doubly linked list for graphic storage 
 *---------------------------------------------------------------------------*/

struct listplot {
            char *type;
	    int  window;
            char *theplot; 
	    struct listplot   *ptrplot;
	    struct listplot   *previous;
} ;

extern struct listplot *ListPFirst ;

#endif 
