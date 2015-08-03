/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __INTERFACE_ADD_H__
#define __INTERFACE_ADD_H__

/* addition double */
void add_ll_D_D(double* l, double* r, size_t size, double* o);
void add_ll_D_DC(double* l, double* r, double* rc, size_t size, double* o, double* oc);
void add_ll_D_D1(double* l, double r, size_t size, double* o);
void add_ll_D_D1C(double* l, double r, double rc, size_t size, double* o, double* oc);
void add_ll_D_E(double* l, size_t size, double* o);

void add_ll_DC_D(double* l, double* lc, double* r, size_t size, double* o, double* oc);
void add_ll_DC_DC(double* l, double* lc, double* r, double* rc, size_t size, double* o, double* oc);
void add_ll_DC_D1(double* l, double* lc, double r, size_t size, double* o, double* oc);
void add_ll_DC_D1C(double* l, double* lc, double r, double rc, size_t size, double* o, double* oc);
void add_ll_DC_E(double* l, double* lc, size_t size, double* o, double* oc);

void add_ll_D1_D(double l, double* r, size_t size, double* o);
void add_ll_D1_DC(double l, double* r, double* rc, size_t size, double* o, double* oc);
void add_ll_D1_D1(double l, double r, double* o);
void add_ll_D1_D1C(double l, double r, double rc, double* o, double* oc);
void add_ll_D1_E(double l, double* o);

void add_ll_D1C_D(double l, double lc, double* r, size_t size, double* o, double* oc);
void add_ll_D1C_DC(double l, double lc, double* r, double* rc, size_t size, double* o, double* oc);
void add_ll_D1C_D1(double l, double lc, double r, double* o, double* oc);
void add_ll_D1C_D1C(double l, double lc, double r, double rc, double* o, double* oc);
void add_ll_D1C_E(double l, double lc, double* o, double* oc);

void add_ll_E_D(double* r, size_t size, double* o);
void add_ll_E_DC(double* r, double* rc, size_t size, double* o, double* oc);
void add_ll_E_D1(double r, double* o);
void add_ll_E_D1C(double r, double rc, double* o, double* oc);
void add_ll_E_E();


#endif /* !__INTERFACE_ADD_H__ */
