/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2001 - INRIA - François Delebecque
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots (format selection)
 *
 * void  ChoixFormatE(fmt, desres, xmin, xmax, xpas) : find a format
 * void  ChoixFormatE1(fmt, desres, xx, nx)          : find a format
 * int   C2F(graduate)(xmi,xma,xi,xa,np1,np2,kminr,kmaxr,ar)
 *                : change [xmi,xmax] for pretty graduation
 *--------------------------------------------------------------------------*/

#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <stdio.h>
#include <string.h>
#include "math_graphics.h"
#include "Format.h"
#include "sci_malloc.h"
#include "GetProperty.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "machine.h"
#include "numericconstants_interface.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Sciwarning.h"

#define MAX(A,B) ((A<B)?B:A)

/** Maximum of ticks for log mode */
#define MAX_LOG_TICKS 15

/* end here */

static void FormatPrec (char *fmt, int *desres, double xmin, double xmax,
                        double xpas);
static void FormatPrec1 (char *fmt, int *desres, double *xx, int nx);
static int Fsepare (char *fmt, int dec, int *l, double xmin, double xmax,
                    double xpas);
static int Fsepare1 (char *fmt, int dec, int *l, double *xx, int nx);
static void graduate1 (double *xmi, double* xma, double* xi, double* xa,
                       int * np1, int * np2, int * kminr, int * kmaxr, int * ar, int count);

static void gradua (double *xmi, double *xma, int * kminr, int *kmaxr, int *ar, int *npr, int *b);
static void decompSup (double x, int * xk, int *  xa, int   b);
static void decompInf (double x, int * xk, int *  xa, int   b);

static void removeIndex(double* changedArray, int size, int ind);
static void removeBadTicks(double* ticks, BOOL * removedTicks, int * nbTicks);
static void GradFixedlog(double minVal, double maxVal, double* ticks, int nbGrads);
static int GradLog(double _min, double _max, double *_grads, int *n_grads, int compNgrads);

/**
 * ChoixFormatE returns a format ("%.*f" or "%.*e")
 * in fmt given xmin,xmax,pas.
 *   fmt : character string
 * fmt gives a format which can be used to display
 * number in range xmin:step:xmax
 * Exemple : ChoixFormatE(format,min,max,step);
 *           fprintf(format,min+k*step);
 * The format is searched so as to give distinct values
 * for the numeric values xmin + k*xpas in [xmin,xmax]
 * and give enough precision.
 */
static void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas)
{
    char c = 0;
    int des = 0, len = 0;
    /* format f minimal  */
    for (des = 0 ; des < 5 ; des++)
    {
        if (Fsepare("%.*f", des, &len, xmin, xmax, xpas))
        {
            break;
        }
    }
    if (des < 5 && len <= 6)
    {
        c = 'f';
        strcpy(fmt, "%.*f");
    }
    else
    {
        for (des = 0 ; des < 5 ; des++)
        {
            if (Fsepare("%.*e", des, &len, xmin, xmax, xpas))
            {
                break;
            }
        }
        c = 'e';
        strcpy(fmt, "%.*e");
    }
    FormatPrec(fmt, &des, xmin, xmax, xpas);
    sprintf(fmt, "%%.%d%c", des, c);
}

/*
 *  checks if given format gives enough precision
 *  if not increase it (i.e increase desres)
 */

static void FormatPrec(char *fmt, int *desres, double xmin, double xmax, double xpas)
{
    char buf1[100], buf2[100];
    int i = 0;
    while (xmin + ((double)i)*xpas < xmax && *desres  < 10)
    {
        double x1, x2, yy1;
        yy1 = xmin + ((double) i) * xpas;
        sprintf(buf1, fmt, *desres, yy1);
        sprintf(buf2, fmt, *desres, yy1 + xpas);
        sscanf(buf1, "%lf", &x1);
        sscanf(buf2, "%lf", &x2);
        if (  Abs((x2 - x1 - xpas) / xpas) >= 0.1)
        {
            *desres += 1;
        }
        if (  Abs((x1 - yy1) / xpas) >= 0.01)
        {
            *desres += 1;
        }
        i++;
    }
}

/*
 *  checks if format fmt gives different values for numbers
 *  from xmin to xmax with step xpas. It also returns in variable l
 *  the string length that will result in using the format
 */

static int Fsepare(char *fmt, int dec, int *l, double xmin, double xmax, double xpas)
{
    double x = xmin;
    char buf1[100], buf2[100];
    *l = 0;
    /**  Take care of : sprintf(buf1,"%.*f",0,1.d230) which overflow in buf1 **/
    /**  we don't use %.*f format if numbers are two big **/
    if (strcmp("%.*f", fmt) == 0 && (Abs(xmax) > 1.e+10 || Abs(xmin) > 1.e+10))
    {
        return (0);
    }
    sprintf(buf1, fmt, dec, xmin);
    while (x < xmax)
    {
        x += xpas;
        strcpy(buf2, buf1);
        sprintf(buf1, fmt, dec, x);
        *l = (((int)strlen(buf1) >= *l) ? (int) strlen(buf1) : *l);
        if (strcmp(buf1, buf2) == 0)
        {
            return (0);
        }
    }
    return (1);
}

void ChoixFormatE1(char *fmt, double *xx, int nx)
{
    char c = 0;
    int des = 0, len = 0;
    /* format f minimal  */
    for (des = 0 ; des < 5 ; des++)
    {
        if (Fsepare1("%.*f", des, &len, xx, nx))
        {
            break;
        }
    }
    if (des < 5 && len <= 6)
    {
        c = 'f';
        strcpy(fmt, "%.*f");
    }
    else
    {
        for (des = 0 ; des < 5 ; des++)
        {
            if (Fsepare1("%.*e", des, &len, xx, nx))
            {
                break;
            }
        }
        c = 'e';
        strcpy(fmt, "%.*e");
    }
    FormatPrec1(fmt, &des, xx, nx);
    sprintf(fmt, "%%.%d%c", des, c);
}


/*----------------------------------------------------------
 *  checks if format fmt gives different values for numbers
 *  from xmin to xmax with step xpas. It also returns in variable l
 *  the string length that will result in using the format
 *------------------------------------------------------*/

static void FormatPrec1(char *fmt, int *desres, double *xx, int nx)
{
    char buf1[100], buf2[100];
    double xpas = 0.;
    int i = 0;
    while (i < nx - 1 && *desres  < 10)
    {
        double x1 = 0., x2 = 0.;
        sprintf(buf1, fmt, *desres, xx[i]);
        sprintf(buf2, fmt, *desres, xx[i + 1]);
        sscanf(buf1, "%lf", &x1);
        sscanf(buf2, "%lf", &x2);
        xpas = xx[i + 1] - xx[i];
        if (xpas != 0.0)
        {
            if (Abs((x2 - x1 - xpas) / xpas) >= 0.1)
            {
                *desres += 1;
            }
            if (Abs((x1 - xx[i]) / xpas) >= 0.1)
            {
                *desres += 1;
            }
        }
        i++;
    }
}

static int Fsepare1(char *fmt, int dec, int *l, double *xx, int nx)
{
    char buf1[100], buf2[100];
    int i = 0;
    *l = 0;
    /**  Take care of : sprintf(buf1,"%.*f",0,1.d230) which overflow in buf1 **/
    /**  we don't use %.*f format if numbers are two big **/
    if (strcmp("%.*f", fmt) == 0 && (Abs(xx[nx - 1]) > 1.e+10 || Abs(xx[0]) > 1.e+10))
    {
        return (0);
    }
    sprintf(buf1, fmt, dec, xx[0]);
    for (i = 1 ; i < nx ; i++)
    {
        strcpy(buf2, buf1);
        sprintf(buf1, fmt, dec, xx[i]);
        *l = (((int)strlen(buf1) >= *l) ? (int) strlen(buf1) : *l);
        if (strcmp(buf1, buf2) == 0)
        {
            return (0);
        }
    }
    return (1);
}

/*----------------------------------------------------
 * int C2F(graduate)(xmi,xma,xi,xa,np1,np2,kminr,kmaxr,ar)
 * (xgraduate at Scilab level)
 * Rescale an interval so as to find a pretty graduation
 * for [xmi,xma] given seeks (xi,xa,np1,np2)
 * such that  xi <= xmi <= xmax <= xa
 * with xi et xa  numbers of type  kminr 10^ar and kmaxr 10^ar.
 * then the interval [xi,xa] can be splited in *np2 sub-intervals
 *  (kminr-kmaxr can be divided by *np2)
 *  x_i= (kminr + i*(kmaxr-kminr)/ (*np2))*10^ar;
 * i=0:(*np2)
 * ecah of the  np2 intervals can in turn be splited in np1 ungraduated
 * subintervals
 * [np1,np2] follow the nax parameters of plot2d.
 *
 *  We also want to keep np2 small (*np2 <=10)
 *  and we want [xi,xa] to be as close as possible to the interval
 *  [xmi,xma]
 *---------------------------------------------------- */

int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, int *np1, int *np2, int *kminr, int *kmaxr, int *ar)
{
    if (*xmi > *xma)
    {
        double xma1 = *xmi, xmi1 = *xma;
        graduate1(&xmi1, &xma1, xi, xa, np1, np2, kminr, kmaxr, ar, 0);
    }
    else
    {
        graduate1(xmi, xma, xi, xa, np1, np2, kminr, kmaxr, ar, 0);
    }
    return (0);
}

static void graduate1(double *xmi, double *xma, double *xi, double *xa, int *np1, int *np2, int *kminr, int *kmaxr, int *ar, int count)
{
    int npr = 0, b = 0, i = 0, dx = 0, dxmi = 0, dxma = 0;
    /* fprintf(stderr,"[%20.10f,%20.10f]\n",*xmi,*xma); */
    /*
     *
     */
    dx   = ((*xma) != (*xmi)) ? (int) ceil(log10(Abs((*xma) - (*xmi)))) : 0;
    dxmi = (*xmi != 0) ? (int) ceil(log10(Abs((*xmi)))) : 0;
    dxma = (*xma != 0) ? (int) ceil(log10(Abs((*xma)))) : 0;
    dx = Max(dx - dxmi, dx - dxma);
    /* il faut limiter b de sorte que dans la decomposition */
    /* avec b les nombres entiers manipules ne deviennent pas trop grands */
    /* on choisit donc b < 10 en considerant que le plus grand entier est */
    /* 0x7FFFFFFF */
    /* on prends aussi un b minimum de 3 : pour avoir des intervalles */
    /* plus serr'es  : ce nombre est 'eventuellement a affiner      */
    b = Max(-dx + 2, 3);
    /* fprintf(stderr,"choix de b=%d",b); */
    if (b >= 10)
    {
        double xmi1 = 0., xma1 = 0.;
        int iexp = 0;
        /* fprintf(stderr,"je ne peux decomposer les 2 nombres sont identiques\n"); */
        /*
        a la precision donnee les deux nombre ne peuvent etre decomposer
        kmin,kmax devrait sinon depasser maxint
        on les ecarte de ce qu'il faut pour pouvoir
        les separer.
        Attention : il faut faire attention de bien choisir iexp
        pour ne pas boucler la dedans
        */
        iexp = 9 - dxmi - 1;
        xmi1 = *xmi - exp10((double) - iexp);
        xma1 = *xmi + exp10((double) - iexp);
        if (count > 1)
        {
            sciprint(_("Internal Error: Loop in graduate1\n"));
            sciprint(_("Please send a Bug report to dev@lists.scilab.org\n"));
        }
        graduate1(&xmi1, &xma1, xi, xa, np1, np2, kminr, kmaxr, ar, count + 1);
        return;
    }
    while (b >= 1)
    {
        /* fprintf(stderr,"\tAppel avec b=%d\n",b); */
        gradua(xmi, xma, kminr, kmaxr, ar, &npr, &b);
        *xi = (*kminr) * exp10((double) * ar);
        *xa = (*kmaxr) * exp10((double) * ar);
        /** fprintf(stderr,"\tRes=[%20.10f,%20.10f]-->[%d,%d,10^%d,%d]\n",*xi,*xa
            ,*kminr,*kmaxr,*ar,npr); */
        *np2 = npr;
        if (*np2 <= 20)
        {
            break;
        }
        else
        {
            b--;
        }
    }
    /*
      on veut essayer de ne pas depasser 10 intervalles (*np2 <= 10)
      pour les intervalle ou on ecrit un nombre,
      or on peut en avoir jusqu'a 20. On regarde si le nombre d'intervalle
      est divisible. on aura alors une graduation en np2 pour l'ecriture
      des nombres et une sous graduation np1 juste avec des tirets.
      */
    *np1 = 2 ;
    if ( *np2 <= 10 )
    {
        return ;
    }
    /* le nombre est > 10 : s'il est impair on rajoute 1
       pour diviser par deux */
    if (*np2 % 2 == 1)
    {
        int step;
        step = (*kmaxr - *kminr) / (*np2);
        (*np2)++;
        *kmaxr += step;
        *xa =  (*kmaxr) * exp10((double) * ar);
    }
    /* On recherche des diviseurs a nouveaux pour diminuer le nombre
       d'intervalles */
    for (i = 2 ; i <= 10 ; i++)
    {
        if (*np2 % i == 0)
        {
            *np1 = i, *np2 /= i;
            return;
        }
    }
    *np1 = *np2;
    *np2 = 1;
}

/*
 *  renvoit kminr,kmaxr et ar tels que
 *  [kminr*10^ar,kmaxr*10^ar] contient [xmi,xma]
 *  b est un parametre de decompSup,decompInf
 *  on doit avoir a l'appel xmi < xma.
 *  le choix se fait entre deux intervalles possibles
 *  on choisit celui qui est le plus proche de [xmi,xma]
 *  a condition que (kmaxr-kminr) <= 20
 *  pour b=1 on sait que (kmaxr-kminr) <= 20
 *  20 intervalles au plus (que l'on obtient si xmin et xmax sont
 *  de signe opposes sinon c'est 10)
 */

/* np2, and np1 must be smaller than maxint */

#define DMAX 0xFFFFFFF

static void gradua(double *xmi, double *xma, int *kminr, int *kmaxr, int *ar, int *npr, int *b)
{
    double x0 = *xmi, x1 = *xma, loc = 0.;
    int x0k = 0, x0a = 0;
    int x1k = 0, x1a = 0;
    int kmin1 = 0, kmax1 = 0, a1 = 0, np1 = 0, kmin2 = 0, kmax2 = 0, a2 = 0, np2 = 0, kmin = 0, kmax = 0, a = 0, np = 0;
    decompInf(x0, &x0k, &x0a, *b);
    decompSup(x1, &x1k, &x1a, *b);
    /** special cases **/
    if (x1 == 0.0)
    {
        x1a = x0a;
    }
    if (x0 == 0.0)
    {
        x0a = x1a;
    }
    loc = Min(floor(x0 * exp10((double) - x1a)), ((double)DMAX));
    if (loc < 0)
    {
        loc = Max(loc, -((double) DMAX));
    }
    kmin1 = (int) loc;
    kmax1 = x1k;
    np1 = Abs(kmax1 - kmin1);
    np1 = (np1 < 0) ? DMAX : np1;
    if (np1 > 10)
    {
        if  ((np1 % 2) == 0)
        {
            np1 /= 2;
        }
        else
        {
            np1++;
            np1 /= 2;
            kmax1++;
        }
    }
    a1 = x1a;
    /* fprintf(stderr,"\t\tsols : [%d,%d].10^%d,n=%d\t",kmin1,kmax1,a1,np1);  */
    kmin2 = x0k;
    loc = Min(ceil(x1 * exp10((double) - x0a)), ((double)DMAX));
    kmax2 = (int) loc;
    np2 = Abs(kmax2 - kmin2);
    np2 = (np2 < 0) ? DMAX : np2;
    if (np2 > 10)
    {
        if (np2 % 2 == 0)
        {
            np2 /= 2;
        }
        else
        {
            np2++;
            kmin2--;
        }
    }
    a2 = x0a;
    /* fprintf(stderr,"[%d,%d].10^%d=%d\n",kmin2,kmax2,a2,np2);  */
    if (np1 * exp10((double)a1) < np2 * exp10((double) a2))
    {
        if (np1 <= 20)
        {
            kmin = kmin1;
            kmax = kmax1;
            np = np1;
            a = a1;
        }
        else
        {
            kmin = kmin2;
            kmax = kmax2;
            np = np2;
            a = a2;
        }
    }
    else
    {
        if (np2 <= 20)
        {
            kmin = kmin2;
            kmax = kmax2;
            np = np2;
            a = a2;
        }
        else
        {
            kmin = kmin1;
            kmax = kmax1;
            np = np1;
            a = a1;
        }
    }
    *kminr = kmin;
    *kmaxr = kmax;
    *ar = a;
    *npr = np;
    if (kmin == kmax)
    {
        /*
         * a la precision demandee les deux [xi,xa] est reduit a un point
         * on elargit l'intervalle
         */
        /* fprintf(stderr,"Arg : kmin=kmax=%d",kmin) ; */
        /* fprintf(stderr," a=%d, x0=%f,x1=%f\n",a,x0,x1); */
        (*kminr)--;
        (*kmaxr)++;
        *npr = 2;
    };
}

/*
 * soit x > 0 reel fixe et b entier fixe : alors il existe un unique couple
 * (k,a) dans NxZ avec k dans [10^(b-1)+1,10^b] tel que
 * (k-1)*10^a < x <= k 10^a
 * donne par  a = ceil(log10(x))-b et k=ceil(x/10^a)
 * decompSup renvoit xk=k et xa=a
 * si x < 0 alors decompSup(x,xk,xa,b)
 *    s'obtient par decompInf(-x,xk,xa,b) et xk=-xk
 * Remarque : la taille de l'entier k obtenu est controle par b
 * il faut choisir b < 10 pour ne pas depasser dans k l'entier maximum
 */

static void decompSup(double x, int *xk, int *xa, int b)
{
    if (x == 0.0)
    {
        *xk = 0;
        *xa = 1; /* jpc */
    }
    else
    {
        if (x > 0)
        {
            double xd;
            static double epsilon;
            static int first = 0;
            if (first == 0)
            {
                epsilon = 10.0 * nc_eps();
                first++;
            }
            /* if x is very near (k+1)10^a (epsilon machine)
             * we increment xk
             */
            *xa = (int) ceil(log10(x)) - b;
            *xk = (int) ceil(x / exp10((double) * xa));
            xd = (*xk - 1) * exp10((double) * xa);
            if (Abs((x - xd) / x) < epsilon)
            {
                *xk -= 1;
            }
        }
        else
        {
            decompInf(-x, xk, xa, b);
            *xk = -(*xk);
        }
    }
}


/*
 * soit x > 0 alors il existe un unique couple
 * (k,a) dans NxZ avec k in [10^(b-1),10^b-1] tel que
 * (k)*10^a <= x < (k+1) 10^a
 * donne par
 * a = floor(log10(x))-b+1 et k = floor(x/10^a)
 * decompInf renvoit xk=k et xa=a
 * si x < 0 alors decompInf(x,xk,xa,b)
 *    s'obtient par decompSup(-x,xk,xa,b) et xk=-xk
 */

static void decompInf(double x, int *xk, int *xa, int b)
{
    if (x == 0.0)
    {
        *xk = 0;
        *xa = 1; /* jpc */
    }
    else
    {
        if (x > 0)
        {
            double xup;
            static double epsilon;
            static int first = 0;
            if (first == 0)
            {
                epsilon = 10.0 * nc_eps();
                first++;
            }
            *xa = (int) floor(log10(x)) - b + 1;
            *xk = (int) floor(x / exp10((double) * xa));
            /* if x is very near (k+1)10^a (epsilon machine)
             * we increment xk
             */
            xup = (*xk + 1) * exp10((double) * xa);
            if (Abs((x - xup) / x) < epsilon)
            {
                *xk += 1;
            }
        }
        else
        {
            decompSup(-x, xk, xa, b);
            *xk = -(*xk);
        }
    }
}

/*--------------------------------------------------------------------------*/
/* remove an element in the array from translating the next
elements on step backward */
static void removeIndex(double* changedArray, int size, int ind)
{
    int i  = 0;
    for (i = ind + 1 ; i < size ; i++)
    {
        changedArray[i - 1] = changedArray[i];
    }
}
/*--------------------------------------------------------------------------*/
/* remove in the ticks array the indices i such as removedTicks[i] */
/* is true. The value nbtics is an in-out variable */
static void removeBadTicks(double* curTicks, BOOL * removedTicks, int * nbTicks)
{
    int i  = 0;
    for (i = *nbTicks - 1 ; i >= 0 ; i--)
    {
        if (removedTicks[i])
        {
            removeIndex(curTicks, *nbTicks, i);
            *nbTicks = *nbTicks - 1;
        }
    }
}
/*--------------------------------------------------------------------------*/
/* compute the graduation of the segment [minVal,maxVal] knowing the number of ticks */
static void GradFixedlog(double minVal, double maxVal, double* outTicks, int nbGrads)
{
    int initSize  = 0;
    int i = 0;

    /* initialize the array as usual */
    double tempTicks[20];
    GradLog(minVal, maxVal, tempTicks, &initSize, FALSE);

    if (initSize > nbGrads)
    {
        /* we create a smaller vector from a bigger one */
        int nbRemove = initSize - nbGrads;

        BOOL * removedTicks = NULL;
        if ((removedTicks = MALLOC(initSize * sizeof(BOOL))) == NULL)
        {
            return;
        }

        for (i = 0 ; i < initSize ; i++)
        {
            removedTicks[i] = FALSE;
        }

        /* we now remove the nbremove indexes : round((0.5 + i) * size / nbRemove) */
        /* i=0..nbReg-1 should do the thing */
        for (i = 0 ; i < nbRemove ; i++)
        {
            int remIndex = 1 + (int) scilab_round( i  * ((double) initSize - 2) / ((double) nbRemove));
            removedTicks[remIndex] = TRUE;
        }

        removeBadTicks(tempTicks, removedTicks, &initSize);

        FREE(removedTicks);

    }
    doubleArrayCopy(outTicks, tempTicks, nbGrads);

}


/* compute the automatic graduation of the segment [_min,_max] and store it in _grads */
/* the number of graduation may be fixed if compNgrads is TRUE or automatically computed */
/* otherwise. */
static int GradLog(double   _min   ,
                   double   _max   ,
                   double* _grads ,
                   int    * n_grads,
                   int      compNgrads)
{
    int i = 0;
    int log_min = 0, log_max = 0;
    int size = 0;

    if (compNgrads)
    {
        GradFixedlog(_min, _max, _grads, *n_grads);
        return 0;
    }

    log_max =  (int) ceil(_max);
    log_min =  (int) floor(_min);

    /* If _min == _max, enlarge the interval*/
    if (log_max == log_min)
    {
        log_max++;
        log_min--;
    }

    size = log_max - log_min + 1;

    *n_grads = 0;

    if (size <= MAX_LOG_TICKS)
    {
        for (i = 0; i < size; i++)
        {
            _grads[i] = log_min + i;
            *n_grads = (*n_grads) + 1;
        }
    }
    else
    {
        int pas = 0, old_pas = 0, j;
        int val = size, passed = 0;

        /* Try to reduce number of ticks, by finding the greatest divider */
        for (j = val - 1; j > 1; j--)
            if (val % j == 0)
            {
                old_pas = pas;
                pas = j;
                passed = 1;

                if ((MAX_LOG_TICKS * pas) <= val)
                {
                    if (old_pas != 0)
                    {
                        pas = old_pas;
                    }
                    break;
                }
            }

        /* If we haven't found a divider or if the number of ticks will be to large */
        /* Use only towo ticks */
        if (passed != 1 || (size / pas) >  MAX_LOG_TICKS)
        {
            pas = size;
        }

        if (pas == size)
        {
            _grads[0] = log_min;
            _grads[1] = log_max;
            *n_grads = 2;
        }
        else
        {
            for (i = 0; i <= (int)(size / pas); i++)
            {
                _grads[i] = log_min + (i * pas);

                *n_grads = (*n_grads) + 1;
            }
        }
    }

    return 0;
}

/**
* get the exponent used for log axis from given data bounds
* @return 0 if OK, -1 if negative bounds.
*/
int sciGetLogExponent(double minBound, double maxBound, double* expMin, double* expMax)
{
    if (minBound > 0)
    {
        *expMin = floor(log10(minBound));
        *expMax = ceil( log10(maxBound));
        return 0;
    }
    *expMax = 1.0;
    *expMin = 0.0;
    return -1;
}
/*--------------------------------------------------------------------------*/
/*
 * This function has been adapted to the MVC framework (property get calls)
 * in order to be able to provide a valid format string when computing
 * default labels for the Axis object. The algorithm is left untouched.
 * Its code ought to be put within the Java part of the Model.
 */
int ComputeC_format(int iObjUID, char * c_format)
{
    int i = 0, j = 0;
    int pos = 0;
    int* piPos = &pos;
    int xy_type = 0;
    int* piXy_type = &xy_type;
    int nx = 0;
    int* piNx = &nx;
    int ny = 0;
    int* piNy = &ny;
    double *x = NULL;
    double *y = NULL;
    double* tmpx = NULL;
    double* tmpy = NULL;
    int iType = -1;
    int *piType = &iType;
    int  xpassed = 0, ypassed = 0, Nx = 0, Ny = 0, x3, y3;
    int parentAxesID;
    int * piParentAxesID = &parentAxesID;
    int logFlag = 0;
    int* piLogFlag = &logFlag;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_AXIS__)
    {
        Scierror(999, _("Error: ComputeFormat must be used with AXIS objects\n"));
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piParentAxesID);

    getGraphicObjectProperty(iObjUID, __GO_TICKS_DIRECTION__, jni_int, (void **)&piPos);
    getGraphicObjectProperty(iObjUID, __GO_TICKS_STYLE__, jni_int, (void **)&piXy_type);

    getGraphicObjectProperty(iObjUID, __GO_X_NUMBER_TICKS__, jni_int, (void **)&piNx);
    getGraphicObjectProperty(iObjUID, __GO_Y_NUMBER_TICKS__, jni_int, (void **)&piNy);

    /* Allocating space before re-copying values to not pollute the good values
    that will be used inside Axes.c */
    if ((x = MALLOC(nx * sizeof(double))) == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "ComputeC_format");
        return -1;
    }

    if ((y = MALLOC(ny * sizeof(double))) == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "ComputeC_format");
        FREE(x);
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_X_TICKS_COORDS__, jni_double_vector, (void **)&tmpx);
    getGraphicObjectProperty(iObjUID, __GO_Y_TICKS_COORDS__, jni_double_vector, (void **)&tmpy);

    for (i = 0; i < nx; i++)
    {
        x[i] = tmpx[i];
    }

    for (i = 0; i < ny; i++)
    {
        y[i] = tmpy[i];
    }

    /* Algo. here */
    if (xy_type == 2)
    {
        if (pos == 0 || pos == 1)
        {
            getGraphicObjectProperty(iObjUID, __GO_X_AXIS_LOG_FLAG__, jni_int, (void **)&piLogFlag);

            if (logFlag == 0)
            {
                while (x[3] > 10)
                {
                    x[3] = floor(x[3] / 2);
                }
            }
            else
            {
                if (x[3] > 12)
                {
                    /* F.Leray arbitrary value=12 for the moment */
                    x3 = (int)x[3];   /* if x[3]>12 algo is triggered to search a divisor */
                    for (j = x3 - 1; j > 1; j--)
                    {
                        if (x3 % j == 0)
                        {
                            x[3] = j;
                            xpassed = 1;
                        }
                    }
                    if (xpassed != 1)
                    {
                        x[3] = 1;
                    }
                }
            }
        }
        else if (pos == 2 || pos == 3)
        {
            getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOG_FLAG__, jni_int, (void **)&piLogFlag);

            if (logFlag == 0)
            {
                while (y[3] > 10)
                {
                    y[3] = floor(y[3] / 2);
                }
            }
            else
            {
                if (y[3] > 12)
                {
                    y3 = (int)y[3];
                    for (j = y3 - 1; j > 1; j--)
                    {
                        if (y3 % j == 0)
                        {
                            y[3] = j;
                            ypassed = 1;
                        }
                    }
                    if (ypassed != 1)
                    {
                        y[3] = 1;
                    }
                }
            }
        }
    }


    /** Real to Pixel values **/
    if (xy_type == 0)
    {
        Nx = nx;
        Ny = ny;
    }
    else if (xy_type == 1)
    {
        if (pos == 0 || pos == 1)
        {
            Nx = (int) x[2] + 1;
        }
        else if (pos == 2 || pos == 3)
        {
            Ny = (int) y[2] + 1;
        }
    }
    else if (xy_type == 2)
    {
        if (pos == 0 || pos == 1)
        {
            Nx = (int) x[3] + 1;
        }
        else if (pos == 2 || pos == 3)
        {
            Ny = (int) y[3] + 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type argument %s.\n"), "Sci_Axis", "xy_type");
        FREE(x);
        x = NULL;
        FREE(y);
        y = NULL;
        return -1;
    }

    if (pos == 0 || pos == 1)
    {
        /** Horizontal axes **/
        /** compute a format **/
        if (xy_type == 0)
        {
            ChoixFormatE1(c_format, x, Nx);
        }
        else if (xy_type == 1)
        {
            ChoixFormatE (c_format, x[0], x[1], (x[1] - x[0]) / x[2]);
        }
        else if (xy_type == 2)
        {
            ChoixFormatE (c_format,
                          (x[0] * exp10(x[2])),
                          (x[1] * exp10(x[2])),
                          ((x[1] * exp10(x[2])) - (x[0] * exp10(x[2]))) / x[3]);
            /* Adding F.Leray 06.05.04 */
        }
        /** the horizontal segment **/
    }
    else if (pos == 2 || pos == 3)
    {
        /** Vertical axes **/
        if (xy_type == 0)
        {
            ChoixFormatE1(c_format, y, Ny);
        }
        else if (xy_type == 1)
        {
            ChoixFormatE(c_format, y[0], y[1], (y[1] - y[0]) / y[2]);
        }
        else if (xy_type == 2)
        {
            ChoixFormatE (c_format,
                          (y[0] * exp10(y[2])),
                          (y[1] * exp10(y[2])),
                          ((y[1] * exp10(y[2])) - (y[0] * exp10(y[2]))) / y[3]);
            /* Adding F.Leray 06.05.04 */
        }
        /** the vertical segment **/
    }

    /* c_format should be filled now */

    FREE(x);
    x = NULL;
    FREE(y);
    y = NULL;

    return 0;
}
/*--------------------------------------------------------------------------*/
/*
 * This function has been updated for the MVC (property get calls).
 * Its code ought to be put within the Java part of the Model.
 */
int ComputeXIntervals(int iObjUID, char xy_type, double ** vector, int * N, int checkdim)
{
    int i = 0;
    double* val = NULL; /* represents the x or y ticks coordinates */
    int nval = 0;

    int n = 0;
    int nx = 0;
    int* piNx = &nx;
    int ny = 0;
    int* piNy = &ny;
    BOOL ishoriz = FALSE;

    getGraphicObjectProperty(iObjUID, __GO_X_NUMBER_TICKS__, jni_int, (void **)&piNx);
    getGraphicObjectProperty(iObjUID, __GO_Y_NUMBER_TICKS__, jni_int, (void **)&piNy);

    /* draw an horizontal axis : YES (horizontal axis) or NO (vertical axis) */
    ishoriz = (nx > ny) ? TRUE : FALSE;

    if (ishoriz == TRUE)
    {
        getGraphicObjectProperty(iObjUID, __GO_X_TICKS_COORDS__, jni_double_vector, (void **)&val);
        nval = nx;
    }
    else
    {
        getGraphicObjectProperty(iObjUID, __GO_Y_TICKS_COORDS__, jni_double_vector, (void **)&val);
        nval = ny;
    }

    if (!val)
    {
        Scierror(999, _("%s: Cannot get coordinates.\n"), "ComputeXIntervals");
        return -1;
    }

    if (xy_type == 'v')
    {
        *N = n = nval;

        if ((*vector = (double *) MALLOC(n * sizeof(double))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "ComputeXIntervals");
            return -1;
        }

        for (i = 0; i < n; i++)
        {
            (*vector)[i] = val[i];
        }
    }
    else if (xy_type == 'r')
    {
        double step = 0;

        *N = n = (int)val[2] + 1; /* intervals number is given by  ppaxes->x or ppaxes->y */

        if (checkdim)
        {
            if (nval != 3)
            {
                Sciwarning(_("Warning: %s must be changed, %s is '%s' and %s dimension is not %d.\n"), "tics_coord", "xy_type", "r", "tics_coord", 3);
            }

            if (nval < 3)
            {
                Scierror(999, _("%s must be changed FIRST, %s is '%s' and %s dimension < %d.\n"), "tics_coord", "xy_type", "r", "tics_coord", 3);
                *vector = (double *) NULL;
                return -1;
            }
        }

        if ((*vector = (double *) MALLOC(n * sizeof(double))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "ComputeXIntervals");
            return -1;
        }

        step = (val[1] - val[0]) / (n - 1);

        for (i = 0; i < n - 1; i++)
        {
            (*vector)[i] = val[0] + i * step;
        }

        (*vector)[n - 1] = val[1]; /* xmax */

    }
    else if (xy_type == 'i')
    {
        double step = 0;

        *N = n = (int)val[3] + 1;

        if (checkdim)
        {
            if (nval != 4)
            {
                Sciwarning(_("Warning: %s must be changed, %s is '%s' and %s dimension is not %d.\n"), "tics_coord", "xy_type", "i", "tics_coord", 4);
            }

            if (nval < 4)
            {
                Scierror(999, _("%s must be changed FIRST, %s is '%s' and %s dimension < %d.\n"), "tics_coord", "xy_type", "i", "tics_coord", 4);
                *vector = (double *) NULL;
                return -1;
            }
        }

        if ((*vector = (double *)  MALLOC(n * sizeof(double))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "ComputeXIntervals");
            return -1;
        }

        step = (val[1] * exp10(val[2]) - val[0] * exp10(val[2])) / val[3];


        for (i = 0; i < n - 1; i++)
        {
            (*vector)[i] = val[0] * exp10(val[2]) + i * step;
        }

        (*vector)[n - 1] = val[1] * exp10(val[2]); /* xmax */

    }

    return 0;
}
/*--------------------------------------------------------------------------*/
/**
 * Compute the default labels of an axis from the positions of the ticks.
 * @param[in/out] pobjUID the axis object UID
 * @return a string matrix containing the labels.
 *         Actually it is a row vector.
 */
StringMatrix * computeDefaultTicsLabels(int iObjUID)
{
    StringMatrix * ticsLabels = NULL  ;
    int            nbTics     = 0     ;
    char           tempFormat[5]      ;
    char         * c_format   = NULL  ;
    double       * vector     = NULL   ; /* position of labels */
    char           curLabelBuffer[257];
    int            i = 0;

    int tmp = 0;
    int* piTmp = &tmp;
    char ticksStyle = 'v';

    getGraphicObjectProperty(iObjUID, __GO_FORMATN__, jni_string, (void **)&c_format);

    /*
     * If different from the empty string, the format is already specified,
     * if equal, it needs to be computed.
     */
    if (c_format && strcmp(c_format, "") == 0)
    {
        ComputeC_format(iObjUID, tempFormat);
        c_format = tempFormat;
    }

    getGraphicObjectProperty(iObjUID, __GO_TICKS_STYLE__, jni_int, (void **)&piTmp);

    if (tmp == 0)
    {
        ticksStyle = 'v';
    }
    else if (tmp == 1)
    {
        ticksStyle = 'r';
    }
    else if (tmp == 2)
    {
        ticksStyle = 'i';
    }

    /* vector is allocated here */
    if (ComputeXIntervals(iObjUID, ticksStyle, &vector, &nbTics, 1) != 0)
    {
        Scierror(999, _("Bad size in %s: you must first increase the size of the %s.\n"), "tics_coord", "tics_coord");
        return 0;
    }

    /* create a vector of strings */
    ticsLabels = newMatrix(1, nbTics);
    if (ticsLabels == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "computeDefaultTicsLabels");
        return NULL;
    }

    for (i = 0 ; i < nbTics ; i++)
    {
        sprintf(curLabelBuffer, c_format, vector[i]) ; /* we can't know for sure the size of the label */
        /* That's why it is first stored in a big array */
        copyStrMatElement(ticsLabels, 0, i, curLabelBuffer);
    }

    FREE(vector);
    vector = NULL;

    return ticsLabels;

}
/*--------------------------------------------------------------------------*/
/**
 * Create a new string which is the result the conversion of a double value
 * using a certain format
 * @param bufferSize size of the buffer used to store the store before the copying
 *                   it to the result. It must greater than the length of the returning string.
 *                   and ideally the same length.
 * @return the newly created strings, or NULL if an error occurred.
 */
static char * copyFormatedValue(double value, const char format[5], int bufferSize)
{
    char * buffer = (char*)MALLOC(bufferSize * sizeof(char));
    char * res = NULL;
    int resLength = 0;

    if (buffer == NULL)
    {
        return NULL;
    }

    sprintf(buffer , format, value);

    resLength =  (int)strlen(buffer) + 1 ; /* + 1 <=> 0 terminating char */

    res = (char*)MALLOC(resLength * sizeof(char));

    if (res == NULL)
    {
        FREE(buffer);
        return NULL;
    }

    strncpy(res, buffer, resLength);

    FREE(buffer);

    return res;
}
/*--------------------------------------------------------------------------*/
char ** copyFormatedArray(const double values[], int nbStrings, const char format[5], int bufferSize)
{
    int i = 0;
    char ** res = MALLOC(nbStrings * sizeof(char *));

    if (res == NULL)
    {
        return NULL;
    }

    for (i = 0 ; i < nbStrings ; i++)
    {
        res[i] = copyFormatedValue(values[i], format, bufferSize);
    }

    return res;

}
/*--------------------------------------------------------------------------*/
/**************************************************
* Global values which are set at this level and
* not redirected to each driver
**************************************************/

static char FPF[32] = {'\0'};

char * getFPF(void)
{
    return (FPF);
}
/*--------------------------------------------------------------------------*/
