/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Enpc - JPC
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/* README :
 * The routines in this file use a pointer to a Matrix
 * at the Fortran Level this pointer is stored as an integer
 * so we cast  pointer to long int
 * f_(fmat)
 *  long *fmat
 *  *fmat = (long)spCreate(*n,0,&error);
 *
 * At fortran level the int must be an int *4
 * in order to store a C long
 * since we  are in scilab
 * the pointer transmitted to f_ is an istk(il1) it can in fact contain
 * something as long as a double

 */


/*
 *  IMPORTS
 *
 *  >>> Import descriptions:
 *  spConfig.h
 *     Macros that customize the sparse matrix routines.
 *  spmatrix.h
 *     Macros and declarations to be imported by the user.
 *  spDefs.h
 *     Matrix type and macro definitions for the sparse matrix routines.
 */

#include "lu.h"
#include "Scierror.h"
/*
 *
 * lufact1  >>> Creation and LU factorization of a sparse matrix
 * Entry <<
 *   val,rc
 *       arrays of size k and kx2 a(rc(i),rc(*k+i))=val(i)
 *       for i=0,(*k-1)
 *   *n : size of the created square matrix
 *   *k : number of given values
 *   *eps : The machine precision number
 *  Return >>
 *   fmat : pointer to a long int which is a cast of an adress
 *   *nrank : The numerical rank
 */


static void spFixThresold(char *eMatrix, double eps, double releps)
{
    MatrixPtr  Matrix = (MatrixPtr)eMatrix;
    Matrix->AbsThreshold = eps;
    Matrix->RelThreshold = releps;
}


static void spGetNumRank(char* eMatrix, int *n)
{
    MatrixPtr  Matrix = (MatrixPtr)eMatrix;
    *n = Matrix->NumRank;
}

void C2F(lufact1)(double *val, int *lln, int *col, int *n, int *nel,
                  int *fmatindex, double *eps, double *releps, int *nrank, int *ierr)
{
    int error, i, i0, i1, k, j;
    char *fmat;
    spREAL *pelement;
    *ierr = 0;
    fmat = spCreate(*n, 0, &error);
    if (error != spOKAY)
    {
        *ierr = 1;
        return;
    }
    *fmatindex = addluptr (fmat);
    if ( *fmatindex == -1)
    {
        spDestroy(fmat);
        *ierr = 1;
        return;
    }

    i0 = 0;
    i1 = i0;
    i = 1;
    for (k = 0 ; k < *nel; k++)
    {
        i0 = i0 + 1;
        while (i0 - i1 > lln[i - 1])
        {
            i1 = i0;
            i = i + 1;
            i0 = i0 + 1;
        }
        j = col[k];

        pelement = spGetElement(fmat, i, j);

        if (pelement == 0)
        {
            removeluptr(fmat);
            spDestroy(fmat);
            *ierr = 2;
            return;
        }
        spADD_REAL_ELEMENT(pelement, (spREAL)(val[k]));

    }
    /* Fix the AbsThresold with scilex %eps */
    spFixThresold(fmat, *eps, *releps);

    /* spPrint((char *) *fmat,1,1,1); */
    error = spFactor(fmat);

    spGetNumRank(fmat, nrank);

    switch (error)
    {
        case spZERO_DIAG:
            Scierror(999, _("%s: A zero was encountered on the diagonal the matrix.\n"), "zero_diag");
            removeluptr(fmat);
            spDestroy(fmat);
            return;
        case spNO_MEMORY:
            *ierr = 3;
            removeluptr(fmat);
            spDestroy(fmat);
            return;
        case spSINGULAR:
            *ierr = -1; /*Singular matrix" */
            return;
        case spSMALL_PIVOT:
            *ierr = -2; /* matrix is singular at precision level */
            return;
    }
}

/*
 * lusolve1  >>> Solves fmat*x=b
 *   *fmat : a pointer to the sparse matrix factored by lufact
 *   b,v
 *      two arrays of size n the matrix size
 */
void C2F(lusolve1)(int *fmatindex, double *b, double *x, int *ierr)
{
    char *fmat;
    if (getluptr((int)*fmatindex, &fmat) == -1)
    {
        *ierr = 1;
        return;
    }
    *ierr = 0;
    spSolve(fmat, (spREAL*)b, (spREAL*)x);
}

/*
 * ludel1  >>> delete sparse matrix
 *   *fmat : a pointer to the sparse matrix factored by lufact
 */

void C2F(ludel1)(int *fmatindex, int *ierr)
{
    char *fmat;
    if (getluptr((int)*fmatindex, &fmat) == -1)
    {
        *ierr = 1;
        return;
    }
    *ierr = 0;
    removeluptr ((int)*fmatindex);
    spDestroy(fmat);

}

/*
 * lusize  >>> returns in n the size of the sparse matrix
 *   *fmat : a pointer to the sparse matrix factored by lufact
 */
/*
**
static void spSize(char* eMatrix, int *n)
{
  MatrixPtr  Matrix = (MatrixPtr)eMatrix;
  *n=Matrix->Size;
}
*/

/*
** @FIXME : Dead code
void C2F(lusize)(long* fmat, int *n)
{
spSize((char *) *fmat,n);
}
*/

/*
 * luget1   >>> extract the LU coded matrix into a full array
 *   sigg,sigd :
 *     two arrays of size n which code permutations
 *   lu :
 *     an array coded matrix of size nxn where lu will be stored
 */


/* filling right permutation */
static void GetSigD(MatrixPtr Matrix, int indsigd[], double sigd[])
{
    int I, J, mc = 0, last = 0;
    int Size = Matrix->Size;
    for (I = 1; I <= Size; I++)
    {
        indsigd[I - 1] = 1;
        indsigd[Size + I - 1] =  Matrix->IntToExtColMap[I];
        sigd[I - 1] = 1.0;
    }
    /* counting missing colums */
    for (I = 1; I <= Size; I++)
        if (Matrix->ExtToIntColMap[I] == -1)
        {
            mc++;
        }
    /* filling missing colums */
    if (mc != 0)
    {
        for (I = Size - (mc) + 1  ; I <= Size; I++)
        {
            for ( J = last + 1; J <= Size; J++)
            {
                if (Matrix->ExtToIntColMap[J] == -1)
                {
                    last = J;
                    break;
                }
            }
            indsigd[I - 1] = 1;
            indsigd[Size + I - 1] = last;
        }
    }
}


/* filling left permutation */

static void GetSigG(MatrixPtr Matrix, int indsigg[], double sigg[])
{
    int Size = Matrix->Size;
    int I, J, mc = 0, last = 0;
    /* counting missing Rows*/
    for (I = 1; I <= Size; I++)
        if (Matrix->ExtToIntRowMap[I] == -1)
        {
            mc++;
        }

    for (I = 1; I <= Size - mc ; I++)
    {
        indsigg[I - 1] = 1;
        indsigg[Size + Matrix->IntToExtRowMap[I] - 1] = I;
        sigg[I - 1] = 1.0;
    }
    /* filling missing Rows */
    if (mc != 0)
    {
        for (I = Size - (mc) + 1  ; I <= Size; I++)
        {
            for ( J = last + 1; J <= Size; J++)
            {
                if (Matrix->ExtToIntRowMap[J] == -1)
                {

                    last = J;
                    break;
                }
            }
            indsigg[I - 1] = 1;
            indsigg[Size + last - 1] = I;
            sigg[I - 1] = 1.0;
        }
    }
}


static void spLuget(char *eMatrix, int *indP, double *P, int* indl,
                    double *l, int *indu, double *u, int *indQ, double *Q)
{
    int I, J;
    int lsize, usize;

    MatrixPtr Matrix = (MatrixPtr) eMatrix;
    ElementPtr  pElement;
    int Size;
    Size = Matrix->Size;
    GetSigD(Matrix, indQ, Q);
    GetSigG(Matrix, indP, P);
    for (J = 1; J <= Size ; J++)
    {
        indl[J - 1] = 0;
        indu[J - 1] = 0;
    }
    lsize = 0;
    usize = 0;
    for (I = 1; I <= Size ; I++)
    {
        indu[I - 1] = indu[I - 1] + 1;
        indu[Size + usize] = I;
        u[usize] = 1.0;
        usize = usize + 1;

        pElement = Matrix->FirstInRow[I];
        while ( pElement != NULL )
        {
            J = pElement->Col;
            if (I >= J)
            {
                indl[I - 1] = indl[I - 1] + 1;
                indl[Size + lsize] = J;
                l[lsize] = (double) pElement->Real ;
                lsize = lsize + 1;

            }
            else
            {
                indu[I - 1] = indu[I - 1] + 1;
                indu[Size + usize] = J;
                u[usize] = (double) pElement->Real ;
                usize = usize + 1;

            }
            pElement = pElement->NextInRow;
        };
    };
}





void C2F(luget1)(int *fmatindex, int *indP, double *P,
                 int *indl, double *l, int *indu, double *u,
                 int *indQ, double *Q, int *ierr)
{
    char *fmat;
    if (getluptr((int)*fmatindex, &fmat) == -1)
    {
        *ierr = 1;
        return;
    }
    *ierr = 0;
    spLuget(fmat, indP, P, indl, l, indu, u, indQ, Q);
}


/*
 * lusiz1   >>> extract the L and U  number of non zero elements
 * lsize and usize
 */


static void spLusiz(char *eMatrix, int *lsize, int *usize)
{
    int J;
    MatrixPtr Matrix = (MatrixPtr) eMatrix;
    ElementPtr  pElement;
    int Size;
    Size = Matrix->Size;
    *lsize = 0;
    *usize = Size;
    for (J = 1; J <= Size ; J++)
    {
        pElement = Matrix->FirstInCol[J];
        while ( pElement != NULL )
        {
            if (pElement->Row >= J)
            {
                *lsize = *lsize + 1;
            }
            else
            {
                *usize = *usize + 1;
            }
            pElement = pElement->NextInCol;
        };
    };
}

void C2F(lusiz1)(int* fmatindex, int* lsize, int* usize, int *ierr)
{
    char *fmat;
    if (getluptr((int)*fmatindex, &fmat) == -1)
    {
        *ierr = 1;
        return;
    }
    *ierr = 0;
    spLusiz(fmat, lsize, usize);
}

char **sci_luptr_table = NULL;
int sci_luptr_table_size = 0;/* allocated size for pointer table*/
int sci_luptr_index = 0;/* max index used (one based)*/


/**addluptr
 * This function adds a pointer on a sparse lu factorization to Scilab internal table
 */
int addluptr (char *ptr)
{
    int i, sel;
    int rsize = 10;
    if (sci_luptr_table_size == 0) /* first call alloacte a small array of pointers*/
    {
        sci_luptr_table = (char **)MALLOC(rsize * sizeof(char *));
        if (sci_luptr_table == NULL)
        {
            return -1;
        }
        sci_luptr_table_size += 10;
    }
    /* look for a free cell in sci_luptr_table*/
    sel = -1;
    for (i = 0; i < sci_luptr_index; i++)
    {
        if ( sci_luptr_table[i] == NULL)
        {
            sel = i;
            break;
        }
    }
    if (sel == -1)
    {
        if (sci_luptr_index < sci_luptr_table_size)
        {
            sel = sci_luptr_index++;
        }
        else
        {
            sci_luptr_table = (char **)REALLOC(sci_luptr_table, (sci_luptr_table_size + rsize) * sizeof(char *));
            if (sci_luptr_table == NULL)
            {
                return -1;
            }
            sci_luptr_table_size += 10;
            sel = sci_luptr_index++;
        }
    }
    sci_luptr_table[sel] = ptr;
    return sel + 1;
}
/**getluptr
 * this function returns a pointer on a sparse lu factorization
 * given its index (one based) in the table
 */
int getluptr(int sel, char **ptr)
{
    if (sel > sci_luptr_index || sel < 1)
    {
        return -1;
    }
    if (sci_luptr_table[sel - 1] == NULL)
    {
        return -1;
    }
    *ptr = sci_luptr_table[sel - 1];
    return 0;
}

/**removeluptr
 * This function removes a pointer on a sparse lu factorization
 * out of  Scilab internal table given its index in the table
 */
int removeluptr (int sel)
{
    if (sel > sci_luptr_index || sel < 1)
    {
        return -1;
    }
    sci_luptr_table[sel - 1] = NULL;
    if (sel == sci_luptr_index)
    {
        sci_luptr_index--;
    }
    return 0;
}

/**resetluptr
 * This function reinitialize the Scilab sparse lu pointer table
 */
void resetluptr()
{
    FREE(sci_luptr_table);
    sci_luptr_table_size = 0;/* allocated size for pointer table*/
    sci_luptr_index = 0;/* max index used (one based)*/
}
