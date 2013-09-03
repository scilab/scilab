/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Serge STEER
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    isequalbitwise and isequal built-ins definition
 *    Serge Steer,   INRIA 2007
 *    Comment:
 *    This file contains the routines used by the isequalbitwise and isequal  builtin functions
 --------------------------------------------------------------------------*/

#include "IsEqualVar.h"
#include "Scierror.h"
#include "BOOL.h"
#include "parse.h"

/* Scilab parser recursion data and functions*/
static int *Ids     = NULL;
static int *Rstk    = NULL;
static int *Pstk    = NULL;
#define Pt (C2F(recu).pt)

static void initStackParameters(void)
{
    Ids     = C2F(recu).ids - nsiz - 1;
    Rstk    = C2F(recu).rstk - 1;
    Pstk    = C2F(recu).pstk - 1;
}

/**intisequalvar
* Gateway for isequalbitwise and isequal builtins
* @param char * fname: the Scilab code of the function name
* @param int * job: if *job==0 the floating point numbers are compared bitwise ,
*      if *job==1 the comparison is made numerically,
*      so NaN elements are not equal
*      elements of with differents data types are raised to the upper types before comparison (to be done)
* @return 0 in any cases
* @author Serge Steer
* @see isequal
*/

int C2F(intisequalvar)(char * fname, int *job, long int fl)
{
    int topk, top1, srhs, k, kmin, l;
    int res = 42; /* Bruno : @TODO initialisation !!! */
    int one = 1;
    int l1, lk, il1, ilk;
    int n1, nk; //memory size used by the variable, only used for overloaded comparison

    initStackParameters();

    /*DEBUG_OVERLOADING("entering intisequal Top=%d, Rhs=%d, Rstk[pt]=%d\n",Top,Rhs,Rstk[Pt]);*/


    SetDoubleCompMode(*job); /* floating point numbers are compared bitwise */
    if (Rstk[Pt] == 914 || Rstk[Pt] == 915) /* coming back after evaluation of overloading function */
    {
        /*DEBUG_OVERLOADING("intisequal called back by the parser Top=%d, Rhs=%d, Pt=%d\n",Top,Rhs,Pt);*/

        /* Restore context */
        kmin = Ids[1 + Pt * nsiz];
        srhs = Ids[2 + Pt * nsiz];
        topk = Ids[3 + Pt * nsiz];
        top1 = Top - 1 - srhs + 1; /* Top-1 because Top has been increased to store the result of overloading function */
    }
    else
    {
        CheckRhs(2, 2000000);
        CheckLhs(1, 1);
        srhs = Rhs;
        top1 = Top - srhs + 1;
        topk = top1 + 1;
        kmin = 2;
        MaxRec = 0;
        Rrec = NULL;
    }

    l1 = *Lstk(top1);
    il1 = iadr(l1);
    n1 = *Lstk(top1 + 1) - l1;
    if (*istk(il1) < 0)
    {
        l1 = *istk(il1 + 1);
        n1 = *istk(il1 + 3);
    }
    for (k = kmin; k <= srhs; k++)
    {

        lk = *Lstk(topk);
        ilk = iadr(lk);
        nk = *Lstk(topk + 1) - lk;
        if (*istk(ilk) < 0)
        {
            lk = *istk(ilk + 1);
            nk = *istk(ilk + 3);
        }

        res = IsEqualVar(stk(l1), n1, stk(lk), nk);
        if (res == -1) /* overloading function evaluation required */
        {
            /* save context */
            Ids[1 + Pt * nsiz] = k;
            Ids[2 + Pt * nsiz] = srhs;
            Ids[3 + Pt * nsiz] = topk;
            return 0;
        }
        else if (res == -2)  /* Memory allocation failed */
        {
            SciError(112);
            FreeRec();
            return 0;
        }
        /*DEBUG_OVERLOADING("k=%d, res=%d\n", k,res);*/

        if (res == 0)
        {
            /* goto END; */
            Top = top1;
            C2F(crebmat)(fname, &top1, &one, &one, &l, (unsigned long)strlen(fname));
            *istk(l) = res;
            FreeRec();

            return 0;
        }
        topk++;
    }


    /* END:*/
    Top = top1;
    C2F(crebmat)(fname, &top1, &one, &one, &l, (unsigned long)strlen(fname));
    *istk(l) = res;
    FreeRec();

    return 0;
}

/**IsEqualOverloaded
* Used to call the overloading function when testing unknown data type  for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param int n1: memory size used by the first variable, only used for overloading
* @param double *d2: pointer on the beginning of the first variable structure
* @param int n2: memory size used by the second variable, only used for overloading
* @return 0 is the variables differ and 1 if they are identical, -1 for recursion purpose
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualOverloaded(double *d1, int n1, double *d2, int n2)
{
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;
    int il, lw;
    int l1, l2;

    initStackParameters();

    if (Rstk[Pt] == 914 || Rstk[Pt] == 915) /* coming back after evaluation of overloading function */
    {
        /* Get the computed value */
        il = iadr(*Lstk(Top));
        Top--;
        Pt--;
        return  *istk(il + 3);
    }

    /* Prepare stack for calling overloading function */
    /* put references to d1 and d2 variable at the top of the stack */
    l1 = *Lstk(1) + (int)(d1 - stk(*Lstk(1))); /*compute index in stk from absolute adress value */
    l2 = *Lstk(1) + (int)(d2 - stk(*Lstk(1))); /*compute index in stk from absolute adress value */

    Top = Top + 1;

    il = iadr(*Lstk(Top));
    *istk(il) = -id1[0];
    *istk(il + 1) = l1; /* index othe first element of the variable in stk */
    *istk(il + 2) = 0; /* variable number unknown */
    *istk(il + 3) = n1; /* variable memory size  */
    *Lstk(Top + 1) = *Lstk(Top) + 2;

    Top = Top + 1;
    il = iadr(*Lstk(Top));
    *istk(il) = -id2[0];
    *istk(il + 1) = l2; /* index othe first element of the variable in stk */
    *istk(il + 2) = 0; /*variable number unknown */
    *istk(il + 3) = n2; /*variable memory size */
    *Lstk(Top + 1) = *Lstk(Top) + 2;

    Ptover(1);
    Rhs = 2;
    lw = Top - 1;

    if ( GetDoubleCompMode() == 0)
    {
        C2F(overload)(&lw, "isequalbitwise", 14L);
        Rstk[Pt] = 914;
    }
    else
    {
        C2F(overload)(&lw, "isequal", 7L);
        Rstk[Pt] = 915;
    }

    /*DEBUG_OVERLOADING("IsEqualVar Overloaded calls the parser Top=%d, Rhs=%d, Pt=%d\n",Top,Rhs,Pt);*/

    return -1;
}

/**IsEqualVar
* Driver used to test a couple of Scilab variable for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param int n1: memory size used by the first variable, only used for overloading
* @param double *d2: pointer on the beginning of the first variable structure
* @param int n2: memory size used by the second variable, only used for overloading
* @return 0 is the variables differ and 1 if they are identical, -1 for recursion purpose, -2 for allocatopn problem
* @author Serge Steer
* @see intisequal
*/
int IsEqualVar(double *d1, int n1, double *d2, int n2)
{
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;
    int res;

    /*DEBUG_BASE("IsEqualVar %d %d\n",id1[0],id2[0]);*/

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    switch (id1[0])
    {
        case 0: /* null */
            return 1;
        case sci_matrix: /* matrix of double precision floating point numbers */
            if ( !IsEqualDoubleMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_poly:/* matrix of polynomials */
            if ( !IsEqualPolyMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_boolean:/* matrix of booleans */
            if ( !IsEqualBoolMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_sparse:/* sparse matrix of double */
            if ( !IsEqualDoubleSparseMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_boolean_sparse:/* sparse matrix of booleans */
            if ( !IsEqualBoolSparseMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_matlab_sparse:/* matlab sparse matrix  */
            if ( !IsEqualMatlabSparseMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_ints : /* matrix of short integers */
            if ( !IsEqualIntegerMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_handles : /* matrix of graphic handles */
            if ( !IsEqualDoubleMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_strings:/* matrix of strings */
            if ( !IsEqualStringMat(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_u_function:/* Uncompiled function */
        case sci_c_function:/* compiled function */
            if ( !IsEqualFunction(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_lib:/* library */
            if ( !IsEqualLib(d1, d2) )
            {
                return 0;
            }
            break;
        case sci_list: /* list */
        case sci_tlist: /* tlist */
        case sci_mlist: /* mlist */
            res = IsEqualList(d1, d2);
            if ( !res )
            {
                return 0;
            }
            if (res < 0)
            {
                return res;
            }
            break;
        case sci_lufact_pointer: /* lufact pointer */
            if ( !IsEqualLUPtr(d1, d2) )
            {
                return 0;
            }
            break;
        default :
            res = IsEqualOverloaded(d1, n1, d2, n2);
            if ( !res )
            {
                return 0;
            }
            if (res == -1)
            {
                return -1;
            }
    }

    return 1;
}


/**IsEqualList
* Used to test a couple of Scilab variable of type list, tlist or mlist for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical, -1 for recursion purpose, -2 for allocatopn problem
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualList(double *d1, double *d2)
{
    /* This code does not use simple recursion, because of possible need of
    * call to Scilab for evaluation of overloading function
    * The redusion is emulated using the Rrec data structure to memorize the path
    * to the current element.
    */
    int l, k, res, nelt;
    int *id1, *id2;
    int *ip1, *ip2;
    double *p1, *p2;
    int krec;

    initStackParameters();

    if (Rstk[Pt] == 914 || Rstk[Pt] == 915) /* coming back after evaluation of overloading function */
    {
        /* Restore context */
        krec = Pstk[Pt];
        MaxRec = Ids[4 + Pt * nsiz] ;
        memcpy(&Rrec, &(Ids[5 + Pt * nsiz]), sizeof(RecursionRecordPtr)); /* recover Rrec pointer */
        k = Rrec[krec].k;
        d1 = Rrec[krec].d1;     /* pointer on the sub-level list 1*/
        d2 = Rrec[krec].d2;     /* pointer on the sub-level list 2*/
        id1 = (int *) d1;
        id2 = (int *) d2;
        nelt = id1[1];
        goto SETLEVEL;
    }
    else   /* regular entry */
    {
        krec = 0;

    }

STARTLEVEL:
    /* the objects pointed to by d1 and d2 are lists */
    /* set current level context */
    if (AllocRecIfRequired(krec) == -2)
    {
        return -2;
    }

    Rrec[krec].d1 = d1;
    Rrec[krec].d2 = d2;
    Rrec[krec].k  = 0;

    /* check the type */
    id1 = (int *) d1;
    id2 = (int *) d2;
    if ((id1[0] != id2[0]))
    {
        return 0;
    }
    /* check the number of elements */
    if (id1[1] != id2[1])
    {
        return 0;
    }
    nelt = id1[1];

    /* check the array of "pointers" on list elements*/
    if (!IsEqualIntegerArray(nelt + 1, id1 + 2, id2 + 2))
    {
        return 0;
    }

    /*DEBUG_LIST("STARTLEVEL nelt=%d\n",nelt);*/

    k = 0;
SETLEVEL:
    /* check the list elements */
    ip1 = id1 + 2;
    ip2 = id2 + 2;

    l = (nelt + 4) / 2; /* the beginning of first field in the double array */
    p1 = d1 + l;
    p2 = d2 + l;

ELEMENT:
    if (k >= nelt)   /* no more element to compare */
    {
        if (krec > 0 )   /* end of a sub-level */
        {
            /* restore upper level context*/
            krec--;
            /*DEBUG_LIST("Sublist ELEMENT  index=%d finished, previous restored from krec=%d\n",k+1,krec);*/

            d1 = Rrec[krec].d1;
            d2 = Rrec[krec].d2;
            k =  Rrec[krec].k + 1;
            /* rebuild pointers */
            id1 = (int *) d1;
            id2 = (int *) d2;
            nelt = id1[1];
            /*DEBUG_LIST("back to lower level nelt=%d  index=%d krec=%d\n",nelt,k+1,krec);*/

            goto  SETLEVEL;
        }
        else /* end of main level */
        {
            return 1;
        }
    }
    /* compare next element */
    if (ip1[k] == ip1[k + 1]) /* undefined element nothing to check */
    {
        k++;
        goto ELEMENT;
    }
    d1 = p1 + ip1[k] - 1;
    d2 = p2 + ip2[k] - 1;
    id1 = (int *)d1;

    if (id1[0] != 15 && id1[0] != 16 && id1[0] != 17) /* elements which are not lists */
    {
        res = IsEqualVar(d1, ip1[k + 1] - ip1[k], d2, ip2[k + 1] - ip2[k]);
        /*DEBUG_LIST("Regular ELEMENT  index=%d res=%d\n",k+1,res);*/
        if (!res)
        {
            return 0;
        }
        if (res == -1)   /*overloading function evaluation required */
        {
            /* preserve context */
            Pstk[Pt] = krec;
            Ids[4 + Pt * nsiz] = MaxRec;
            /* Store Rrec pointer into Ids[5 + Pt * nsiz] and Ids[6 + Pt * nsiz] */
            memcpy(&(Ids[5 + Pt * nsiz]), &Rrec, sizeof(RecursionRecordPtr));
            return -1;
        }
        k++;
        goto ELEMENT;

    }
    else   /* sub list found*/
    {
        /*DEBUG_LIST("Sublist ELEMENT  index=%d started, previous stored in krec=%d\n",k+1,krec);*/

        Rrec[krec].k  = k;
        krec++;

        goto STARTLEVEL;
    }
}


/**IsEqualLib
* Used to test a couple of Scilab variable of type library (14) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualLib(double *d1, double *d2)
{
    int n, l;
    int nclas = 29;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the path length */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the path" */
    n = id1[1];
    if (!IsEqualIntegerArray(n, id1 + 2, id2 + 2))
    {
        return 0;
    }
    l = n + 2;

    /* Check the number of names */
    if (id1[l] != id2[l])
    {
        return 0;
    }
    n = id1[l];
    l++;

    /* check the table */
    if (!IsEqualIntegerArray(nclas, id1 + l, id2 + l))
    {
        return 0;
    }
    l += nclas;
    /* Check the sequence of names */
    if (!IsEqualIntegerArray(n * nsiz, id1 + l, id2 + l))
    {
        return 0;
    }

    return 1;
}

/**IsEqualDoubleMat
* Used to test a couple of Scilab variable of type 1 (matrix of floating point numbers)
* or 9 (graphic handles) for equality*
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualDoubleMat(double *d1, double *d2)
{
    int n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the real/complex flag */
    if (id1[3] != id2[3])
    {
        return 0;
    }


    n = id1[1] * id1[2] * (id1[3] + 1); /* number of double precision floating point numbers */
    /* check the array of numbers */
    if (!IsEqualDoubleArray(n, d1 + 2, d2 + 2))
    {
        return 0;
    }

    return 1;
}
/**IsEqualIntegerMat
* Used to test a couple of Scilab variable of type 8 (int)  for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualIntegerMat(double *d1, double *d2)
{
    int n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the subtype */
    if (id1[3] != id2[3])
    {
        return 0;
    }


    n = id1[1] * id1[2]; /* number of double precision floating point numbers */
    /* check the array of numbers */
    if (!IsEqualShortIntegerArray(id1[3], n, id1 + 4, id2 + 4))
    {
        return 0;
    }

    return 1;
}

/**IsEqualBoolMat
* Used to test a couple of Scilab variable of type 4 (boolean) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualBoolMat(double *d1, double *d2)
{
    int n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the data */
    n = id1[1] * id1[2]; /* number of double precision floating point numbers */
    /* check the array of numbers */
    if (!IsEqualIntegerArray(n, id1 + 3, id2 + 3))
    {
        return 0;
    }
    return 1;
}
/**IsEqualStringMat
* Used to test a couple of Scilab variable of type 10 (string) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualStringMat(double *d1, double *d2)
{
    int n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the array of "pointers" */
    n = id1[1] * id1[2];
    if ( !IsEqualIntegerArray(n + 1, id1 + 4, id2 + 4) )
    {
        return 0;
    }

    /* Check the array of character codes (int)  */
    if (!IsEqualIntegerArray(id1[4 + n] - 1, id1 + 5 + n, id2 + 5 + n))
    {
        return 0;
    }
    return 1;
}


/**IsEqualPolyMat
* Used to test a couple of Scilab variable of type 2 (matrix of polynomials) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualPolyMat(double *d1, double *d2)
{
    int l, n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the real/complex flag */
    if (id1[3] != id2[3])
    {
        return 0;
    }
    /* Check the formal variable name */
    if ( !IsEqualIntegerArray(4, id1 + 4, id2 + 4) )
    {
        return 0;
    }


    /* Check the array of "pointers" */
    n = id1[1] * id1[2];
    if ( !IsEqualIntegerArray(n, id1 + 8, id2 + 8) )
    {
        return 0;
    }

    /* Check the array of double precision numbers */
    l = (n + 10) / 2; /* the beginning of first field in th double array */

    /* check the array of numbers */
    if ( !IsEqualDoubleArray(id1[8 + n] - 1, d1 + l, d2 + l) )
    {
        return 0;
    }

    return 1;
}

/**IsEqualDoubleSparseMat
* Used to test a couple of Scilab variable of type 5 (sparse matrix) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualDoubleSparseMat(double *d1, double *d2) /* a faire */
{
    int l, nel;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the real/complex flag */
    if (id1[3] != id2[3])
    {
        return 0;
    }

    /* Check the number of non zero elements */
    if (id1[4] != id2[4])
    {
        return 0;
    }
    nel = id1[4];
    l = 5;
    /* Check the array of number of non zero element per row */
    if ( !IsEqualIntegerArray(id1[1], id1 + l, id2 + l) )
    {
        return 0;
    }
    l += id1[1];

    /* Check the column index of non zero elements */
    if ( !IsEqualIntegerArray(nel, id1 + l, id2 + l) )
    {
        return 0;
    }
    l += nel;

    /* Check the non zero elements */
    l = (l + 1) / 2;
    if ( !IsEqualDoubleArray(nel * (id1[3] + 1), d1 + l, d2 + l) )
    {
        return 0;
    }

    return 1;
}

/**IsEqualMatlabSparseMat
* Used to test a couple of Scilab variable of type 7 (Matlab sparse matrix) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualMatlabSparseMat(double *d1, double *d2) /* a faire */
{
    int l, nel;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the real/complex flag */
    if (id1[3] != id2[3])
    {
        return 0;
    }

    /* Check the number of non zero elements */
    if (id1[4] != id2[4])
    {
        return 0;
    }
    nel = id1[4];
    l = 5;
    /* Check the array of number of non zero element per column */
    if ( !IsEqualIntegerArray(id1[2], id1 + l, id2 + l) )
    {
        return 0;
    }
    l += id1[2];

    /* Check the column index of non zero elements */
    if ( !IsEqualIntegerArray(nel, id1 + l, id2 + l) )
    {
        return 0;
    }
    l += nel;

    /* Check the non zero elements */
    l = (l + 1) / 2;
    if ( !IsEqualDoubleArray(nel * (id1[3] + 1), d1 + l, d2 + l) )
    {
        return 0;
    }

    return 1;
}

/**IsEqualBoolSparseMat
* Used to test a couple of Scilab variable of type 6 (Boolean sparse matrix) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualBoolSparseMat(double *d1, double *d2) /* a faire */
{
    int l, nel;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }


    /* Check the number of non zero elements */
    if (id1[4] != id2[4])
    {
        return 0;
    }
    nel = id1[4];
    l = 5;
    /* Check the array of number of non zero element per row */
    if ( !IsEqualIntegerArray(id1[1], id1 + l, id2 + l) )
    {
        return 0;
    }
    l += id1[1];

    /* Check the column index of non zero elements */
    if ( !IsEqualIntegerArray(nel, id1 + l, id2 + l) )
    {
        return 0;
    }

    return 1;
}

/**IsEqualFunction
* Used to test a couple of Scilab variable of type 11 or 13 (function) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualFunction(double *d1, double *d2)
{
    int l, n;
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }
    l = 1;
    /* Check the number of output args */
    if (id1[l] != id2[l])
    {
        return 0;
    }
    /* Check the output args names*/
    n = id1[l];
    l++;
    if ( !IsEqualIntegerArray(n * nsiz, id1 + l, id2 + l) )
    {
        return 0;
    }
    l += n * nsiz;

    /* Check the number of input args */
    if (id1[l] != id2[l])
    {
        return 0;
    }
    /* Check the input args names*/
    n = id1[l];
    l++;
    if ( !IsEqualIntegerArray(n * nsiz, id1 + l, id2 + l) )
    {
        return 0;
    }
    l += n * nsiz;

    /* Check the number of int in instructions */
    if (id1[l] != id2[l])
    {
        return 0;
    }
    n = id1[l];
    l++;
    if ( !IsEqualIntegerArray(n, id1 + l, id2 + l) )
    {
        return 0;
    }


    return 1;
}

/**IsEqualLUPtr
* Used to test a couple of Scilab variable of type 128  (pointer on LU factorization) for equality
* @param double *d1: pointer on the beginning of the first variable structure
* @param double *d2: pointer on the beginning of the first variable structure
* @return 0 is the variables differ and 1 if they are identical
* @author Serge Steer
* @see IsEqualVar
*/
int IsEqualLUPtr(double *d1, double *d2)
{
    int *id1 = (int *) d1;
    int *id2 = (int *) d2;

    /* Check the type */
    if ((id1[0] != id2[0]))
    {
        return 0;
    }

    /* Check the number of rows */
    if (id1[1] != id2[1])
    {
        return 0;
    }

    /* Check the number of columns */
    if (id1[2] != id2[2])
    {
        return 0;
    }

    /* Check the real/complex flag */
    if (id1[3] != id2[3])
    {
        return 0;
    }

    /* Check the pointer value */
    if (d1[2] != d2[2])
    {
        return 0;
    }

    return 1;
}

/**IsEqualDoubleArrayIEEE
* compare if two double precision arrays of size n, are identical.
* NaN entries are supposed to be different from all values included NaN
* NaN != NaN
* @param int n: array size
* @param double *d1: pointer on the beginning of the first array
* @param double *d2: pointer on the beginning of the second array
* @return 0 is the arrays differ and 1 if they are identical
* @author Serge Steer
*/
int IsEqualDoubleArrayIEEE(int n, double *d1, double *d2)
{
    int i;
    /*DEBUG_BASE("IEEE comparison of %d doubles\n",n);*/

    if (n == 0)
    {
        return 1;
    }
    for (i = 0; i < n; i++)
    {
        if (d1[i] != d2[i])
        {
            return 0;
        }
    }
    return 1;
}

/** IsEqualDoubleArrayBinary
* compare if two  arrays of long long integers of size n, are identical.
* @param int n: array size
* @param long long *d1: pointer on the beginning of the first array
* @param long long *d2: pointer on the beginning of the second array
* @return 0 is the arrays differ and 1 if they are identical
* @author Serge Steer
*/
int IsEqualDoubleArrayBinary(int n, double *d1, double *d2)
{
    int i;
    long long *l1 = (long long *)d1;
    long long *l2 = (long long *)d2;

    /*DEBUG_BASE("binary comparison of %d doubles\n",n);*/

    if (n == 0)
    {
        return 1;
    }
    for (i = 0; i < n; i++)
    {
        if (l1[i] != l2[i])
        {
            return 0;
        }
    }
    return 1;
}


/**IsEqualDoubleArray
* compare if two double precision arrays of size n, are identical.
* If the arrays conatins NaN the meaning depends on the value of the global flag IEEE_comp
*  - if DoubleCompMode==1, double numbers are compared using "==", so Nan != NaN.
*  - if DoubleCompMode==0, double numbers are compared bitwise.
* @param int n: array size
* @param double *d1: pointer on the beginning of the first array
* @param double *d2: pointer on the beginning of the second array
* @return 0 is the arrays differ and 1 if they are identical
* @author Serge Steer
*/
int IsEqualDoubleArray(int n, double *d1, double *d2)
{
    if ( GetDoubleCompMode())
    {
        return IsEqualDoubleArrayIEEE(n, d1, d2);
    }
    else
    {
        return IsEqualDoubleArrayBinary(n, d1, d2);
    }
}



/**IsEqualIntegerArray
* compare if two int arrays of size n, are identical
* @param int n: array size
* @param int *d1: pointer on the beginning of the first array
* @param int *d2: pointer on the beginning of the second array
* @return 0 is the arrays differ and 1 if they are identical
* @author Serge Steer
*/
int IsEqualIntegerArray(int n, int *d1, int *d2)
{
    int i;

    /*DEBUG_BASE("comparison of %d ints\n",n);*/

    if (n == 0)
    {
        return 1;
    }
    for (i = 0; i < n; i++)
    {
        if (d1[i] != d2[i])
        {
            return 0;
        }
    }
    return 1;
}

typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define ISEQUAL(Type) {\
    Type *A;\
    Type *B;\
    A=(Type *)d1;\
    B=(Type *)d2;\
    for (i = 0; i <n; ++i) {\
    if (A[i] != B[i]) return 0;\
    }\
}


/**IsEqualShortIntegerArray
* compare if two short int (1,2 or 4 bytes) arrays of size n, are identical
* @param int type: int type 1, 2, 4 or 11, 12, 14 for unsigned int
* @param int n: array size
* @param int *d1: pointer on the beginning of the first array
* @param int *d2: pointer on the beginning of the second array
* @return 0 is the arrays differ and 1 if they are identical
* @author Serge Steer
*/
int IsEqualShortIntegerArray(int typ, int n, int *d1, int *d2)
{
    int i;

    /*DEBUG_BASE("comparison of %d int %d bytes\n",n,typ);*/
    if (n == 0)
    {
        return 1;
    }
    switch (typ)
    {
        case 0:
            ISEQUAL(double);
            break;
        case 1:
            ISEQUAL(integer1);
            break;
        case 2:
            ISEQUAL(integer2);
            break;
        case 4:
            ISEQUAL(int) ;
            break;
        case 11:
            ISEQUAL(unsigned char);
            break;
        case 12:
            ISEQUAL(unsigned short);
            break;
        case 14:
            ISEQUAL(unsigned int);
            break;
    }

    return 1;
}

/**FreeRec
* Utility function to free the list recursion table
* @author Serge Steer
*/
void FreeRec(void)
{
    if ( MaxRec > 0 )
    {
        FREE(Rrec);
        Rrec = NULL;
        MaxRec = 0;
    }
}

/** AllocRecIfRequired
* Utility function to allocate or reallocate the list recursion table
* @param int krec: minimum size requested
* @author Serge Steer
*/
int AllocRecIfRequired(int krec)
{
    /*Allocation is made by block of size 10 */
    if (MaxRec <= krec)
    {
        if ((Rrec = (RecursionRecordPtr)REALLOC(Rrec, (MaxRec + 10) * sizeof(RecursionRecord))) == NULL)
        {
            return -2;
        }
        MaxRec = MaxRec + 10;
    }
    return 0;
}

/**SetDoubleCompMode
* Utility function used to set the way double numbers are compared
* @param int mode. 1 means that IEEE comparison is used, 0 means binary comparison
* @author Serge Steer
*/

void SetDoubleCompMode(int mode)
{
    DoubleCompMode = mode;
}
/**GetDoubleCompMode
* Utility function used to get the way double numbers are compared
* @return 1 means that IEEE comparison is used, 0 means binary comparison
* @author Serge Steer
*/

int GetDoubleCompMode(void)
{
    return DoubleCompMode;
}
