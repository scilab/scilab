/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include <algorithm>
#include "types_addition.hxx"
#include "operations.hxx"
#include "double.hxx"
#include "int.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "matrix_addition.h"
#include "elem_common.h" //dset
#include "Sciwarning.h"
}

using namespace types;
//define arrays on operation functions
static add_function pAddfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillAddFunction()
{
#define scilab_fill_add(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pAddfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (add_function)&add_##func<typeIn1, typeIn2, typeOut>

    //Double
    //Matrix + Matrix
    scilab_fill_add(Double, Double, M_M, Double, Double, Double);
    scilab_fill_add(Double, Int8, M_M, Double, Int8, Int8);
    scilab_fill_add(Double, UInt8, M_M, Double, UInt8, UInt8);
    scilab_fill_add(Double, Int16, M_M, Double, Int16, Int16);
    scilab_fill_add(Double, UInt16, M_M, Double, UInt16, UInt16);
    scilab_fill_add(Double, Int32, M_M, Double, Int32, Int32);
    scilab_fill_add(Double, UInt32, M_M, Double, UInt32, UInt32);
    scilab_fill_add(Double, Int64, M_M, Double, Int64, Int64);
    scilab_fill_add(Double, UInt64, M_M, Double, UInt64, UInt64);
    scilab_fill_add(Double, Bool, M_M, Double, Bool, Double);
    scilab_fill_add(Double, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_add(Double, Sparse, M_M, Double, Sparse, Double);

    //Matrix + Matrix Complex
    scilab_fill_add(Double, DoubleComplex, M_MC, Double, Double, Double);
    scilab_fill_add(Double, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(Double, SparseComplex, M_M, Double, Sparse, Double);

    //Matrix + Scalar
    scilab_fill_add(Double, ScalarDouble, M_S, Double, Double, Double);
    scilab_fill_add(Double, ScalarInt8, M_S, Double, Int8, Int8);
    scilab_fill_add(Double, ScalarUInt8, M_S, Double, UInt8, UInt8);
    scilab_fill_add(Double, ScalarInt16, M_S, Double, Int16, Int16);
    scilab_fill_add(Double, ScalarUInt16, M_S, Double, UInt16, UInt16);
    scilab_fill_add(Double, ScalarInt32, M_S, Double, Int32, Int32);
    scilab_fill_add(Double, ScalarUInt32, M_S, Double, UInt32, UInt32);
    scilab_fill_add(Double, ScalarInt64, M_S, Double, Int64, Int64);
    scilab_fill_add(Double, ScalarUInt64, M_S, Double, UInt64, UInt64);
    scilab_fill_add(Double, ScalarBool, M_S, Double, Bool, Double);
    scilab_fill_add(Double, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Matrix + Scalar Complex
    scilab_fill_add(Double, ScalarDoubleComplex, M_SC, Double, Double, Double);
    scilab_fill_add(Double, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Matrix + Empty
    scilab_fill_add(Double, Empty, M_E, Double, Double, Double);


    //Matrix Complex + Matrix
    scilab_fill_add(DoubleComplex, Double, MC_M, Double, Double, Double);
    scilab_fill_add(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Double);
    scilab_fill_add(DoubleComplex, ScalarDouble, MC_S, Double, Double, Double);
    scilab_fill_add(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Double);
    scilab_fill_add(DoubleComplex, Empty, MC_E, Double, Double, Double);
    scilab_fill_add(DoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_add(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_add(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(DoubleComplex, Sparse, M_M, Double, Sparse, Double);
    scilab_fill_add(DoubleComplex, SparseComplex, M_M, Double, Sparse, Double);

    //Scalar + Matrix
    scilab_fill_add(ScalarDouble, Double, S_M, Double, Double, Double);
    scilab_fill_add(ScalarDouble, Int8, S_M, Double, Int8, Int8);
    scilab_fill_add(ScalarDouble, UInt8, S_M, Double, UInt8, UInt8);
    scilab_fill_add(ScalarDouble, Int16, S_M, Double, Int16, Int16);
    scilab_fill_add(ScalarDouble, UInt16, S_M, Double, UInt16, UInt16);
    scilab_fill_add(ScalarDouble, Int32, S_M, Double, Int32, Int32);
    scilab_fill_add(ScalarDouble, UInt32, S_M, Double, UInt32, UInt32);
    scilab_fill_add(ScalarDouble, Int64, S_M, Double, Int64, Int64);
    scilab_fill_add(ScalarDouble, UInt64, S_M, Double, UInt64, UInt64);
    scilab_fill_add(ScalarDouble, Bool, S_M, Double, Bool, Double);
    scilab_fill_add(ScalarDouble, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_add(ScalarDouble, Sparse, M_M, Double, Sparse, Double);

    //Scalar + Matrix Complex
    scilab_fill_add(ScalarDouble, DoubleComplex, S_MC, Double, Double, Double);
    scilab_fill_add(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(ScalarDouble, SparseComplex, M_M, Double, Sparse, Double);

    //Scalar + Scalar
    scilab_fill_add(ScalarDouble, ScalarDouble, S_S, Double, Double, Double);
    scilab_fill_add(ScalarDouble, ScalarInt8, S_S, Double, Int8, Int8);
    scilab_fill_add(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, UInt8);
    scilab_fill_add(ScalarDouble, ScalarInt16, S_S, Double, Int16, Int16);
    scilab_fill_add(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, UInt16);
    scilab_fill_add(ScalarDouble, ScalarInt32, S_S, Double, Int32, Int32);
    scilab_fill_add(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, UInt32);
    scilab_fill_add(ScalarDouble, ScalarInt64, S_S, Double, Int64, Int64);
    scilab_fill_add(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, UInt64);
    scilab_fill_add(ScalarDouble, ScalarBool, S_S, Double, Bool, Double);
    scilab_fill_add(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Scalar + Scalar Complex
    scilab_fill_add(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Double);
    scilab_fill_add(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);

    //Scalar + Empty
    scilab_fill_add(ScalarDouble, Empty, S_E, Double, Double, Double);

    //Scalar Complex + Matrix
    scilab_fill_add(ScalarDoubleComplex, Double, SC_M, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_add(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, Double);
    //Scalar Complex + Matrix Complex
    scilab_fill_add(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_add(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, Double);
    //Scalar Complex + Scalar
    scilab_fill_add(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    //Scalar Complex + Scalar Complex
    scilab_fill_add(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Scalar Complex + Empty
    scilab_fill_add(ScalarDoubleComplex, Empty, SC_E, Double, Double, Double);

    //Empty + Matrix
    scilab_fill_add(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_add(Empty, Int8, E_M, Double, Int8, Int8);
    scilab_fill_add(Empty, UInt8, E_M, Double, UInt8, UInt8);
    scilab_fill_add(Empty, Int16, E_M, Double, Int16, Int16);
    scilab_fill_add(Empty, UInt16, E_M, Double, UInt16, UInt16);
    scilab_fill_add(Empty, Int32, E_M, Double, Int32, Int32);
    scilab_fill_add(Empty, UInt32, E_M, Double, UInt32, UInt32);
    scilab_fill_add(Empty, Int64, E_M, Double, Int64, Int64);
    scilab_fill_add(Empty, UInt64, E_M, Double, UInt64, UInt64);
    scilab_fill_add(Empty, Bool, E_M, Double, Bool, Double);
    scilab_fill_add(Empty, String, E_M, Double, String, String);
    scilab_fill_add(Empty, Polynom, E_M, Double, Polynom, Polynom);
    scilab_fill_add(Empty, PolynomComplex, E_M, Double, Polynom, Polynom);
    scilab_fill_add(Empty, Sparse, E_M, Double, Sparse, Sparse);
    scilab_fill_add(Empty, SparseComplex, E_M, Double, Sparse, Sparse);

    //Empty + Matrix Complex
    scilab_fill_add(Empty, DoubleComplex, E_MC, Double, Double, Double);
    //Empty + Scalar
    scilab_fill_add(Empty, ScalarDouble, E_S, Double, Double, Double);
    scilab_fill_add(Empty, ScalarInt8, E_S, Double, Int8, Int8);
    scilab_fill_add(Empty, ScalarUInt8, E_S, Double, UInt8, UInt8);
    scilab_fill_add(Empty, ScalarInt16, E_S, Double, Int16, Int16);
    scilab_fill_add(Empty, ScalarUInt16, E_S, Double, UInt16, UInt16);
    scilab_fill_add(Empty, ScalarInt32, E_S, Double, Int32, Int32);
    scilab_fill_add(Empty, ScalarUInt32, E_S, Double, UInt32, UInt32);
    scilab_fill_add(Empty, ScalarInt64, E_S, Double, Int64, Int64);
    scilab_fill_add(Empty, ScalarUInt64, E_S, Double, UInt64, UInt64);
    scilab_fill_add(Empty, ScalarBool, E_S, Double, Bool, Double);
    scilab_fill_add(Empty, ScalarString, E_S, Double, String, String);
    scilab_fill_add(Empty, ScalarPolynom, E_S, Double, Polynom, Polynom);

    //Empty + Scalar Complex
    scilab_fill_add(Empty, ScalarDoubleComplex, E_SC, Double, Double, Double);
    scilab_fill_add(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Polynom);
    //Empty + Empty
    scilab_fill_add(Empty, Empty, E_E, Double, Double, Double);
    //Empty + eye
    scilab_fill_add(Empty, Identity, E_I, Double, Double, Double);
    scilab_fill_add(Empty, IdentityComplex, E_IC, Double, Double, Double);

    //Matrix + Identity
    scilab_fill_add(Double, Identity, M_I, Double, Double, Double);
    scilab_fill_add(Double, IdentityComplex, M_IC, Double, Double, Double);
    scilab_fill_add(DoubleComplex, Identity, MC_I, Double, Double, Double);
    scilab_fill_add(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Double);
    scilab_fill_add(ScalarDouble, Identity, S_I, Double, Double, Double);
    scilab_fill_add(ScalarDouble, IdentityComplex, S_IC, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, Identity, SC_I, Double, Double, Double);
    scilab_fill_add(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Double);


    //Int8
    //Matrix + Matrix
    scilab_fill_add(Int8, Double, M_M, Int8, Double, Int8);
    scilab_fill_add(Int8, Int8, M_M, Int8, Int8, Int8);
    scilab_fill_add(Int8, UInt8, M_M, Int8, UInt8, UInt8);
    scilab_fill_add(Int8, Int16, M_M, Int8, Int16, Int16);
    scilab_fill_add(Int8, UInt16, M_M, Int8, UInt16, UInt16);
    scilab_fill_add(Int8, Int32, M_M, Int8, Int32, Int32);
    scilab_fill_add(Int8, UInt32, M_M, Int8, UInt32, UInt32);
    scilab_fill_add(Int8, Int64, M_M, Int8, Int64, Int64);
    scilab_fill_add(Int8, UInt64, M_M, Int8, UInt64, UInt64);
    scilab_fill_add(Int8, Bool, M_M, Int8, Bool, Int8);
    scilab_fill_add(Int8, Empty, M_E, Int8, Double, Int8);

    //Matrix + Scalar
    scilab_fill_add(Int8, ScalarDouble, M_S, Int8, Double, Int8);
    scilab_fill_add(Int8, ScalarInt8, M_S, Int8, Int8, Int8);
    scilab_fill_add(Int8, ScalarUInt8, M_S, Int8, UInt8, UInt8);
    scilab_fill_add(Int8, ScalarInt16, M_S, Int8, Int16, Int16);
    scilab_fill_add(Int8, ScalarUInt16, M_S, Int8, UInt16, UInt16);
    scilab_fill_add(Int8, ScalarInt32, M_S, Int8, Int32, Int32);
    scilab_fill_add(Int8, ScalarUInt32, M_S, Int8, UInt32, UInt32);
    scilab_fill_add(Int8, ScalarInt64, M_S, Int8, Int64, Int64);
    scilab_fill_add(Int8, ScalarUInt64, M_S, Int8, UInt64, UInt64);
    scilab_fill_add(Int8, ScalarBool, M_S, Int8, Bool, Int8);

    //Scalar + Matrix
    scilab_fill_add(ScalarInt8, Double, S_M, Int8, Double, Int8);
    scilab_fill_add(ScalarInt8, Int8, S_M, Int8, Int8, Int8);
    scilab_fill_add(ScalarInt8, UInt8, S_M, Int8, UInt8, UInt8);
    scilab_fill_add(ScalarInt8, Int16, S_M, Int8, Int16, Int16);
    scilab_fill_add(ScalarInt8, UInt16, S_M, Int8, UInt16, UInt16);
    scilab_fill_add(ScalarInt8, Int32, S_M, Int8, Int32, Int32);
    scilab_fill_add(ScalarInt8, UInt32, S_M, Int8, UInt32, UInt32);
    scilab_fill_add(ScalarInt8, Int64, S_M, Int8, Int64, Int64);
    scilab_fill_add(ScalarInt8, UInt64, S_M, Int8, UInt64, UInt64);
    scilab_fill_add(ScalarInt8, Bool, S_M, Int8, Bool, Int8);
    scilab_fill_add(ScalarInt8, Empty, S_E, Int8, Double, Int8);

    //Scalar + Scalar
    scilab_fill_add(ScalarInt8, ScalarDouble, S_S, Int8, Double, Int8);
    scilab_fill_add(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Int8);
    scilab_fill_add(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, UInt8);
    scilab_fill_add(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Int16);
    scilab_fill_add(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, UInt16);
    scilab_fill_add(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Int32);
    scilab_fill_add(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, UInt32);
    scilab_fill_add(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Int64);
    scilab_fill_add(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, UInt64);
    scilab_fill_add(ScalarInt8, ScalarBool, S_S, Int8, Bool, Int8);

    //UInt8
    //Matrix + Matrix
    scilab_fill_add(UInt8, Double, M_M, UInt8, Double, UInt8);
    scilab_fill_add(UInt8, Int8, M_M, UInt8, Int8, UInt8);
    scilab_fill_add(UInt8, UInt8, M_M, UInt8, UInt8, UInt8);
    scilab_fill_add(UInt8, Int16, M_M, UInt8, Int16, UInt16);
    scilab_fill_add(UInt8, UInt16, M_M, UInt8, UInt16, UInt16);
    scilab_fill_add(UInt8, Int32, M_M, UInt8, Int32, UInt32);
    scilab_fill_add(UInt8, UInt32, M_M, UInt8, UInt32, UInt32);
    scilab_fill_add(UInt8, Int64, M_M, UInt8, Int64, UInt64);
    scilab_fill_add(UInt8, UInt64, M_M, UInt8, UInt64, UInt64);
    scilab_fill_add(UInt8, Bool, M_M, UInt8, Bool, UInt8);
    scilab_fill_add(UInt8, Empty, M_E, UInt8, Double, UInt8);

    //Matrix + Scalar
    scilab_fill_add(UInt8, ScalarDouble, M_S, UInt8, Double, UInt8);
    scilab_fill_add(UInt8, ScalarInt8, M_S, UInt8, Int8, UInt8);
    scilab_fill_add(UInt8, ScalarUInt8, M_S, UInt8, UInt8, UInt8);
    scilab_fill_add(UInt8, ScalarInt16, M_S, UInt8, Int16, UInt16);
    scilab_fill_add(UInt8, ScalarUInt16, M_S, UInt8, UInt16, UInt16);
    scilab_fill_add(UInt8, ScalarInt32, M_S, UInt8, Int32, UInt32);
    scilab_fill_add(UInt8, ScalarUInt32, M_S, UInt8, UInt32, UInt32);
    scilab_fill_add(UInt8, ScalarInt64, M_S, UInt8, Int64, UInt64);
    scilab_fill_add(UInt8, ScalarUInt64, M_S, UInt8, UInt64, UInt64);
    scilab_fill_add(UInt8, ScalarBool, M_S, UInt8, Bool, UInt8);

    //Scalar + Matrix
    scilab_fill_add(ScalarUInt8, Double, S_M, UInt8, Double, UInt8);
    scilab_fill_add(ScalarUInt8, Int8, S_M, UInt8, Int8, UInt8);
    scilab_fill_add(ScalarUInt8, UInt8, S_M, UInt8, UInt8, UInt8);
    scilab_fill_add(ScalarUInt8, Int16, S_M, UInt8, Int16, UInt16);
    scilab_fill_add(ScalarUInt8, UInt16, S_M, UInt8, UInt16, UInt16);
    scilab_fill_add(ScalarUInt8, Int32, S_M, UInt8, Int32, UInt32);
    scilab_fill_add(ScalarUInt8, UInt32, S_M, UInt8, UInt32, UInt32);
    scilab_fill_add(ScalarUInt8, Int64, S_M, UInt8, Int64, UInt64);
    scilab_fill_add(ScalarUInt8, UInt64, S_M, UInt8, UInt64, UInt64);
    scilab_fill_add(ScalarUInt8, Bool, S_M, UInt8, Bool, UInt8);
    scilab_fill_add(ScalarUInt8, Empty, S_E, UInt8, Double, UInt8);

    //Scalar + Scalar
    scilab_fill_add(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, UInt8);
    scilab_fill_add(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, UInt8);
    scilab_fill_add(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, UInt8);
    scilab_fill_add(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, UInt16);
    scilab_fill_add(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, UInt16);
    scilab_fill_add(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, UInt32);
    scilab_fill_add(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, UInt32);
    scilab_fill_add(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, UInt64);
    scilab_fill_add(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, UInt64);
    scilab_fill_add(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, UInt8);

    //Int16
    //Matrix + Matrix
    scilab_fill_add(Int16, Double, M_M, Int16, Double, Int16);
    scilab_fill_add(Int16, Int8, M_M, Int16, Int8, Int16);
    scilab_fill_add(Int16, UInt8, M_M, Int16, UInt8, UInt16);
    scilab_fill_add(Int16, Int16, M_M, Int16, Int16, Int16);
    scilab_fill_add(Int16, UInt16, M_M, Int16, UInt16, UInt16);
    scilab_fill_add(Int16, Int32, M_M, Int16, Int32, Int32);
    scilab_fill_add(Int16, UInt32, M_M, Int16, UInt32, UInt32);
    scilab_fill_add(Int16, Int64, M_M, Int16, Int64, Int64);
    scilab_fill_add(Int16, UInt64, M_M, Int16, UInt64, UInt64);
    scilab_fill_add(Int16, Bool, M_M, Int16, Bool, Int16);
    scilab_fill_add(Int16, Empty, M_E, Int16, Double, Int16);

    //Matrix + Scalar
    scilab_fill_add(Int16, ScalarDouble, M_S, Int16, Double, Int16);
    scilab_fill_add(Int16, ScalarInt8, M_S, Int16, Int8, Int16);
    scilab_fill_add(Int16, ScalarUInt8, M_S, Int16, UInt8, UInt16);
    scilab_fill_add(Int16, ScalarInt16, M_S, Int16, Int16, Int16);
    scilab_fill_add(Int16, ScalarUInt16, M_S, Int16, UInt16, UInt16);
    scilab_fill_add(Int16, ScalarInt32, M_S, Int16, Int32, Int32);
    scilab_fill_add(Int16, ScalarUInt32, M_S, Int16, UInt32, UInt32);
    scilab_fill_add(Int16, ScalarInt64, M_S, Int16, Int64, Int64);
    scilab_fill_add(Int16, ScalarUInt64, M_S, Int16, UInt64, UInt64);
    scilab_fill_add(Int16, ScalarBool, M_S, Int16, Bool, Int16);

    //Scalar + Matrix
    scilab_fill_add(ScalarInt16, Double, S_M, Int16, Double, Int16);
    scilab_fill_add(ScalarInt16, Int8, S_M, Int16, Int8, Int16);
    scilab_fill_add(ScalarInt16, UInt8, S_M, Int16, UInt8, UInt16);
    scilab_fill_add(ScalarInt16, Int16, S_M, Int16, Int16, Int16);
    scilab_fill_add(ScalarInt16, UInt16, S_M, Int16, UInt16, UInt16);
    scilab_fill_add(ScalarInt16, Int32, S_M, Int16, Int32, Int32);
    scilab_fill_add(ScalarInt16, UInt32, S_M, Int16, UInt32, UInt32);
    scilab_fill_add(ScalarInt16, Int64, S_M, Int16, Int64, Int64);
    scilab_fill_add(ScalarInt16, UInt64, S_M, Int16, UInt64, UInt64);
    scilab_fill_add(ScalarInt16, Bool, S_M, Int16, Bool, Int16);
    scilab_fill_add(ScalarInt16, Empty, S_E, Int16, Double, Int16);

    //Scalar + Scalar
    scilab_fill_add(ScalarInt16, ScalarDouble, S_S, Int16, Double, Int16);
    scilab_fill_add(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Int16);
    scilab_fill_add(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, UInt16);
    scilab_fill_add(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Int16);
    scilab_fill_add(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, UInt16);
    scilab_fill_add(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Int32);
    scilab_fill_add(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, UInt32);
    scilab_fill_add(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Int64);
    scilab_fill_add(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, UInt64);
    scilab_fill_add(ScalarInt16, ScalarBool, S_S, Int16, Bool, Int16);

    //UInt16
    //Matrix + Matrix
    scilab_fill_add(UInt16, Double, M_M, UInt16, Double, UInt16);
    scilab_fill_add(UInt16, Int8, M_M, UInt16, Int8, UInt16);
    scilab_fill_add(UInt16, UInt8, M_M, UInt16, UInt8, UInt16);
    scilab_fill_add(UInt16, Int16, M_M, UInt16, Int16, UInt16);
    scilab_fill_add(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_add(UInt16, Int32, M_M, UInt16, Int32, UInt32);
    scilab_fill_add(UInt16, UInt32, M_M, UInt16, UInt32, UInt32);
    scilab_fill_add(UInt16, Int64, M_M, UInt16, Int64, UInt64);
    scilab_fill_add(UInt16, UInt64, M_M, UInt16, UInt64, UInt64);
    scilab_fill_add(UInt16, Bool, M_M, UInt16, Bool, UInt16);
    scilab_fill_add(UInt16, Empty, M_E, UInt16, Double, UInt16);

    //Matrix + Scalar
    scilab_fill_add(UInt16, ScalarDouble, M_S, UInt16, Double, UInt16);
    scilab_fill_add(UInt16, ScalarInt8, M_S, UInt16, Int8, UInt16);
    scilab_fill_add(UInt16, ScalarUInt8, M_S, UInt16, UInt8, UInt16);
    scilab_fill_add(UInt16, ScalarInt16, M_S, UInt16, Int16, UInt16);
    scilab_fill_add(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_add(UInt16, ScalarInt32, M_S, UInt16, Int32, UInt32);
    scilab_fill_add(UInt16, ScalarUInt32, M_S, UInt16, UInt32, UInt32);
    scilab_fill_add(UInt16, ScalarInt64, M_S, UInt16, Int64, UInt64);
    scilab_fill_add(UInt16, ScalarUInt64, M_S, UInt16, UInt64, UInt64);
    scilab_fill_add(UInt16, ScalarBool, M_S, UInt16, Bool, UInt16);

    //Scalar + Matrix
    scilab_fill_add(ScalarUInt16, Double, S_M, UInt16, Double, UInt16);
    scilab_fill_add(ScalarUInt16, Int8, S_M, UInt16, Int8, UInt16);
    scilab_fill_add(ScalarUInt16, UInt8, S_M, UInt16, UInt8, UInt16);
    scilab_fill_add(ScalarUInt16, Int16, S_M, UInt16, Int16, UInt16);
    scilab_fill_add(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_add(ScalarUInt16, Int32, S_M, UInt16, Int32, UInt32);
    scilab_fill_add(ScalarUInt16, UInt32, S_M, UInt16, UInt32, UInt32);
    scilab_fill_add(ScalarUInt16, Int64, S_M, UInt16, Int64, UInt64);
    scilab_fill_add(ScalarUInt16, UInt64, S_M, UInt16, UInt64, UInt64);
    scilab_fill_add(ScalarUInt16, Bool, S_M, UInt16, Bool, UInt16);
    scilab_fill_add(ScalarUInt16, Empty, S_E, UInt16, Double, UInt16);

    //Scalar + Scalar
    scilab_fill_add(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, UInt16);
    scilab_fill_add(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, UInt16);
    scilab_fill_add(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, UInt16);
    scilab_fill_add(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, UInt16);
    scilab_fill_add(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_add(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, UInt32);
    scilab_fill_add(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, UInt32);
    scilab_fill_add(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, UInt64);
    scilab_fill_add(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, UInt64);
    scilab_fill_add(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, UInt16);

    //Int32
    //Matrix + Matrix
    scilab_fill_add(Int32, Double, M_M, Int32, Double, Int32);
    scilab_fill_add(Int32, Int8, M_M, Int32, Int8, Int32);
    scilab_fill_add(Int32, UInt8, M_M, Int32, UInt8, UInt32);
    scilab_fill_add(Int32, Int16, M_M, Int32, Int16, Int32);
    scilab_fill_add(Int32, UInt16, M_M, Int32, UInt16, UInt32);
    scilab_fill_add(Int32, Int32, M_M, Int32, Int32, Int32);
    scilab_fill_add(Int32, UInt32, M_M, Int32, UInt32, UInt32);
    scilab_fill_add(Int32, Int64, M_M, Int32, Int64, Int64);
    scilab_fill_add(Int32, UInt64, M_M, Int32, UInt64, UInt64);
    scilab_fill_add(Int32, Bool, M_M, Int32, Bool, Int32);
    scilab_fill_add(Int32, Empty, M_E, Int32, Double, Int32);

    //Matrix + Scalar
    scilab_fill_add(Int32, ScalarDouble, M_S, Int32, Double, Int32);
    scilab_fill_add(Int32, ScalarInt8, M_S, Int32, Int8, Int32);
    scilab_fill_add(Int32, ScalarUInt8, M_S, Int32, UInt8, UInt32);
    scilab_fill_add(Int32, ScalarInt16, M_S, Int32, Int16, Int32);
    scilab_fill_add(Int32, ScalarUInt16, M_S, Int32, UInt16, UInt32);
    scilab_fill_add(Int32, ScalarInt32, M_S, Int32, Int32, Int32);
    scilab_fill_add(Int32, ScalarUInt32, M_S, Int32, UInt32, UInt32);
    scilab_fill_add(Int32, ScalarInt64, M_S, Int32, Int64, Int64);
    scilab_fill_add(Int32, ScalarUInt64, M_S, Int32, UInt64, UInt64);
    scilab_fill_add(Int32, ScalarBool, M_S, Int32, Bool, Int32);

    //Scalar + Matrix
    scilab_fill_add(ScalarInt32, Double, S_M, Int32, Double, Int32);
    scilab_fill_add(ScalarInt32, Int8, S_M, Int32, Int8, Int32);
    scilab_fill_add(ScalarInt32, UInt8, S_M, Int32, UInt8, UInt32);
    scilab_fill_add(ScalarInt32, Int16, S_M, Int32, Int16, Int32);
    scilab_fill_add(ScalarInt32, UInt16, S_M, Int32, UInt16, UInt32);
    scilab_fill_add(ScalarInt32, Int32, S_M, Int32, Int32, Int32);
    scilab_fill_add(ScalarInt32, UInt32, S_M, Int32, UInt32, UInt32);
    scilab_fill_add(ScalarInt32, Int64, S_M, Int32, Int64, Int64);
    scilab_fill_add(ScalarInt32, UInt64, S_M, Int32, UInt64, UInt64);
    scilab_fill_add(ScalarInt32, Bool, S_M, Int32, Bool, Int32);
    scilab_fill_add(ScalarInt32, Empty, S_E, Int32, Double, Int32);

    //Scalar + Scalar
    scilab_fill_add(ScalarInt32, ScalarDouble, S_S, Int32, Double, Int32);
    scilab_fill_add(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Int32);
    scilab_fill_add(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, UInt32);
    scilab_fill_add(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Int32);
    scilab_fill_add(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, UInt32);
    scilab_fill_add(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Int32);
    scilab_fill_add(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, UInt32);
    scilab_fill_add(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Int64);
    scilab_fill_add(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, UInt64);
    scilab_fill_add(ScalarInt32, ScalarBool, S_S, Int32, Bool, Int32);

    //UInt32
    //Matrix + Matrix
    scilab_fill_add(UInt32, Double, M_M, UInt32, Double, UInt32);
    scilab_fill_add(UInt32, Int8, M_M, UInt32, Int8, UInt32);
    scilab_fill_add(UInt32, UInt8, M_M, UInt32, UInt8, UInt32);
    scilab_fill_add(UInt32, Int16, M_M, UInt32, Int16, UInt32);
    scilab_fill_add(UInt32, UInt16, M_M, UInt32, UInt16, UInt32);
    scilab_fill_add(UInt32, Int32, M_M, UInt32, Int32, UInt32);
    scilab_fill_add(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_add(UInt32, Int64, M_M, UInt32, Int64, UInt64);
    scilab_fill_add(UInt32, UInt64, M_M, UInt32, UInt64, UInt64);
    scilab_fill_add(UInt32, Bool, M_M, UInt32, Bool, UInt32);
    scilab_fill_add(UInt32, Empty, M_E, UInt32, Double, UInt32);

    //Matrix + Scalar
    scilab_fill_add(UInt32, ScalarDouble, M_S, UInt32, Double, UInt32);
    scilab_fill_add(UInt32, ScalarInt8, M_S, UInt32, Int8, UInt32);
    scilab_fill_add(UInt32, ScalarUInt8, M_S, UInt32, UInt8, UInt32);
    scilab_fill_add(UInt32, ScalarInt16, M_S, UInt32, Int16, UInt32);
    scilab_fill_add(UInt32, ScalarUInt16, M_S, UInt32, UInt16, UInt32);
    scilab_fill_add(UInt32, ScalarInt32, M_S, UInt32, Int32, UInt32);
    scilab_fill_add(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_add(UInt32, ScalarInt64, M_S, UInt32, Int64, UInt64);
    scilab_fill_add(UInt32, ScalarUInt64, M_S, UInt32, UInt64, UInt64);
    scilab_fill_add(UInt32, ScalarBool, M_S, UInt32, Bool, UInt32);

    //Scalar + Matrix
    scilab_fill_add(ScalarUInt32, Double, S_M, UInt32, Double, UInt32);
    scilab_fill_add(ScalarUInt32, Int8, S_M, UInt32, Int8, UInt32);
    scilab_fill_add(ScalarUInt32, UInt8, S_M, UInt32, UInt8, UInt32);
    scilab_fill_add(ScalarUInt32, Int16, S_M, UInt32, Int16, UInt32);
    scilab_fill_add(ScalarUInt32, UInt16, S_M, UInt32, UInt16, UInt32);
    scilab_fill_add(ScalarUInt32, Int32, S_M, UInt32, Int32, UInt32);
    scilab_fill_add(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_add(ScalarUInt32, Int64, S_M, UInt32, Int64, UInt64);
    scilab_fill_add(ScalarUInt32, UInt64, S_M, UInt32, UInt64, UInt64);
    scilab_fill_add(ScalarUInt32, Bool, S_M, UInt32, Bool, UInt32);
    scilab_fill_add(ScalarUInt32, Empty, S_E, UInt32, Double, UInt32);

    //Scalar + Scalar
    scilab_fill_add(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_add(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, UInt64);
    scilab_fill_add(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, UInt64);
    scilab_fill_add(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, UInt32);

    //Int64
    //Matrix + Matrix
    scilab_fill_add(Int64, Double, M_M, Int64, Double, Int64);
    scilab_fill_add(Int64, Int8, M_M, Int64, Int8, Int64);
    scilab_fill_add(Int64, UInt8, M_M, Int64, UInt8, UInt64);
    scilab_fill_add(Int64, Int16, M_M, Int64, Int16, Int64);
    scilab_fill_add(Int64, UInt16, M_M, Int64, UInt16, UInt64);
    scilab_fill_add(Int64, Int32, M_M, Int64, Int32, Int64);
    scilab_fill_add(Int64, UInt32, M_M, Int64, UInt32, UInt64);
    scilab_fill_add(Int64, Int64, M_M, Int64, Int64, Int64);
    scilab_fill_add(Int64, UInt64, M_M, Int64, UInt64, UInt64);
    scilab_fill_add(Int64, Bool, M_M, Int64, Bool, Int64);
    scilab_fill_add(Int64, Empty, M_E, Int64, Double, Int64);

    //Matrix + Scalar
    scilab_fill_add(Int64, ScalarDouble, M_S, Int64, Double, Int64);
    scilab_fill_add(Int64, ScalarInt8, M_S, Int64, Int8, Int64);
    scilab_fill_add(Int64, ScalarUInt8, M_S, Int64, UInt8, UInt64);
    scilab_fill_add(Int64, ScalarInt16, M_S, Int64, Int16, Int64);
    scilab_fill_add(Int64, ScalarUInt16, M_S, Int64, UInt16, UInt64);
    scilab_fill_add(Int64, ScalarInt32, M_S, Int64, Int32, Int64);
    scilab_fill_add(Int64, ScalarUInt32, M_S, Int64, UInt32, UInt64);
    scilab_fill_add(Int64, ScalarInt64, M_S, Int64, Int64, Int64);
    scilab_fill_add(Int64, ScalarUInt64, M_S, Int64, UInt64, UInt64);
    scilab_fill_add(Int64, ScalarBool, M_S, Int64, Bool, Int64);

    //Scalar + Matrix
    scilab_fill_add(ScalarInt64, Double, S_M, Int64, Double, Int64);
    scilab_fill_add(ScalarInt64, Int8, S_M, Int64, Int8, Int64);
    scilab_fill_add(ScalarInt64, UInt8, S_M, Int64, UInt8, UInt64);
    scilab_fill_add(ScalarInt64, Int16, S_M, Int64, Int16, Int64);
    scilab_fill_add(ScalarInt64, UInt16, S_M, Int64, UInt16, UInt64);
    scilab_fill_add(ScalarInt64, Int32, S_M, Int64, Int32, Int64);
    scilab_fill_add(ScalarInt64, UInt32, S_M, Int64, UInt32, UInt64);
    scilab_fill_add(ScalarInt64, Int64, S_M, Int64, Int64, Int64);
    scilab_fill_add(ScalarInt64, UInt64, S_M, Int64, UInt64, UInt64);
    scilab_fill_add(ScalarInt64, Bool, S_M, Int64, Bool, Int64);
    scilab_fill_add(ScalarInt64, Empty, S_E, Int64, Double, Int64);

    //Scalar + Scalar
    scilab_fill_add(ScalarInt64, ScalarDouble, S_S, Int64, Double, Int64);
    scilab_fill_add(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Int64);
    scilab_fill_add(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, UInt64);
    scilab_fill_add(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Int64);
    scilab_fill_add(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, UInt64);
    scilab_fill_add(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Int64);
    scilab_fill_add(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, UInt64);
    scilab_fill_add(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Int64);
    scilab_fill_add(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, UInt64);
    scilab_fill_add(ScalarInt64, ScalarBool, S_S, Int64, Bool, Int64);

    //UInt64
    //Matrix + Matrix
    scilab_fill_add(UInt64, Double, M_M, UInt64, Double, UInt64);
    scilab_fill_add(UInt64, Int8, M_M, UInt64, Int8, UInt64);
    scilab_fill_add(UInt64, UInt8, M_M, UInt64, UInt8, UInt64);
    scilab_fill_add(UInt64, Int16, M_M, UInt64, Int16, UInt64);
    scilab_fill_add(UInt64, UInt16, M_M, UInt64, UInt16, UInt64);
    scilab_fill_add(UInt64, Int32, M_M, UInt64, Int32, UInt64);
    scilab_fill_add(UInt64, UInt32, M_M, UInt64, UInt32, UInt64);
    scilab_fill_add(UInt64, Int64, M_M, UInt64, Int64, UInt64);
    scilab_fill_add(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);
    scilab_fill_add(UInt64, Bool, M_M, UInt64, Bool, UInt64);
    scilab_fill_add(UInt64, Empty, M_E, UInt64, Double, UInt64);

    //Matrix + Scalar
    scilab_fill_add(UInt64, ScalarDouble, M_S, UInt64, Double, UInt64);
    scilab_fill_add(UInt64, ScalarInt8, M_S, UInt64, Int8, UInt64);
    scilab_fill_add(UInt64, ScalarUInt8, M_S, UInt64, UInt8, UInt64);
    scilab_fill_add(UInt64, ScalarInt16, M_S, UInt64, Int16, UInt64);
    scilab_fill_add(UInt64, ScalarUInt16, M_S, UInt64, UInt16, UInt64);
    scilab_fill_add(UInt64, ScalarInt32, M_S, UInt64, Int32, UInt64);
    scilab_fill_add(UInt64, ScalarUInt32, M_S, UInt64, UInt32, UInt64);
    scilab_fill_add(UInt64, ScalarInt64, M_S, UInt64, Int64, UInt64);
    scilab_fill_add(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);
    scilab_fill_add(UInt64, ScalarBool, M_S, UInt64, Bool, UInt64);

    //Scalar + Matrix
    scilab_fill_add(ScalarUInt64, Double, S_M, UInt64, Double, UInt64);
    scilab_fill_add(ScalarUInt64, Int8, S_M, UInt64, Int8, UInt64);
    scilab_fill_add(ScalarUInt64, UInt8, S_M, UInt64, UInt8, UInt64);
    scilab_fill_add(ScalarUInt64, Int16, S_M, UInt64, Int16, UInt64);
    scilab_fill_add(ScalarUInt64, UInt16, S_M, UInt64, UInt16, UInt64);
    scilab_fill_add(ScalarUInt64, Int32, S_M, UInt64, Int32, UInt64);
    scilab_fill_add(ScalarUInt64, UInt32, S_M, UInt64, UInt32, UInt64);
    scilab_fill_add(ScalarUInt64, Int64, S_M, UInt64, Int64, UInt64);
    scilab_fill_add(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);
    scilab_fill_add(ScalarUInt64, Bool, S_M, UInt64, Bool, UInt64);
    scilab_fill_add(ScalarUInt64, Empty, S_E, UInt64, Double, UInt64);

    //Scalar + Scalar
    scilab_fill_add(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
    scilab_fill_add(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, UInt64);

    //Bool
    //Matrix + Matrix
    scilab_fill_add(Bool, Double, M_M, Bool, Double, Double);
    scilab_fill_add(Bool, Int8, M_M, Bool, Int8, Int8);
    scilab_fill_add(Bool, UInt8, M_M, Bool, UInt8, UInt8);
    scilab_fill_add(Bool, Int16, M_M, Bool, Int16, Int16);
    scilab_fill_add(Bool, UInt16, M_M, Bool, UInt16, UInt16);
    scilab_fill_add(Bool, Int32, M_M, Bool, Int32, Int32);
    scilab_fill_add(Bool, UInt32, M_M, Bool, UInt32, UInt32);
    scilab_fill_add(Bool, Int64, M_M, Bool, Int64, Int64);
    scilab_fill_add(Bool, UInt64, M_M, Bool, UInt64, UInt64);
    scilab_fill_add(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_add(Bool, Empty, M_E, Bool, Double, Double);

    //Matrix + Scalar
    scilab_fill_add(Bool, ScalarDouble, M_S, Bool, Double, Double);
    scilab_fill_add(Bool, ScalarInt8, M_S, Bool, Int8, Int8);
    scilab_fill_add(Bool, ScalarUInt8, M_S, Bool, UInt8, UInt8);
    scilab_fill_add(Bool, ScalarInt16, M_S, Bool, Int16, Int16);
    scilab_fill_add(Bool, ScalarUInt16, M_S, Bool, UInt16, UInt16);
    scilab_fill_add(Bool, ScalarInt32, M_S, Bool, Int32, Int32);
    scilab_fill_add(Bool, ScalarUInt32, M_S, Bool, UInt32, UInt32);
    scilab_fill_add(Bool, ScalarInt64, M_S, Bool, Int64, Int64);
    scilab_fill_add(Bool, ScalarUInt64, M_S, Bool, UInt64, UInt64);
    scilab_fill_add(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar + Matrix
    scilab_fill_add(ScalarBool, Double, S_M, Bool, Double, Double);
    scilab_fill_add(ScalarBool, Int8, S_M, Bool, Int8, Int8);
    scilab_fill_add(ScalarBool, UInt8, S_M, Bool, UInt8, UInt8);
    scilab_fill_add(ScalarBool, Int16, S_M, Bool, Int16, Int16);
    scilab_fill_add(ScalarBool, UInt16, S_M, Bool, UInt16, UInt16);
    scilab_fill_add(ScalarBool, Int32, S_M, Bool, Int32, Int32);
    scilab_fill_add(ScalarBool, UInt32, S_M, Bool, UInt32, UInt32);
    scilab_fill_add(ScalarBool, Int64, S_M, Bool, Int64, Int64);
    scilab_fill_add(ScalarBool, UInt64, S_M, Bool, UInt64, UInt64);
    scilab_fill_add(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_add(ScalarBool, Empty, S_E, Bool, Double, Double);

    //Scalar + Scalar
    scilab_fill_add(ScalarBool, ScalarDouble, S_S, Bool, Double, Double);
    scilab_fill_add(ScalarBool, ScalarInt8, S_S, Bool, Int8, Int8);
    scilab_fill_add(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, UInt8);
    scilab_fill_add(ScalarBool, ScalarInt16, S_S, Bool, Int16, Int16);
    scilab_fill_add(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, UInt16);
    scilab_fill_add(ScalarBool, ScalarInt32, S_S, Bool, Int32, Int32);
    scilab_fill_add(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, UInt32);
    scilab_fill_add(ScalarBool, ScalarInt64, S_S, Bool, Int64, Int64);
    scilab_fill_add(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, UInt64);
    scilab_fill_add(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //String
    scilab_fill_add(String, String, M_M, String, String, String);
    scilab_fill_add(String, ScalarString, M_S, String, String, String);
    scilab_fill_add(String, Empty, M_E, String, Double, String);

    scilab_fill_add(ScalarString, String, S_M, String, String, String);
    scilab_fill_add(ScalarString, ScalarString, S_S, String, String, String);
    scilab_fill_add(ScalarString, Empty, S_E, String, Double, String);

    //Identity
    scilab_fill_add(Identity, Double, I_M, Double, Double, Double);
    scilab_fill_add(Identity, DoubleComplex, I_MC, Double, Double, Double);
    scilab_fill_add(Identity, ScalarDouble, I_S, Double, Double, Double);
    scilab_fill_add(Identity, ScalarDoubleComplex, I_SC, Double, Double, Double);
    scilab_fill_add(Identity, Identity, I_I, Double, Double, Double);
    scilab_fill_add(Identity, IdentityComplex, I_IC, Double, Double, Double);
    scilab_fill_add(Identity, Empty, I_E, Double, Double, Double);

    scilab_fill_add(Identity, Polynom, I_M, Double, Polynom, Polynom);
    scilab_fill_add(Identity, PolynomComplex, I_M, Double, Polynom, Polynom);
    scilab_fill_add(Identity, ScalarPolynom, I_M, Double, Polynom, Polynom);
    scilab_fill_add(Identity, ScalarPolynomComplex, I_M, Double, Polynom, Polynom);
    scilab_fill_add(Identity, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_add(Identity, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_add(IdentityComplex, Double, IC_M, Double, Double, Double);
    scilab_fill_add(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Double);
    scilab_fill_add(IdentityComplex, ScalarDouble, IC_S, Double, Double, Double);
    scilab_fill_add(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Double);
    scilab_fill_add(IdentityComplex, Identity, IC_I, Double, Double, Double);
    scilab_fill_add(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Double);
    scilab_fill_add(IdentityComplex, Empty, IC_E, Double, Double, Double);

    scilab_fill_add(IdentityComplex, Polynom, I_M, Double, Polynom, Polynom);
    scilab_fill_add(IdentityComplex, PolynomComplex, I_M, Double, Polynom, Polynom);
    scilab_fill_add(IdentityComplex, ScalarPolynom, I_M, Double, Polynom, Polynom);
    scilab_fill_add(IdentityComplex, ScalarPolynomComplex, I_M, Double, Polynom, Polynom);
    scilab_fill_add(IdentityComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_add(IdentityComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Polynom

    //poly + poly
    scilab_fill_add(Polynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(Polynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(PolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + scalar poly
    scilab_fill_add(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + double
    scilab_fill_add(Polynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_add(Polynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + scalar double
    scilab_fill_add(Polynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_add(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + []
    scilab_fill_add(Polynom, Empty, M_E, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, Empty, M_E, Polynom, Double, Polynom);

    //poly + eye
    scilab_fill_add(Polynom, Identity, M_I, Polynom, Double, Polynom);
    scilab_fill_add(Polynom, IdentityComplex, M_I, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, Identity, M_I, Polynom, Double, Polynom);
    scilab_fill_add(PolynomComplex, IdentityComplex, M_I, Polynom, Double, Polynom);

    //scalar poly + poly
    scilab_fill_add(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + scalar poly
    scilab_fill_add(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_add(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + double
    scilab_fill_add(ScalarPolynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + scalar double
    scilab_fill_add(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + []
    scilab_fill_add(ScalarPolynom, Empty, M_E, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Polynom);

    //scalar poly + eye
    scilab_fill_add(ScalarPolynom, Identity, M_I, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynom, IdentityComplex, M_I, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, Identity, M_I, Polynom, Double, Polynom);
    scilab_fill_add(ScalarPolynomComplex, IdentityComplex, M_I, Polynom, Double, Polynom);

    //Sparse
    scilab_fill_add(Sparse, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_add(Sparse, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_add(Sparse, Double, M_M, Sparse, Double, Double);
    scilab_fill_add(Sparse, DoubleComplex, M_M, Sparse, Double, Double);
    scilab_fill_add(Sparse, ScalarDouble, M_M, Sparse, Double, Double);
    scilab_fill_add(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, Double);

    scilab_fill_add(Sparse, Empty, M_E, Sparse, Double, Sparse);
    scilab_fill_add(Sparse, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_add(Sparse, IdentityComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_add(SparseComplex, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_add(SparseComplex, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_add(SparseComplex, Double, M_M, Sparse, Double, Double);
    scilab_fill_add(SparseComplex, DoubleComplex, M_M, Sparse, Double, Double);
    scilab_fill_add(SparseComplex, ScalarDouble, M_M, Sparse, Double, Double);
    scilab_fill_add(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, Double);

    scilab_fill_add(SparseComplex, Empty, M_E, Sparse, Double, Sparse);
    scilab_fill_add(SparseComplex, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_add(SparseComplex, IdentityComplex, M_M, Sparse, Double, Sparse);

#undef scilab_fill_add
}

InternalType *GenericPlus(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    add_function add = pAddfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (add)
    {
        pResult = add(_pLeftOperand, _pRightOperand);
        if (pResult)
        {
            return pResult;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

int AddSparseToSparse(Sparse* sp1, Sparse* sp2, Sparse** pSpRes)
{
    //check scalar hidden in a sparse ;)
    if (sp1->getRows() == 1 && sp1->getCols() == 1)
    {
        //do scalar + sp
        Double* pDbl = NULL;
        if (sp1->isComplex())
        {
            std::complex<double> dbl = sp1->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(sp1->get(0, 0));
        }

        AddSparseToDouble(sp2, pDbl, (GenericType**)pSpRes);
        delete pDbl;
        return 0;
    }

    if (sp2->getRows() == 1 && sp2->getCols() == 1)
    {
        //do sp + scalar
        Double* pDbl = NULL;
        if (sp2->isComplex())
        {
            std::complex<double> dbl = sp2->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(sp2->get(0, 0));
        }

        AddSparseToDouble(sp1, pDbl, (GenericType**)pSpRes);
        delete pDbl;
        return 0;
    }

    if (sp1->getRows() != sp2->getRows() || sp1->getCols() != sp2->getCols())
    {
        //dimensions not match
        return 1;
    }

    if (sp1->nonZeros() == 0)
    {
        //sp([]) + sp
        *pSpRes = new Sparse(*sp2);
        return 0;
    }

    if (sp2->nonZeros() == 0)
    {
        //sp + sp([])
        *pSpRes = new Sparse(*sp1);
        return 0;
    }

    //copy sp1 in pSpRes
    *pSpRes = sp1->add(*sp2);
    return 0;
}

int AddSparseToDouble(Sparse* sp, Double* d, GenericType** pDRes)
{
    int iOne = 1; //fortran
    bool bComplex1 = sp->isComplex();
    bool bComplex2 = d->isComplex();

    if (d->isIdentity())
    {
        //convert to sp
        Sparse* pS = new Sparse(sp->getRows(), sp->getCols(), d->isComplex());
        if (pS->isComplex())
        {
            for (int i = 0 ; i < std::min(sp->getRows(), sp->getCols()) ; i++)
            {
                pS->set(i, i, std::complex<double>(d->get(0), d->getImg(0)), false);
            }
        }
        else
        {
            for (int i = 0 ; i < std::min(sp->getRows(), sp->getCols()) ; i++)
            {
                pS->set(i, i, d->get(0), false);
            }
        }

        pS->finalize();
        AddSparseToSparse(sp, pS, (Sparse**)pDRes);
        delete pS;
        return 0;
    }

    if (sp->isScalar() && d->isScalar())
    {
        //sp + d
        Double* pRes = (Double*)d->clone();
        pRes->setComplex(bComplex1 | bComplex2);

        if (bComplex1)
        {
            std::complex<double> dbl = sp->getImg(0, 0);
            pRes->set(0, pRes->get(0) + dbl.real());
            pRes->setImg(0, pRes->getImg(0) + dbl.imag());
        }
        else
        {
            pRes->set(0, pRes->get(0) + sp->getReal(0));
        }

        *pDRes = pRes;
        return 0;
    }

    if (d->isScalar())
    {
        //SP + d
        Double* pRes = new Double(sp->getRows(), sp->getCols(), bComplex1 | bComplex2);
        int iSize = sp->getSize();
        double dblVal = d->get(0);
        C2F(dset)(&iSize, &dblVal, pRes->get(), &iOne);
        if (bComplex2)
        {
            double dblValI = d->getImg(0);
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }
        else if (bComplex1)
        {
            //initialize imag part at 0
            double dblValI = 0;
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }

        int nonZeros = static_cast<int>(sp->nonZeros());
        int* pRows = new int[nonZeros * 2];
        sp->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = sp->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + sp->get(iRow, iCol));
            }
        }
        *pDRes = pRes;

        //clear
        delete[] pRows;

        return 0;
    }

    if (sp->isScalar())
    {
        //sp + D
        Double* pRes = (Double*)d->clone();
        pRes->setComplex(bComplex1 | bComplex2);

        if (bComplex1)
        {
            double* pReal = pRes->get();
            double* pImg = pRes->getImg();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                std::complex<double> dbl = sp->getImg(0, 0);
                pReal[i] += dbl.real();
                pImg[i] += dbl.imag();
            }
        }
        else
        {
            double* pReal = pRes->get();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                pReal[i] += sp->get(0, 0);
            }
        }

        *pDRes = pRes;
        return 0;
    }


    if (sp->getRows() == d->getRows() && sp->getCols() == d->getCols())
    {
        //SP + D
        Double* pRes = (Double*)d->clone();
        pRes->setComplex(bComplex1 | bComplex2);

        int nonZeros = static_cast<int>(sp->nonZeros());
        int* pRows = new int[nonZeros * 2];
        sp->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = sp->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + sp->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;
        *pDRes = pRes;
        return 0;
    }
    return 1;
}

int AddDoubleToSparse(Double* d, Sparse* sp, GenericType** pDRes)
{
    /* uses commutativity */
    return AddSparseToDouble(sp, d, pDRes);
}

template<class T, class U, class O>
InternalType* add_M_M(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);

    add(_pL->get(), (size_t)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_M_MC(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);

    add(_pL->get(), (size_t)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    add(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_M_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}


template<class T, class U, class O>
InternalType* add_MC_M(T *_pL, U *_pR)
{
    return add_M_MC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_MC_MC(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);

    add(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_MC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}


template<class T, class U, class O>
InternalType* add_S_M(T *_pL, U *_pR)
{
    return add_M_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_S_MC(T *_pL, U *_pR)
{
    return add_MC_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    add(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_S_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}


template<class T, class U, class O>
InternalType* add_SC_M(T *_pL, U *_pR)
{
    return add_M_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_SC_MC(T *_pL, U *_pR)
{
    return add_MC_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_SC_S(T *_pL, U *_pR)
{
    return add_S_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* add_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_SC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}


template<class T, class U, class O>
InternalType* add_E_M(T *_pL, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O>
InternalType* add_E_MC(T *_pL, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O>
InternalType* add_E_S(T *_pL, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O>
InternalType* add_E_SC(T *_pL, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O>
InternalType* add_E_E(T * /*_pL*/, U * /*_pR*/)
{
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O>
InternalType* add_I_M(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)_pR->clone();
    double* pdblOut = pOut->get();
    double* pdblRight = _pR->get();
    double dblLeft = _pL->get(0);
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    piIndex[0] = 0;

    //find smaller dims
    for (int i = 1 ; i < iDims ; ++i)
    {
        //init
        piIndex[i] = 0;

        if (iLeadDims > piDims[i])
        {
            iLeadDims = piDims[i];
        }
    }

    for (int i = 0 ; i < iLeadDims ; ++i)
    {
        for (int j = 0 ; j < iDims ; ++j)
        {
            piIndex[j] = i;
        }

        int index = _pR->getIndex(piIndex);
        add(dblLeft, pdblRight[index], pdblOut + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* add_I_MC(T *_pL, U *_pR)
{
    return add_I_M<T, U, O>(_pL, _pR);
}

template<class T, class U, class O>
InternalType* add_IC_M(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)_pR->clone();
    pOut->setComplex(true);
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    piIndex[0] = 0;
    //find smaller dims
    for (int i = 1 ; i < iDims ; ++i)
    {
        //init
        piIndex[i] = 0;

        if (iLeadDims > piDims[i])
        {
            iLeadDims = piDims[i];
        }
    }

    for (int i = 0 ; i < iLeadDims ; ++i)
    {
        for (int j = 0 ; j < iDims ; ++j)
        {
            piIndex[j] = i;
        }

        int index = _pR->getIndex(piIndex);
        add(_pR->get() + index, 1, _pL->get(0), _pL->getImg(0), pOut->get() + index, pOut->getImg() + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* add_IC_MC(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)_pR->clone();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    piIndex[0] = 0;
    //find smaller dims
    for (int i = 1 ; i < iDims ; ++i)
    {
        //init
        piIndex[i] = 0;

        if (iLeadDims > piDims[i])
        {
            iLeadDims = piDims[i];
        }
    }

    for (int i = 0 ; i < iLeadDims ; ++i)
    {
        for (int j = 0 ; j < iDims ; ++j)
        {
            piIndex[j] = i;
        }

        int index = _pR->getIndex(piIndex);

        add(_pL->get(0), _pL->getImg(0), _pR->get(index), _pR->getImg(index), pOut->get() + index, pOut->getImg() + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* add_I_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    add(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_IC_S(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add( _pR->get(), 1, _pL->get(0), _pL->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_I_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* add_IC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(), _pL->getImg(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> InternalType* add_M_I(T *_pL, U *_pR)
{
    return add_I_M<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_MC_I(T *_pL, U *_pR)
{
    return add_I_MC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_M_IC(T *_pL, U *_pR)
{
    return add_IC_M<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_MC_IC(T *_pL, U *_pR)
{
    return add_IC_MC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_S_I(T *_pL, U *_pR)
{
    return add_I_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_SC_I(T *_pL, U *_pR)
{
    return add_I_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_S_IC(T *_pL, U *_pR)
{
    return add_IC_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_SC_IC(T *_pL, U *_pR)
{
    return add_IC_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_I_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    add(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O> InternalType* add_I_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    add(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> InternalType* add_IC_I(T *_pL, U *_pR)
{
    return add_I_IC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O> InternalType* add_IC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    add(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> types::InternalType* add_I_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O> types::InternalType* add_IC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O> types::InternalType* add_E_I(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<class T, class U, class O> types::InternalType* add_E_IC(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

//specifiaction for String Matrix + String Matrix
template<>
InternalType* add_M_M<String, String, String>(String* _pL, String* _pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    String* pOut = new String(iDimsL, piDimsL);
    int size = _pL->getSize();
    int* sizeOut = new int[size];
    for (int i = 0 ; i < size ; ++i)
    {
        wchar_t* pwstL = _pL->get(i);
        wchar_t* pwstR = _pR->get(i);
        int sizeL = (int)wcslen(pwstL);
        int sizeR = (int)wcslen(pwstR);

        sizeOut[i] = sizeL + sizeR + 1;
        wchar_t* pwstOut = (wchar_t*) MALLOC(sizeOut[i] * sizeof(wchar_t));
        //assign ptr without strdup
        pOut->get()[i] = pwstOut;
    }

    add(_pL->get(), size, _pR->get(), sizeOut, pOut->get());
    delete[] sizeOut;
    return pOut;
}

//specifiaction for String Matrix + String Scalar
template<>
InternalType* add_S_M<String, String, String>(String* _pL, String* _pR)
{
    String* pOut = new String(_pR->getDims(), _pR->getDimsArray());
    int size = _pR->getSize();
    int* sizeOut = new int[size];
    wchar_t* pwstL = _pL->get(0);
    int sizeL = (int)wcslen(pwstL);

    for (int i = 0 ; i < size ; ++i)
    {
        wchar_t* pwstR = _pR->get(i);
        int sizeR = (int)wcslen(pwstR);

        sizeOut[i] = sizeL + sizeR + 1;
        wchar_t* pwstOut = (wchar_t*) MALLOC(sizeOut[i] * sizeof(wchar_t));
        //assign ptr without strdup
        pOut->get()[i] = pwstOut;
    }

    add(pwstL, size, _pR->get(), sizeOut, pOut->get());
    delete[] sizeOut;
    return pOut;
}

//specifiaction for String Scalar + String MAtrix
template<>
InternalType* add_M_S<String, String, String>(String* _pL, String* _pR)
{
    String* pOut = new String(_pL->getDims(), _pL->getDimsArray());
    int size = _pL->getSize();
    int* sizeOut = new int[size];
    wchar_t* pwstR = _pR->get(0);
    int sizeR = (int)wcslen(pwstR);

    for (int i = 0 ; i < size ; ++i)
    {
        wchar_t* pwstL = _pL->get(i);
        int sizeL = (int)wcslen(pwstL);

        sizeOut[i] = sizeL + sizeR + 1;
        wchar_t* pwstOut = (wchar_t*) MALLOC(sizeOut[i] * sizeof(wchar_t));
        //assign ptr without strdup
        pOut->get()[i] = pwstOut;
    }

    add(_pL->get(), size, pwstR, sizeOut, pOut->get());
    delete[] sizeOut;
    return pOut;
}

//specifiaction for String Scalar + String Scalar
template<>
InternalType* add_S_S<String, String, String>(String* _pL, String* _pR)
{
    String* pOut = new String(1, 1);
    wchar_t* pwstL = _pL->get(0);
    wchar_t* pwstR = _pR->get(0);
    int sizeL = (int)wcslen(pwstL);
    int sizeR = (int)wcslen(pwstR);

    int sizeOut = sizeL + sizeR + 1;
    wchar_t* pwstOut = (wchar_t*) MALLOC(sizeOut * sizeof(wchar_t));
    //assign ptr without strdup
    pOut->get()[0] = pwstOut;
    add(pwstL, pwstR, sizeOut, *pOut->get());
    return pOut;
}

template<>
InternalType* add_M_E<String, Double, String>(String* _pL, Double* /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<>
InternalType* add_S_E<String, Double, String>(String* _pL, Double* /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<>
InternalType* add_E_M<Double, String, String>(Double* /*_pL*/, String* _pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<>
InternalType* add_E_S<Double, String, String>(Double* /*_pL*/, String* _pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation +: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pR;
    }
    Sciwarning(_("operation +: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

template<> InternalType* add_M_M<Polynom, Polynom, Polynom>(Polynom* _pL, Polynom* _pR)
{
    Polynom* pLSave = _pL;
    Polynom* pRSave = _pR;

    Polynom* pOut = NULL;
    if (_pL->getVariableName() != _pR->getVariableName())
    {
        std::wostringstream os;
        os << _W("variables don't have the same formal variable");
        //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
        throw ast::InternalError(os.str());
    }

    if (_pR->isIdentity())
    {
        //clone to avoid modification of original variable.
        _pR = _pR->clone();
        SinglePoly *sp  = _pR->get(0);

        int iDims = _pL->getDims();
        int* piDims = _pL->getDimsArray();
        int iLeadDims = piDims[0];
        _pR->resize(piDims, iDims);
        //find smaller dims
        for (int i = 1 ; i < iDims ; ++i)
        {
            if (iLeadDims > piDims[i])
            {
                iLeadDims = piDims[i];
            }
        }
        for (int i = 1 ; i < iLeadDims ; ++i)
        {
            _pR->set(i, i, sp);
        }
    }

    if (_pL->isIdentity())
    {
        //clone to avoid modification of original variable.
        _pL = _pL->clone();

        SinglePoly *sp = _pL->get(0);

        int iDims = _pR->getDims();
        int* piDims = _pR->getDimsArray();
        int iLeadDims = piDims[0];
        _pL->resize(piDims, iDims);
        //find smaller dims
        for (int i = 1 ; i < iDims ; ++i)
        {
            if (iLeadDims > piDims[i])
            {
                iLeadDims = piDims[i];
            }
        }

        for (int i = 1 ; i < iLeadDims ; ++i)
        {
            _pL->set(i, i, sp);
        }
    }

    if (_pL->isScalar())
    {
        int *pRank = new int[_pR->getSize()];
        int *pRank1 = new int[_pR->getSize()];
        int *pRank2 = new int[_pR->getSize()];
        bool bComplex1 = _pL->isComplex();
        bool bComplex2 = _pR->isComplex();

        memset(pRank1, 0x00, _pR->getSize() * sizeof(int));

        _pL->getRank(pRank1);
        _pR->getRank(pRank2);
        for (int i = 0 ; i < _pR->getSize() ; i++)
        {
            pRank[i] = std::max(pRank1[0], pRank2[i]);
        }

        pOut = new Polynom(_pR->getVariableName(), _pR->getDims(), _pR->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            pOut->setComplex(true);
        }

        //Result P1(0) + P2(i)
        SinglePoly *pCoef1 = _pL->get(0);
        double *p1R = pCoef1->get();
        double *p1I = pCoef1->getImg();
        for (int i = 0 ; i < _pR->getSize() ; i++)
        {
            SinglePoly *pCoef2 = _pR->get(i);
            double *p2R = pCoef2->get();
            double *p2I = pCoef2->getImg();

            SinglePoly *pCoefR = pOut->get(i);
            double *pRR = pCoefR->get();
            double *pRI = pCoefR->getImg();

            for (int j = 0 ; j < std::min(pRank1[0], pRank2[i]) + 1; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[0] > pRank2[i] ? p1R : p2R);
            for (int j = std::min(pRank1[0], pRank2[i]) + 1; j < std::max(pRank1[0], pRank2[i]) + 1; j++)
            {
                pRR[j] = pTemp[j];
            }

            if (pOut->isComplex())
            {
                for (int j = 0 ; j < std::min(pRank1[0], pRank2[i]) + 1; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[0] > pRank2[i] ? p1I : p2I);
                for (int j = std::min(pRank1[0], pRank2[i]) + 1; j < std::max(pRank1[0], pRank2[i]) + 1; j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;
        if (pLSave != _pL)
        {
            _pL->killMe();
        }

        if (pRSave != _pR)
        {
            _pR->killMe();
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        int *pRank = new int[_pL->getSize()];
        int *pRank1 = new int[_pL->getSize()];
        int *pRank2 = new int[_pL->getSize()];
        bool bComplex1 = _pL->isComplex();
        bool bComplex2 = _pR->isComplex();

        memset(pRank2, 0x00, _pL->getSize() * sizeof(int));

        _pL->getRank(pRank1);
        _pR->getRank(pRank2);
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            pRank[i] = std::max(pRank1[i], pRank2[0]);
        }

        pOut = new Polynom(_pL->getVariableName(), _pL->getDims(), _pL->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            pOut->setComplex(true);
        }

        //Result P1(i) + P2(0)
        SinglePoly *pCoef2 = _pR->get(0);
        double *p2R = pCoef2->get();
        double *p2I = pCoef2->getImg();

        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            SinglePoly *pCoef1 = _pL->get(i);
            double *p1R = pCoef1->get();
            double *p1I = pCoef1->getImg();

            SinglePoly *pCoefR = pOut->get(i);
            double *pRR = pCoefR->get();
            double *pRI = pCoefR->getImg();

            for (int j = 0 ; j < std::min(pRank1[i], pRank2[0]) + 1 ; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[i] > pRank2[0] ? p1R : p2R);
            for (int j = std::min(pRank1[i], pRank2[0]) + 1; j < std::max(pRank1[i], pRank2[0]) + 1; j++)
            {
                pRR[j] = pTemp[j];
            }

            if (pOut->isComplex())
            {
                for (int j = 0 ; j < std::min(pRank1[i], pRank2[0]) + 1; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[i] > pRank2[0] ? p1I : p2I);
                for (int j = std::min(pRank1[i], pRank2[0]) + 1; j < std::max(pRank1[i], pRank2[0]) + 1; j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;

        if (pLSave != _pL)
        {
            _pL->killMe();
        }

        if (pRSave != _pR)
        {
            _pR->killMe();
        }

        return pOut;
    }

    int iDims1 = _pL->getDims();
    int iDims2 = _pR->getDims();

    if (iDims1 != iDims2)
    {
        if (pLSave != _pL)
        {
            _pL->killMe();
        }

        if (pRSave != _pR)
        {
            _pR->killMe();
        }

        return nullptr;
    }

    int* piDims1 = _pL->getDimsArray();
    int* piDims2 = _pR->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if ((piDims1[i] != piDims2[i]))
        {
            if (pLSave != _pL)
            {
                _pL->killMe();
            }

            if (pRSave != _pR)
            {
                _pR->killMe();
            }

            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(),  L"+", _pL->DimToString().c_str(), _pR->DimToString().c_str());
            throw ast::InternalError(pMsg);
        }
    }

    int *pRank = new int[_pL->getSize()];
    int *pRank1 = new int[_pL->getSize()];
    int *pRank2 = new int[_pR->getSize()];
    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();

    _pL->getRank(pRank1);
    _pR->getRank(pRank2);
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pRank[i] = std::max(pRank1[i], pRank2[i]);
    }

    pOut = new Polynom(_pR->getVariableName(), _pL->getDims(), _pL->getDimsArray(), pRank);
    if (_pL->isComplex() || _pR->isComplex())
    {
        pOut->setComplex(true);
    }

    if (bComplex1 == false && bComplex2 == false)
    {
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            iAddScilabPolynomToScilabPolynom(
                _pL->get(i)->get(), pRank1[i] + 1,
                _pR->get(i)->get(), pRank2[i] + 1,
                pOut->get(i)->get(), pRank[i] + 1);
        }
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            iAddScilabPolynomToComplexPoly(
                _pL->get(i)->get(), pRank1[i] + 1,
                _pR->get(i)->get(), _pR->get(i)->getImg(), pRank2[i] + 1,
                pOut->get(i)->get(), pOut->get(i)->getImg(), pRank[i] + 1);
        }
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            iAddScilabPolynomToComplexPoly(
                _pR->get(i)->get(), pRank2[i] + 1,
                _pL->get(i)->get(), _pL->get(i)->getImg(), pRank1[i] + 1,
                pOut->get(i)->get(), pOut->get(i)->getImg(), pRank[i] + 1);
        }
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            iAddComplexPolyToComplexPoly(
                _pL->get(i)->get(), _pL->get(i)->getImg(), pRank1[i] + 1,
                _pR->get(i)->get(), _pR->get(i)->getImg(), pRank2[i] + 1,
                pOut->get(i)->get(), pOut->get(i)->getImg(), pRank[i] + 1);
        }
    }

    delete[] pRank;
    delete[] pRank1;
    delete[] pRank2;

    if (pLSave != _pL)
    {
        _pL->killMe();
    }

    if (pRSave != _pR)
    {
        _pR->killMe();
    }

    if (pOut != NULL)
    {
        pOut->updateRank();
    }

    return pOut;
}

template<> InternalType* add_M_M<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    return add_M_M<Double, Polynom, Polynom>(_pR, _pL);
}

template<> InternalType* add_M_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = NULL;
    bool bComplex1 = _pR->isComplex();
    bool bComplex2 = _pL->isComplex();

    double *pInDblR = _pL->getReal();
    double *pInDblI = _pL->getImg();

    if (_pL->isEmpty())
    {
        return _pR;
    }

    if (_pR->isScalar())
    {
        int *piRank = new int[_pL->getSize()];
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            piRank[i] = _pR->get(0)->getRank();
        }

        pOut = new Polynom(_pR->getVariableName(), _pL->getDims(), _pL->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            pOut->setComplex(true);
        }

        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pInPoly  = _pR->get(0);
            SinglePoly *pOutPoly = pOut->get(i);
            double *pInPolyR     = pInPoly->get();
            double *pOutPolyR    = pOutPoly->get();

            pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

            for (int j = 1 ; j < pInPoly->getSize() ; j++)
            {
                pOutPolyR[j] = pInPolyR[j];
            }
        }

        if (pOut->isComplex())
        {
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                SinglePoly *pInPoly  = _pR->get(0);
                SinglePoly *pOutPoly = pOut->get(i);
                double *pInPolyI     = pInPoly->getImg();
                double *pOutPolyI    = pOutPoly->getImg();

                pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

                for (int j = 1 ; j < pInPoly->getSize() ; j++)
                {
                    pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
                }
            }
        }

        return pOut;
    }

    if (_pL->isScalar())
    {
        pOut = (Polynom*)_pR->clone();

        if (bComplex1 && bComplex2)
        {
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                SinglePoly *pSPOut   = pOut->get(i);
                double *pOutPolyR    = pSPOut->get();
                double *pOutPolyI    = pSPOut->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] += pInDblI[0];
            }
        }
        else if (bComplex2)
        {
            pOut->setComplex(true);
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                SinglePoly *pSPOut   = pOut->get(i);
                double *pOutPolyR    = pSPOut->get();
                double *pOutPolyI    = pSPOut->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] = pInDblI[0];
            }
        }
        else
        {
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                SinglePoly *pSPOut = pOut->get(i);
                double *pOutPolyR  = pSPOut->get();

                pOutPolyR[0] += pInDblR[0];
            }
        }

        return pOut;
    }

    int iDims1 = _pR->getDims();
    int iDims2 = _pL->getDims();

    if (iDims1 != iDims2)
    {
        return nullptr;
    }

    int* piDims1 = _pR->getDimsArray();
    int* piDims2 = _pL->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(),  L"+", _pL->DimToString().c_str(), _pR->DimToString().c_str());
            throw ast::InternalError(pMsg);
        }
    }

    pOut = (Polynom*)_pR->clone();
    if (bComplex1 && bComplex2)
    {
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut   = pOut->get(i);
            double *pOutPolyR    = pSPOut->get();
            double *pOutPolyI    = pSPOut->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] += pInDblI[i];
        }
    }
    else if (bComplex2)
    {
        pOut->setComplex(true);
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut   = pOut->get(i);
            double *pOutPolyR    = pSPOut->get();
            double *pOutPolyI    = pSPOut->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] = pInDblI[i];
        }
    }
    else
    {
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut = pOut->get(i);
            double *pOutPolyR  = pSPOut->get();

            pOutPolyR[0] += pInDblR[i];
        }
    }

    return pOut;
}

//poly + eye
template<> InternalType* add_M_I<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    Polynom* pOut = (Polynom*)_pL->clone();
    bool isComplex = _pL->isComplex() || _pR->isComplex();
    pOut->setComplex(isComplex);

    int iDims = _pL->getDims();
    int* piDims = _pL->getDimsArray();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    piIndex[0] = 0;
    //find smaller dims
    for (int i = 1 ; i < iDims ; ++i)
    {
        //init
        piIndex[i] = 0;

        if (iLeadDims > piDims[i])
        {
            iLeadDims = piDims[i];
        }
    }

    double dblR = _pR->get(0);

    if (isComplex)
    {
        SinglePoly** pSP = pOut->get();

        double dblI = 0;
        if (_pR->isComplex())
        {
            dblI = _pR->getImg(0);
        }

        for (int i = 0 ; i < iLeadDims ; ++i)
        {
            for (int j = 0 ; j < iDims ; ++j)
            {
                piIndex[j] = i;
            }

            int index = _pL->getIndex(piIndex);
            add(dblR, dblI, pSP[index]->get(0), pSP[index]->getImg(0), pSP[index]->get(), pSP[index]->getImg());
        }
    }
    else
    {
        SinglePoly** pSP = pOut->get();
        for (int i = 0 ; i < iLeadDims ; ++i)
        {
            for (int j = 0 ; j < iDims ; ++j)
            {
                piIndex[j] = i;
            }

            int index = _pL->getIndex(piIndex);
            add(dblR, pSP[index]->get(0), pSP[index]->get());
        }
    }

    delete[] piIndex;
    return pOut;
}

template<> InternalType* add_I_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    return add_M_I<Polynom, Double, Polynom>(_pR, _pL);
}

//sp + sp
template<> InternalType* add_M_M<Sparse, Sparse, Sparse>(Sparse* _pL, Sparse* _pR)
{
    Sparse* pOut = NULL;

    //check scalar hidden in a sparse ;)
    /* if (_pL->getRows() == 1 && _pL->getCols() == 1)
     {
         //do scalar + sp
         Double* pDbl = NULL;
         if (_pL->isComplex())
         {
             std::complex<double> dbl = _pL->getImg(0, 0);
             pDbl = new Double(dbl.real(), dbl.imag());
         }
         else
         {
             pDbl = new Double(_pL->get(0, 0));
         }

         AddSparseToDouble(_pR, pDbl, (GenericType**)pOut);
         delete pDbl;
         return pOut;
     }

     if (_pR->getRows() == 1 && _pR->getCols() == 1)
     {
         //do sp + scalar
         Double* pDbl = NULL;
         if (_pR->isComplex())
         {
             std::complex<double> dbl = _pR->getImg(0, 0);
             pDbl = new Double(dbl.real(), dbl.imag());
         }
         else
         {
             pDbl = new Double(_pR->get(0, 0));
         }

         AddSparseToDouble(_pL, pDbl, (GenericType**)pOut);
         delete pDbl;
         return 0;
     }

     if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
     {
         //dimensions not match
         throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
     }

     if (_pL->nonZeros() == 0)
     {
         //sp([]) + sp
         return _pR;
     }

     if (_pR->nonZeros() == 0)
     {
         //sp + sp([])
         return _pL;
     }*/

    return _pL->add(*_pR);
}

//d + sp
template<> InternalType* add_M_M<Double, Sparse, Double>(Double* _pL, Sparse* _pR)
{
    return add_M_M<Sparse, Double, Double>(_pR, _pL);
}

//sp + d
template<> InternalType* add_M_M<Sparse, Double, Double>(Sparse* _pL, Double* _pR)
{
    Double* pOut = NULL;
    int iOne = 1; //fortran
    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();

    if (_pL->isScalar() && _pR->isScalar())
    {
        //sp + d
        pOut = (Double*)_pR->clone();
        pOut->setComplex(bComplex1 | bComplex2);
        if (bComplex1)
        {
            std::complex<double> dbl = _pL->getImg(0, 0);
            pOut->set(0, pOut->get(0) + dbl.real());
            pOut->setImg(0, pOut->getImg(0) + dbl.imag());
        }
        else
        {
            pOut->set(0, pOut->get(0) + _pL->get(0, 0));
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        //SP + d
        pOut = new Double(_pL->getRows(), _pL->getCols(), bComplex1 | bComplex2);
        int iSize = _pL->getSize();
        double dblVal = _pR->get(0);
        C2F(dset)(&iSize, &dblVal, pOut->get(), &iOne);
        if (bComplex2)
        {
            double dblValI = _pR->getImg(0);
            C2F(dset)(&iSize, &dblValI, pOut->getImg(), &iOne);
        }
        else if (bComplex1)
        {
            //initialize imag part at 0
            double dblValI = 0;
            C2F(dset)(&iSize, &dblValI, pOut->getImg(), &iOne);
        }

        int nonZeros = static_cast<int>(_pL->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pL->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pL->getImg(iRow, iCol);
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) + dbl.real());
                pOut->setImg(iRow, iCol, pOut->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) + _pL->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;

        return pOut;
    }

    if (_pL->isScalar())
    {
        //sp + D
        pOut = (Double*)_pR->clone();
        pOut->setComplex(bComplex1 | bComplex2);

        if (bComplex1)
        {
            double* pReal = pOut->get();
            double* pImg = pOut->getImg();
            int size = pOut->getSize();
            for (int i = 0 ; i < size ; i++)
            {
                std::complex<double> dbl = _pL->getImg(0, 0);
                pReal[i] += dbl.real();
                pImg[i] += dbl.imag();
            }
        }
        else
        {
            double* pReal = pOut->get();
            int size = pOut->getSize();
            for (int i = 0 ; i < size ; i++)
            {
                pReal[i] += _pL->get(0, 0);
            }
        }

        return pOut;
    }


    if (_pR->getDims() == 2 && _pL->getRows() == _pR->getRows() && _pL->getCols() == _pR->getCols())
    {
        //SP + D
        pOut = (Double*)_pR->clone();
        pOut->setComplex(bComplex1 | bComplex2);

        int nonZeros = static_cast<int>(_pL->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pL->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pL->getImg(iRow, iCol);
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) + dbl.real());
                pOut->setImg(iRow, iCol, pOut->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) + _pL->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;
        return pOut;
    }
    else
    {
        return nullptr;
    }
}

//[] + sp
template<> InternalType* add_M_M<Double, Sparse, Sparse>(Double* _pL, Sparse* _pR)
{
    return add_M_M<Sparse, Double, Sparse>(_pR, _pL);
}

//sp + []
template<> InternalType* add_M_M<Sparse, Double, Sparse>(Sparse* _pL, Double* _pR)
{
    Sparse* pOut = NULL;
    if (_pR->isIdentity())
    {
        //convert to _pL
        pOut  = _pL->clone()->getAs<Sparse>();
        bool isComplex = _pL->isComplex() || _pR->isComplex();
        if (isComplex)
        {
            pOut->toComplex();
        }

        int size = std::min(_pL->getRows(), _pL->getCols());
        double dblR = _pR->get(0);

        if (isComplex)
        {
            std::complex<double> cplx_add(dblR, 0);
            if (_pR->isComplex())
            {
                cplx_add.imag(_pR->getImg(0));
            }

            for (int i = 0 ; i < size ; i++)
            {
                pOut->set(i, i, pOut->getImg(i, i) + cplx_add, false);
            }
        }
        else
        {
            for (int i = 0 ; i < size ; i++)
            {
                pOut->set(i, i, _pL->get(i, i) + dblR, false);
            }
        }

        pOut->finalize();
        return pOut;
    }
    else
    {
        //is []
        return _pL;
    }
}
