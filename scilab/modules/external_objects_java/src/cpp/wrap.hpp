/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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
