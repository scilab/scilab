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
extern int facet_facing_rear (integer facteur, integer * x, integer * y,
                              integer * c, integer * size, integer * xr,
                              integer * yr, integer * cr, integer * rsize);

/*-------------------------------------------------------------------------
 *   plo3dn function  ( 3D Plotting of surfaces given by z=f(x,y)  )
 *-------------------------------------------------------------------------*/

void C2F (plot3dn) (sciPointObj * pobj, double *x, double *y, double *z,
                    integer * p, integer * q, int *DPI)
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
  int xx[4]; /* used to load the object foreground and dashes color */
  integer v=0;
  double dv=0.;
  
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
  dc = pSURFACE_FEATURE (pobj)->flag[0]; /* color_mode */
  flagcolor = pSURFACE_FEATURE (pobj)->flagcolor;

  xx[0] = sciGetForeground (pobj);

  /** initialisation **/
  polyx = graphic_alloc (0, 5 * (*q), sizeof (int));
  polyy = graphic_alloc (1, 5 * (*q), sizeof (int));
  fill = graphic_alloc (2, (*q), sizeof (int));
  if ((polyx == NULL) || (polyy == NULL) || (fill == NULL))
    {
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
  switch (cache)
    {
    case 0:
      /*       printf(" case 0\n");fflush(NULL); */
      if (ppsubwin->axes.reverse[0] == TRUE)    /* x is reversed */
        {
          /*  printf(" x is reversed\n");fflush(NULL); */
          for (i = (*p) - 2; i >= 0; i--)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  /*      printf(" y is reversed\n");fflush(NULL); */
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
			C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else              /* y is NOT reversed */
                {
                  /*   printf(" y is NOT reversed\n");fflush(NULL); */
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);

                    }
                }

            }
        }
      else                      /* x is NOT reversed */
        {
          /*  printf(" x is NOT reversed\n");fflush(NULL); */
          for (i = 0; i < (*p) - 1; i++)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  /*  printf(" y is reversed\n");fflush(NULL); */
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else              /* y is NOT reversed */
                {
                  /*   printf(" y is NOT reversed\n");fflush(NULL); */
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }
		      
                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      /*    printf("\n\n");fflush(NULL); */
      break;
    case 1:
      /*   printf(" case 1\n"); */
      /*       fflush(NULL); */
      if (ppsubwin->axes.reverse[0] == TRUE)
        {
          for (i = (*p) - 2; i >= 0; i--)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else              /* y is NOT reversed */
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      else
        {
          for (i = 0; i < (*p) - 1; i++)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                 x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      break;
    case 2:
      if (ppsubwin->axes.reverse[0] == TRUE)    /* x is reversed */
        {
          for (i = 0; i < (*p) - 1; i++)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else              /* y is NOT reversed */
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      else                      /* x is NOT reversed */
        {
          for (i = (*p) - 2; i >= 0; i--)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                    x,y,ztmp,i,(*q)-2-j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i,
                                       (*q) - 2 - j, npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      break;
    case 3:
      if (ppsubwin->axes.reverse[0] == TRUE)    /* x is reversed */
        {
          for (i = 0; i < (*p) - 1; i++)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else              /* y is NOT reversed */
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
            }
        }
      else                      /* x is NOT reversed */
        {
          for (i = (*p) - 2; i >= 0; i--)
            {
              int npolyok = 0;

              if (ppsubwin->axes.reverse[1] == TRUE)    /* y is reversed */
                {
                  for (j = (*q) - 2; j >= 0; j--)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, ytmp, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
                }
              else
                {
                  for (j = 0; j < (*q) - 1; j++)
                    {
                      /*  npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax, */
                      /*                                       x,y,ztmp,i,j,npolyok,p,dc,fg1); */
                      npolyok +=
                        (Gen3DPoints) (flagcolor, polyx, polyy, fill, whiteid,
                                       zmin, zmax, xtmp, y, ztmp, i, j,
                                       npolyok, p, dc, fg1);
                    }
                  if (npolyok != 0)
                    {
                      if (sciGetIsLine (pobj)){
			C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                        C2F (dr) ("xliness", "str", polyx, polyy, fill,
                                  &npolyok, &polysize, PI0, PD0, PD0, PD0,
                                  PD0, 0L, 0L);
		      }

                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, 5 * npolyok, polyx, polyy,DPI);
                    }
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
                   double * cvect, integer * p, integer * q, int *DPI)
{
  integer polysize, npoly, whiteid, verbose = 0, narg, hiddencolor, front_size;
  integer *polyx, *polyy, *locindex, fill[4], col[4], rear_col[5];
  integer rear_x[5], rear_y[5], rear_size;
  static double zmin, zmax, *polyz;
  integer i, k, color_mode, color_flag;
  sciPointObj *psubwin = NULL;
  sciSubWindow *ppsubwin = NULL;
  int facteur = 1;
  integer rear;
  int xx[4]; /* used to load the object foreground and dashes color */
  integer v=0;
  double dv=0.;
 
  int u;
  double *xtmp = NULL;
  double *ytmp = NULL;
  double *ztmp = NULL;

  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  ppsubwin = pSUBWIN_FEATURE (psubwin);

  zmin = pSUBWIN_FEATURE (psubwin)->SRect[4];   /*zmin */
  zmax = pSUBWIN_FEATURE (psubwin)->SRect[5];   /*zmax */

  hiddencolor = pSURFACE_FEATURE (pobj)->hiddencolor;
  color_mode = pSURFACE_FEATURE (pobj)->flag[0];
  color_flag = pSURFACE_FEATURE (pobj)->flagcolor;

  xx[0] = sciGetForeground (pobj);

  polyz = graphic_alloc (5, (*q), sizeof (double));
  if ((polyz == NULL) && (*q) != 0)
    {
      Scistring ("plot3dg_ : malloc No more Place\n");
      return;
    }
  /** Allocation  **/
  polyx = graphic_alloc (0, (*p) + 1L, sizeof (int));
  polyy = graphic_alloc (1, (*p) + 1L, sizeof (int));
  locindex = graphic_alloc (2, (*q), sizeof (int));
  if ((polyx == NULL) || (polyy == NULL) || (locindex == NULL))
    {
      Scistring ("plot3dg_ : malloc No more Place\n");
      return;
    }

  xtmp = MALLOC ((*p) * (*q) * sizeof (double));
  ytmp = MALLOC ((*p) * (*q) * sizeof (double));
  ztmp = MALLOC ((*p) * (*q) * sizeof (double));

  for (u = 0; u < (*p) * (*q); u++)
    {
      xtmp[u] = x[u];
      ytmp[u] = y[u];
      ztmp[u] = z[u];
    }

  ReverseDataFor3D (psubwin, xtmp, ytmp, ztmp, (*p) * (*q));

  /** Painter's Algorithm : the facets are sorted  **/

  for (i = 0; i < *q; i++)
    {
      double zdmin1, zdmin, xmoy = 0.00, ymoy = 0.00, zmoy = 0.00;
      int j = 0;
      zdmin1 = TRZ (xtmp[(*p) * i], ytmp[(*p) * i], ztmp[(*p) * i]);
      for (j = 0; j < *p; j++)
        {
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
  for (i = 0; i < (*q); i++)
    {
      locindex[i] -= 1;
      if (locindex[i] >= *q)
        sciprint (" index[%d]=%d\r\n", i, locindex[i]);
      locindex[i] = Min (Max (0, locindex[i]), *q - 1);
    }
  polysize = (*p) + 1;
  npoly = 1;

  C2F (dr) ("xget", "lastpattern", &verbose, &whiteid, &narg, PI0, PI0, PI0,
            PD0, PD0, PD0, PD0, 0L, 0L);

  /* facteur is used below */
  if (ppsubwin->axes.reverse[0] == TRUE)
    facteur = -facteur;
  if (ppsubwin->axes.reverse[1] == TRUE)
    facteur = -facteur;
  if (ppsubwin->axes.reverse[2] == TRUE)
    facteur = -facteur;


  for (i = (*q) - 1; i >= 0; i--)
    {
      int j, nok = 0;
      for (j = 0; j < (*p); j++)
        {
          if (trans3d
              (psubwin, 1, &(polyx[j]), &(polyy[j]),
               &(xtmp[(*p) * locindex[i] + j]),
               &(ytmp[(*p) * locindex[i] + j]),
               &(ztmp[(*p) * locindex[i] + j])) == 0)
            {
              nok = 1;
              break;
            }
        }

      if (nok == 0)
        {
          polyx[(*p)] = polyx[0];
          polyy[(*p)] = polyy[0];

          /* Preparation of the color vector. In the case color_flag==1,
             the color is proportional to the Z-level. */

          front_size = *p;      /* initial size of the front facing facet */


          if (color_flag == 3)
            {
              /* interpolated shading */

              for (k = 0; k < *p; k++)
                col[k] = (int) cvect[(*p) * locindex[i] + k];

            }
          else if (color_flag == 2)
            {
              /* flat shading */

              for (k = 0; k < *p; k++)
                col[k] = (int) cvect[locindex[i]];

            }
	  else if (color_flag == 4)
            {
              /* flat shading "a la Matlab" */

	      for (k = 0; k < *p; k++)
                col[k] = (int) cvect[locindex[i]];
	      
	    }
          else if (color_flag == 1)
            {
              /* Z-level flat shading. Computing of color has been moved here for clarity. */

              double zl = 0;
              for (k = 0; k < *p; k++)
                zl += z[(*p) * locindex[i] + k];        /* F.Leray 29.11.04 : DO NOT REPLACE z by ztmp here : zmin & zmax are computed to work with z ! */
              fill[0] =
                inint ((whiteid - 1) * ((zl / (*p)) - zmin) / (zmax - zmin)) +
                1;
              for (k = 0; k < *p; k++)
                col[k] = -fill[0];
            }
          else
            {
	      /* No shading at all, fixed facecolor */

              for (k = 0; k < *p; k++)
                col[k] = 0;
            }

          /* The following call processes the case where the rear of the facet
             is facing the view point and hiddencolor is positive (the rear
             of the surface is painted with a uniform color). When hiddencolor=-1
             then the rear facets are processed for each specific value
             of color_flag (see below). */

          rear =
            facet_facing_rear (facteur, polyx, polyy, col, &front_size,
                               rear_x, rear_y, rear_col, &rear_size);

	  rear_x[4] = rear_x[0];
	  rear_y[4] = rear_y[0];
	  rear_col[4] = rear_col[0];

          if (hiddencolor > 0 && rear)
            {
              if (color_flag != 0)
                fill[0] = (color_mode < 0) ? -hiddencolor : hiddencolor;
              else
                {
                  if (color_mode == 0)
                    fill[0] = 0;
                  else if (color_mode > 0)
                    fill[0] = hiddencolor;
                  else
                    fill[0] = -hiddencolor;
                }

              if (sciGetIsLine (pobj)){
		C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
		C2F (dr) ("xliness", "str", rear_x, rear_y, fill, &npoly,
                          &rear_size, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
	      }
	      
              if (sciGetIsMark (pobj))
                DrawMarks3D (pobj, rear_size, rear_x, rear_y,DPI);

              /* If the polygon is a triangle or a "butterfly" there is nothing left to draw */

              if (rear_size == *p)
                continue;
            }

          if (color_flag == 1)
            {
              /* Flat shading. This is the case where the color index is constant for a facet */

              if (hiddencolor < 0 && rear)
                {               /* draw the rear facing part of the facet */
                  integer rear_fill = col[0];
                  if (color_mode >= 0)  /* The edge of the facet is forcibly drawn if color_mode >= 0 */
                    rear_fill = Abs (col[0]);
                  if (sciGetIsLine (pobj)){
		    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                    C2F (dr) ("xliness", "str", rear_x, rear_y, &rear_fill,
                              &npoly, &rear_size, PI0, PD0, PD0, PD0, PD0, 0L,0L);
		  }
		  
                  if (sciGetIsMark (pobj))
                    DrawMarks3D (pobj, rear_size, rear_x, rear_y,DPI);

                  if (rear_size == *p)
                    continue;
                }
              /* Draw the remaining (front) part of the facet if applicable */

              if (color_mode > 0)       /* Force the drawing of the mesh */
                col[0] = Abs (col[0]);
	      else if (color_mode < 0)
		col[0] = - Abs (col[0]);
	      else
		col[0] = 0;
              if (sciGetIsLine (pobj)){
		C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                C2F (dr) ("xliness", "str", polyx, polyy, col, &npoly,
                          &front_size, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
	      }
	      
              if (sciGetIsMark (pobj))
                DrawMarks3D (pobj, front_size, polyx, polyy,DPI);

            }


	  /* NEW */
	  
          else if ((color_flag == 2) || (color_flag == 4))
            {
              /* Flat shading. This is the case where the color index is constant for a facet */

              if (hiddencolor < 0 && rear)
                {               /* draw the rear facing part of the facet */
                  integer rear_fill = col[0];
                  if (color_mode >= 0)  /* The edge of the facet is forcibly drawn if color_mode >= 0 */
                    rear_fill = Abs (col[0]);
                  if (sciGetIsLine (pobj)){
		    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                    C2F (dr) ("xliness", "str", rear_x, rear_y, &rear_fill,
                              &npoly, &rear_size, PI0, PD0, PD0, PD0, PD0, 0L,0L);
		  }
		  
                  if (sciGetIsMark (pobj))
                    DrawMarks3D (pobj, rear_size, rear_x, rear_y,DPI);

                  if (rear_size == *p)
                    continue;
                }
              /* Draw the remaining (front) part of the facet if applicable */
	      
	      if (color_mode > 0)       /* Force the drawing of the mesh */
                col[0] = Abs (col[0]);
	      else if (color_mode < 0)
		col[0] = - Abs (col[0]);
	      else
		col[0] = 0;
	      
	      if (sciGetIsLine (pobj)){
		C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                C2F (dr) ("xliness", "str", polyx, polyy, col, &npoly,
                          &front_size, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
	      }
	      
              if (sciGetIsMark (pobj))
                DrawMarks3D (pobj, front_size, polyx, polyy,DPI);

            }



	  /* END NEW */








          else if (color_flag == 3)
            {
              /* Interpolated shading */

              if ((*p) != 3 && (*p) != 4)
                {
                  Scistring
                    ("plot3d1 : interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
                  return;
                }
              else
                {

                  if (hiddencolor < 0 && rear)
                    {           /* draw the rear facing part of the facet */

                      if (sciGetIsLine (pobj))
                        {

                          /* The edge of the facet is forcibly drawn if color_mode >= 0 */

                          if (color_mode >= 0){	
			    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
			    shade (rear_x, rear_y, rear_col, rear_size, 1);
			  }
                          else{
			    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
			    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
			    shade (rear_x, rear_y, rear_col, rear_size, -1);
			  }
                        }
                      if (sciGetIsMark (pobj))
                        DrawMarks3D (pobj, rear_size, rear_x, rear_y,DPI);
                      if (rear_size == *p)
                        continue;
                    }
                  /* Draw the remaining (front) part of the facet if applicable */

                  if (sciGetIsLine (pobj)){
		    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
		    shade (polyx, polyy, col, front_size, color_mode);
		  }
                  if (sciGetIsMark (pobj))
                    DrawMarks3D (pobj, front_size, polyx, polyy,DPI);
                }
            }
          else
            {
              /* color_flag == 0, the facet is drawn with default facecolor and edgecolor (foreground) */

              fill[0] = color_mode;

              if (hiddencolor < 0 && rear)
                {               /* draw the rear facing part of the facet */
                  if (sciGetIsLine (pobj)){
		    C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		    C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                    C2F (dr) ("xliness", "str", rear_x, rear_y, fill, &npoly,
                              &rear_size, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
		  }
		  
                  if (sciGetIsMark (pobj))
                    DrawMarks3D (pobj, rear_size, rear_x, rear_y,DPI);
                  if (rear_size == *p)
                    continue;
                }
              if (sciGetIsLine (pobj)){
		C2F (dr) ("xset", "dashes",     xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 6L);
		C2F (dr) ("xset", "foreground", xx,   xx,   xx+4, xx+4, xx+4, &v, &dv, &dv, &dv, &dv, 5L, 10L);
                C2F (dr) ("xliness", "str", polyx, polyy, fill, &npoly,
                          &front_size, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
	      }
	      
              if (sciGetIsMark (pobj))
                DrawMarks3D (pobj, front_size, polyx, polyy,DPI);

            }
        }
    }

  FREE (xtmp);
  xtmp = NULL;
  FREE (ytmp);
  ytmp = NULL;
  FREE (ztmp);
  ztmp = NULL;

}


int
facet_facing_rear (integer facteur, integer * x, integer * y, integer * c,
                   integer * size, integer * xr, integer * yr, integer * cr,
                   integer * rsize)
{
  double determ, determ0, determ1, determ2, determ3,
    t_param, x1mx0, x2mx1, x3mx2, x0mx3, y1my0, y2my1, y3my2, y0my3;
  integer i, p, swp, flag_det, flag_det0, flag_det1, flag_det2, flag_det3;

  p = (*size);

  for (i = 0; i < p; i++)
    {
      xr[i] = x[i]; yr[i] = y[i]; cr[i] = c[i];
    }

  (*rsize) = p;

  x1mx0 = x[1] - x[0]; y1my0 = y[1] - y[0];
  x2mx1 = x[2] - x[1]; y2my1 = y[2] - y[1];

  determ1 = (y2my1 * x1mx0 - x2mx1 * y1my0) * facteur;
  flag_det1 = (determ1 < 0);
  if (p > 3)
    {                           /* further tests for the quadrilateral case */
      x3mx2 = x[3] - x[2];
      y3my2 = y[3] - y[2];
      x0mx3 = x[0] - x[3];
      y0my3 = y[0] - y[3];
      determ0 = (x0mx3 * y1my0 - y0my3 * x1mx0) * facteur;
      determ2 = (x2mx1 * y3my2 - y2my1 * x3mx2) * facteur;
      determ3 = (x3mx2 * y0my3 - y3my2 * x0mx3) * facteur;
      flag_det0 = (determ0 < 0);
      flag_det2 = (determ2 < 0);
      flag_det3 = (determ3 < 0);
    }
  else
    return (flag_det1);

  /* flag_det allows to precisely detect the orientation of the facet
     for the case of the quadrilateral. If flag_det==2, then the
     quadrilateral is projected as a butterfly, and both the rear and
     the top of the facet are facing the vewpoint. This needs a different
     processing which has to be implemented */

  flag_det = flag_det0 + flag_det1 + flag_det2 + flag_det3;

  if (flag_det == 0)
    return 0;
  else if (flag_det == 4)
    return 1;
  else if (flag_det == 2)
    {
      /* Butterfly case */

      (*rsize) = 3;
      (*size) = 3;

      if ((flag_det2 && flag_det3) || (flag_det0 && flag_det1))
        {
          /* the (1,2) and (0,3) segments are crossing */

          determ = (x2mx1 * y0my3 - y2my1 * x0mx3) * facteur;
          t_param = determ0 / determ;

          /* Computation of the intersection point (i) */

          xr[0] = linint (x[1] + t_param * x2mx1);
          yr[0] = linint (y[1] + t_param * y2my1);
          cr[0] = linint (c[1] + t_param * (c[2] - c[1]));

          if (flag_det2)
            {
              /* This is the case where the triangle (0,1,i) is front and (i,2,3) is rear */

              xr[1] = x[3];yr[1] = y[3];cr[1] = c[3];
              x[2] = xr[0];y[2] = yr[0];c[2] = cr[0];
            }
          else
            {
              /* This is the case where the triangle (0,1,i) is rear and (i,2,3) is front */

              xr[2] = x[0];yr[2] = y[0];cr[2] = c[0];
              x[0] = xr[0];y[0] = yr[0];c[0] = cr[0];
              x[1] = x[3];y[1] = y[3];c[1] = c[3];
            }
        }
      else
        {
          /* the (0,1) and (2,3) segments are crossing */

          determ = (x1mx0 * y3my2 - y1my0 * x3mx2) * facteur;
          t_param = determ3 / determ;

          /* Computation of the intersection point (i) */

          xr[1] = linint (x[0] + t_param * x1mx0);
          yr[1] = linint (y[0] + t_param * y1my0);
          cr[1] = linint (c[0] + t_param * (c[1] - c[0]));
          if (flag_det0)
            {
              /* This is the case where the triangle (0,i,3) is rear and (i,1,2) is front */

              xr[2] = x[3];yr[2] = y[3];cr[2] = c[3];
              x[0] = xr[1];y[0] = yr[1];c[0] = cr[1];
            }
          else
            {
              /* This is the case where the triangle (0,i,3) is front and (i,1,2) is back */

              xr[0] = x[1];yr[0] = y[1];cr[0] = c[1];
              x[1] = xr[1];y[1] = yr[1];c[1] = cr[1];
              x[2] = x[3];y[2] = y[3];c[2] = c[3];
            }
        }
      return 1;
    }
  else
    {
      if (flag_det0 == flag_det2)
        {
          /* If the quadrilateral is not convex, then the vertices must be
             correctly numbered, if interpolated shading is to be used, because
             a (0,1,2,3) quadrilateral is splitted in triangles (0,1,2) and (0,2,3). */

          swp = x[0];x[0] = x[1];x[1] = x[2];x[2] = x[3];x[3] = swp;
          swp = y[0];y[0] = y[1];y[1] = y[2];y[2] = y[3];y[3] = swp;
          swp = c[0];c[0] = c[1];c[1] = c[2];c[2] = c[3];c[3] = swp;
          swp = xr[0];xr[0] = xr[1];xr[1] = xr[2];xr[2] = xr[3];xr[3] = swp;
          swp = yr[0];yr[0] = yr[1];yr[1] = yr[2];yr[2] = yr[3];yr[3] = swp;
          swp = cr[0];cr[0] = cr[1];cr[1] = cr[2];cr[2] = cr[3];cr[3] = swp;
        }
      if (flag_det == 3)
        return 1;
      else
        return 0;
    }
}
