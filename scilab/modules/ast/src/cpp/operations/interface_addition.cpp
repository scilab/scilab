/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "types_addition.hxx"
extern "C"
{
#include "matrix_addition.h"
}

//D +
void add_ll_D_D(double* l, size_t size, double* r, double* o)
{
    add(l, size, r, o);
}

void add_ll_D_DC(double* l, size_t size, double* r, double* rc, double* o, double* oc)
{
    add(l, size, r, rc, o, oc);
}

void add_ll_D_D1(double* l, size_t size, double r, double* o)
{
    add(l, size, r, o);
}

void add_ll_D_D1C(double* l, size_t size, double r, double rc, double* o, double* oc)
{
    add(l, size, r, rc, o, oc);
}

void add_ll_D_E(double* l, size_t size, double* o)
{
    add(l, size, o);
}

//DC +
void add_ll_DC_D(double* l, double* lc, size_t size, double* r, double* o, double* oc)
{
    add(l, lc, size, r, o, oc);
}

void add_ll_DC_DC(double* l, double* lc, size_t size, double* r, double* rc, double* o, double* oc)
{
    add(l, lc, size, r , rc, o, oc);
}

void add_ll_DC_D1(double* l, double* lc, size_t size, double r, double* o, double* oc)
{
    add(l, lc, size, r, o, oc);
}

void add_ll_DC_D1C(double* l, double* lc, size_t size, double r, double rc, double* o, double* oc)
{
    add(l, lc, size, r, rc, o, oc);
}

void add_ll_DC_E(double* l, double* lc, size_t size, double* o, double* oc)
{
    add(l, lc, size, o, oc);
}

//D1 +
void add_ll_D1_D(double l, size_t size, double* r, double* o)
{
    add(l, size, r, o);
}

void add_ll_D1_DC(double l, size_t size, double* r, double* rc, double* o, double* oc)
{
    add(l, size, r, rc, o, oc);
}

void add_ll_D1_D1(double l, double r, double* o)
{
    add(l, r, o);
}

void add_ll_D1_D1C(double l, double r, double rc, double* o, double* oc)
{
    add(&l, 1, r, rc, o, oc);
}

void add_ll_D1_E(double l, double* o)
{
    add(l, o);
}

//D1C +
void add_ll_D1C_D(double l, double lc, size_t size, double* r, double* o, double* oc)
{
    add(l, lc, size, r, o, oc);
}

void add_ll_D1C_DC(double l, double lc, size_t size, double* r, double* rc, double* o, double* oc)
{
    add(l, lc, size, r, rc, o, oc);
}

void add_ll_D1C_D1(double l, double lc, double r, double* o, double* oc)
{
    add(&l, &lc, 1, r, o, oc);
}

void add_ll_D1C_D1C(double l, double lc, double r, double rc, double* o, double* oc)
{
    add(l, lc, r, rc, o, oc);
}

void add_ll_D1C_E(double l, double lc, double* o, double* oc)
{
    add(l, lc, o, oc);
}

//E +
void add_ll_E_D(double* r, size_t size, double* o)
{
    add(r, size, o);
}

void add_ll_E_DC(double* r, double* rc, size_t size, double* o, double* oc)
{
    add(r, rc, size, o, oc);
}

void add_ll_E_D1(double r, double* o)
{
    add(r, o);
}

void add_ll_E_D1C(double r, double rc, double* o, double* oc)
{
    add(r, rc, o, oc);
}

void add_ll_E_E()
{
    add();
}
