/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *
 *    modified by Bruno Pincon 01/02/2001 for gain in speed and added 
 *    possibilities to set zmin, zmax by the user and also to set the 
 *    first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr)
 *
for entities handling
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Math.h" 
#include "Graphics.h"
#include "Entities.h" /* NG */
extern int version_flag(void); /* NG */


/* functions used by the modified version : */
static void PaintTriangle __PARAMS((double sx[], double sy[], double fxy[], 
				    int zxy[], 
				    double zlevel[], int fill[]));
static void PermutOfSort __PARAMS((int tab[], int perm[]));
static void FindIntersection __PARAMS((double sx[], double sy[], double fxy[],
				       double z, int inda, int indb, 
				       integer *xint, integer *yint));
void newfec __PARAMS((integer *xm,integer *ym,double *triangles,double *func,integer *Nnode,
		      integer *Ntr,double *zminmax,integer *colminmax));
extern void initsubwin();

/*------------------------------------------------------------
 *  Iso contour with grey level or colors 
 *  for a function defined by finite elements 
 *  ( f is linear on triangles )
 *  we give two versions of the function : 
 *     - a quick version wich only fill triangles according to the average 
 *     value of f on a triangle (no more such version now ?)
 *     - and a slow version but more sexy which use the fact that f is linear
 *     on each triangle.
 *  Nodes (x[no],y[no])
 *  Triangles (Matrix: [ numero, no1,no2,no3,iflag;...]
 *  func[no] : Function value on Nodes.
 *  Nnode : number of nodes 
 *  Ntr   : number of triangles 
 *  strflag,legend,brect,aint : see plot2d
 *  zminmax   : to set (optionnaly) the min and max level
 *  colminmax : to set (optionnaly) the first and last color to use
 *
 *  modified by Bruno Pincon 01/02/2001 for gain in speed and added 
 *  possibilities to set zmin, zmax by the user and also to set the 
 *  first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr)
---------------------------------------------------------------*/

int C2F(fec)(double *x, double *y, double *triangles, double *func, integer *Nnode, integer *Ntr, char *strflag, char *legend, double *brect, integer *aaint, double *zminmax, integer *colminmax, integer lstr1, integer lstr2)
{
  integer *xm,*ym,n1=1;

  /* Fec code */

  /* NG  beg */
   if (version_flag() == 0){
     long hdltab[2];
     int cmpt=0,styl[2];
     sciPointObj *pptabofpointobj;
     sciPointObj  *psubwin;

     if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
       sciXbasc(); 
       initsubwin();
       sciRedrawFigure();
     } 
  
    /*---- Boundaries of the frame ----*/
     if ((sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->autoscaling)
       update_frame_bounds(0,"gnn",x,y,&n1,Nnode,aaint,strflag,brect);
     psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
     sciSetIsClipping (psubwin,0); 
 
     strncpy(pSUBWIN_FEATURE (psubwin)->strflag, strflag, strlen(strflag));
     pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;

     sciDrawObj(sciGetSelectedSubWin (sciGetCurrentFigure ()));
     
     sciSetCurrentObj (ConstructFec 
		       ((sciPointObj *)
	                sciGetSelectedSubWin (sciGetCurrentFigure ()),
	                x,y,triangles,func,*Nnode,*Ntr,zminmax,colminmax)); 
     pptabofpointobj = sciGetCurrentObj();
     hdltab[cmpt]=sciGetHandle(pptabofpointobj);   
     cmpt++;   
     sciDrawObj(sciGetCurrentObj ());  
     /** Drawing the Legends **/
     if ((int)strlen(strflag) >=1  && strflag[0] == '1'){
       n1=1; styl[0]=1;styl[1]=0;
       sciSetCurrentObj (ConstructLegend
			 ((sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ()),
			  legend, strlen(legend), n1, styl, &pptabofpointobj)); 
       sciSetIsMark(pptabofpointobj, TRUE);
       sciSetMarkStyle (pptabofpointobj, *styl);
       sciDrawObj(sciGetCurrentObj ()); 
       hdltab[cmpt]=sciGetHandle(sciGetCurrentObj ()); 
       cmpt++;
     } 
     sciSetCurrentObj(ConstructAgregation (hdltab, cmpt));  /** construct agregation **/
   }
   else { /* NG end */
     /** Boundaries of the frame **/
     update_frame_bounds(0,"gnn",x,y,&n1,Nnode,aaint,strflag,brect);

     /* Storing values if using the Record driver */
     if ((GetDriver()=='R') && (version_flag() != 0)) /* NG */
       /* added zminmax and colminmax (bruno) */
       StoreFec("fec_n",x,y,triangles,func,Nnode,Ntr,strflag,legend,brect,aaint,zminmax,colminmax);

     /** Allocation **/
     xm = graphic_alloc(0,*Nnode,sizeof(int));
     ym = graphic_alloc(1,*Nnode,sizeof(int));
     if ( xm == 0 || ym == 0) {
       sciprint("Running out of memory \n"); return 0;}      
  
     C2F(echelle2d)(x,y,xm,ym,Nnode,&n1,"f2i",3L);

     newfec(xm,ym,triangles,func,Nnode,Ntr,zminmax,colminmax);
     axis_draw(strflag); 

     /** Drawing the Legends **/
     if ((int)strlen(strflag) >=1  && strflag[0] == '1')
       {
	 integer style = -1;
	 n1=1;
	 Legends(&style,&n1,legend);
       }        
   } /** version_flag ***/
   
   return(0);
   
}

void newfec(integer *xm,integer *ym,double *triangles,double *func,integer *Nnode,integer *Ntr,double *zminmax,integer *colminmax)
{
  /*************************************
   *	 code modified by Bruno 01/02/2001  
   *************************************/
    
  integer nz,i,j,k;
  integer verbose=0,whiteid,narg;
    
  double *zlevel, dz, zmin, zmax, fxy[3], sx[3], sy[3];
  int *zone, *fill, kp, perm[3], zxy[3], color_min;
  integer ii[3];

  /* choice between zmin and zmax given by the user or computed
   *   with the min and max z values. In matdes.c I have put 
   * zminmax[0]= zminmax[1]=0 if the user don't give this argument 
   */
 
  frame_clip_on();
  if ( zminmax[0]==zminmax[1] ) {  
    zmin=(double) Mini(func,*Nnode); 
    zmax=(double) Maxi(func,*Nnode);
  } 
  else {
    zmin = Min( zminmax[0] , zminmax[1] );
    zmax = Max( zminmax[0] , zminmax[1] );
  };
  
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,
	  PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  nz=whiteid;
    
  /* choice for the colormap (in case of a user 's choice 
   *   verify the parameter). For the automatic choice I have
   * put colminmax[0]=colominmax[1]=1 in matdes.c  
   */

  if ( colminmax[0] == colminmax[1] )  /* automatic choice (see matdes.c) */
    color_min=1; 
  else if ( colminmax[0] < 1 || colminmax[1] > nz || colminmax[0] > colminmax[1] ) {
    /* ici on pourrait plutot forcer les choses en imposant 1<= colmin < colmax <= nz */
    sciprint("\n\r fec : colminmax badly choosen ! ");
    return;
  } 
  else {
    color_min = colminmax[0];
    nz = colminmax[1] - colminmax[0] + 1;
  };
      
  /* 
   *  1/ the purpose of the first part is to to compute the "zone" of each point :
   *    
   *    - the array zlevel are the boundaries between the differents zones :
   *
   *        zlevel[0] = zmin, zlevel[nz] = zmax 
   *     and zlevel[i] = zmin + i*(zmax-zmin)/nz
   *  
   *     - if  zlevel[j-1] <= func[i] < zlevel[j]  then zone[i] = j
   *       if func[i] > zmax  then zone[i] = nz+1
   *       if func[i] < zmin  then zone[i] = 0
   *     - the zone j is filled with color fill[j] with
   *       fill[j] = -(j-1 + color_min) if 1 <= j <= nz
   *       fill[0] = color attributed for fill[1]     ---> this behavior may be changed ...
   *       fill[nz+1] = color attributed for fill[nz] --/
   */
 
  /* allocations for some arrays ... */

  zone = graphic_alloc(2,(*Nnode),sizeof(int));
  zlevel = graphic_alloc(3,nz+1,sizeof(double));
  fill  = graphic_alloc(4,nz+2,sizeof(int));
  if ( (zone == NULL) || (zlevel == NULL) || (fill  == NULL)) 
    {
      Scistring("fec: malloc No more Place\n");
      return;
    }
  /* compute the fill array (fill = - num color) */
  fill[1] = - color_min;
  for ( i = 2 ; i <= nz ; i++ ) fill[i] = fill[i-1] - 1;
  fill[0] = fill[1] ; fill[nz+1] = fill[nz];

  /* compute the zlevels */
  dz = (zmax - zmin)/nz;
  for (i = 0 ; i < nz ; i++) zlevel[i] = zmin + i*dz;
  zlevel[nz] = zmax;

  /* finaly compute the zone of each point */
  for ( i = 0 ; i < (*Nnode) ; i++ ) {
    if ( func[i] > zmax )
      zone[i] = nz+1;
    else if ( func[i] < zmin )
      zone[i] = 0;
    else
      zone[i] = floor( (func[i] - zmin)/dz ) + 1;
  };
  /* 
     2/ loop of the triangles : each triangle is finally decomposed 
     into its differents zones (polygons) by the function PaintTriangle   
  */
  for ( j = 0 ; j < *Ntr ; j++) {

    /* retrieve node numbers and functions values */
    for ( k = 0 ; k < 3 ; k++ ) {
      ii[k] = (integer) triangles[j+(*Ntr)*(k+1)] - 1;      
      zxy[k] = zone[ii[k]];  
    }

    /* get the permutation perm so as zxy[perm] is sorted */
    PermutOfSort(zxy, perm); 

    /* apply the permutation to get the triangle 's vertices
       in increasing zone (zxy[0] <= zxy[1] <= zxy[2]) */
    for ( k = 0 ; k < 3 ; k++ ) {
      kp = perm[k];
      sx[k]  = xm[ii[kp]];   sy[k]  = ym[ii[kp]];
      fxy[k] = func[ii[kp]]; zxy[k] = zone[ii[kp]];
    };

    /* call the "painting" function */
    PaintTriangle(sx, sy, fxy, zxy, zlevel, fill);
  }
  frame_clip_off();
}


/********************************************************************
 * functions used by the modified code (Bruno 01/02/2001)
 ********************************************************************/

static void PermutOfSort (int *tab, int *perm)
{
  /* 
   * get the permutation perm[3] which sort the array tab[3] in increasing order 
   */
  perm[0]=0; perm[1] = 1; perm[2] = 2;
  if ( tab[1] < tab[0] ) {
    perm[1]=0 ; perm[0] = 1;
  };
  if ( tab[2] < tab[perm[1]] ) {   /* sort not finish */
    if ( tab[2] < tab[perm[0]] ) {
      perm[2] = perm[1]; perm[1] = perm[0]; perm[0] = 2; 
    }
    else {
      perm[2] = perm[1] ; perm[1] = 2;
    };
  };
}


static void PaintTriangle (double *sx, double *sy, double *fxy, int *zxy, double *zlevel, int *fill)
{
  /* 
     arguments :
     ---------
     sx, sy : vertices coordinates of a triangle (Pi=(sx[i],sy[i]) i=0,1,2)
     fxy    : fxy[i], (i=0,1,2) value of an affine function on the vertex Pi
     zxy    : zone of Pi : zxy[i]=j if  zlevel[j-1] <= fxy[i] < zlevel[j]
     zlevel : a (0..nz) vector given the boundaries for color filling
     fill   : fill[j] is the color pattern associated with zone[j] 
     
     purpose : this function decompose the triangle into its different
     -------   zones (which gives polygones) and send them to the
               graphic driver. This is something like the shade function
               (see Plo3d.c) but a little different as in shade
               a color is directly associated with each vertex.
  */

  int nb0, edge, izone, color;
  integer ncont,nr, resx[5],resy[5];
  integer xEdge2, yEdge2, xEdge, yEdge; 

  /* 
     case of only one color for the triangle : 
  */

  if ( zxy[0] == zxy[2] ) {
    resx[0]=inint(sx[0]); resx[1]=inint(sx[1]);  resx[2]=inint(sx[2]);
    resy[0]=inint(sy[0]); resy[1]=inint(sy[1]);  resy[2]=inint(sy[2]);
    color = fill[zxy[0]]; nr = 3;
    C2F(dr)("xliness","str",resx,resy,&color,(ncont=1,&ncont),&nr, 
	    PI0,PD0,PD0,PD0,PD0,0L,0L);
    return;
  }

  /* 
     at least 2 colors for painting the triangle : it is divided in elementary
     polygons. The number of polygons is npolys = zxy[2]-zxy[0]+1.

                             P2           as zxy[0] <= zxy[1] <  zxy[2] or 
     Notations/Hints :       /\              zxy[0] <  zxy[1] <= zxy[2]
                     edge2  /  \ edge1    from a previus sort. All the polygons
                           /    \         have 2 points on edge2, the others points
                          /______\        are on edge0 and/or edge1. I name the 2 ends
                        P0        P1      points on each poly PEdge2 and Pedge, they are 
                            edge0         the 2 first points of the next poly. I start
                                          from P0 to form the first poly (a triangle or
     a 4 sides depending if zxy[0]=zxy[1]), then the 2, 3, .., npolys - 1 (if they exist)
     and finally the last one which comprise the P2 vertex.  In some special cases
     we can have a degenerate poly but it doesn't matter ! 				  
  */
  
  nb0 = zxy[1]-zxy[0]; /* number of intersection points on edge 0 */

  /*----------------------------+
  |   compute the first poly    |
  +----------------------------*/
  
  resx[0]=inint(sx[0]); resy[0]=inint(sy[0]); nr = 1; edge = 0;
  if ( nb0 == 0 ) {    /* the intersection point is on Edge1 but the next point
                          of the poly is P1 */
    resx[1]=inint(sx[1]); resy[1]=inint(sy[1]); nr++;
    edge = 1;          /* the next intersection points will be on edge1 */
  } else nb0--;
  /* the intersection point on edge (0 or 1) : */
  FindIntersection(sx, sy, fxy, zlevel[zxy[0]], edge, edge+1, &xEdge, &yEdge);
  resx[nr]=xEdge; resy[nr]=yEdge; nr++;
  /* the last point of the first poly (edge 2) : */
  FindIntersection(sx, sy, fxy, zlevel[zxy[0]], 0, 2, &xEdge2, &yEdge2);
  resx[nr]=xEdge2; resy[nr]=yEdge2; nr++;
  color = fill[zxy[0]];
  C2F(dr)("xliness","str",resx,resy,&color,(ncont=1,&ncont),&nr, 
	  PI0,PD0,PD0,PD0,PD0,0L,0L);

  /*------------------------------------+ 
  | compute the intermediary polygon(s) |
  +------------------------------------*/

  for ( izone = zxy[0]+1 ; izone < zxy[2] ; izone++ ) {
    resx[0] = xEdge2; resy[0] = yEdge2;          /* the 2 first points are known */
    resx[1] = xEdge;  resy[1] = yEdge; nr = 2;
    if ( edge == 0 ) {  /* the intersection point is perhaps on edge 0 */
      if (nb0 == 0 ) {  /* no it is on edge 1 but the next point of the poly is P1 */
	resx[2]=inint(sx[1]); resy[2]=inint(sy[1]); nr++;
	edge = 1;          /* the next intersection points will be on edge1 */
      } else nb0--;
    };
    /* the intersection point on edge (0 or 1) : */
    FindIntersection(sx, sy, fxy, zlevel[izone], edge, edge+1, &xEdge, &yEdge);
    resx[nr]=xEdge; resy[nr]=yEdge; nr++;
    /* the last point of the first poly (edge 2) : */
    FindIntersection(sx, sy, fxy, zlevel[izone], 0, 2, &xEdge2, &yEdge2);
    resx[nr]=xEdge2; resy[nr]=yEdge2; nr++;
    color = fill[izone];
    C2F(dr)("xliness","str",resx,resy,&color,(ncont=1,&ncont),&nr, 
	    PI0,PD0,PD0,PD0,PD0,0L,0L);
  };

  /*-----------------------+ 
  | compute the last poly  |
  +-----------------------*/

  resx[0] = xEdge2; resy[0] = yEdge2;         /* the 2 first points are known */
  resx[1] = xEdge;  resy[1] = yEdge; nr = 2;
  if ( edge == 0 ) {                          /* the next point of the poly is P1 */
    resx[2]=inint(sx[1]); resy[2]=inint(sy[1]); nr++;
  };
  /* the last point is P2 */
  resx[nr] = inint(sx[2]); resy[nr] = inint(sy[2]); nr++;
  color = fill[zxy[2]];
  C2F(dr)("xliness","str",resx,resy,&color,(ncont=1,&ncont),&nr, 
	  PI0,PD0,PD0,PD0,PD0,0L,0L);
}

static void FindIntersection(double *sx, double *sy, double *fxy, double z, int inda, int indb, integer *xint, integer *yint)
{
  double alpha;
  alpha = (z - fxy[inda])/(fxy[indb] - fxy[inda]);
  *xint = inint((1 - alpha)*sx[inda] + alpha*sx[indb]);
  *yint = inint((1 - alpha)*sy[inda] + alpha*sy[indb]);
}









