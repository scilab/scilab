/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/*
 * Template to handle several wrapping operations
 */
/*--------------------------------------------------------------------------*/
#include "ScilabJavaObject.hxx"
#include "GiwsException.hxx"

#include "JIMSwrapunwrap.h"
/*--------------------------------------------------------------------------*/
template <typename U, class V>
int wrapSingle(U x)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        return V::wrap(vm, x);
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
template <typename U, class V>
int wrapRow(U *x, int len)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        return V::wrap(vm, x, len);
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
template <typename U, class V>
int wrapMat(U *x, int r, int c)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        if (getMethodOfConv())
        {
            U **xx = new U*[r];
            int i, j;
            for (i = 0; i < r; i++)
            {
                xx[i] = new U[c];
                for (j = 0; j < c; j++)
                {
                    xx[i][j] = x[j * r + i];
                }
            }
            j = V::wrap(vm, xx, r, c);
            for (i = 0; i < r; delete [] xx[i++]);
            delete [] xx;
            return j;
        }
        else
        {
            U **xx = new U*[c];
            int i;
            xx[0] = x;
            for (i = 1; i < c; xx[i] = xx[i++ - 1] + r);
            i = V::wrap(vm, xx, c, r);
            delete [] xx;
            return i;
        }
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
