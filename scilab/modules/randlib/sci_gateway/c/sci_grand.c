
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) ENPC
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------
*    Interface for grand
*    jpc@cermics.enpc.fr
*    stuff to deal with several generators added
*         by Bruno Pincon (12/11/2001)
*
--------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
#endif
#include "localization.h"
#include "stack-c.h"

/** external functions to be called through this interface **/

#include "grand.h"
#include "clcg4.h"
#include "others_generators.h"
#include "sciprint.h"
#include "Scierror.h"
#include "gw_randlib.h"

enum {MT, KISS, CLCG4, CLCG2, URAND, FSULTRA};

/* the current generator : */
static int current_gen = MT;

/* for clcg4 : the current virtual gen (current_clcg4 in [0, Maxgen]) */
static int current_clcg4 = 0;

/* clcg4 must be called with the virtual generator number */
static unsigned long int clcg4_with_gen(void)
{
    return ( clcg4(current_clcg4) );
}

#define NbGenInScilab 6

/*  pointers onto the generators func */
unsigned long int (*gen[NbGenInScilab])() = { randmt, kiss,  clcg4_with_gen, clcg2 , urandc , fsultra};

/*  names at the scilab level */
static char *names_gen[NbGenInScilab] = { "mt",  "kiss","clcg4", "clcg2", "urand", "fsultra" };

/* all the generators provided integers in [0, RngMaxInt] :        */
static
unsigned long RngMaxInt[NbGenInScilab] = { 4294967295ul,  /* mt    */
4294967295ul,  /* kiss  */
2147483646ul,  /* clcg4 */
2147483561ul,  /* clcg2 */
2147483647ul,  /* urand */
4294967295ul}; /* fsultra*/
/* the factors (1/(RngMaxInt+1)) to get reals in [0,1) :           */
static
double factor[NbGenInScilab] = { 2.3283064365386963e-10,  /* mt    */
2.3283064365386963e-10,  /* kiss  */
4.6566128752457969e-10,  /* clcg4 */
4.6566130595601735e-10,  /* clcg2 */
4.6566128730773926e-10,  /* urand */
2.3283064365386963e-10}; /* fsultra*/

double C2F(ranf)(void)
{
    /* random deviate from U[0,1) */
    return ( (double) gen[current_gen]() * factor[current_gen] );
}

double ignlgi(void)
{
    /* random deviate from Ui[0,RngMaxInt] (direct output of the current gen) */
    return ( (double) gen[current_gen]() );
}

double C2F(ignuin)(double *a, double *b)
{
    /*  random deviate from Ui[a,b]
    *  it is assumed that : (i)  a and b are integers (stored in double)
    *                       (ii) b-a+1 <= RngMaxInt[current_gen]
    *  (these verif are done at the calling level)
    *
    *  We use the classic method with a minor difference : to choose
    *  uniformly an int in [a,b] (ie d=b-a+1 numbers) with a generator
    *  which provides uniformly integers in [0,RngMaxInt] (ie m=RngMaxInt+1
    *  numbers) we do the Euclidian division :
    *                                           m = q d + r,   r in [0,d-1]
    *
    *  and accept only numbers l in [0, qd-1], then the output is k = a + (l mod d)
    *  (ie numbers falling in [qd , RngMaxInt] are rejected).
    *  The problem is that RngMaxInt is 2^32-1 for mt and kiss so that RngMaxInt+1 = 0
    *  with the 32 bits unsigned int arithmetic. So in place of rejected r
    *  numbers we reject r+1 by using RngMaxInt in place of m. The constraint is
    *  then that (b-a+1) <= RngMaxInt and if we doesn't want to deal we each generator
    *  we take (b-a+1) <= Min RngMaxInt =  2147483561 (clcg2)
    */
    unsigned long k, d = (unsigned long)((*b-*a)+1), qd;

    if ( d == 1)
        return (*a);

    qd = RngMaxInt[current_gen] - RngMaxInt[current_gen] % d;
    do
    {
        k = (unsigned long)ignlgi();
    }
    while ( k >= qd );
    return ( *a + (double)(k % d) );
}

/**************************************************
*  hand written interface for the randlib
***********************************************************************/

int sci_Rand(char *fname,unsigned long fname_len)
{
    int minrhs = 1,maxrhs = 10,minlhs=1,maxlhs=2;
    int ResL,ResC,suite,m2,n2,l2,m1,n1,l1,ls,ms,ns,la,lr,lb,lc;
    int l3,l4;
    int i;

    Nbvars = 0;
    CheckRhs(minrhs,maxrhs);
    CheckLhs(minlhs,maxlhs);
    if ( GetType(1) != sci_matrix)
    {
        int un=1,deux=2, dim_state_mt=625, dim_state_fsultra = 40, dim_state_4=4;
        GetRhsVar(1,STRING_DATATYPE,&ms,&ns,&ls);
        if ( strcmp(cstk(ls),"getsd")==0)
        {
            if ( Rhs != 1 )
            {
                Scierror(999,_("%s: Wrong number of input argument: %d expected with option '%s'.\n"),fname,1,"getsd");
                return 0;
            }
            if ( Lhs != 1 )
            {
                Scierror(999,_("%s: Wrong number of output argument: %d expected the option '%s'.\n"),fname,1,"getsd");
                return 0;
            }

            switch(current_gen)
            {
            case(MT) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&dim_state_mt,&un,&lr);
                get_state_mt(stk(lr));
                break;
            case(KISS) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&dim_state_4,&un,&lr);
                get_state_kiss(stk(lr));
                break;
            case(CLCG4) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&dim_state_4,&un,&lr);
                get_state_clcg4(current_clcg4, stk(lr));
                break;
            case(CLCG2) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&deux,&un,&lr);
                get_state_clcg2(stk(lr));
                break;
            case(URAND) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr);
                get_state_urand(stk(lr));
                break;
            case(FSULTRA) :
                CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&dim_state_fsultra,&un,&lr);
                get_state_fsultra(stk(lr));
                break;
            };
            LhsVar(1) = Rhs+2;
            PutLhsVar();
            return 0;
        }
        else if ( strcmp(cstk(ls),"setall")==0 )
        {
            if ( current_gen != CLCG4 )
                sciprint(_("The %s option affects only the clcg4 generator\n"),"setall");
            if ( Rhs != 5 )
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,5,"setall");
                return 0;
            }
            GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
            if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname);
            return 0;}
            GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);
            if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for third input argument: Scalar expected.\n"),fname);
            return 0;}
            GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);
            if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for fourth input argument: Scalar expected.\n"),fname);
            return 0;}
            GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);
            if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for fifth input argument: Scalar expected.\n"),fname);
            return 0;}

            if (! set_initial_seed_clcg4(*stk(l1),*stk(l2), *stk(l3), *stk(l4)) )
            {   /* => seeds were not good  (info is displayed by the function) */
                SciError(999);return 0;
            }
            LhsVar(1) = 1;
            PutLhsVar();
            return(0);
        }
        else if ( strcmp(cstk(ls),"setsd")==0 )
        {
            switch(current_gen)
            {
            case(MT) :
                if ( Rhs != 2 )
                {
                    Scierror(999,_("%s: Wrong number of input arguments: %d expected for '%s' with the %s generator.\n"),fname,2,"setall","mt");
                    return 0;
                }
                GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                if ( m1*n1 == 1)          /* simple init of mt     */
                { if (! set_state_mt_simple(*stk(l1)) ) {SciError(999); return(0);}; }
                else if ( m1*n1 == 625 )  /* init of all the state */
                { if (! set_state_mt(stk(l1))) {SciError(999); return(0);}; }
                else
                {
                    Scierror(999,_("%s: Wrong values for input argument: Vector of %d or %d values for %s expected.\n"),fname,1, 625,"mt");
                    return 0;
                };
                break;

            case(FSULTRA) :
                if ( Rhs == 2 ) /* init via a "complete" state */
                {
                    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                    if ( m1 != 40  ||  n1 != 1)
                    {
                        Scierror(999,_("%s: Wrong size for second input argument: %dx%d expected.\n"),fname,40,1);
                        return 0;
                    };
                    if (! set_state_fsultra(stk(l1)) ) {SciError(999); return(0);};
                }
                else if ( Rhs == 3 ) /* init with 2 integers (like before) */
                {
                    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                    if ( m1*n1 != 1)
                    { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname); return 0;};
                    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);
                    if ( m1*n1 != 1)
                    { Scierror(999,_("%s: Wrong type for third input argument: Scalar expected.\n"),fname); return 0;};
                    if (! set_state_fsultra_simple(*stk(l1),*stk(l2)) ) {SciError(999); return(0);};
                }
                else
                {
                    Scierror(999,_("%s: Wrong number of input arguments: %d or %d expected for '%s' option with the %s generator.\n"),fname,2,3,"setsd","fsultra");
                    return 0;
                }
                break;

            case(KISS) :
            case(CLCG4) :
                if ( Rhs != 5 )
                {
                    Scierror(999,_("%s: Wrong number of input arguments: expected %d for '%s' option with the %s or %s generator.\n"),fname,5,"setsd","kiss","clcg4");
                    return 0;
                }
                GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname); return 0;}
                GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for third input argument: Scalar expected.\n"),fname); return 0;}
                GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for fourth input argument: Scalar expected.\n"),fname); return 0;}
                GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for fifth input argument: Scalar expected.\n"),fname); return 0;}
                if (current_gen == KISS)
                {if (! set_state_kiss(*stk(l1),*stk(l2),*stk(l3),*stk(l4))) {SciError(999); return 0;};}
                else
                {if (! set_seed_clcg4(current_clcg4,*stk(l1),*stk(l2),*stk(l3),*stk(l4)))
                {SciError(999); return 0;};}
                break;

            case(CLCG2) :
                if ( Rhs != 3 )
                {
                    Scierror(999,_("%s: Wrong number of input arguments: %d expected for '%s' option with the %s generator.\n"),fname,3,"setsd","clcg2");
                    return 0;
                }
                GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname); return 0;};
                GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for third input argument: Scalar expected.\n"),fname); return 0;};
                if (! set_state_clcg2(*stk(l1),*stk(l2)))
                { SciError(999); return 0;};
                break;

            case(URAND) :
                if ( Rhs != 2 )
                {
                    Scierror(999,_("%s: Wrong number of input arguments: %d expected for '%s' option with the %s generator.\n"),fname,2,"setsd","urand");
                    return 0;
                }
                GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
                if ( m1*n1 != 1)
                { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname); return 0;};
                if (! set_state_urand(*stk(l1)))
                {SciError(999); return 0;};
                break;
            };
            LhsVar(1) = 0;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("phr2sd",cstk(ls)) == 0)
        {
            if ( Rhs != 2 )
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,2,"phr2sd");
                return 0;
            }
            if ( Lhs > 1 ) 
            {
                Scierror(999,_("%s: Wrong number of output argument: %d expected with option '%s'.\n"),fname,1,"phr2sd");

                return 0;
            }
            GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
            CreateVar(3,MATRIX_OF_INTEGER_DATATYPE,&un,&deux,&l2);

            C2F(phrtsd)(cstk(l1),&m1,istk(l2),istk(l2+1),m1);
            LhsVar(1) = 3;
            PutLhsVar();
            return 0;
        }

        else if (strcmp("initgn",cstk(ls))==0)
        {
            SeedType Where;
            if ( current_gen != CLCG4 )
                sciprint(_("%s: The %s option affects only the %s generator\n"),fname,"initgn","clcg4");
            if ( Rhs != 2)
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,2,"initgn");
                return 0;
            }
            GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
            if ( *istk(l1) != 0 && *istk(l1)!= -1 && *istk(l1) != 1)
            {
                Scierror(999,_("%s: Wrong value for second input argument: %d, %d or %d expected.\n"),fname, -1, 0, 1);
                return 0;
            }
            Where = (SeedType) (*istk(l1) + 1);
            init_generator_clcg4(current_clcg4, Where);
            LhsVar(1) = 2;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("setcgn",cstk(ls))==0)
        {
            if ( current_gen != CLCG4 )
                sciprint(_("The %s option affects only the %s generator\n"),"setcgn","clcg4");
            if ( Rhs != 2)
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,2,"setcgn");
                return 0;
            }
            GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
            if ( *istk(l1) < 0 || *istk(l1) > Maxgen )
            {
                Scierror(999,_("%s: Wrong value for second input argument: Must be between %d and %d.\n"),fname,0, Maxgen);
                return 0;
            }
            current_clcg4 = *istk(l1);
            LhsVar(1) = 2;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("advnst",cstk(ls))==0)
        {
            int k;
            if ( current_gen != CLCG4 )
                sciprint(_("The %s option affects only the %s generator\n"),"advnst","clcg4");
            if ( Rhs != 2)
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,2,"advnst");
                return 0;
            }
            GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
            k = *istk(l1);
            if ( k < 1 )
            {
                Scierror(999,_("%s: Wrong value for second input argument: Must be > %d.\n"),fname,0);
                return 0;
            }
            advance_state_clcg4(current_clcg4, k);
            LhsVar(1) = 2;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("getcgn",cstk(ls))==0)
        {
            if ( Rhs != 1)
            {
                Scierror(999,_("%s: Wrong number of input argument: %d expected with option '%s'.\n"),fname,1,"getcgn");
                return 0;
            }
            if ( current_gen != CLCG4 )
                sciprint(_("This information concerns only the clcg4 generator\n"));
            CreateVar(2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l1);
            *istk(l1) = current_clcg4;
            LhsVar(1) = 2;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("setgen",cstk(ls))==0)
        {
            int msb, nsb, lsb;
            if ( Rhs != 2)
            {
                Scierror(999,_("%s: Wrong number of input arguments: %d expected with option '%s'.\n"),fname,2,"setgen");
                return 0;
            }
            GetRhsVar(2,STRING_DATATYPE,&msb,&nsb,&lsb);
            if (strcmp("mt",cstk(lsb))==0)
                current_gen = MT;
            else if (strcmp("kiss",cstk(lsb))==0)
                current_gen = KISS;
            else if (strcmp("clcg4",cstk(lsb))==0)
                current_gen = CLCG4;
            else if (strcmp("clcg2",cstk(lsb))==0)
                current_gen = CLCG2;
            else if (strcmp("urand",cstk(lsb))==0)
                current_gen = URAND;
            else if (strcmp("fsultra",cstk(lsb))==0)
                current_gen = FSULTRA;
            else
            {
                Scierror(999,_("%s: Wrong value for second input argument: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"),fname,"mt","kiss","clcg4","clcg2","urand","fsultra");
                return 0;
            }
            LhsVar(1) = 2;
            PutLhsVar();
            return 0;
        }
        else if (strcmp("getgen",cstk(ls))==0)
        {
            int l_un=1;
            if ( Rhs != 1)
            {
                Scierror(999,_("%s: Wrong number of input argument: %d expected with option '%s'.\n"),fname,1,"getgen");
                return 0;
            }
            CreateVarFromPtr( Rhs+2,MATRIX_OF_STRING_DATATYPE, &l_un, &l_un, &names_gen[current_gen]);
            LhsVar(1) = Rhs+2;
            PutLhsVar();
            return 0;
        }
        else
        {
            Scierror(999,_("%s Wrong value for first input argument %s.\n"),fname,cstk(ls));

            return 0;
        }
    }
    minrhs = 2;
    CheckRhs(minrhs,maxrhs);
    if ( GetType(2) == sci_matrix ) /** m,n,'string' */
    {
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        if ( m1*n1 != 1)
        { Scierror(999,_("%s: Wrong type for first input argument: Scalar expected.\n"),fname);return 0;}
        ResL= *istk(l1);
        GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
        if ( m2*n2 != 1)
        { Scierror(999,_("%s: Wrong type for second input argument: Scalar expected.\n"),fname);return 0;}
        ResC= *istk(l2);
        GetRhsVar(3,STRING_DATATYPE, &ms, &ns, &ls);
        suite=4;
    }
    else
    {
        GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE, &ResL, &ResC, &l1);
        GetRhsVar(2,STRING_DATATYPE, &ms, &ns, &ls);
        suite = 3;
    }
    if ( strcmp(cstk(ls),"bet")==0)
    {
        double minlog=1.e-37;
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing A and B for beta law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"A");return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"B");return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) < minlog || *stk(lb) < minlog)
        {
            Scierror(999,_("Rand(...,'bet',..): A or B < %f\n"),minlog);
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(genbet)(stk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"f")==0)
    {
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing Dfn and Dfd for F law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"Dfn");return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"Dfd");return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) <= 0.0 || *stk(lb) <= 0.0)
        {
            Scierror(999,_("Degrees of freedom nonpositive\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(genf)(stk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"mul")==0)
    {
        int l_i,nn,ncat;
        double ptot;
        if ( suite != 3 || ResL*ResC != 1)
        { Scierror(999,_("%s: Wrong value for first input argument: Must be the number of random deviate.\n"),fname);
        return 0;
        }
        nn= *istk(l1);
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing N and P for MULtinomial law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"N");return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &lb);
        if ( n2 != 1 )
        {
            Scierror(999,_("%s: Wrong size for input argument: Column vector expected.\n"),fname);
            return 0;
        }
        ncat = m2+1;
        CreateVar(suite+2,MATRIX_OF_INTEGER_DATATYPE,&ncat,&nn,&lr);
        if ( *istk(la) < 0 )
        {
            Scierror(999,_("N < 0\n"));
            return 0;
        }
        if ( ncat <= 1)
        {
            Scierror(999,_("Ncat <= 1\n"));
            return 0;
        }
        ptot = 0.0;
        for ( l_i= 0 ; l_i < ncat -1 ; l_i++ )
        {
            if ( *stk(lb+l_i) < 0.0 )
            {
                Scierror(999,_("P(%d) < 0\n"),l_i+1);
                return 0;
            }
            if ( *stk(lb+l_i) > 1.0 )
            {
                Scierror(999,_("P(%d) > 1\n"),l_i+1);
                return 0;
            }
            ptot += *stk(lb+l_i);
        }
        if ( ptot > 1.0)
        {
            Scierror(999,_("Sum of P(i) > 1\n"));
            return 0;
        }
        for ( l_i=0 ; l_i < nn ; l_i++)
        {
            C2F(genmul)(istk(la),stk(lb),&ncat,istk(lr+ncat*l_i));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"gam")==0)
    {
        if ( Rhs != suite + 1)

            /*  ETRE PLUS CONSISTANT ICI : choisir entre shape , scale ou
            bien A et R (idem pour le man)
            */
        { Scierror(999,_("Missing shape and scale for Gamma law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"shape");return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"scale");return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( (*stk(la)) <= 0.0 ||  (*stk(lb)) <= 0.0 )
        {
            Scierror(999,_("grand(..'gam',A,R) : A <= 0.0 or R <= 0.0\n")); return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            /** WARNING : order is changed in parameters for
            compatibility between Rand(...'gam',..) and cdfgam
            **/
            *stk(lr+i)= C2F(gengam)(stk(lb),stk(la));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }

    else if ( strcmp(cstk(ls),"nor")==0)
    {
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing Av and Sd for Normal law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"Av");return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong size for input argument: Scalar expected for %s.\n"),fname,"Sd");return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(lb) < 0 )
        {
            Scierror(999,_("SD < 0.0\n"));return 0;}
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(gennor)(stk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"unf")==0)
    {
        double low = 0, high = 0;
        if ( Rhs != suite + 1)
        { 
            Scierror(999,_("Missing Low and High for Uniform Real law\n"));
            return 0;
        }

        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) 
        { 
            Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
            return 0;
        }

        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) 
        { 
            Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
            return 0;
        }

        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        low = *stk(la);
        high =  *stk(lb);
        if ( low > high )
        {
            Scierror(999,_("%s: Wrong type for input argument. Low < High expected.\n"), fname);
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= low + (high - low)* C2F(ranf)();
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"uin") == 0)
    {
        double a = 0, b = 0;
        if ( Rhs != suite + 1)
        { 
            Scierror(999,_("Missing Low and High for Uniform int law\n"));
            return 0;
        }

        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);

        if ( m1*n1 != 1) 
        { 
            Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
            return 0;
        }

        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) 
        { 
            Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);
            return 0;
        }
        a = *stk(la);
        b = *stk(lb);

        if ( a > b )
        {
            Scierror(999,_("%s: Wrong type for input argument. Low < High expected.\n"), fname);
            return 0;
        }

        if ( a != floor(a) || b != floor(b) || (b-a+1) > 2147483561 )
        {
            Scierror(999,_("a and b must integers with (b-a+1) <= 2147483561"));
            return 0;
        }

        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(ignuin)(stk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"lgi")==0)
    {
        if ( Rhs != suite -1 )
        {
            Scierror(999,_("%s: Wrong number of input argument: %d expected with option '%s'.\n"),fname, suite-1,"lgi");
            return 0;
        }
        CreateVar(suite,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        for ( i=0 ; i < ResL*ResC ; i++)
            *stk(lr+i)= ignlgi();
        LhsVar(1) = suite;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"prm")==0)
    {
        int nn;
        if ( suite != 3 || ResL*ResC != 1)
        {
            Scierror(999,_("%s: Wrong value for input argument: Number of random simulation expected.\n"),fname);
            return 0;
        }
        nn= *istk(l1);
        if ( Rhs != suite)
        {
            Scierror(999,_("Missing vect for random permutation\n"));
            return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Column vector expected.\n"),fname);
        return 0;}
        CreateVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&nn,&lr);
        for ( i=0 ; i < nn ; i++)
        {
            int j ;
            for (j=0; j < m1 ; j++ ) *stk(lr+(m1)*i+j)= *stk(la+j);
            C2F(genprm)(stk(lr+(m1)*i),&m1);
        }
        LhsVar(1) = suite+1;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"nbn")==0)
    {
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing N and P for Negative Binomial law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 )
        {
            Scierror(999,_("P is not in [0,1]\n"));
            return 0;
        }
        if ( *istk(la) < 0 )
        {
            Scierror(999,_("N < 0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= (double) C2F(ignnbn)(istk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"bin")==0)
    {
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing N and P for Binomial law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 )
        {
            Scierror(999,_("P is not in [0,1]\n"));
            return 0;
        }
        if ( *istk(la) < 0 )
        {
            Scierror(999,_("N < 0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= (double) C2F(ignbin)(istk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }

    else if ( strcmp(cstk(ls),"mn")==0)
    {
        int nn,un=1,work,mp,parm,ierr;
        if ( suite != 3 || ResL*ResC != 1)
        { Scierror(999,_("%s: Wrong value for first input argument: Must be the number of random simulation.\n"),fname);return 0;
        }
        nn= *istk(l1);
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing Mean and Cov for Multivariate Normal law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Column vector expected.\n"),fname);return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &lb);
        if ( m2 != n2 ) { Scierror(999,_("%s: Wrong type for input argument: Square matrix expected.\n"),fname);return 0;}
        if ( m2 != m1 ) { Scierror(999,_("%s: Wrong type for input arguments: Mean and Cov have incompatible dimensions\n"),fname);return 0;}

        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&nn,&lr);
        CreateVar(suite+3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&un,&work);
        mp=m1*(m1+3)/2 + 1;
        CreateVar(suite+4,MATRIX_OF_DOUBLE_DATATYPE,&mp,&un,&parm);
        if ( m1 <= 0 )
        {
            Scierror(999,_("%s: Wrong size for input arguments: Mean and Cov are of null size.\n"),fname);
            return 0;
        }
        C2F(setgmn)(stk(la),stk(lb),&m2,&m1,stk(parm),&ierr);
        if ( ierr == 1)
        {
            SciError(999);return 0;
        }
        for ( i=0 ; i < nn ; i++)
        {
            C2F(genmn)(stk(parm),stk(lr+(m1)*i),stk(work));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"markov")==0)
    {
        int nn,n1p1,lr1,j,icur,mm,jj;
        if ( suite != 3 || ResL*ResC != 1)
        { Scierror(999,_("%s: Wrong value for first input argument: Must be the number of random simulation.\n"),fname);return 0;
        }
        nn= *istk(l1);
        if ( Rhs != suite +1 )
        { Scierror(999,_("%s: Missing P matrix and X0 for Markov chain\n"),"fname");return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        GetRhsVar(suite+1,MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &lb);
        if ( m1 != n1 && m1 != 1 )
        {
            Scierror(999,_("%s: Wrong second input argument: Square matrix or row vector expected.\n"), fname);return 0;
        }

        if ( m2*n2 == 0 ) { Scierror(999,_("X0 is empty\n"));return 0;}

        for ( i = 0 ; i < m2*n2 ; i++)
            if ( *istk(lb+i)-1 < 0 || *istk(lb+i)-1 >= n1 )
            {
                Scierror(999,_("%s: X0(%d) must be in the range [1,%d]\n"),fname,i,n1);
                return 0;
            }
            mm= m2*n2;
            CreateVar(suite+2,MATRIX_OF_INTEGER_DATATYPE,&mm,&nn,&lr);

            n1p1=n1+1;
            CreateVar(suite+3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1p1,&lr1);
            for ( i= 0 ; i < m1 ; i++ )
            {
                double ptot = 0.0;
                for ( j = 0 ; j < n1 ; j++ )
                {
                    if ( *stk(la+i+m1*j) < 0 )
                    {
                        Scierror(999,_("P(%d,%d) < 0\n"),i+1,j+1);
                        return 0;
                    }
                    if ( *stk(la+i+m1*j) > 1 )
                    {
                        Scierror(999,_("P(%d,%d) > 1\n"),i+1,j+1);
                        return 0;
                    }
                    ptot += *stk(la+i+m1*j) ;
                }
                if ( fabs(ptot-1.0) > 1e-8 )
                {
                    Scierror(999,_("Sum of P(%d,1:%d)=%f ~= 1\n"),i+1,n1,ptot);
                    return 0;
                }
            }
            /** Computing the cumulative sum of the P matrix **/
            for ( i = 0 ; i < m1 ; i++)
            {
                double cumsum=0.0;
                *stk(lr1 +i) = cumsum;
                for ( j= 1; j < n1p1 ; j++ )
                {
                    cumsum += *stk(la + i + m1*(j-1));
                    *stk(lr1+i+m1*j) = cumsum;
                }
            }
            for ( jj = 0 ; jj < mm ; jj++)
            {
                icur = *istk(lb+jj)-1;
                for ( i=0 ; i < nn ; i++)
                {
                    int niv=0;
                    double rr = C2F(ranf)();
                    if ( m1 == 1 ) icur =0;
                    while ( rr >= *stk(lr1+ icur +m1*niv) && niv < n1p1 )
                    {
                        niv++;
                    }
                    /** projection to avoid boundaries **/
                    niv = Max(Min(niv,n1),1);
                    *istk(lr+jj+mm*i)= niv ;
                    icur=niv-1;
                }
            }
            LhsVar(1) = suite+2;
            PutLhsVar();
            return 0;
    }
    else if ( strcmp(cstk(ls),"def")==0)
    {
        if ( Rhs != suite -1 )
        { Scierror(999,_("%s: Wrong number of input argument.\n"),fname);return 0;}
        CreateVar(suite,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(ranf)();
        }
        LhsVar(1) = suite;
        PutLhsVar();
        return 0;
    }

    else if ( strcmp(cstk(ls),"nch")==0)
    {
        if ( Rhs != suite + 1)
        { Scierror(999,_("Missing Df and Xnonc for non-central chi-square law\n"));return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) < 1.0 || *stk(lb) < 0.0 )
        {
            Scierror(999,_("DF < 1 or XNONC < 0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(gennch)(stk(la),stk(lb));
        }
        LhsVar(1) = suite+2;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"nf")==0)
    {
        if ( Rhs != suite + 2)
        {
            Scierror(999,_("Missing Dfn, Dfd and Xnonc for non-central F law\n"));
            return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        GetRhsVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lb);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        GetRhsVar(suite+2,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lc);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+3,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) < 1.0 || *stk(lb) < 0.0 || *stk(lc) < 0.0 )
        {
            Scierror(999,_("DF < 1.0 or DF <= 0.0 or Xnonc < 0.0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(gennf)(stk(la),stk(lb),stk(lc));
        }
        LhsVar(1) = suite+3;
        PutLhsVar();
        return 0;
    }

    else if ( strcmp(cstk(ls),"chi")==0)
    {
        if ( Rhs != suite )
        { Scierror(999,_("Missing Df for chi-square law\n"));
        return 0;
        }
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if  ( *stk(la) <= 0.0)
        {
            Scierror(999,_("Rand: DF <= 0\n"));return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(genchi)(stk(la));
        }
        LhsVar(1) = suite+1;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"poi")==0)
    {
        if ( Rhs != suite )
        { Scierror(999,_("Missing Av for Poisson law\n"));
        return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) < 0.0 )
        {
            Scierror(999,_("Av < 0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= (double) C2F(ignpoi)(stk(la));
        }
        LhsVar(1) = suite+1;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(ls),"geom")==0)
    {
        double p;
        if ( Rhs != suite )
        { Scierror(999,_("Missing p for Geometric law\n"));
        return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        p = *stk(la);
        if ( p < 1.3e-307 || p > 1 ) { Scierror(999,_("%s: Wrong value for input argument: Must be between '%s' and %d.\n"),fname,"pmin",1);return 0;}

        CreateVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= igngeom(p);
        }
        LhsVar(1) = suite+1;
        PutLhsVar();
        return 0;
    }

    else if ( strcmp(cstk(ls),"exp")==0)
    {
        if ( Rhs != suite )
        { Scierror(999,_("Missing Av for exponential law\n"));
        return 0;}
        GetRhsVar(suite,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &la);
        if ( m1*n1 != 1) { Scierror(999,_("%s: Wrong type for input argument: Scalar expected.\n"),fname);return 0;}
        CreateVar(suite+1,MATRIX_OF_DOUBLE_DATATYPE,&ResL,&ResC,&lr);
        if ( *stk(la) < 0.0 )
        {
            Scierror(999,_("Av < 0.0\n"));
            return 0;
        }
        for ( i=0 ; i < ResL*ResC ; i++)
        {
            *stk(lr+i)= C2F(genexp)(stk(la));
        }
        LhsVar(1) = suite+1;
        PutLhsVar();
        return 0;
    }

    else
    {
        Scierror(999,_("%s: Wrong value for input argument %s.\n"),fname,cstk(ls));
        return 0;
    }
}
