
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 --------------------------------------------------------------------------*/

#include "math_graphics.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "Format.h"
#include "Contour.h"

#include "localization.h"

typedef void (level_f)(int ival, double Cont, double xncont, double yncont);
typedef void (*ptr_level_f)(int ival, double Cont, double xncont, double yncont);


static int contourI (ptr_level_f, double *, double *, double *, double *, int *, int *, int *);

static void
look(ptr_level_f, int i, int j, int ib, int jb, int qq, double Cont, int style);

static int ffnd (ptr_level_f, int, int, int, int, int,
                 int, int, int, int, int,
                 double, int *);

static int Gcont_size = 0;

static level_f GContStore2;
static void GContStore2Last(void);
static double x_cont(int i);
static double y_cont (int i);
static double phi_cont (int, int);
static double f_intercept  (double, double, double, double, double);
static int not_same_sign  (double val1, double val2);
static int get_itg_cont  (int i, int j);
static void inc_itg_cont  (int i, int j, int val);
static int oddp  (int i);

/*-----------------------------------------------------------------------
 *  Level curves
 *  The computer journal vol 15 nul 4 p 382 (1972)
 *  from the Lisp Macsyma source (M.I.T)
 * -------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * General functions (could be changed in #define or
 *   inline functions to increase speed)
 *---------------------------------------------------------------------------*/

static double *GX, *GY, *GZ;
static int Gn1, Gn2;

static double* Gxcont = NULL;
static double* Gycont = NULL;
static int currentContSize = 0;

static void InitValues(double *x, double *y, double *z, int n1, int n2)
{
    Gn1 = n1;
    Gn2 = n2;
    GX = x;
    GY = y;
    GZ = z;
}

/*--------return the  value of f for a point on the grid-----*/

static double phi_cont(int i, int j)
{
    return (GZ[i + Gn1 * j]);
}

/*---------return the coordinates between  [xi,xj] along one axis
 *  for which the value of f is zCont */

static double f_intercept(double zCont, double fi, double xi, double fj, double xj)
{
    return (xi + (zCont - fi) * (xj - xi) / (fj - fi));
}

/* check for boundary points */

static int  bdyp(int i, int j)
{
    return ( j == 0 || i == 0 || j == Gn2 - 1 || i == Gn1 - 1);
}

/* store or get flag values */

static  int *itg_cont, *xbd_cont, *ybd_cont;

static int get_itg_cont(int i, int j)
{
    return (itg_cont[i + Gn1 * j]);
}

static void inc_itg_cont(int i, int j, int val)
{
    itg_cont[i + Gn1 * j] += val;
}

static int not_same_sign(double val1, double val2)
{
    if (ISNAN(val1) == 1 || ISNAN(val2) == 1)
    {
        return (0);
    }
    /** 0.0 est consid\'er\'e comme positif **/
    if (val1 >= 0.0)
    {
        if (val2 < 0.0)
        {
            return (1) ;
        }
        else
        {
            return (0);
        }
    }
    else
    {
        if (val2 >= 0.0)
        {
            return (1) ;
        }
        else
        {
            return (0);
        }
    }
}

static int oddp(int i)
{
    return (i == 1 || i == 3);
}

/*---------return the x-value of a grid point--------*/

static double x_cont(int i)
{
    return GX[i] ;
}

/*---------return the y-value of a grid point --------*/

static double y_cont(int i)
{
    return GY[i] ;
}


static char   ContNumFormat[100];

/*--------------------------------------------------------------------
*  the level curve is crossing the segment (i,j) (ib,jb)
*  look store the level curve point and try to find the next segment to look at
*  Cont: value of f along the contour
*  ncont: number of contour
*  c: indice of the contour Cont
*---------------------------------------------------------------------*/

static void look(ptr_level_f func, int i, int j, int ib, int jb, int qq, double Cont, int style)
{
    int ip = 0, jp = 0, im = 0, jm = 0, zds = 0, ent = 0, flag = 0, wflag = 0;
    jp = j + 1;
    ip = i + 1;
    jm = j - 1;
    im = i - 1;
    /*  on regarde comment est le segment de depart */
    if  (jb == jm)
    {
        flag = 1;
    }
    else
    {
        if (ib == im)
        {
            flag = 2;
        }
        else
        {
            if (jb == jp)
            {
                flag = 3;
            }
            else  if (ib == ip)
            {
                flag = 4;
            }
        }
    }
    switch  ( flag)
    {
        case  1 :
            if  (get_itg_cont(i, jm) > 1)
            {
                return;
            }
            ent = 1 ; /* le segment est vertical vers le bas */
            /* Storing intersection point */
            (*func)(0, Cont, x_cont(i),
                    f_intercept(Cont, phi_cont(i, jm),
                                y_cont(jm), phi_cont(i, j), y_cont(j)));
            break;
        case 2 :
            if  (get_itg_cont(im, j) == 1 || get_itg_cont(im, j) == 3)
            {
                return;
            }
            ent = 2 ; /* le segment est horizontal gauche */
            /* Storing intersection point */
            (*func)(0, Cont,
                    f_intercept(Cont, phi_cont(im, j),
                                x_cont(im), phi_cont(i, j), x_cont(i)), y_cont(j));
            break ;
        case 3 :
            if  (get_itg_cont(i, j) > 1)
            {
                return;
            }
            ent = 3 ; /* le segment est vertical haut */
            /* Storing intersection point */
            (*func)(0, Cont, x_cont(i), f_intercept(Cont, phi_cont(i, j),
                                                    y_cont(j), phi_cont(i, jp), y_cont(jp)));
            break;
        case 4 :
            if  (get_itg_cont(i, j) == 1 || get_itg_cont(i, j) == 3)
            {
                return;
            }
            ent = 4 ; /* le segment est horizontal droit */
            /* Storing intersection point */
            (*func)(0, Cont, f_intercept(Cont, phi_cont(i, j),
                                         x_cont(i), phi_cont(ip, j), x_cont(ip)),
                    y_cont(j));
            break;
        default :
            break;
    }
    wflag = 1;
    while (wflag)
    {
        jp = j + 1;
        ip = i + 1;
        jm = j - 1;
        im = i - 1;
        switch  (ent)
        {
            case 1 :
                inc_itg_cont(i, jm, 2L);
                ent = ffnd(func, i, ip, ip, i, j, j, jm, jm, ent, qq, Cont, &zds);
                /* on calcule le nouveau point, ent donne la
                direction du segment a explorer */
                switch (ent)
                {
                    case -1:
                        wflag = 0;
                        break;
                    case 1 :
                        i = ip ;
                        break;
                    case 2 :
                        i = ip;
                        j = jm;
                        break;
                }
                break;
            case 2  :
                inc_itg_cont(im, j, 1L);
                ent = ffnd(func, i, i, im, im, j, jm, jm, j, ent, qq, Cont, &zds);
                switch (ent)
                {
                    case -1:
                        wflag = 0;
                        break;
                    case 2 :
                        j = jm ;
                        break;
                    case  3  :
                        i = im;
                        j = jm;
                        break;
                }
                break;
            case 3 :
                inc_itg_cont(i, j, 2L);
                ent = ffnd(func, i, im, im, i, j, j, jp, jp, ent, qq, Cont, &zds);
                switch (ent)
                {
                    case -1:
                        wflag = 0;
                        break;
                    case 3 :
                        i = im;
                        break;
                    case 4 :
                        i = im;
                        j = jp;
                        break;
                }
                break;
            case 4 :
                inc_itg_cont(i, j, 1L);
                ent = ffnd(func, i, i, ip, ip, j, jp, jp, j, ent, qq, Cont, &zds);
                switch (ent)
                {
                    case -1:
                        wflag = 0;
                        break;
                    case 4 :
                        j = jp;
                        break;
                    case 1 :
                        i = ip;
                        j = jp;
                        break;
                }
                break;
        }

        /** new segment is on the boundary **/
        if (zds == 1)
        {
            switch (ent)
            {
                case 1 :
                    inc_itg_cont(i, (j - 1), 2L);
                    break ;
                case 2 :
                    inc_itg_cont(i - 1, j, 1L);
                    break ;
                case 3 :
                    inc_itg_cont(i, j, 2L);
                    break ;
                case 4 :
                    inc_itg_cont(i, j, 1L);
                    break ;
            }
            /** we must quit the while loop **/
            wflag = 0;
        }
        /**  init point was inside the domain */
        if (qq == 2)
        {
            switch (ent)
            {
                case 1 :
                    if  (get_itg_cont (i, j - 1)  > 1)
                    {
                        wflag = 0 ;
                    }
                    break ;
                case 2 :
                    if  (oddp(get_itg_cont(i - 1, j)))
                    {
                        wflag = 0 ;
                    }
                    break ;
                case 3 :
                    if  (get_itg_cont(i, j) > 1)
                    {
                        wflag = 0 ;
                    }
                    break ;
                case 4 :
                    if  (oddp(get_itg_cont(i, j)))
                    {
                        wflag = 0 ;
                    }
                    break ;
            }
        }
    }
    if (func == GContStore2)
    {
        GContStore2Last();
    }
    else
    {
        /* contour2di only computes level curves, not display them. */
        sciprint(_("%s is only made to compute level curves and not display them.\n"), "Contourdi");
    }
}


/*-------------------------------------------------------
*  The function f is given on a grid and we want the level curves
*  for the zCont[N[2]] values
*  x : of size N[0] gives the x-values of the grid
*  y : of size N[1] gives the y-values of the grid
*  z : of size N[0]*N[1]  gives the f-values on the grid
*  style: size ncont (=N[2]) or empty int pointer
*  gives the dash style for contour i
*-------------------------------------------------------*/

static int contourI(ptr_level_f func, double *x, double *y, double *z, double *zCont, int *N, int *style, int *err)
{
    int check = 1;
    char *F = NULL;
    int n1 = 0, n2 = 0, ncont = 0, i = 0, c = 0, j = 0, k = 0, n5 = 0;
    int stylec = 0;
    n1 = N[0];
    n2 = N[1];
    ncont = N[2];
    F = getFPF();
    if (F[0] == '\0')
    {
        ChoixFormatE1(ContNumFormat, zCont, N[2]);
    }
    InitValues(x, y, z, n1, n2);
    n5 =  2 * (n1) + 2 * (n2) - 3;
    /* Allocation */
    Gcont_size = 0; /** initialize the array indices for storing contours **/
    xbd_cont = (int*)MALLOC(n5 * sizeof(int));
    ybd_cont = (int*)MALLOC(n5 * sizeof(int));
    itg_cont = (int*)MALLOC(n1 * n2 * sizeof(int));
    if ((xbd_cont == NULL) && n5 != 0)
    {
        check = 0;
    }
    if ((ybd_cont == NULL) && n5 != 0)
    {
        check = 0;
    }
    if ((itg_cont == NULL) && n1 * n2 != 0)
    {
        check = 0;
    }
    if (check == 0)
    {
        FREE(xbd_cont);
        FREE(ybd_cont);
        FREE(itg_cont);
        Scierror(999, _("%s: No more memory.\n"), "contourI");
        return -1;
    }
    /* just a parametrization of the boundary points */
    for (i = 0 ; i <  n2 ; i++)
    {
        ybd_cont[i] = i;
        xbd_cont[i] = 0;
    }
    for (i = 1 ; i <  n1 ; i++)
    {
        ybd_cont[n2 + i - 1] = n2 - 1;
        xbd_cont[n2 + i - 1] = i ;
    }
    for (i = n2 - 2;  i >= 0  ; i--)
    {
        ybd_cont[2 * n2 + n1 - 3 - i] = i;
        xbd_cont[2 * n2 + n1 - 3 - i] = n1 - 1 ;
    }
    for (i = n1 - 2 ; i >= 0 ; i--)
    {
        ybd_cont[2 * n2 + 2 * n1 - 4 - i] = 0;
        xbd_cont[2 * n2 + 2 * n1 - 4 - i] = i ;
    }
    for (c = 0 ; c < ncont ; c++)
    {
        stylec = (style != (int *) 0) ? style[c] : c;
        /** itg-cont is a flag array to memorize checked parts of the grid **/
        for (i = 0 ; i < n1; i++)
            for (j = 0 ; j < n2 ; j++)
            {
                itg_cont[i + n1 * j] = 0;
            }
        /** all the boundary segments **/
        for (k = 1 ; k < n5 ; k++)
        {
            int ib, jb;
            i = xbd_cont[k] ;
            j = ybd_cont[k];
            ib = xbd_cont[k - 1] ;
            jb = ybd_cont[k - 1];
            if  (not_same_sign (phi_cont(i, j) - zCont[c] ,
                                phi_cont(ib, jb) - zCont[c]))
            {
                look(func, i, j, ib, jb, 1L, zCont[c], stylec);
            }
        }
        /** inside segments **/
        for (i = 1 ; i < n1 - 1; i++)
            for (j = 1 ; j < n2 - 1 ; j++)
                if  (not_same_sign (phi_cont(i, j) - zCont[c] ,
                                    phi_cont(i, j - 1) - zCont[c]))
                {
                    look(func, i, j, i, j - 1, 2L, zCont[c], stylec);
                }
    }
    FREE(xbd_cont);
    FREE(ybd_cont);
    FREE(itg_cont);

    return 0;
}

int C2F(contourif)(double *x, double *y, double *z, int *n1, int *n2, int *flagnz, int *nz, double *zz, int *style)
{
    int err = 0;
    static double *zconst = NULL;
    double zmin = 0., zmax = 0.;
    int N[3], i = 0;

    zmin = (double) Mini(z, *n1 * (*n2));
    zmax = (double) Maxi(z, *n1 * (*n2));

    if (*flagnz == 0)
    {
        if ((zconst = (double*)MALLOC((*nz) * sizeof(double))) == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "contourif");
            return -1;
        }
        for (i = 0 ; i < *nz ; i++)
        {
            zconst[i] = zmin + (i + 1) * (zmax - zmin) / (*nz + 1);
        }
        N[0] = *n1;
        N[1] = *n2;
        N[2] = *nz;
        contourI(GContStore2, x, y, z, zconst, N, style, &err);
        FREE(zconst);
        zconst = NULL;
    }
    else
    {
        N[0] = *n1;
        N[1] = *n2;
        N[2] = *nz;
        contourI(GContStore2, x, y, z, zz, N, style, &err);
    }

    return 0;
}





/*-----------------------------------------------------------------------
 *   ffnd : cette fonction  recoit en entree quatre points
 *       on sait que la courbe de niveau passe entre le point 1 et le quatre
 *       on cherche a savoir ou elle resort,
 *       et on fixe une nouvelle valeur de ent qui indiquera le segment
 *       suivant a explorer
 *-----------------------------------------------------------------------*/

static int ffnd (ptr_level_f func, int i1, int i2, int i3, int i4, int jj1, int jj2, int jj3, int jj4, int ent, int qq, double Cont, int *zds)
{
    double phi1 = 0., phi2 = 0., phi3 = 0., phi4 = 0., xav = 0., yav = 0., phiav = 0.;
    int revflag = 0, i = 0;
    phi1 = phi_cont(i1, jj1) - Cont;
    phi2 = phi_cont(i2, jj2) - Cont;
    phi3 = phi_cont(i3, jj3) - Cont;
    phi4 = phi_cont(i4, jj4) - Cont;
    revflag = 0;
    *zds = 0;
    /* le point au centre du rectangle */
    xav = (x_cont(i1) + x_cont(i3)) / 2.0 ;
    yav = (y_cont(jj1) + y_cont(jj3)) / 2.0 ;
    phiav = (phi1 + phi2 + phi3 + phi4) / 4.0;
    if (ISNAN(phiav) == 1)
    {
        return -1;
    }
    if ( not_same_sign(phiav, phi4))
    {
        int l1, k1;
        double phi;
        revflag = 1 ;
        l1 = i4;
        k1 = jj4;
        i4 = i1;
        jj4 = jj1;
        i1 = l1;
        jj1 = k1;
        l1 = i3;
        k1 = jj3;
        i3 = i2;
        jj3 = jj2;
        i2 = l1;
        jj2 = k1;
        phi = phi1;
        phi1 = phi4;
        phi4 = phi;
        phi = phi3;
        phi3 = phi2;
        phi2 = phi;
    }
    /* on stocke un nouveau point  */
    (*func)(1, Cont, f_intercept(0.0, phi1, x_cont(i1), phiav, xav),
            f_intercept(0.0, phi1, y_cont(jj1), phiav, yav));
    /*
     * on parcourt les segments du rectangle pour voir sur quelle face
     * on sort
     */
    for  (i = 0 ;  ; i++)
    {
        int l1, k1;
        double phi;
        if (not_same_sign (phi1, phi2))   /** sortir du for **/
        {
            break ;
        }
        if  (phiav != 0.0)
        {
            (*func)(1, Cont, f_intercept(0.0, phi2, x_cont(i2), phiav, xav),
                    f_intercept(0.0, phi2, y_cont(jj2), phiav, yav));
        }
        /** on permutte les points du rectangle **/
        l1 = i1;
        k1 = jj1;
        i1 = i2;
        jj1 = jj2;
        i2 = i3;
        jj2 = jj3;
        i3 = i4;
        jj3 = jj4;
        i4 = l1;
        jj4 = k1;
        phi = phi1;
        phi1 = phi2;
        phi2 = phi3;
        phi3 = phi4;
        phi4 = phi;
    }
    (*func)(1, Cont, f_intercept(0.0, phi1, x_cont(i1), phi2, x_cont(i2)),
            f_intercept(0.0, phi1, y_cont(jj1), phi2, y_cont(jj2)));
    if (qq == 1 && bdyp(i1, jj1) && bdyp(i2, jj2))
    {
        *zds = 1;
    }
    if (revflag == 1  &&  ! oddp (i))
    {
        i = i + 2;
    }
    return (1 + ( (i + ent + 2) % 4));
}

/*--------------------------------------------------------------
 * Following code is used to store the current level curves as
 * double in order to access to the stored data at Scilab level
 *----------------------------------------------------------------*/

static int last = -1;
static int count = 0;

/** used to bring back data to Scilab Stack **/

int C2F(getconts)(double **x, double **y, int *mm, int *n)
{
    *x = Gxcont;
    *y = Gycont;
    *mm = 1;
    *n = Gcont_size;
    return 0;
}

static void GContStore2(int ival, double Cont, double xncont, double yncont)
{
    int n = 0;
    if (ival == 0)
    {
        /* Here : ival == 0 means stop the current level curve and
         * store data at the end but do reset Gcont_size to zero
         */
        n = Gcont_size + 2;
        if (Gxcont == NULL)
        {
            Gxcont = (double*)MALLOC(n * sizeof(double));
            Gycont = (double*)MALLOC(n * sizeof(double));
        }
        else
        {
            if (n > currentContSize)
            {
                currentContSize = (int)(n * 1.1);
                Gxcont = (double*)REALLOC(Gxcont, currentContSize * sizeof(double));
                Gycont = (double*)REALLOC(Gycont, currentContSize * sizeof(double));
            }
        }
        if ((Gxcont == NULL) && n != 0)
        {
            return ;
        }
        if ((Gycont == NULL) && n != 0)
        {
            return;
        }
        Gxcont[Gcont_size] = Cont;
        if (last != -1 && last < n)
        {
            Gycont[last] = count;
        }
        last = Gcont_size;
        Gcont_size++;
        count = 0;
    }
    else
    {
        n = Gcont_size + 1;
        if (Gxcont == NULL)
        {
            Gxcont = (double*)MALLOC(n * sizeof(double));
            Gycont = (double*)MALLOC(n * sizeof(double));
        }
        else
        {
            if (n > currentContSize)
            {
                currentContSize = (int)(n * 1.1);
                Gxcont = (double*)REALLOC(Gxcont, currentContSize * sizeof(double));
                Gycont = (double*)REALLOC(Gycont, currentContSize * sizeof(double));
            }
        }
        if ((Gxcont == NULL) && n != 0)
        {
            return ;
        }
        if ((Gycont == NULL) && n != 0)
        {
            return;
        }
    }
    Gxcont[Gcont_size] = xncont;
    Gycont[Gcont_size++] = yncont;
    count++;
}

static void GContStore2Last(void)
{
    if (last != -1)
    {
        Gycont[last] = count;
    }
}











