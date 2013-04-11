/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 1998-2008 - INRIA - Allan CORNET
 * Copyright (C) 1998-2008 - INRIA - Serge STEER
 * Copyright (C) 1998-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 1998-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */
/*------------------------------------------------------------------------
*    Scilab Memory Management library (Stack API)
--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------
* Interface Library:   ilib
*---------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "os_strdup.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "sciprint.h"
#include "stack2.h"
#include "cvstr.h"
#include "parse.h"
#include "men_Sutils.h"
#include "int2db.h"
#include "rea2b.h"
#include "Scierror.h"
#include "localization.h"
#include "callinterf.h"
#include "call_scilab.h"
#include "recursionFunction.h"
#include "doublecomplex.h"

#ifdef _MSC_VER
#define abs(x) ((x) >= 0 ? (x) : -(x))  /* pour abs  C2F(mvfromto) line 2689 */
#endif

/* Table of constant values */
static int cx1 = 1;
static int cx0 = 0;

static char *Get_Iname(void);
static int C2F(mvfromto) (int *itopl, int *);

static int rhs_opt_find(char *name, rhs_opts opts[]);
static void rhs_opt_print_names(rhs_opts opts[]);
extern int C2F(isbyref) (int *fun);

/*------------------------------------------------*/
void strcpy_tws(char *str1, char *str2, int len);
int C2F(copyvarfromsciptr) (int lw, int n, int l);
static int intersci_push(void);
static void intersci_pop(void);
static int C2F(getopcode) (char *string, unsigned long string_len);

static void ConvertData(unsigned char *type, int size, int l);

/*------------------------------------------------
* checkrhs: checks right hand side arguments
*-----------------------------------------------*/

int C2F(checkrhs) (char *fname, int *iMin, int *iMax, unsigned long fname_len)
{
    /*
     * store the name in recu array, fname can be a non null terminated char array
     * Get_Iname() can be used in other function to get the interface name
     */

    C2F(cvname) (&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], fname, &cx0, fname_len);

    return TRUE;
}

/*------------------------------------------------
* checkrhs: checks left hand side arguments
*-----------------------------------------------*/

int C2F(checklhs) (char *fname, int *iMin, int *iMax, unsigned long fname_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* isopt:
* returns the status of the variable number k
* if its an optional variable f(x=...)
* returns .true. and variable name in namex
* namex must have a size of nlgh + 1
*---------------------------------------------------------------------*/

int C2F(isopt) (int *k, char *namex, unsigned long name_len)
{
    //int i1 = *k + Top - Rhs;

    //if (C2F(isoptlw) (&Top, &i1, namex, name_len) == FALSE)
    //    return FALSE;
    ///* add a '\0' at the end of the string removing trailing blanks */
    //for (i1 = nlgh - 1; i1 >= 0; i1--)
    //{
    //    if (namex[i1] != ' ')
    //        break;
    //}
    //namex[i1 + 1] = '\0';
    return TRUE;
}

/*--------------------------------------------------------------
* freeptr : free ip pointer
*--------------------------------------------------------------*/

void C2F(freeptr) (double *ip[])
{
    if (ip)
        FREE((char *)(*ip));
}

/*---------------------------------------
* isoptlw :
* returns the status of the variable at position lw in the stack
* if its an optional variable f(x=...)
* returns .true. and variable name in namex
*--------------------------------------- */

int C2F(isoptlw) (int *topk, int *lw, char *namex, unsigned long name_len)
{
    if (*Infstk(*lw) != 1)
        return FALSE;
    C2F(cvname) (&C2F(vstk).idstk[(*lw) * nsiz - nsiz], namex, &cx1, name_len);
    return TRUE;
}

/*---------------------------------------
* firstopt :
* return the position of the first optionnal argument
* given as xx=val in the calling sequence.
* If no such argument it returns Rhs+1.
*--------------------------------------- */
int C2F(firstopt) (void)
{
    //int k;

    //for (k = 1; k <= Rhs; ++k)
    //    if (*Infstk(k + Top - Rhs) == 1)
    //        return k;
    //return (Rhs + 1);
    return 0;
}

/*---------------------------------------
* findopt :
* checks if option str has been passed.
* If yes returns the position of the variable
* If no  returns 0
*--------------------------------------- */

int C2F(findopt) (char *str, rhs_opts opts[])
{
    //int i;

    //i = rhs_opt_find(str, opts);
    //if (i >= 0)
    //    if (opts[i].position > 0)
    //        return opts[i].position;

    return 0;
}

/*---------------------------------------
* numopt :
*  returns the number of optional variables
*  given as xx=val in the caling sequence
*  top must have a correct value when using this function
*--------------------------------------- */

int C2F(numopt) (void)
{
    //int k, ret = 0;

    ////for (k = 1; k <= Rhs; ++k)
    ////    if (*Infstk(k + Top - Rhs) == 1)
    ////        ret++;
    //return ret;
    return 0;
}

/*---------------------------------------------------------------------
* vartype:
*   type of variable number number in the stack
*---------------------------------------------------------------------*/

int C2F(vartype) (int *number)
{
    //int ix1 = *number + Top - Rhs;

    //return C2F(gettype) (&ix1);
    return 0;
}

/*------------------------------------------------
* gettype:
*    returns the type of object at position lw in the stack
*------------------------------------------------*/

int C2F(gettype) (int *lw)
{
    int il;

    il = iadr(*Lstk(*lw));
    if (*istk(il) < 0)
        il = iadr(*istk(il + 1));
    return *istk(il);
}

/*------------------------------------------------
* overloadtype:
*    set mechanism to overloaded function fname if object type
*    does not fit given type
*------------------------------------------------*/

static int overloadtype(int *lw, char *fname, unsigned char *typ)
{
    int il = 0;
    int ityp = 0;

    il = iadr(*Lstk(*lw));
    if (*istk(il) < 0)
        il = iadr(*istk(il + 1));
    switch (*typ)
    {
        case 'c':                  /* string */
        case 'S':                  /* string Matrix */
            ityp = sci_strings;
            break;
        case 'd':
        case 'i':
        case 'r':
        case 'z':                  /* numeric */
            ityp = sci_matrix;
            break;
        case 'b':                  /* boolean */
            ityp = sci_boolean;
            break;
        case 'h':                  /* handle */
            ityp = sci_handles;
            break;
        case 'l':                  /* list */
            ityp = sci_list;
            break;
        case 't':                  /* tlist */
            ityp = sci_tlist;
            break;
        case 'm':                  /* mlist */
            ityp = sci_mlist;
            break;
        case 'f':                  /* external */
            ityp = sci_c_function;
            break;
        case 'p':                  /* pointer */
            ityp = sci_pointer;     /* used to be sci_lufact_pointer before Scilab 5.2 */
            break;
        case 's':                  /* sparse */
            ityp = sci_sparse;
            break;
        case 'I':                  /* int matrix */
            ityp = sci_ints;
            break;
        case 'x':                  /* polynomial matrix */
            ityp = sci_poly;
            break;

    }
    if (*istk(il) != ityp)
    {
        return C2F(overload) (lw, fname, (unsigned long)strlen(fname));
    }
    return 1;
}

/*------------------------------------------------
* overload
*    set mechanism to overloaded function fname for object lw
*------------------------------------------------*/

int C2F(overload) (int *lw, char *fname, unsigned long l)
{
    C2F(putfunnam) (fname, lw, l);
    C2F(com).fun = -1;
    return 0;
}

/*------------------------------------------------
* ogettype : unused
*------------------------------------------------*/
int C2F(ogettype) (int *lw)
{
    return *istk(iadr(*Lstk(*lw)));
}

/*----------------------------------------------------
* Optional arguments f(....., arg =val,...)
*          in interfaces
* function get_optionals : example is provided in
*    examples/addinter-examples/intex2c.c
*----------------------------------------------------*/

int get_optionals(char *fname, rhs_opts opts[])
{
    //int k, i = 0;
    //char name[nlgh + 1];
    //int nopt = NumOpt();        /* optional arguments on the stack */

    ///* reset first field since opts is declared static in calling function */
    ///* this could be avoided with ansi compilers by removing static in the
    // * opts declaration */

    //while (opts[i].name != NULL)
    //{
    //    opts[i].position = -1;
    //    i++;
    //}

    ///* Walking through last arguments */

    //for (k = Rhs - nopt + 1; k <= Rhs; k++)
    //{
    //    if (IsOpt(k, name) == 0)
    //    {
    //        Scierror(999, _("%s: Optional arguments name=val must be at the end.\n"), fname);
    //        return 0;
    //    }
    //    else
    //    {
    //        int isopt = rhs_opt_find(name, opts);

    //        if (isopt >= 0)
    //        {
    //            rhs_opts *ro = &opts[isopt];

    //            ro->position = k;
    //            if (ro->type[0] != '?')
    //                GetRhsVar(ro->position, ro->type, &ro->m, &ro->n, &ro->l);
    //        }
    //        else
    //        {
    //            sciprint(_("%s: Unrecognized optional arguments %s.\n"), fname, name);
    //            rhs_opt_print_names(opts);
    //            SciError(999);
    //            return (0);
    //        }
    //    }
    //}

    return 1;
}

/* Is name in opts */

int rhs_opt_find(char *name, rhs_opts opts[])
{
    int rep = -1, i = 0;

    //while (opts[i].name != NULL)
    //{
    //    int cmp;

    //    /* name is terminated by white space and we want to ignore them */
    //    if ((cmp = strcmp(name, opts[i].name)) == 0)
    //    {
    //        rep = i;
    //        break;
    //    }
    //    else if (cmp < 0)
    //    {
    //        break;
    //    }
    //    else
    //    {
    //        i++;
    //    }
    //}
    return rep;
}

void rhs_opt_print_names(rhs_opts opts[])
/* array of optinal names (in alphabetical order)
* the array is null terminated */
{
    //int i = 0;

    //if (opts[i].name == NULL)
    //{
    //    sciprint(_("Optional argument list is empty.\n"));
    //    return;
    //}
    //sciprint(_("Optional arguments list: \n"));
    //while (opts[i + 1].name != NULL)
    //{
    //    sciprint("%s, ", opts[i].name);
    //    i++;
    //}
    //sciprint(_("and %s.\n"), opts[i].name);
}

/*---------------------------------------------------------------------
* isref :
*   checks if variable number lw is on the stack
*   or is just a reference to a variable on the stack
*---------------------------------------------------------------------*/

int IsRef(int number)
{
    return C2F(isref) (&number);
}

int C2F(isref) (int *number)
{
    //int il, lw;

    //lw = *number + Top - Rhs;
    //if (*number > Rhs)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), "isref", "isref");
    //    return FALSE;
    //}
    //il = iadr(*Lstk(lw));
    //if (*istk(il) < 0)
    //    return TRUE;
    //else
    //    return FALSE;
    return 0;
}

/*---------------------------------------------------------------------
*     create a variable number lw in the stack of type
*     type and size m,n
*     the argument must be of type type ('c','d','r','i','l','b')
*     return values m,n,lr
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*     b : boolean matrix
*     l : a list  (m-> number of elements and n->1)
*         for each element of the list an other function
*         must be used to <<get>> them
*     side effects : arguments in the common intersci are modified
*     see examples in addinter-examples
*---------------------------------------------------------------------*/

int C2F(createvar) (int *lw, char *typex, int *m, int *n, int *lr, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     create a variable number lw in the stack of type
*     type and size m,n
*     the argument must be of type type ('d','r','i')
*     return values m,n,lr
*     d,r,i : matrix of double,float or integer
*     side effects : arguments in the common intersci are modified
*     see examples in addinter-examples
*     Like createvar but for complex matrices
*---------------------------------------------------------------------*/

int C2F(createcvar) (int *lw, char *typex, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     create a variable number lw on the stack of type
*     list with nel elements
*---------------------------------------------------------------------*/

int C2F(createlist) (int *lw, int *nel)
{
    //char *fname = Get_Iname();
    //int lr, lw1;

    //if (*lw > intersiz)
    //{
    //    Scierror(999, _("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"), fname, "createlist");
    //    return FALSE;
    //}
    //Nbvars = Max(*lw, Nbvars);
    //lw1 = *lw + Top - Rhs;
    //if (*lw < 0)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), fname, "createlist");
    //    return FALSE;
    //}
    //C2F(crelist) (&lw1, nel, &lr);
    //C2F(intersci).ntypes[*lw - 1] = '$';
    //C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
    //C2F(intersci).lad[*lw - 1] = lr;
    return TRUE;
}

/*---------------------------------------------------------------------
*     create a variable number lw on the stack of type
*     type and size m,n
*     the argument must be of type type ('c','d','r','i','b')
*     return values m,n,lr,lar
*     lar is also an input value
*     if lar != -1 var is filled with data stored at lar
*---------------------------------------------------------------------*/

int C2F(createvarfrom) (int *lw, char *typex, int *m, int *n, int *lr, int *lar, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     create a variable number lw on the stack of type
*     type and size m,n
*     the argument must be of type type ('d','r','i')
*     return values it,m,n,lr,lc,lar,lac
*     lar is also an input value
*     if lar != -1 var is filled with data stored at lar
*     idem for lac
*     ==> like createvarfrom for complex matrices
*---------------------------------------------------------------------*/

int C2F(createcvarfrom) (int *lw, char *typex, int *it, int *m, int *n, int *lr, int *lc, int *lar, int *lac, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     This function must be called after createvar(lnumber,'l',...)
*     Argument lnumber is a list
*     we want here to get its argument number number
*     the argument must be of type type ('c','d','r','i','b')
*     input values lnumber,number,type,lar
*     lar : input value ( -1 or the adress of an object which is used
*           to fill the new variable data slot.
*     lar must be a variable since it is used as input and output
*     return values m,n,lr,lar
*         (lar --> data is coded at stk(lar)
*          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
*                  or cstk(lr)
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*---------------------------------------------------------------------*/

int C2F(createlistvarfrom) (int *lnumber, int *number, char *typex, int *m, int *n, int *lr, int *lar, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* create a complex list variable from data
*---------------------------------------------------------------------*/

int C2F(createlistcvarfrom) (int *lnumber, int *number, char *typex, int *it, int *m, int *n, int *lr, int *lc, int *lar, int *lac,
                             unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     This function must be called after createvar(lnumber,'l',...)
*     Argument lnumber is a list
*     we want here to get its argument number number
*     the argument must be of type type ('c','d','r','i','b')
*     input values lnumber,number,type,lar
*     lar : input value ( -1 or the adress of an object which is used
*           to fill the new variable data slot.
*     lar must be a variable since it is used as input and output
*     return values m,n,lr,lar
*         (lar --> data is coded at stk(lar)
*          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
*                  or cstk(lr)
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*---------------------------------------------------------------------*/

int C2F(createlistvarfromptr) (int *lnumber, int *number, char *typex, int *m, int *n, void *iptr, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     This function must be called after createvar(lnumber,'l',...)
*     Argument lnumber is a list
*     we want here to get its argument number number
*     the argument must be of type type ('c','d','r','i','b')
*     input values lnumber,number,type,lar
*     lar : input value ( -1 or the adress of an object which is used
*           to fill the new variable data slot.
*     lar must be a variable since it is used as input and output
*     return values m,n,lr,lar
*         (lar --> data is coded at stk(lar)
*          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
*                  or cstk(lr)
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*---------------------------------------------------------------------*/

int C2F(createlistcvarfromptr) (int *lnumber, int *number, char *typex, int *it, int *m, int *n, void *iptr, void *iptc, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* use the rest of the stack as working area
* the allowed size (in double) is returned in m
*---------------------------------------------------------------------*/

int C2F(creatework) (int *number, int *m, int *lr)
{
    //int n, it = 0, lw1, lcs, il;
    //char *fname = Get_Iname();

    //if (*number > intersiz)
    //{

    //    Scierror(999, _("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"), fname, "creatework");
    //    return FALSE;
    //}
    //Nbvars = Max(*number, Nbvars);
    //lw1 = *number + Top - Rhs;
    //if (lw1 < 0)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), fname, "creatework");
    //    return FALSE;
    //}
    //il = iadr(*Lstk(lw1));
    //*m = *Lstk(Bot) - sadr(il + 4);
    //n = 1;
    //if (!C2F(cremat) (fname, &lw1, &it, m, &n, lr, &lcs, nlgh))
    //    return FALSE;
    return TRUE;
}

/*---------------------------------------------------------------------
* This can be used with creatework to
* set the size of object which was intialy sized to the whole
* remaining space with creatework
* Moreover information the objet is recorded
*---------------------------------------------------------------------*/

int C2F(setworksize) (int *number, int *size)
{
    //int lw1;
    //char *fname = Get_Iname();

    //if (*number > intersiz)
    //{
    //    Scierror(999, _("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"), fname, "creatework");
    //    return FALSE;
    //}
    //Nbvars = Max(*number, Nbvars);
    //lw1 = *number + Top - Rhs;
    //if (lw1 < 0)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), fname, "setworksize");
    //    return FALSE;
    //}
    //*Lstk(lw1 + 1) = *Lstk(lw1) + *size;
    //C2F(intersci).ntypes[*number - 1] = '$';
    //C2F(intersci).iwhere[*number - 1] = *Lstk(lw1);
    //C2F(intersci).lad[*number - 1] = 0; /* not to be used XXXX */
    return TRUE;
}

/*---------------------------------------------------------------------
* getmatdims :
*     check if argument number <<number>> is a matrix and
*     returns its dimensions
*---------------------------------------------------------------------*/

int C2F(getmatdims) (int *number, int *m, int *n)
{
    //char *fname = Get_Iname();
    //int il, lw, typ;

    //lw = *number + Top - Rhs;
    //if (*number > Rhs)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), fname, "getmatdims");
    //    return FALSE;
    //}

    //il = iadr(*Lstk(lw));
    //if (*istk(il) < 0)
    //    il = iadr(*istk(il + 1));
    //typ = *istk(il);
    //if (typ > sci_strings)
    //{
    //    Scierror(199, _("%s: Wrong type for argument %d: Matrix expected.\n"), fname, *number);
    //    return FALSE;
    //}
    //*m = *istk(il + 1);
    //*n = *istk(il + 2);
    return TRUE;
}

/*---------------------------------------------------------------------
* getrhsvar :
*     get the argument number <<number>>
*     the argument must be of type type ('c','d','r','i','f','l','b')
*     return values m,n,lr
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*     f : external (function)
*     b : boolean matrix
*     l : a list  (m-> number of elements and n->1)
*         for each element of the list an other function
*         must be used to <<get>> them
*     side effects : arguments in the common intersci are modified
*     see examples in addinter-examples
*---------------------------------------------------------------------*/

int C2F(getrhsvar) (int *number, char *typex, int *m, int *n, int *lr, unsigned long type_len)
{
    return FALSE;
}

/*---------------------------------------------------------------------
* getrhsvcar :
*     get the argument number <<number>>
*     the argument must be of type type ('d','r','i')
*     like getrhsvar but for complex matrices
*---------------------------------------------------------------------*/

int C2F(getrhscvar) (int *number, char *typex, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* elementtype:
*   returns the type of the element indexed by *number in the list
*   whose variable number is *lnumber. If the indexed element does not exist
*   the function returns 0.
*---------------------------------------------------------------------*/

int C2F(elementtype) (int *lnumber, int *number)
{
    return 0;
}

/*---------------------------------------------------------------------
*     This function must be called after getrhsvar(lnumber,'l',...)
*     Argument lnumber is a list
*     we want here to get its argument number number
*     the argument must be of type type ('c','d','r','i','b')
*     return values m,n,lr,lar
*         (lar --> data is coded at stk(lar)
*          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
*                  or cstk(lr)
*     c : string  (m-> number of characters and n->1)
*     d,r,i : matrix of double,float or integer
*---------------------------------------------------------------------*/

int C2F(getlistrhsvar) (int *lnumber, int *number, char *typex, int *m, int *n, int *lr, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* for complex
*---------------------------------------------------------------------*/

int C2F(getlistrhscvar) (int *lnumber, int *number, char *typex, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     creates variable number number of type "type" and dims m,n
*     from pointer ptr
*
*---------------------------------------------------------------------*/

int C2F(createvarfromptr) (int *number, char *typex, int *m, int *n, void *iptr, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
*     for complex
*---------------------------------------------------------------------*/

int C2F(createcvarfromptr) (int *number, char *typex, int *it, int *m, int *n, double *iptr, double *iptc, unsigned long type_len)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* mklistfromvars :
*     replace the last n variables created at postions pos:pos-1+n
*     by a list of these variables at position pos
*---------------------------------------------------------------------*/

int C2F(mklistfromvars) (int *pos, int *n)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* mktlistfromvars :
*     similar to mklistfromvars but create a tlist
*---------------------------------------------------------------------*/

int C2F(mktlistfromvars) (int *pos, int *n)
{
    //int type = 16;
    //int tops = Top;
    //int k;

    //for (k = *pos; k < *pos + *n; k++)
    //    C2F(convert2sci) (&k);
    //Top = Top - Rhs + *pos - 1 + *n;
    //C2F(mklistt) (n, &type);
    //Top = tops;
    //C2F(intersci).ntypes[*pos - 1] = '$';
    return TRUE;
}

/*---------------------------------------------------------------------
* mktlistfromvars :
*     similar to mklistfromvars but create a mlist
*---------------------------------------------------------------------*/

int C2F(mkmlistfromvars) (int *pos, int *n)
{
    //int type = sci_mlist;
    //int tops = Top;
    //int k;

    //for (k = *pos; k < *pos + *n; k++)
    //    C2F(convert2sci) (&k);
    //Top = Top - Rhs + *pos - 1 + *n;
    //C2F(mklistt) (n, &type);
    //Top = tops;
    //C2F(intersci).ntypes[*pos - 1] = '$';
    return TRUE;
}

/*---------------------------------------------------------------------
* call a Scilab function given its name
*---------------------------------------------------------------------*/

int C2F(callscifun) (char *string, unsigned long string_len)
{
    int id[nsiz];

    C2F(cvname) (id, string, &cx0, string_len);
    C2F(putid) (&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], id);
    C2F(com).fun = -1;
    return 0;
}

/*---------------------------------------------------------------------
* scifunction(number,ptr,mlhs,mrhs) >
*     execute scilab function with mrhs input args and mlhs output
*     variables
*     input args are supposed to be stored in the top of the stack
*     at positions top-mrhs+1:top
*---------------------------------------------------------------------*/

int C2F(scifunction) (int *number, int *ptr, int *mlhs, int *mrhs)
{
    return FALSE;
}

/*---------------------------------------------------------------------
* scistring :
*   executes scilab string (name of a scilab function) with mrhs
*     input args and mlhs output variables
*     input args are supposed to be indexed by ifirst,ifirst+1,...
*     thestring= string made of the name of a Scilab function
*     mlhs,mlhs = number of lhs and rhs parameters of the function
*     ifisrt,thestring,mlhs and mrhs are input parameters.
*---------------------------------------------------------------------*/
int C2F(scistring) (int *ifirst, char *thestring, int *mlhs, int *mrhs, unsigned long thestring_len)
{
    return 0;
}

int C2F(getopcode) (char *string, unsigned long string_len)
{
    return 0;
}

/*---------------------------------------------------------------------
*     same as scifunction: executes scilab built-in function (ifin,ifun)
*
*     =(interface-number, function-nmber-in-interface)
*     for the input parameters located at number, number+1, ....
*     mlhs,mrhs = # of lhs and rhs parameters of the function.
*---------------------------------------------------------------------*/

int C2F(scibuiltin) (int *number, int *ifun, int *ifin, int *mlhs, int *mrhs)
{
    return FALSE;
}

/*---------------------------------------------------------------------
*     same as scibuiltin: executes scilab operation op
*     for the input parameters located at number, number+1, ....
*     mlhs,mrhs = # of lhs and rhs parameters of the operation.
*---------------------------------------------------------------------*/

int C2F(sciops) (int *number, int *op, int *mlhs, int *mrhs)
{
    //int ifin, ifun, srhs = Rhs, slhs = Lhs, ix, intop = Top, lw;

    //Fin = *op;
    //Top = Top - Rhs + *number + *mrhs - 1;
    //Lhs = *mlhs;
    //Rhs = *mrhs;

    //while (1)
    //{
    //    //C2F(allops) ();
    //    if (Err > 0)
    //    {
    //        return FALSE;
    //    };
    //    if (C2F(com).fun == 0)
    //        break;
    //    Top = intop;
    //    ifun = C2F(com).fun;
    //    ifin = Fin;
    //    if (!C2F(scibuiltin) (number, &ifun, &ifin, mlhs, mrhs))
    //    {
    //        return FALSE;
    //    };
    //    if (Err > 0)
    //    {
    //        return FALSE;
    //    };
    //}
    //Lhs = slhs;
    //Rhs = srhs;
    //Top = intop;

    //for (ix = 1; ix <= *mlhs; ++ix)
    //{
    //    lw = Top - Rhs + *number + ix - 1;
    //    C2F(intersci).ntypes[lw - 1] = '$';
    //}
    //C2F(com).fun = 0;
    //Fin = *op;
    //C2F(recu).icall = 0;
    return TRUE;
}

/*-------------------------------------------------------------
*     test and return linear system (syslin tlist)
*     inputs: lw = variable number
*     outputs:
*     N=size of A matrix (square)
*     M=number of inputs = col. dim B matrix
*     P=number of outputs = row. dim of C matrix
*     ptr(A,B,C,D,X0) adresses of A,B,C,D,X0 in stk
*     h=type   h=0.0  continuous system
*              h=1.0  discrete time system
*              h=h    sampled system h=sampling period
-------------------------------------------------------------*/

int C2F(getrhssys) (int *lw, int *n, int *m, int *p, int *ptra, int *ptrb, int *ptrc, int *ptrd, int *ptrx0, double *hx)
{
    return TRUE;
}

/*---------------------------------------------------
* call Scilab error function (for Fortran use)
*---------------------------------------------------*/

int C2F(errorinfo) (char *fname, int *info, unsigned long fname_len)
{
    Scierror(998, _("%s: internal error, info=%d.\n"), get_fname(fname, fname_len), *info);
    return 0;
}

/*-------------------------------------------------------------
*  returns Maximal available size in scilab stack
*  for variable <<number>> lw
*  In a Fortran call
*     lw =
*     type= 'd','r','i','c'
*     type_len is here for C/Fortran calling conventions
*  This function is used for creating a working array of Maximal dimension
*  Example :
*     lwork=Maxvol(nb,'d')
*     if(.not.createvar(nb,'d',lwork,1,idwork)) return
*     call pipo(   ,stk(idwork),[lwork],...)
*-------------------------------------------------------------*/

int C2F(maxvol) (int *lw, char *lw_type, unsigned long type_len)
{
    return 0;
}

/*---------------------------------------------
* This function checks all the variables which
* where references and restore their contents
* to Scilab value.
*---------------------------------------------*/

static int Check_references()
{
    return TRUE;
}

/*---------------------------------------------------------------------
* int C2F(putlhsvar)()
*     This function put on the stack the lhs
*     variables which are at position lhsvar(i)
*     on the calling stack
*     Warning : this function supposes that the last
*     variable on the stack is at position top-rhs+nbvars
*---------------------------------------------------------------------*/

int C2F(putlhsvar) ()
{
    return TRUE;
}

/*---------------------------------------------------------------------
* mvfromto :
*     this routines copies the variable number i
*     (created by getrhsvar or createvar or by mvfromto itself in a precedent call)
*     from its position on the stack to position itopl
*     returns false if there's no more stack space available
*     - if type(i) # '$'  : This variable is at
*                         position lad(i) on the stack )
*                         and itopl must be the first free position
*                         on the stack
*                         copy is performed + type conversion (type(i))
*     - if type(i) == '$': then it means that object at position i
*                         is the result of a previous call to mvfromto
*                         a copyobj is performed and itopl can
*                         can be any used position on the stack
*                         the object which was at position itopl
*                         is replaced by object at position i
*                         (and access to object itopl+1 can be lost if
*                         the object at position i is <> from object at
*                         position itopl
*---------------------------------------------------------------------*/

static int C2F(mvfromto) (int *itopl, int *ix)
{
    return TRUE;
}

/*---------------------------------------------------------------------
* copyref
* copy object at position from to position to
* without changing data.
* The copy is only performed if object is a reference
* and ref object is replaced by its value
*---------------------------------------------------------------------*/

int Ref2val(int from, int to)
{
    //int il, lw;

    //lw = from + Top - Rhs;
    //if (from > Rhs)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), "copyref", "isref");
    //    return FALSE;
    //}
    //il = iadr(*Lstk(lw));
    //if (*istk(il) < 0)
    //{
    //    int lwd;

    //    /* from contains a reference */
    //    lw = *istk(il + 2);
    //    lwd = to + Top - Rhs;
    //    C2F(copyobj) ("copyref", &lw, &lwd, (unsigned long)strlen("copyref"));
    //}
    return 0;
}

/*---------------------------------------------------------------------
* convert2sci :
*     this routine converts data of variable number num
*     to scilab standard data code
*     see how it is used in matdes.c
*---------------------------------------------------------------------*/

int C2F(convert2sci) (int *ix)
{
    //int ix1 = Top - Rhs + *ix;

    //if (!C2F(mvfromto) (&ix1, ix))
    //    return FALSE;
    //C2F(intersci).ntypes[*ix - 1] = '$';
    return TRUE;
}

/*-----------------------------------------------------------
* strcpy_tws : fname[0:nlgh-2]=' '
* fname[nlgh-1] = '\0'
* then second string is copied into first one
* ------------------------------------------------------------*/

void strcpy_tws(char *str1, char *str2, int len)
{
    int i;

    for (i = 0; i < (int)strlen(str2); i++)
        str1[i] = str2[i];
    for (i = (int)strlen(str2); i < len; i++)
        str1[i] = ' ';
    str1[len - 1] = '\0';
}

/*---------------------------------------------------------------------
*     conversion from Scilab code --> ascii
*     + add a 0 at end of string
*---------------------------------------------------------------------*/

int C2F(in2str) (int *n, int *line, char *str, unsigned long str_len)
{
    C2F(codetoascii) (n, line, str, str_len);
    str[*n] = '\0';
    return 0;
}

/*---------------------------------------------------------------------
* Get_Iname:
* Get the name (interfcae name) which was stored in ids while in checkrhs
*---------------------------------------------------------------------*/

static char Fname[nlgh + 1];

static char *Get_Iname()
{
    int i;

    C2F(cvname) (&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], Fname, &cx1, nlgh);
    /** remove trailing blanks **/
    for (i = 0; i < nlgh; i++)
        if (Fname[i] == ' ')
        {
            Fname[i] = '\0';
            break;
        }
    Fname[nlgh] = '\0';
    return Fname;
}

/*---------------------------------------------------------------------
* Utility for error message
*---------------------------------------------------------------------*/

static char arg_position[56];   /* @TODO WTF is 56 ? */

char *CharPosition(int i)
{
    char *tmp_buffer = NULL;

    switch (i + 1)
    {
        case 1:
            tmp_buffer = os_strdup(_("first"));
            break;
        case 2:
            tmp_buffer = os_strdup(_("second"));
            break;
        case 3:
            tmp_buffer = os_strdup(_("third"));
            break;
        case 4:
            tmp_buffer = os_strdup(_("fourth"));
            break;
        default:
            tmp_buffer = os_strdup(" ");
            break;
    }
    return tmp_buffer;
}

char *ArgPosition(int i)
{
    char *tmp_buffer = NULL;

    if (i > 0 && i <= 4)
    {
        tmp_buffer = CharPosition(i - 1);
        sprintf(arg_position, _("%s argument"), tmp_buffer);
        FREE(tmp_buffer);
    }
    else
    {
        sprintf(arg_position, _("argument #%d"), i);
    }
    return arg_position;
}

char *ArgsPosition(int i, int j)
{
    char *tmp_buffer_1 = NULL, *tmp_buffer_2 = NULL;

    if (i > 0 && i <= 4)
    {
        if (j > 0 && j <= 4)
        {
            tmp_buffer_1 = CharPosition(i - 1);
            tmp_buffer_2 = CharPosition(j - 1);
            sprintf(arg_position, _("%s and %s arguments"), tmp_buffer_1, tmp_buffer_2);
            FREE(tmp_buffer_1);
            FREE(tmp_buffer_2);
        }
        else
        {
            tmp_buffer_1 = CharPosition(i - 1);
            sprintf(arg_position, _("%s argument and argument #%d"), tmp_buffer_1, j);
            FREE(tmp_buffer_1);
        }
    }
    else
    {
        if (j > 0 && j <= 4)
        {
            tmp_buffer_1 = CharPosition(j - 1);
            sprintf(arg_position, _("%s argument and argument #%d"), tmp_buffer_1, i);
            FREE(tmp_buffer_1);
        }
        else
            sprintf(arg_position, _("arguments #%d and #%d"), i, j);
    }
    return arg_position;
}

/*---------------------------------------------------------------------
* Utility for back convertion to Scilab format
* (can be used with GetListRhsVar )
*---------------------------------------------------------------------*/

static void ConvertData(unsigned char *type, int size, int l)
{
    int zero = 0, mu = -1;
    int laddr;
    int prov, m, n, it;
    double wsave;

    switch (type[0])
    {
        case 'c':
            C2F(cvstr1) (&size, (int *)cstk(l), cstk(l), &zero, size);
            break;
        case 'r':
            C2F(rea2db) (&size, sstk(l), &mu, (double *)sstk(l), &mu);
            break;
        case 'i':
            C2F(int2db) (&size, istk(l), &mu, (double *)istk(l), &mu);
            break;
        case 'z':
            if (*istk(iadr(iadr(l)) - 2) == 133)
            {
                /* values @ even adress */
                prov = *istk(iadr(iadr(l)) - 1);
                m = *istk(prov);
                n = *istk(prov + 1);
                it = 1;
                laddr = iadr(l);
                wsave = *stk(laddr);
                /* make header */
                *istk(iadr(iadr(l)) - 2) = 1;
                *istk(iadr(iadr(l)) - 1) = m;
                *istk(iadr(iadr(l))) = n;
                *istk(iadr(iadr(l)) + 1) = it;
                /* convert values */
                z2double(stk(laddr), stk(laddr + 1), m * n, m * n);
                *stk(laddr + 1) = wsave;
            }
            else
            {
                F77ToSci((double *)zstk(l), size, size);
            }
    }
}

/*---------------------------------------------------------------------
* Utility for checking properties
*---------------------------------------------------------------------*/

static int check_prop(char *mes, int posi, int m)
{
    if (m)
    {
        /* XXXX moduler 999 en fn des messages */
        Scierror(999, "%s: %s %s\n", Get_Iname(), ArgPosition(posi), mes);
        return FALSE;
    }
    return TRUE;
}

int check_square(int posi, int m, int n)
{
    return check_prop(_("should be square"), posi, m != n);
}

int check_vector(int posi, int m, int n)
{
    return check_prop(_("should be a vector"), posi, m != 1 && n != 1);
}

int check_row(int posi, int m, int n)
{
    return check_prop(_("should be a row vector"), posi, m != 1);
}

int check_col(int posi, int m, int n)
{
    return check_prop(_("should be a column vector"), posi, n != 1);
}

int check_scalar(int posi, int m, int n)
{
    return check_prop(_("should be a scalar"), posi, n != 1 || m != 1);
}

int check_dims(int posi, int m, int n, int m1, int n1)
{
    if (m != m1 || n != n1)
    {
        Scierror(999, _("%s: %s has wrong dimensions (%d,%d), expecting (%d,%d).\n"), Get_Iname(), ArgPosition(posi), m, n, m1, n1);
        return FALSE;
    }
    return TRUE;
}

int check_one_dim(int posi, int dim, int val, int valref)
{
    if (val != valref)
    {
        Scierror(999, _("%s: %s has wrong %s dimension (%d), expecting (%d).\n"), Get_Iname(), ArgPosition(posi),
                 (dim == 1) ? _("first") : _("second"), val, valref);
        return FALSE;
    }
    return TRUE;
}

int check_length(int posi, int m, int m1)
{
    if (m != m1)
    {
        Scierror(999, _("%s: %s has wrong length %d, expecting (%d).\n"), Get_Iname(), ArgPosition(posi), m, m1);
        return FALSE;
    }
    return TRUE;
}

int check_same_dims(int i, int j, int m1, int n1, int m2, int n2)
{
    if (m1 == m2 && n1 == n2)
        return TRUE;
    Scierror(999, _("%s: %s have incompatible dimensions (%dx%d) # (%dx%d)\n"), Get_Iname(), ArgsPosition(i, j), m1, n1, m2, n2);
    return FALSE;
}

int check_dim_prop(int i, int j, int flag)
{
    if (flag)
    {
        Scierror(999, _("%s: %s have incompatible dimensions.\n"), Get_Iname(), ArgsPosition(i, j));
        return FALSE;
    }
    return TRUE;
}

static int check_list_prop(char *mes, int lpos, int posi, int m)
{
    if (m)
    {
        Scierror(999, _("%s: %s should be a list with %d-element being %s.\n"), Get_Iname(), ArgPosition(posi), posi, mes);
        return FALSE;
    }
    return TRUE;
}

int check_list_square(int lpos, int posi, int m, int n)
{
    return check_list_prop(_("square"), lpos, posi, (m != n));
}

int check_list_vector(int lpos, int posi, int m, int n)
{
    return check_list_prop(_("a vector"), lpos, posi, m != 1 && n != 1);
}

int check_list_row(int lpos, int posi, int m, int n)
{
    return check_list_prop(_("a row vector"), lpos, posi, m != 1);
}

int check_list_col(int lpos, int posi, int m, int n)
{
    return check_list_prop(_("a column vector"), lpos, posi, n != 1);
}

int check_list_scalar(int lpos, int posi, int m, int n)
{
    return check_list_prop(_("a scalar"), lpos, posi, n != 1 || m != 1);
}

int check_list_one_dim(int lpos, int posi, int dim, int val, int valref)
{
    if (val != valref)
    {
        Scierror(999, _("%s: argument %d(%d) has wrong %s dimension (%d), expecting (%d).\n"), Get_Iname(), lpos, posi,
                 (dim == 1) ? _("first") : _("second"), val, valref);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------
* Utility for hand writen data extraction or creation
*---------------------------------------------------------------------*/

int C2F(createdata) (int *lw, int n)
{
    //int lw1;
    //char *fname = Get_Iname();

    //if (*lw > intersiz)
    //{
    //    Scierror(999, _("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"), fname, "createdata");
    //    return FALSE;
    //}
    //Nbvars = Max(*lw, Nbvars);
    //lw1 = *lw + Top - Rhs;
    //if (*lw < 0)
    //{
    //    Scierror(999, _("%s: bad call to %s! (1rst argument).\n"), fname, "createdata");
    //    return FALSE;
    //}
    //if (!C2F(credata) (fname, &lw1, n, nlgh))
    //    return FALSE;
    //C2F(intersci).ntypes[*lw - 1] = '$';
    //C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
    //C2F(intersci).lad[*lw - 1] = *Lstk(lw1);
    return TRUE;
}

/*---------------------------------------------------------------------
* copyvarfromsciptr
*     copy a Scilab variable given by
*      - its first adress l in stk
*      - its size n
*    to the variable position  lw
*----------------------------------------------------------------------*/
int C2F(copyvarfromsciptr) (int lw, int n, int l)
{
    //int ret, un = 1;

    //if ((ret = C2F(createdata) (&lw, n)) == FALSE)
    //    return ret;
    //C2F(unsfdcopy) (&n, stk(l), &un, stk(*Lstk(lw + Top - Rhs)), &un);
    return TRUE;
}

void *GetVarPtr(int n)
/* return  the pointer on the first int of the n th variable  data structure  */
{
    //int l1 = *Lstk(n + Top - Rhs);
    //int *loci = (int *)stk(l1);

    //return loci;
    return 0;
}

void *GetData(int lw)
/* Usage: header = (int *) GetData(lw); header[0] = type of variable lw etc */
{
    //int lw1 = lw + Top - Rhs;
    //int l1 = *Lstk(lw1);
    //int *loci = (int *)stk(l1);

    //if (loci[0] < 0)
    //{
    //    l1 = loci[1];
    //    loci = (int *)stk(l1);
    //}
    //C2F(intersci).ntypes[lw - 1] = '$';
    //C2F(intersci).iwhere[lw - 1] = l1;
    //C2F(intersci).lad[lw - 1] = l1;
    //return loci;
    return 0;
}

int GetDataSize(int lw)
/* get memory used by the argument lw in double world etc */
{
    //int lw1 = lw + Top - Rhs;
    //int l1 = *Lstk(lw1);
    //int *loci = (int *)stk(l1);
    //int n = *Lstk(lw1 + 1) - *Lstk(lw1);

    //if (loci[0] < 0)
    //{
    //    l1 = loci[1];
    //    loci = (int *)stk(l1);
    //    n = loci[3];
    //}
    //return n;
    return 0;
}

void *GetRawData(int lw)
/* same as GetData BUT does not go to the pointed variable if lw is a reference */
{
    //int lw1 = lw + Top - Rhs;
    //int l1 = *Lstk(lw1);
    //int *loci = (int *)stk(l1);

    //C2F(intersci).ntypes[lw - 1] = '$';
    //C2F(intersci).iwhere[lw - 1] = l1;
    ///*  C2F(intersci).lad[lw - 1] = l1;  to be checked */
    //return loci;
    return 0;
}

void *GetDataFromName(char *name)
/* usage:  header = (int *) GetDataFromName("pipo"); header[0] = type of variable pipo etc... */
{
    void *header;
    int lw;
    int fin;

    if (C2F(objptr) (name, &lw, &fin, (unsigned long)strlen(name)))
    {
        header = istk(iadr(*Lstk(fin)));
        return (void *)header;
    }
    else
    {
        Scierror(999, _("GetDataFromName: variable %s not found.\n"), name);
        return (void *)0;
    }
}

int C2F(createreference) (int number, int pointed)
/* variable number is created as a reference to variable pointed */
{
    //int offset;
    //int point_ed;
    //int *header;

    //CreateData(number, 4 * sizeof(int));
    //header = GetRawData(number);
    //offset = Top - Rhs;
    //point_ed = offset + pointed;
    //header[0] = -*istk(iadr(*Lstk(point_ed)));  /* reference : 1st entry (type) is opposite */
    //header[1] = *Lstk(point_ed);    /* pointed adress */
    //header[2] = point_ed;       /* pointed variable */
    //header[3] = *Lstk(point_ed + 1) - *Lstk(point_ed);  /* size of pointed variable */
    //C2F(intersci).ntypes[number - 1] = '-';
    return 1;
}

int C2F(changetoref) (int number, int pointed)
/* variable number is changed as a reference to variable pointed */
{
    //int offset;
    //int point_ed;
    //int *header;

    //header = GetRawData(number);
    //offset = Top - Rhs;
    //point_ed = offset + pointed;
    //header[0] = -*istk(iadr(*Lstk(point_ed)));  /* reference : 1st entry (type) is opposite */
    //header[1] = *Lstk(point_ed);    /* pointed adress */
    //header[2] = pointed;        /* pointed variable */
    //header[3] = *Lstk(point_ed + 1) - *Lstk(point_ed);  /* size of pointed variable */
    //C2F(intersci).ntypes[number - 1] = '-';
    return 1;
}

int C2F(createreffromname) (int number, char *name)
/* variable number is created as a reference pointing to variable "name" */
/* name must be an existing Scilab variable */
{
    int *header;
    int lw;
    int fin;

    CreateData(number, 4 * sizeof(int));
    header = (int *)GetData(number);
    if (C2F(objptr) (name, &lw, &fin, (unsigned long)strlen(name)))
    {
        header[0] = -*istk(iadr(*Lstk(fin)));   /* type of reference = - type of pointed variable */
        header[1] = lw;         /* pointed adress */
        header[2] = fin;        /* pointed variable */
        header[3] = *Lstk(fin + 1) - *Lstk(fin);    /*size of pointed variable */
        return 1;
    }
    else
    {
        Scierror(999, _("%s: Variable %s not found.\n"), "CreateRefFromName", name);
        return 0;
    }
}

/*-------------------------------------------------------
* protect the intersci common during recursive calls
*-------------------------------------------------------*/

typedef struct inter_s_
{
    int iwhere, nbrows, nbcols, itflag, ntypes, lad, ladc, lhsvar;
} intersci_state;

typedef struct inter_l
{
    intersci_state *state;
    int nbvars;
    struct inter_l *next;
} intersci_list;

static intersci_list *L_intersci;

static int intersci_push(void)
{
    int i;
    intersci_list *loc;
    intersci_state *new;

    new = MALLOC(Nbvars * sizeof(intersci_state));
    if (new == 0)
        return 0;
    loc = MALLOC(sizeof(intersci_list));
    if (loc == NULL)
        return 0;
    loc->next = L_intersci;
    loc->state = new;
    loc->nbvars = Nbvars;
    for (i = 0; i < Nbvars; i++)
    {
        loc->state[i].iwhere = C2F(intersci).iwhere[i];
        loc->state[i].ntypes = C2F(intersci).ntypes[i];
        loc->state[i].lad = C2F(intersci).lad[i];
        loc->state[i].lhsvar = C2F(intersci).lhsvar[i];
    }
    L_intersci = loc;
    return 1;
}

static void intersci_pop(void)
{
    int i;
    intersci_list *loc = L_intersci;

    if (loc == NULL)
        return;
    Nbvars = loc->nbvars;
    for (i = 0; i < Nbvars; i++)
    {
        C2F(intersci).iwhere[i] = loc->state[i].iwhere;
        C2F(intersci).ntypes[i] = loc->state[i].ntypes;
        C2F(intersci).lad[i] = loc->state[i].lad;
        C2F(intersci).lhsvar[i] = loc->state[i].lhsvar;
    }
    L_intersci = loc->next;
    FREE(loc->state);
    FREE(loc);
}

/*
static void intersci_show()
{
int i;
fprintf(stderr,"======================\n");
for ( i = 0 ; i < C2F(intersci).nbvars ; i++ )
{
fprintf(stderr,"%d %d %d\n",i,
C2F(intersci).iwhere[i],
C2F(intersci).ntypes[i]);
}
fprintf(stderr,"======================\n");
}

*/
