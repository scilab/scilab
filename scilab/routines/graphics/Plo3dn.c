/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 2003-2004 Inria/Djalel Abdemouche
 --------------------------------------------------------------------------*/

#include <string.h>
#include <math.h>
#include <stdio.h>
#include "Math.h"
#include "PloEch.h"

#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "BuildObjects.h"


extern double C2F (dsort) ();
/*-------------------------------------------------------------------------
 *   plo3dn function  ( 3D Plotting of surfaces given by z=f(x,y)  )
 *-------------------------------------------------------------------------*/

void C2F (plot3dn) (sciPointObj * pobj, double *x, double *y, double *z,
                    integer * p, integer * q)
{
  static integer fg1, dc;
  /*   int jj, kk; */
  integer polysize, whiteid, verbose = 0, narg;
  integer *polyx, *polyy, *fill;
  static integer cache;
  static double zmin, zmax;
  integer i, j, flagcolor;
  sciPointObj *psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); /* a laisser!!!!!!!! */
  sciSubWindow *ppsubwin = pSUBWIN_FEATURE (psubwin);

  /* Test to enable reverse axis in 3D *//* F.Leray 14.10.04 */
  int u;
  double *xtmp = NULL;
  double *ytmp = NULL;
  double *ztmp = NULL;

  xtmp = MALLOC ((*p) * sizeof (double));
  ytmp = MALLOC ((*q) * sizeof (double));
  ztmp = MALLOC ((*p) * (*q) * sizeof (double));

  for (u = 0; u < (*p); u++)
    xtmp[u] = x[u];
  for (u = 0; u < (*q); u++)
    ytmp[u] = y[u];
  for (u = 0; u < (*p) * (*q); u++)
    ztmp[u] = z[u];

  ReverseDataFor3DXonly (psubwin, xtmp, (*p));
  ReverseDataFor3DYonly (psubwin, ytmp, (*q));
  ReverseDataFor3DZonly (psubwin, ztmp, (*p) * (*q));

  zmin = pSUBWIN_FEATURE (psubwin)->SRect[4];   /*zmin */
  zmax = pSUBWIN_FEATURE (psubwin)->SRect[5];   /*zmax */
  cache = pSUBWIN_FEATURE (psubwin)->hiddenstate;

  fg1 = pSURFACE_FEATURE (pobj)->hiddencolor;
  dc = pSURFACE_FEATURE (pobj)->flag[0];
  flagcolor = pSURFACE_FEATURE (pobj)->flagcolor;

  /** initialisation **/
  polyx = graphic_alloc (0, 5 * (*q), sizeof (int));
  polyy = graphic_alloc (1, 5 * (*q), sizeof (int));
  fill = graphic_alloc (2, (*q), sizeof (int));
  if ((polyx == NULL) || (polyy == NULL) || (fill == NULL)) {
    Scistring ("plot3dg_ : malloc No more Place\n");
    return;
  }


  C2F (dr) ("xget", "lastpattern", &verbose, &whiteid, &narg, PI0, PI0, PI0,
            PD0, PD0, PD0, PD0, 0L, 0L);
  /*  if (fg1<=0) fg1=dc; */
  for (i = 0; i < (*q) - 1; i++)
    fill[i] = dc;
  polysize = 5;

  /** The 3d plot **/
  /** Choix de l'ordre de parcourt **/
  switch (cache) {
  case 0:
    /*       printf(" case 0\n");fflush(NULL); */
    if (ppsubwin->axes.reverse[0] == TRUE) {    /* x is reversed */
      /*  printf(" x is reversed\n");fflush(NULL); */
      for (i = (*p) - 2; i >= 0; i--) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          /*      printf(" y is reversed\n");fflush(NULL); */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0) {
            /*  int ii; */
            /*          for(ii=0;ii<100000000;ii++); */

            /*  printf("ICI\n");fflush(NULL); */
            C2F (dr) ("xliness", "str", polyx, polyy, fill,
                      &npolyok, &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
          }
        }
        else {                  /* y is NOT reversed */

          /*   printf(" y is NOT reversed\n");fflush(NULL); */
          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0) {
            /*  int ii; */
            /*          for(ii=0;ii<100000000;ii++); */

            /*  printf("ICI\n");fflush(NULL); */
            C2F (dr) ("xliness", "str", polyx, polyy, fill,
                      &npolyok, &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
          }
        }

      }
    }
    else {                      /* x is NOT reversed */

      /*  printf(" x is NOT reversed\n");fflush(NULL); */
      for (i = 0; i < (*p) - 1; i++) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          /*  printf(" y is reversed\n");fflush(NULL); */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0) {
            C2F (dr) ("xliness", "str", polyx, polyy, fill,
                      &npolyok, &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
          }
        }
        else {                  /* y is NOT reversed */

          /*   printf(" y is NOT reversed\n");fflush(NULL); */
          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0) {
            C2F (dr) ("xliness", "str", polyx, polyy, fill,
                      &npolyok, &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
          }
        }
      }
    }
    /*    printf("\n\n");fflush(NULL); */
    break;
  case 1:
    /*   printf(" case 1\n"); */
    /*       fflush(NULL); */
    if (ppsubwin->axes.reverse[0] == TRUE) {
      for (i = (*p) - 2; i >= 0; i--) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {                  /* y is NOT reversed */

          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    else {
      for (i = 0; i < (*p) - 1; i++) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {
          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    break;
  case 2:
    if (ppsubwin->axes.reverse[0] == TRUE) {    /* x is reversed */
      for (i = 0; i < (*p) - 1; i++) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {                  /* y is NOT reversed */

          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    else {                      /* x is NOT reversed */

      for (i = (*p) - 2; i >= 0; i--) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {
          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i,
                             (*q) - 2 - j, npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    break;
  case 3:
    if (ppsubwin->axes.reverse[0] == TRUE) {    /* x is reversed */
      for (i = 0; i < (*p) - 1; i++) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {                  /* y is NOT reversed */

          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    else {                      /* x is NOT reversed */

      for (i = (*p) - 2; i >= 0; i--) {
        int npolyok = 0;

        if (ppsubwin->axes.reverse[1] == TRUE) {        /* y is reversed */
          for (j = (*q) - 2; j >= 0; j--) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, ytmp, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
        else {
          for (j = 0; j < (*q) - 1; j++) {
            /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
            /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
            npolyok +=
              (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                             zmin, zmax, xtmp, y, ztmp, i, j,
                             npolyok, p, dc, fg1);
          }
          if (npolyok != 0)
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npolyok,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
    }
    break;
  }

  FREE (ztmp);
  ztmp = NULL;
  FREE (xtmp);
  xtmp = NULL;
  FREE (ytmp);
  ytmp = NULL;
}

void C2F (fac3dn) (sciPointObj * pobj, double *x, double *y, double *z,
                   integer * cvect, integer * p, integer * q)
{
  integer polysize, npoly, whiteid, verbose = 0, narg, fg1;
  integer *polyx, *polyy, *locindex, fill[4];
  static double zmin, zmax, *polyz;
  integer i, flag, iflag;
  sciPointObj *psubwin = NULL;
  sciSubWindow *ppsubwin = NULL;
  int facteur = 1;
  integer flag_det=0,flag_det0=0,flag_det1=0,flag_det2=0,flag_det3=0;

  int u;
  double *xtmp = NULL;
  double *ytmp = NULL;
  double *ztmp = NULL;

  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  zmin = pSUBWIN_FEATURE (psubwin)->SRect[4];   /*zmin */
  zmax = pSUBWIN_FEATURE (psubwin)->SRect[5];   /*zmax */

  fg1 = pSURFACE_FEATURE (pobj)->hiddencolor;
  flag = pSURFACE_FEATURE (pobj)->flag[0];
  iflag = pSURFACE_FEATURE (pobj)->flagcolor;

  polyz = graphic_alloc (5, (*q), sizeof (double));
  if ((polyz == NULL) && (*q) != 0) {
    Scistring ("plot3dg_ : malloc No more Place\n");
    return;
  }
/** Allocation  **/
  polyx = graphic_alloc (0, (*p) + 1L, sizeof (int));
  polyy = graphic_alloc (1, (*p) + 1L, sizeof (int));
  locindex = graphic_alloc (2, (*q), sizeof (int));
  if ((polyx == NULL) || (polyy == NULL) || (locindex == NULL)) {
    Scistring ("plot3dg_ : malloc No more Place\n");
    return;
  }

  xtmp = MALLOC ((*p) * (*q) * sizeof (double));
  ytmp = MALLOC ((*p) * (*q) * sizeof (double));
  ztmp = MALLOC ((*p) * (*q) * sizeof (double));

  for (u = 0; u < (*p) * (*q); u++) {
    xtmp[u] = x[u];
    ytmp[u] = y[u];
    ztmp[u] = z[u];
  }

  ReverseDataFor3D (psubwin, xtmp, ytmp, ztmp, (*p) * (*q));

/** le tri des facettes **/
  for (i = 0; i < *q; i++) {
    double zdmin1, zdmin, xmoy = 0.00, ymoy = 0.00, zmoy = 0.00;
    int j = 0;
    zdmin1 = TRZ (xtmp[(*p) * i], ytmp[(*p) * i], ztmp[(*p) * i]);
    for (j = 0; j < *p; j++) {
      xmoy += xtmp[j + (*p) * i];
      ymoy += ytmp[j + (*p) * i];
      zmoy += ztmp[j + (*p) * i];
      zdmin =
        TRZ (xtmp[j + (*p) * i], ytmp[j + (*p) * i], ztmp[j + (*p) * i]);
      if (zdmin1 < zdmin)
        zdmin1 = zdmin;
    }
    polyz[i] = TRZ (xmoy, ymoy, zmoy);
  }
  C2F (dsort) (polyz, q, locindex);
  for (i = 0; i < (*q); i++) {
    locindex[i] -= 1;
    if (locindex[i] >= *q)
      sciprint (" index[%d]=%d\r\n", i, locindex[i]);
    locindex[i] = Min (Max (0, locindex[i]), *q - 1);
  }
  polysize = (*p) + 1;
  npoly = 1;

  C2F (dr) ("xget", "lastpattern", &verbose, &whiteid, &narg, PI0, PI0, PI0,
            PD0, PD0, PD0, PD0, 0L, 0L);
  fill[0] = flag;
/*if (fg1<=0) fg1= flag;*/

/* facteur is used below */
  if (ppsubwin->axes.reverse[0] == TRUE)
    facteur = -facteur;
  if (ppsubwin->axes.reverse[1] == TRUE)
    facteur = -facteur;
  if (ppsubwin->axes.reverse[2] == TRUE)
    facteur = -facteur;


  for (i = (*q) - 1; i >= 0; i--) {
    int j, nok = 0;
    for (j = 0; j < (*p); j++) {
      if (trans3d
          (psubwin, 1, &(polyx[j]), &(polyy[j]),
           &(xtmp[(*p) * locindex[i] + j]),
           &(ytmp[(*p) * locindex[i] + j]),
           &(ztmp[(*p) * locindex[i] + j])) == 0) {
        nok = 1;
        break;
      }
    }
    if (nok == 0) {
      polyx[(*p)] = polyx[0];
      polyy[(*p)] = polyy[0];
      fill[0] = flag;

      if ((fg1 >= 0) && (*p > 2)) { /* Detection of facet orientation,    */
        double determ;                /* improved by Mottelet 25/01/2005    */ 
        determ =                     /* to take in account quadrilaterals. */
          ((polyx[1] - polyx[0]) * (polyy[2] - polyy[0]) -
           (polyy[1] - polyy[0]) * (polyx[2] - polyx[0]))*facteur;
        flag_det1 = (determ < 0); /* First test for a triangle */
        if (*p > 3) { /* Second test for the quadrilateral case */
          determ =
            ((polyx[2] - polyx[0]) * (polyy[3] - polyy[0]) -
             (polyy[2] - polyy[0]) * (polyx[3] - polyx[0]))*facteur;
          flag_det3 = (determ < 0);
          determ =
            ((polyx[2] - polyx[1]) * (polyy[3] - polyy[1]) -
             (polyy[2] - polyy[1]) * (polyx[3] - polyx[1]))*facteur;
          flag_det2 = (determ < 0);
          determ =
            ((polyx[3] - polyx[1]) * (polyy[0] - polyy[1]) -
             (polyy[3] - polyy[1]) * (polyx[0] - polyx[1]))*facteur;
          flag_det0 = (determ < 0);
        }
/*        printf("%d %d %d %d\n",flag_det0,flag_det1,flag_det2,flag_det3); */
	  }
	  flag_det=flag_det0+flag_det1+flag_det2+flag_det3;
      if (flag_det > 3 || (flag_det==1 && (*p)==3)) {
        if (iflag != 0)
          fill[0] = (flag < 0) ? -fg1 : fg1;
        else
          fill[0] = (flag != 0) ? fg1 : flag; /*if (flag==0) fill[0]=0;*/
        if (fg1 > 0) {
          C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly,
                    &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
        }
      }
      else if (iflag == 1) {
        double zl = 0;
        int k;
        for (k = 0; k < *p; k++)
          zl += z[(*p) * locindex[i] + k];      /* F.Leray 29.11.04 : DO NOT REPLACE z by ztmp here : zmin & zmax are computed to work with z ! */

        fill[0] =
          inint ((whiteid - 1) * ((zl / (*p)) - zmin) / (zmax - zmin)) + 1;
        if (flag < 0)
          fill[0] = -fill[0];
        C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly,
                  &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      }
      else if (iflag == 2) {
        fill[0] = cvect[locindex[i]];
        if (flag < 0)
          fill[0] = -fill[0];
        C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly,
                  &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      }
      else if (iflag == 3) {
        int k;
        ;
        if ((*p) != 3 && (*p) != 4) {
          Scistring
            ("plot3d1 : interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
          return;
        }
        else {
          for (k = 0; k < *p; k++)
            fill[k] = cvect[(*p) * locindex[i] + k];
          shade (polyx, polyy, fill, *p, flag);
/** draw if requested but just check on the first color **/
          if (cvect[(*p) * locindex[i]] <= 0) {
            fill[0] = 0;
            C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly,
                      &polysize, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
          }
        }
      }
      else
        C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly, &polysize,
                  PI0, PD0, PD0, PD0, PD0, 0L, 0L);

    }
  }

  FREE (xtmp);
  xtmp = NULL;
  FREE (ytmp);
  ytmp = NULL;
  FREE (ztmp);
  ztmp = NULL;
}
