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
template <typename T, typename U,class V>
int wrapSingleWithCast(T x)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        return V::wrap(vm, static_cast<U>(x));
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
template <typename T, typename U, class V>
int wrapRowWithCast(T *x, int len)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        int i;
        U *l = new U[len];
        for (i = 0; i < len ; i++)
        {
            l[i] = static_cast<U>(x[i]);
        }

        return V::wrap(vm, l, len);
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
template <typename T, typename U, class V>
int wrapMatWithCast(T *x, int r, int c)
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
                    xx[i][j] = static_cast<U>(x[j * r + i]);
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
            int i, j;
            for (i = 0; i < c; i++)
            {
                xx[i] = new U[r];
                for (j = 0; j < r; j++)
                {
                    xx[i][j] = static_cast<U>(x[i * r + j]);
                }
            }
            j = V::wrap(vm, xx, c, r);
            for (i = 0; i < c; delete [] xx[i++]);
            delete [] xx;
            return j;
        }
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
