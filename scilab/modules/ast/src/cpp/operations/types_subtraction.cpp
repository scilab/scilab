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

#include "types_subtraction.hxx"
#include "types_opposite.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "generic_operations.hxx"

extern "C"
{
#include "elem_common.h" //dset
#include "Sciwarning.h" //Sciwarning
}

using namespace types;

//define arrays on operation functions
static sub_function pSubfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillSubtractFunction()
{
#define scilab_fill_sub(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pSubfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (sub_function)&sub_##func<typeIn1, typeIn2, typeOut>

    //Double
    //Matrix - Matrix
    scilab_fill_sub(Double, Double, M_M, Double, Double, Double);
    scilab_fill_sub(Double, Int8, M_M, Double, Int8, Int8);
    scilab_fill_sub(Double, UInt8, M_M, Double, UInt8, UInt8);
    scilab_fill_sub(Double, Int16, M_M, Double, Int16, Int16);
    scilab_fill_sub(Double, UInt16, M_M, Double, UInt16, UInt16);
    scilab_fill_sub(Double, Int32, M_M, Double, Int32, Int32);
    scilab_fill_sub(Double, UInt32, M_M, Double, UInt32, UInt32);
    scilab_fill_sub(Double, Int64, M_M, Double, Int64, Int64);
    scilab_fill_sub(Double, UInt64, M_M, Double, UInt64, UInt64);
    scilab_fill_sub(Double, Bool, M_M, Double, Bool, Double);
    scilab_fill_sub(Double, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(Double, Sparse, M_M, Double, Sparse, Double);

    //Matrix - Matrix Complex
    scilab_fill_sub(Double, DoubleComplex, M_MC, Double, Double, Double);
    scilab_fill_sub(Double, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(Double, SparseComplex, M_M, Double, Sparse, Double);

    //Matrix - Scalar
    scilab_fill_sub(Double, ScalarDouble, M_S, Double, Double, Double);
    scilab_fill_sub(Double, ScalarInt8, M_S, Double, Int8, Int8);
    scilab_fill_sub(Double, ScalarUInt8, M_S, Double, UInt8, UInt8);
    scilab_fill_sub(Double, ScalarInt16, M_S, Double, Int16, Int16);
    scilab_fill_sub(Double, ScalarUInt16, M_S, Double, UInt16, UInt16);
    scilab_fill_sub(Double, ScalarInt32, M_S, Double, Int32, Int32);
    scilab_fill_sub(Double, ScalarUInt32, M_S, Double, UInt32, UInt32);
    scilab_fill_sub(Double, ScalarInt64, M_S, Double, Int64, Int64);
    scilab_fill_sub(Double, ScalarUInt64, M_S, Double, UInt64, UInt64);
    scilab_fill_sub(Double, ScalarBool, M_S, Double, Bool, Double);
    scilab_fill_sub(Double, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Matrix - Scalar Complex
    scilab_fill_sub(Double, ScalarDoubleComplex, M_SC, Double, Double, Double);
    scilab_fill_sub(Double, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Matrix - Empty
    scilab_fill_sub(Double, Empty, M_E, Double, Double, Double);


    //Matrix Complex - Matrix
    scilab_fill_sub(DoubleComplex, Double, MC_M, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, ScalarDouble, MC_S, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, Empty, MC_E, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(DoubleComplex, Sparse, M_M, Double, Sparse, Double);
    scilab_fill_sub(DoubleComplex, SparseComplex, M_M, Double, Sparse, Double);

    //Scalar - Matrix
    scilab_fill_sub(ScalarDouble, Double, S_M, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, Int8, S_M, Double, Int8, Int8);
    scilab_fill_sub(ScalarDouble, UInt8, S_M, Double, UInt8, UInt8);
    scilab_fill_sub(ScalarDouble, Int16, S_M, Double, Int16, Int16);
    scilab_fill_sub(ScalarDouble, UInt16, S_M, Double, UInt16, UInt16);
    scilab_fill_sub(ScalarDouble, Int32, S_M, Double, Int32, Int32);
    scilab_fill_sub(ScalarDouble, UInt32, S_M, Double, UInt32, UInt32);
    scilab_fill_sub(ScalarDouble, Int64, S_M, Double, Int64, Int64);
    scilab_fill_sub(ScalarDouble, UInt64, S_M, Double, UInt64, UInt64);
    scilab_fill_sub(ScalarDouble, Bool, S_M, Double, Bool, Double);
    scilab_fill_sub(ScalarDouble, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(ScalarDouble, Sparse, M_M, Double, Sparse, Double);

    //Scalar - Matrix Complex
    scilab_fill_sub(ScalarDouble, DoubleComplex, S_MC, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(ScalarDouble, SparseComplex, M_M, Double, Sparse, Double);

    //Scalar - Scalar
    scilab_fill_sub(ScalarDouble, ScalarDouble, S_S, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, ScalarInt8, S_S, Double, Int8, Int8);
    scilab_fill_sub(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, UInt8);
    scilab_fill_sub(ScalarDouble, ScalarInt16, S_S, Double, Int16, Int16);
    scilab_fill_sub(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, UInt16);
    scilab_fill_sub(ScalarDouble, ScalarInt32, S_S, Double, Int32, Int32);
    scilab_fill_sub(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, UInt32);
    scilab_fill_sub(ScalarDouble, ScalarInt64, S_S, Double, Int64, Int64);
    scilab_fill_sub(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, UInt64);
    scilab_fill_sub(ScalarDouble, ScalarBool, S_S, Double, Bool, Double);
    scilab_fill_sub(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Scalar - Scalar Complex
    scilab_fill_sub(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);

    //Scalar - Empty
    scilab_fill_sub(ScalarDouble, Empty, S_E, Double, Double, Double);

    //Scalar Complex - Matrix
    scilab_fill_sub(ScalarDoubleComplex, Double, SC_M, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, Double);
    //Scalar Complex - Matrix Complex
    scilab_fill_sub(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_sub(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, Double);
    //Scalar Complex - Scalar
    scilab_fill_sub(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    //Scalar Complex - Scalar Complex
    scilab_fill_sub(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Scalar Complex - Empty
    scilab_fill_sub(ScalarDoubleComplex, Empty, SC_E, Double, Double, Double);

    //Empty - Matrix
    scilab_fill_sub(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_sub(Empty, Int8, E_M, Double, Int8, Int8);
    scilab_fill_sub(Empty, UInt8, E_M, Double, UInt8, UInt8);
    scilab_fill_sub(Empty, Int16, E_M, Double, Int16, Int16);
    scilab_fill_sub(Empty, UInt16, E_M, Double, UInt16, UInt16);
    scilab_fill_sub(Empty, Int32, E_M, Double, Int32, Int32);
    scilab_fill_sub(Empty, UInt32, E_M, Double, UInt32, UInt32);
    scilab_fill_sub(Empty, Int64, E_M, Double, Int64, Int64);
    scilab_fill_sub(Empty, UInt64, E_M, Double, UInt64, UInt64);
    //scilab_fill_sub(Empty, Bool, E_M, Double, Bool, Double);
    scilab_fill_sub(Empty, Polynom, E_M, Double, Polynom, Polynom);
    scilab_fill_sub(Empty, PolynomComplex, E_MC, Double, Polynom, Polynom);
    scilab_fill_sub(Empty, Sparse, E_M, Double, Sparse, Sparse);
    scilab_fill_sub(Empty, SparseComplex, E_MC, Double, Sparse, Sparse);

    //Empty - Matrix Complex
    scilab_fill_sub(Empty, DoubleComplex, E_MC, Double, Double, Double);
    //Empty - Scalar
    scilab_fill_sub(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_sub(Empty, ScalarInt8, E_M, Double, Int8, Int8);
    scilab_fill_sub(Empty, ScalarUInt8, E_M, Double, UInt8, UInt8);
    scilab_fill_sub(Empty, ScalarInt16, E_M, Double, Int16, Int16);
    scilab_fill_sub(Empty, ScalarUInt16, E_M, Double, UInt16, UInt16);
    scilab_fill_sub(Empty, ScalarInt32, E_M, Double, Int32, Int32);
    scilab_fill_sub(Empty, ScalarUInt32, E_M, Double, UInt32, UInt32);
    scilab_fill_sub(Empty, ScalarInt64, E_M, Double, Int64, Int64);
    scilab_fill_sub(Empty, ScalarUInt64, E_M, Double, UInt64, UInt64);
    scilab_fill_sub(Empty, ScalarBool, E_M, Double, Bool, Double);
    scilab_fill_sub(Empty, ScalarPolynom, E_M, Double, Polynom, Polynom);

    //Empty - Scalar Complex
    scilab_fill_sub(Empty, ScalarDoubleComplex, E_MC, Double, Double, Double);
    scilab_fill_sub(Empty, ScalarPolynomComplex, E_MC, Double, Polynom, Polynom);
    //Empty - Empty
    scilab_fill_sub(Empty, Empty, E_E, Double, Double, Double);
    //Empty - eye
    scilab_fill_sub(Empty, Identity, E_I, Double, Double, Double);
    scilab_fill_sub(Empty, IdentityComplex, E_IC, Double, Double, Double);

    //Matrix - Identity
    scilab_fill_sub(Double, Identity, M_I, Double, Double, Double);
    scilab_fill_sub(Double, IdentityComplex, M_IC, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, Identity, MC_I, Double, Double, Double);
    scilab_fill_sub(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, Identity, S_I, Double, Double, Double);
    scilab_fill_sub(ScalarDouble, IdentityComplex, S_IC, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, Identity, SC_I, Double, Double, Double);
    scilab_fill_sub(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Double);

    //Int8
    //Matrix - Matrix
    scilab_fill_sub(Int8, Double, M_M, Int8, Double, Int8);
    scilab_fill_sub(Int8, Int8, M_M, Int8, Int8, Int8);
    scilab_fill_sub(Int8, UInt8, M_M, Int8, UInt8, UInt8);
    scilab_fill_sub(Int8, Int16, M_M, Int8, Int16, Int16);
    scilab_fill_sub(Int8, UInt16, M_M, Int8, UInt16, UInt16);
    scilab_fill_sub(Int8, Int32, M_M, Int8, Int32, Int32);
    scilab_fill_sub(Int8, UInt32, M_M, Int8, UInt32, UInt32);
    scilab_fill_sub(Int8, Int64, M_M, Int8, Int64, Int64);
    scilab_fill_sub(Int8, UInt64, M_M, Int8, UInt64, UInt64);
    scilab_fill_sub(Int8, Bool, M_M, Int8, Bool, Int8);
    scilab_fill_sub(Int8, Empty, M_E, Int8, Double, Int8);

    //Matrix - Scalar
    scilab_fill_sub(Int8, ScalarDouble, M_S, Int8, Double, Int8);
    scilab_fill_sub(Int8, ScalarInt8, M_S, Int8, Int8, Int8);
    scilab_fill_sub(Int8, ScalarUInt8, M_S, Int8, UInt8, UInt8);
    scilab_fill_sub(Int8, ScalarInt16, M_S, Int8, Int16, Int16);
    scilab_fill_sub(Int8, ScalarUInt16, M_S, Int8, UInt16, UInt16);
    scilab_fill_sub(Int8, ScalarInt32, M_S, Int8, Int32, Int32);
    scilab_fill_sub(Int8, ScalarUInt32, M_S, Int8, UInt32, UInt32);
    scilab_fill_sub(Int8, ScalarInt64, M_S, Int8, Int64, Int64);
    scilab_fill_sub(Int8, ScalarUInt64, M_S, Int8, UInt64, UInt64);
    scilab_fill_sub(Int8, ScalarBool, M_S, Int8, Bool, Int8);

    //Scalar - Matrix
    scilab_fill_sub(ScalarInt8, Double, S_M, Int8, Double, Int8);
    scilab_fill_sub(ScalarInt8, Int8, S_M, Int8, Int8, Int8);
    scilab_fill_sub(ScalarInt8, UInt8, S_M, Int8, UInt8, UInt8);
    scilab_fill_sub(ScalarInt8, Int16, S_M, Int8, Int16, Int16);
    scilab_fill_sub(ScalarInt8, UInt16, S_M, Int8, UInt16, UInt16);
    scilab_fill_sub(ScalarInt8, Int32, S_M, Int8, Int32, Int32);
    scilab_fill_sub(ScalarInt8, UInt32, S_M, Int8, UInt32, UInt32);
    scilab_fill_sub(ScalarInt8, Int64, S_M, Int8, Int64, Int64);
    scilab_fill_sub(ScalarInt8, UInt64, S_M, Int8, UInt64, UInt64);
    scilab_fill_sub(ScalarInt8, Bool, S_M, Int8, Bool, Int8);
    scilab_fill_sub(ScalarInt8, Empty, S_E, Int8, Double, Int8);

    //Scalar - Scalar
    scilab_fill_sub(ScalarInt8, ScalarDouble, S_S, Int8, Double, Int8);
    scilab_fill_sub(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Int8);
    scilab_fill_sub(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, UInt8);
    scilab_fill_sub(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Int16);
    scilab_fill_sub(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, UInt16);
    scilab_fill_sub(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Int32);
    scilab_fill_sub(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, UInt32);
    scilab_fill_sub(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Int64);
    scilab_fill_sub(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, UInt64);
    scilab_fill_sub(ScalarInt8, ScalarBool, S_S, Int8, Bool, Int8);

    //UInt8
    //Matrix - Matrix
    scilab_fill_sub(UInt8, Double, M_M, UInt8, Double, UInt8);
    scilab_fill_sub(UInt8, Int8, M_M, UInt8, Int8, UInt8);
    scilab_fill_sub(UInt8, UInt8, M_M, UInt8, UInt8, UInt8);
    scilab_fill_sub(UInt8, Int16, M_M, UInt8, Int16, UInt16);
    scilab_fill_sub(UInt8, UInt16, M_M, UInt8, UInt16, UInt16);
    scilab_fill_sub(UInt8, Int32, M_M, UInt8, Int32, UInt32);
    scilab_fill_sub(UInt8, UInt32, M_M, UInt8, UInt32, UInt32);
    scilab_fill_sub(UInt8, Int64, M_M, UInt8, Int64, UInt64);
    scilab_fill_sub(UInt8, UInt64, M_M, UInt8, UInt64, UInt64);
    scilab_fill_sub(UInt8, Bool, M_M, UInt8, Bool, UInt8);
    scilab_fill_sub(UInt8, Empty, M_E, UInt8, Double, UInt8);

    //Matrix - Scalar
    scilab_fill_sub(UInt8, ScalarDouble, M_S, UInt8, Double, UInt8);
    scilab_fill_sub(UInt8, ScalarInt8, M_S, UInt8, Int8, UInt8);
    scilab_fill_sub(UInt8, ScalarUInt8, M_S, UInt8, UInt8, UInt8);
    scilab_fill_sub(UInt8, ScalarInt16, M_S, UInt8, Int16, UInt16);
    scilab_fill_sub(UInt8, ScalarUInt16, M_S, UInt8, UInt16, UInt16);
    scilab_fill_sub(UInt8, ScalarInt32, M_S, UInt8, Int32, UInt32);
    scilab_fill_sub(UInt8, ScalarUInt32, M_S, UInt8, UInt32, UInt32);
    scilab_fill_sub(UInt8, ScalarInt64, M_S, UInt8, Int64, UInt64);
    scilab_fill_sub(UInt8, ScalarUInt64, M_S, UInt8, UInt64, UInt64);
    scilab_fill_sub(UInt8, ScalarBool, M_S, UInt8, Bool, UInt8);

    //Scalar - Matrix
    scilab_fill_sub(ScalarUInt8, Double, S_M, UInt8, Double, UInt8);
    scilab_fill_sub(ScalarUInt8, Int8, S_M, UInt8, Int8, UInt8);
    scilab_fill_sub(ScalarUInt8, UInt8, S_M, UInt8, UInt8, UInt8);
    scilab_fill_sub(ScalarUInt8, Int16, S_M, UInt8, Int16, UInt16);
    scilab_fill_sub(ScalarUInt8, UInt16, S_M, UInt8, UInt16, UInt16);
    scilab_fill_sub(ScalarUInt8, Int32, S_M, UInt8, Int32, UInt32);
    scilab_fill_sub(ScalarUInt8, UInt32, S_M, UInt8, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt8, Int64, S_M, UInt8, Int64, UInt64);
    scilab_fill_sub(ScalarUInt8, UInt64, S_M, UInt8, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt8, Bool, S_M, UInt8, Bool, UInt8);
    scilab_fill_sub(ScalarUInt8, Empty, S_E, UInt8, Double, UInt8);

    //Scalar - Scalar
    scilab_fill_sub(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, UInt8);
    scilab_fill_sub(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, UInt8);
    scilab_fill_sub(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, UInt8);
    scilab_fill_sub(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, UInt16);
    scilab_fill_sub(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, UInt16);
    scilab_fill_sub(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, UInt32);
    scilab_fill_sub(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, UInt64);
    scilab_fill_sub(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, UInt8);

    //Int16
    //Matrix - Matrix
    scilab_fill_sub(Int16, Double, M_M, Int16, Double, Int16);
    scilab_fill_sub(Int16, Int8, M_M, Int16, Int8, Int16);
    scilab_fill_sub(Int16, UInt8, M_M, Int16, UInt8, UInt16);
    scilab_fill_sub(Int16, Int16, M_M, Int16, Int16, Int16);
    scilab_fill_sub(Int16, UInt16, M_M, Int16, UInt16, UInt16);
    scilab_fill_sub(Int16, Int32, M_M, Int16, Int32, Int32);
    scilab_fill_sub(Int16, UInt32, M_M, Int16, UInt32, UInt32);
    scilab_fill_sub(Int16, Int64, M_M, Int16, Int64, Int64);
    scilab_fill_sub(Int16, UInt64, M_M, Int16, UInt64, UInt64);
    scilab_fill_sub(Int16, Bool, M_M, Int16, Bool, Int16);
    scilab_fill_sub(Int16, Empty, M_E, Int16, Double, Int16);

    //Matrix - Scalar
    scilab_fill_sub(Int16, ScalarDouble, M_S, Int16, Double, Int16);
    scilab_fill_sub(Int16, ScalarInt8, M_S, Int16, Int8, Int16);
    scilab_fill_sub(Int16, ScalarUInt8, M_S, Int16, UInt8, UInt16);
    scilab_fill_sub(Int16, ScalarInt16, M_S, Int16, Int16, Int16);
    scilab_fill_sub(Int16, ScalarUInt16, M_S, Int16, UInt16, UInt16);
    scilab_fill_sub(Int16, ScalarInt32, M_S, Int16, Int32, Int32);
    scilab_fill_sub(Int16, ScalarUInt32, M_S, Int16, UInt32, UInt32);
    scilab_fill_sub(Int16, ScalarInt64, M_S, Int16, Int64, Int64);
    scilab_fill_sub(Int16, ScalarUInt64, M_S, Int16, UInt64, UInt64);
    scilab_fill_sub(Int16, ScalarBool, M_S, Int16, Bool, Int16);

    //Scalar - Matrix
    scilab_fill_sub(ScalarInt16, Double, S_M, Int16, Double, Int16);
    scilab_fill_sub(ScalarInt16, Int8, S_M, Int16, Int8, Int16);
    scilab_fill_sub(ScalarInt16, UInt8, S_M, Int16, UInt8, UInt16);
    scilab_fill_sub(ScalarInt16, Int16, S_M, Int16, Int16, Int16);
    scilab_fill_sub(ScalarInt16, UInt16, S_M, Int16, UInt16, UInt16);
    scilab_fill_sub(ScalarInt16, Int32, S_M, Int16, Int32, Int32);
    scilab_fill_sub(ScalarInt16, UInt32, S_M, Int16, UInt32, UInt32);
    scilab_fill_sub(ScalarInt16, Int64, S_M, Int16, Int64, Int64);
    scilab_fill_sub(ScalarInt16, UInt64, S_M, Int16, UInt64, UInt64);
    scilab_fill_sub(ScalarInt16, Bool, S_M, Int16, Bool, Int16);
    scilab_fill_sub(ScalarInt16, Empty, S_E, Int16, Double, Int16);

    //Scalar - Scalar
    scilab_fill_sub(ScalarInt16, ScalarDouble, S_S, Int16, Double, Int16);
    scilab_fill_sub(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Int16);
    scilab_fill_sub(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, UInt16);
    scilab_fill_sub(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Int16);
    scilab_fill_sub(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, UInt16);
    scilab_fill_sub(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Int32);
    scilab_fill_sub(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, UInt32);
    scilab_fill_sub(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Int64);
    scilab_fill_sub(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, UInt64);
    scilab_fill_sub(ScalarInt16, ScalarBool, S_S, Int16, Bool, Int16);

    //UInt16
    //Matrix - Matrix
    scilab_fill_sub(UInt16, Double, M_M, UInt16, Double, UInt16);
    scilab_fill_sub(UInt16, Int8, M_M, UInt16, Int8, UInt16);
    scilab_fill_sub(UInt16, UInt8, M_M, UInt16, UInt8, UInt16);
    scilab_fill_sub(UInt16, Int16, M_M, UInt16, Int16, UInt16);
    scilab_fill_sub(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_sub(UInt16, Int32, M_M, UInt16, Int32, UInt32);
    scilab_fill_sub(UInt16, UInt32, M_M, UInt16, UInt32, UInt32);
    scilab_fill_sub(UInt16, Int64, M_M, UInt16, Int64, UInt64);
    scilab_fill_sub(UInt16, UInt64, M_M, UInt16, UInt64, UInt64);
    scilab_fill_sub(UInt16, Bool, M_M, UInt16, Bool, UInt16);
    scilab_fill_sub(UInt16, Empty, M_E, UInt16, Double, UInt16);

    //Matrix - Scalar
    scilab_fill_sub(UInt16, ScalarDouble, M_S, UInt16, Double, UInt16);
    scilab_fill_sub(UInt16, ScalarInt8, M_S, UInt16, Int8, UInt16);
    scilab_fill_sub(UInt16, ScalarUInt8, M_S, UInt16, UInt8, UInt16);
    scilab_fill_sub(UInt16, ScalarInt16, M_S, UInt16, Int16, UInt16);
    scilab_fill_sub(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_sub(UInt16, ScalarInt32, M_S, UInt16, Int32, UInt32);
    scilab_fill_sub(UInt16, ScalarUInt32, M_S, UInt16, UInt32, UInt32);
    scilab_fill_sub(UInt16, ScalarInt64, M_S, UInt16, Int64, UInt64);
    scilab_fill_sub(UInt16, ScalarUInt64, M_S, UInt16, UInt64, UInt64);
    scilab_fill_sub(UInt16, ScalarBool, M_S, UInt16, Bool, UInt16);

    //Scalar - Matrix
    scilab_fill_sub(ScalarUInt16, Double, S_M, UInt16, Double, UInt16);
    scilab_fill_sub(ScalarUInt16, Int8, S_M, UInt16, Int8, UInt16);
    scilab_fill_sub(ScalarUInt16, UInt8, S_M, UInt16, UInt8, UInt16);
    scilab_fill_sub(ScalarUInt16, Int16, S_M, UInt16, Int16, UInt16);
    scilab_fill_sub(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_sub(ScalarUInt16, Int32, S_M, UInt16, Int32, UInt32);
    scilab_fill_sub(ScalarUInt16, UInt32, S_M, UInt16, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt16, Int64, S_M, UInt16, Int64, UInt64);
    scilab_fill_sub(ScalarUInt16, UInt64, S_M, UInt16, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt16, Bool, S_M, UInt16, Bool, UInt16);
    scilab_fill_sub(ScalarUInt16, Empty, S_E, UInt16, Double, UInt16);

    //Scalar - Scalar
    scilab_fill_sub(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, UInt16);
    scilab_fill_sub(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, UInt16);
    scilab_fill_sub(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, UInt16);
    scilab_fill_sub(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, UInt16);
    scilab_fill_sub(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_sub(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, UInt32);
    scilab_fill_sub(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, UInt64);
    scilab_fill_sub(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, UInt16);

    //Int32
    //Matrix - Matrix
    scilab_fill_sub(Int32, Double, M_M, Int32, Double, Int32);
    scilab_fill_sub(Int32, Int8, M_M, Int32, Int8, Int32);
    scilab_fill_sub(Int32, UInt8, M_M, Int32, UInt8, UInt32);
    scilab_fill_sub(Int32, Int16, M_M, Int32, Int16, Int32);
    scilab_fill_sub(Int32, UInt16, M_M, Int32, UInt16, UInt32);
    scilab_fill_sub(Int32, Int32, M_M, Int32, Int32, Int32);
    scilab_fill_sub(Int32, UInt32, M_M, Int32, UInt32, UInt32);
    scilab_fill_sub(Int32, Int64, M_M, Int32, Int64, Int64);
    scilab_fill_sub(Int32, UInt64, M_M, Int32, UInt64, UInt64);
    scilab_fill_sub(Int32, Bool, M_M, Int32, Bool, Int32);
    scilab_fill_sub(Int32, Empty, M_E, Int32, Double, Int32);

    //Matrix - Scalar
    scilab_fill_sub(Int32, ScalarDouble, M_S, Int32, Double, Int32);
    scilab_fill_sub(Int32, ScalarInt8, M_S, Int32, Int8, Int32);
    scilab_fill_sub(Int32, ScalarUInt8, M_S, Int32, UInt8, UInt32);
    scilab_fill_sub(Int32, ScalarInt16, M_S, Int32, Int16, Int32);
    scilab_fill_sub(Int32, ScalarUInt16, M_S, Int32, UInt16, UInt32);
    scilab_fill_sub(Int32, ScalarInt32, M_S, Int32, Int32, Int32);
    scilab_fill_sub(Int32, ScalarUInt32, M_S, Int32, UInt32, UInt32);
    scilab_fill_sub(Int32, ScalarInt64, M_S, Int32, Int64, Int64);
    scilab_fill_sub(Int32, ScalarUInt64, M_S, Int32, UInt64, UInt64);
    scilab_fill_sub(Int32, ScalarBool, M_S, Int32, Bool, Int32);

    //Scalar - Matrix
    scilab_fill_sub(ScalarInt32, Double, S_M, Int32, Double, Int32);
    scilab_fill_sub(ScalarInt32, Int8, S_M, Int32, Int8, Int32);
    scilab_fill_sub(ScalarInt32, UInt8, S_M, Int32, UInt8, UInt32);
    scilab_fill_sub(ScalarInt32, Int16, S_M, Int32, Int16, Int32);
    scilab_fill_sub(ScalarInt32, UInt16, S_M, Int32, UInt16, UInt32);
    scilab_fill_sub(ScalarInt32, Int32, S_M, Int32, Int32, Int32);
    scilab_fill_sub(ScalarInt32, UInt32, S_M, Int32, UInt32, UInt32);
    scilab_fill_sub(ScalarInt32, Int64, S_M, Int32, Int64, Int64);
    scilab_fill_sub(ScalarInt32, UInt64, S_M, Int32, UInt64, UInt64);
    scilab_fill_sub(ScalarInt32, Bool, S_M, Int32, Bool, Int32);
    scilab_fill_sub(ScalarInt32, Empty, S_E, Int32, Double, Int32);

    //Scalar - Scalar
    scilab_fill_sub(ScalarInt32, ScalarDouble, S_S, Int32, Double, Int32);
    scilab_fill_sub(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Int32);
    scilab_fill_sub(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, UInt32);
    scilab_fill_sub(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Int32);
    scilab_fill_sub(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, UInt32);
    scilab_fill_sub(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Int32);
    scilab_fill_sub(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, UInt32);
    scilab_fill_sub(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Int64);
    scilab_fill_sub(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, UInt64);
    scilab_fill_sub(ScalarInt32, ScalarBool, S_S, Int32, Bool, Int32);

    //UInt32
    //Matrix - Matrix
    scilab_fill_sub(UInt32, Double, M_M, UInt32, Double, UInt32);
    scilab_fill_sub(UInt32, Int8, M_M, UInt32, Int8, UInt32);
    scilab_fill_sub(UInt32, UInt8, M_M, UInt32, UInt8, UInt32);
    scilab_fill_sub(UInt32, Int16, M_M, UInt32, Int16, UInt32);
    scilab_fill_sub(UInt32, UInt16, M_M, UInt32, UInt16, UInt32);
    scilab_fill_sub(UInt32, Int32, M_M, UInt32, Int32, UInt32);
    scilab_fill_sub(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_sub(UInt32, Int64, M_M, UInt32, Int64, UInt64);
    scilab_fill_sub(UInt32, UInt64, M_M, UInt32, UInt64, UInt64);
    scilab_fill_sub(UInt32, Bool, M_M, UInt32, Bool, UInt32);
    scilab_fill_sub(UInt32, Empty, M_E, UInt32, Double, UInt32);

    //Matrix - Scalar
    scilab_fill_sub(UInt32, ScalarDouble, M_S, UInt32, Double, UInt32);
    scilab_fill_sub(UInt32, ScalarInt8, M_S, UInt32, Int8, UInt32);
    scilab_fill_sub(UInt32, ScalarUInt8, M_S, UInt32, UInt8, UInt32);
    scilab_fill_sub(UInt32, ScalarInt16, M_S, UInt32, Int16, UInt32);
    scilab_fill_sub(UInt32, ScalarUInt16, M_S, UInt32, UInt16, UInt32);
    scilab_fill_sub(UInt32, ScalarInt32, M_S, UInt32, Int32, UInt32);
    scilab_fill_sub(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_sub(UInt32, ScalarInt64, M_S, UInt32, Int64, UInt64);
    scilab_fill_sub(UInt32, ScalarUInt64, M_S, UInt32, UInt64, UInt64);
    scilab_fill_sub(UInt32, ScalarBool, M_S, UInt32, Bool, UInt32);

    //Scalar - Matrix
    scilab_fill_sub(ScalarUInt32, Double, S_M, UInt32, Double, UInt32);
    scilab_fill_sub(ScalarUInt32, Int8, S_M, UInt32, Int8, UInt32);
    scilab_fill_sub(ScalarUInt32, UInt8, S_M, UInt32, UInt8, UInt32);
    scilab_fill_sub(ScalarUInt32, Int16, S_M, UInt32, Int16, UInt32);
    scilab_fill_sub(ScalarUInt32, UInt16, S_M, UInt32, UInt16, UInt32);
    scilab_fill_sub(ScalarUInt32, Int32, S_M, UInt32, Int32, UInt32);
    scilab_fill_sub(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt32, Int64, S_M, UInt32, Int64, UInt64);
    scilab_fill_sub(ScalarUInt32, UInt64, S_M, UInt32, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt32, Bool, S_M, UInt32, Bool, UInt32);
    scilab_fill_sub(ScalarUInt32, Empty, S_E, UInt32, Double, UInt32);

    //Scalar - Scalar
    scilab_fill_sub(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_sub(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, UInt64);
    scilab_fill_sub(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, UInt32);

    //Int64
    //Matrix - Matrix
    scilab_fill_sub(Int64, Double, M_M, Int64, Double, Int64);
    scilab_fill_sub(Int64, Int8, M_M, Int64, Int8, Int64);
    scilab_fill_sub(Int64, UInt8, M_M, Int64, UInt8, UInt64);
    scilab_fill_sub(Int64, Int16, M_M, Int64, Int16, Int64);
    scilab_fill_sub(Int64, UInt16, M_M, Int64, UInt16, UInt64);
    scilab_fill_sub(Int64, Int32, M_M, Int64, Int32, Int64);
    scilab_fill_sub(Int64, UInt32, M_M, Int64, UInt32, UInt64);
    scilab_fill_sub(Int64, Int64, M_M, Int64, Int64, Int64);
    scilab_fill_sub(Int64, UInt64, M_M, Int64, UInt64, UInt64);
    scilab_fill_sub(Int64, Bool, M_M, Int64, Bool, Int64);
    scilab_fill_sub(Int64, Empty, M_E, Int64, Double, Int64);

    //Matrix - Scalar
    scilab_fill_sub(Int64, ScalarDouble, M_S, Int64, Double, Int64);
    scilab_fill_sub(Int64, ScalarInt8, M_S, Int64, Int8, Int64);
    scilab_fill_sub(Int64, ScalarUInt8, M_S, Int64, UInt8, UInt64);
    scilab_fill_sub(Int64, ScalarInt16, M_S, Int64, Int16, Int64);
    scilab_fill_sub(Int64, ScalarUInt16, M_S, Int64, UInt16, UInt64);
    scilab_fill_sub(Int64, ScalarInt32, M_S, Int64, Int32, Int64);
    scilab_fill_sub(Int64, ScalarUInt32, M_S, Int64, UInt32, UInt64);
    scilab_fill_sub(Int64, ScalarInt64, M_S, Int64, Int64, Int64);
    scilab_fill_sub(Int64, ScalarUInt64, M_S, Int64, UInt64, UInt64);
    scilab_fill_sub(Int64, ScalarBool, M_S, Int64, Bool, Int64);

    //Scalar - Matrix
    scilab_fill_sub(ScalarInt64, Double, S_M, Int64, Double, Int64);
    scilab_fill_sub(ScalarInt64, Int8, S_M, Int64, Int8, Int64);
    scilab_fill_sub(ScalarInt64, UInt8, S_M, Int64, UInt8, UInt64);
    scilab_fill_sub(ScalarInt64, Int16, S_M, Int64, Int16, Int64);
    scilab_fill_sub(ScalarInt64, UInt16, S_M, Int64, UInt16, UInt64);
    scilab_fill_sub(ScalarInt64, Int32, S_M, Int64, Int32, Int64);
    scilab_fill_sub(ScalarInt64, UInt32, S_M, Int64, UInt32, UInt64);
    scilab_fill_sub(ScalarInt64, Int64, S_M, Int64, Int64, Int64);
    scilab_fill_sub(ScalarInt64, UInt64, S_M, Int64, UInt64, UInt64);
    scilab_fill_sub(ScalarInt64, Bool, S_M, Int64, Bool, Int64);
    scilab_fill_sub(ScalarInt64, Empty, S_E, Int64, Double, Int64);

    //Scalar - Scalar
    scilab_fill_sub(ScalarInt64, ScalarDouble, S_S, Int64, Double, Int64);
    scilab_fill_sub(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Int64);
    scilab_fill_sub(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, UInt64);
    scilab_fill_sub(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Int64);
    scilab_fill_sub(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, UInt64);
    scilab_fill_sub(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Int64);
    scilab_fill_sub(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, UInt64);
    scilab_fill_sub(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Int64);
    scilab_fill_sub(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, UInt64);
    scilab_fill_sub(ScalarInt64, ScalarBool, S_S, Int64, Bool, Int64);

    //UInt64
    //Matrix - Matrix
    scilab_fill_sub(UInt64, Double, M_M, UInt64, Double, UInt64);
    scilab_fill_sub(UInt64, Int8, M_M, UInt64, Int8, UInt64);
    scilab_fill_sub(UInt64, UInt8, M_M, UInt64, UInt8, UInt64);
    scilab_fill_sub(UInt64, Int16, M_M, UInt64, Int16, UInt64);
    scilab_fill_sub(UInt64, UInt16, M_M, UInt64, UInt16, UInt64);
    scilab_fill_sub(UInt64, Int32, M_M, UInt64, Int32, UInt64);
    scilab_fill_sub(UInt64, UInt32, M_M, UInt64, UInt32, UInt64);
    scilab_fill_sub(UInt64, Int64, M_M, UInt64, Int64, UInt64);
    scilab_fill_sub(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);
    scilab_fill_sub(UInt64, Bool, M_M, UInt64, Bool, UInt64);
    scilab_fill_sub(UInt64, Empty, M_E, UInt64, Double, UInt64);

    //Matrix - Scalar
    scilab_fill_sub(UInt64, ScalarDouble, M_S, UInt64, Double, UInt64);
    scilab_fill_sub(UInt64, ScalarInt8, M_S, UInt64, Int8, UInt64);
    scilab_fill_sub(UInt64, ScalarUInt8, M_S, UInt64, UInt8, UInt64);
    scilab_fill_sub(UInt64, ScalarInt16, M_S, UInt64, Int16, UInt64);
    scilab_fill_sub(UInt64, ScalarUInt16, M_S, UInt64, UInt16, UInt64);
    scilab_fill_sub(UInt64, ScalarInt32, M_S, UInt64, Int32, UInt64);
    scilab_fill_sub(UInt64, ScalarUInt32, M_S, UInt64, UInt32, UInt64);
    scilab_fill_sub(UInt64, ScalarInt64, M_S, UInt64, Int64, UInt64);
    scilab_fill_sub(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);
    scilab_fill_sub(UInt64, ScalarBool, M_S, UInt64, Bool, UInt64);

    //Scalar - Matrix
    scilab_fill_sub(ScalarUInt64, Double, S_M, UInt64, Double, UInt64);
    scilab_fill_sub(ScalarUInt64, Int8, S_M, UInt64, Int8, UInt64);
    scilab_fill_sub(ScalarUInt64, UInt8, S_M, UInt64, UInt8, UInt64);
    scilab_fill_sub(ScalarUInt64, Int16, S_M, UInt64, Int16, UInt64);
    scilab_fill_sub(ScalarUInt64, UInt16, S_M, UInt64, UInt16, UInt64);
    scilab_fill_sub(ScalarUInt64, Int32, S_M, UInt64, Int32, UInt64);
    scilab_fill_sub(ScalarUInt64, UInt32, S_M, UInt64, UInt32, UInt64);
    scilab_fill_sub(ScalarUInt64, Int64, S_M, UInt64, Int64, UInt64);
    scilab_fill_sub(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt64, Bool, S_M, UInt64, Bool, UInt64);
    scilab_fill_sub(ScalarUInt64, Empty, S_E, UInt64, Double, UInt64);

    //Scalar - Scalar
    scilab_fill_sub(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
    scilab_fill_sub(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, UInt64);

    //Bool
    //Matrix - Matrix
    scilab_fill_sub(Bool, Double, M_M, Bool, Double, Double);
    scilab_fill_sub(Bool, Int8, M_M, Bool, Int8, Int8);
    scilab_fill_sub(Bool, UInt8, M_M, Bool, UInt8, UInt8);
    scilab_fill_sub(Bool, Int16, M_M, Bool, Int16, Int16);
    scilab_fill_sub(Bool, UInt16, M_M, Bool, UInt16, UInt16);
    scilab_fill_sub(Bool, Int32, M_M, Bool, Int32, Int32);
    scilab_fill_sub(Bool, UInt32, M_M, Bool, UInt32, UInt32);
    scilab_fill_sub(Bool, Int64, M_M, Bool, Int64, Int64);
    scilab_fill_sub(Bool, UInt64, M_M, Bool, UInt64, UInt64);
    scilab_fill_sub(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_sub(Bool, Empty, M_E, Bool, Double, Double);

    //Matrix - Scalar
    scilab_fill_sub(Bool, ScalarDouble, M_S, Bool, Double, Double);
    scilab_fill_sub(Bool, ScalarInt8, M_S, Bool, Int8, Int8);
    scilab_fill_sub(Bool, ScalarUInt8, M_S, Bool, UInt8, UInt8);
    scilab_fill_sub(Bool, ScalarInt16, M_S, Bool, Int16, Int16);
    scilab_fill_sub(Bool, ScalarUInt16, M_S, Bool, UInt16, UInt16);
    scilab_fill_sub(Bool, ScalarInt32, M_S, Bool, Int32, Int32);
    scilab_fill_sub(Bool, ScalarUInt32, M_S, Bool, UInt32, UInt32);
    scilab_fill_sub(Bool, ScalarInt64, M_S, Bool, Int64, Int64);
    scilab_fill_sub(Bool, ScalarUInt64, M_S, Bool, UInt64, UInt64);
    scilab_fill_sub(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar - Matrix
    scilab_fill_sub(ScalarBool, Double, S_M, Bool, Double, Double);
    scilab_fill_sub(ScalarBool, Int8, S_M, Bool, Int8, Int8);
    scilab_fill_sub(ScalarBool, UInt8, S_M, Bool, UInt8, UInt8);
    scilab_fill_sub(ScalarBool, Int16, S_M, Bool, Int16, Int16);
    scilab_fill_sub(ScalarBool, UInt16, S_M, Bool, UInt16, UInt16);
    scilab_fill_sub(ScalarBool, Int32, S_M, Bool, Int32, Int32);
    scilab_fill_sub(ScalarBool, UInt32, S_M, Bool, UInt32, UInt32);
    scilab_fill_sub(ScalarBool, Int64, S_M, Bool, Int64, Int64);
    scilab_fill_sub(ScalarBool, UInt64, S_M, Bool, UInt64, UInt64);
    scilab_fill_sub(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_sub(ScalarBool, Empty, S_E, Bool, Double, Double);

    //Scalar - Scalar
    scilab_fill_sub(ScalarBool, ScalarDouble, S_S, Bool, Double, Double);
    scilab_fill_sub(ScalarBool, ScalarInt8, S_S, Bool, Int8, Int8);
    scilab_fill_sub(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, UInt8);
    scilab_fill_sub(ScalarBool, ScalarInt16, S_S, Bool, Int16, Int16);
    scilab_fill_sub(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, UInt16);
    scilab_fill_sub(ScalarBool, ScalarInt32, S_S, Bool, Int32, Int32);
    scilab_fill_sub(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, UInt32);
    scilab_fill_sub(ScalarBool, ScalarInt64, S_S, Bool, Int64, Int64);
    scilab_fill_sub(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, UInt64);
    scilab_fill_sub(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Identity
    scilab_fill_sub(Identity, Double, I_M, Double, Double, Double);
    scilab_fill_sub(Identity, DoubleComplex, I_MC, Double, Double, Double);
    scilab_fill_sub(Identity, ScalarDouble, I_S, Double, Double, Double);
    scilab_fill_sub(Identity, ScalarDoubleComplex, I_SC, Double, Double, Double);
    scilab_fill_sub(Identity, Identity, I_I, Double, Double, Double);
    scilab_fill_sub(Identity, IdentityComplex, I_IC, Double, Double, Double);
    scilab_fill_sub(Identity, Empty, I_E, Double, Double, Double);

    scilab_fill_sub(Identity, Polynom, I_M, Double, Polynom, Polynom);
    scilab_fill_sub(Identity, PolynomComplex, I_MC, Double, Polynom, Polynom);
    scilab_fill_sub(Identity, ScalarPolynom, I_M, Double, Polynom, Polynom);
    scilab_fill_sub(Identity, ScalarPolynomComplex, I_MC, Double, Polynom, Polynom);
    scilab_fill_sub(Identity, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_sub(Identity, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_sub(IdentityComplex, Double, IC_M, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, ScalarDouble, IC_S, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, Identity, IC_I, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Double);
    scilab_fill_sub(IdentityComplex, Empty, IC_E, Double, Double, Double);

    scilab_fill_sub(IdentityComplex, Polynom, IC_M, Double, Polynom, Polynom);
    scilab_fill_sub(IdentityComplex, PolynomComplex, IC_MC, Double, Polynom, Polynom);
    scilab_fill_sub(IdentityComplex, ScalarPolynom, IC_M, Double, Polynom, Polynom);
    scilab_fill_sub(IdentityComplex, ScalarPolynomComplex, IC_MC, Double, Polynom, Polynom);
    scilab_fill_sub(IdentityComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_sub(IdentityComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_sub(Identity, ScalarInt8, I_S, Double, Int8, Int8);
    scilab_fill_sub(Identity, ScalarUInt8, I_S, Double, UInt8, UInt8);
    scilab_fill_sub(Identity, ScalarInt16, I_S, Double, Int16, Int16);
    scilab_fill_sub(Identity, ScalarUInt16, I_S, Double, UInt16, UInt16);
    scilab_fill_sub(Identity, ScalarInt32, I_S, Double, Int32, Int32);
    scilab_fill_sub(Identity, ScalarUInt32, I_S, Double, UInt32, UInt32);
    scilab_fill_sub(Identity, ScalarInt64, I_S, Double, Int64, Int64);
    scilab_fill_sub(Identity, ScalarUInt64, I_S, Double, UInt64, UInt64);

    scilab_fill_sub(Identity, Int8, I_M, Double, Int8, Int8);
    scilab_fill_sub(Identity, UInt8, I_M, Double, UInt8, UInt8);
    scilab_fill_sub(Identity, Int16, I_M, Double, Int16, Int16);
    scilab_fill_sub(Identity, UInt16, I_M, Double, UInt16, UInt16);
    scilab_fill_sub(Identity, Int32, I_M, Double, Int32, Int32);
    scilab_fill_sub(Identity, UInt32, I_M, Double, UInt32, UInt32);
    scilab_fill_sub(Identity, Int64, I_M, Double, Int64, Int64);
    scilab_fill_sub(Identity, UInt64, I_M, Double, UInt64, UInt64);

    //Polynom

    //poly - poly
    scilab_fill_sub(Polynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(Polynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(PolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly - scalar poly
    scilab_fill_sub(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly - double
    scilab_fill_sub(Polynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(Polynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //poly - scalar double
    scilab_fill_sub(Polynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //poly - []
    scilab_fill_sub(Polynom, Empty, M_E, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, Empty, M_E, Polynom, Double, Polynom);

    //poly - eye
    scilab_fill_sub(Polynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(Polynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(PolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //scalar poly - poly
    scilab_fill_sub(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly - scalar poly
    scilab_fill_sub(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly - double
    scilab_fill_sub(ScalarPolynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly - scalar double
    scilab_fill_sub(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly - []
    scilab_fill_sub(ScalarPolynom, Empty, M_E, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Polynom);

    //scalar poly - eye
    scilab_fill_sub(ScalarPolynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_sub(ScalarPolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //Sparse
    scilab_fill_sub(Sparse, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_sub(Sparse, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_sub(Sparse, Double, M_M, Sparse, Double, Double);
    scilab_fill_sub(Sparse, DoubleComplex, M_M, Sparse, Double, Double);
    scilab_fill_sub(Sparse, ScalarDouble, M_M, Sparse, Double, Double);
    scilab_fill_sub(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, Double);

    scilab_fill_sub(Sparse, Empty, M_E, Sparse, Double, Sparse);
    scilab_fill_sub(Sparse, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_sub(Sparse, IdentityComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_sub(SparseComplex, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_sub(SparseComplex, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_sub(SparseComplex, Double, M_M, Sparse, Double, Double);
    scilab_fill_sub(SparseComplex, DoubleComplex, M_M, Sparse, Double, Double);
    scilab_fill_sub(SparseComplex, ScalarDouble, M_M, Sparse, Double, Double);
    scilab_fill_sub(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, Double);

    scilab_fill_sub(SparseComplex, Empty, M_E, Sparse, Double, Sparse);
    scilab_fill_sub(SparseComplex, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_sub(SparseComplex, IdentityComplex, M_M, Sparse, Double, Sparse);

#undef scilab_fill_sub
}

InternalType* GenericMinus(InternalType* _pLeftOperand, InternalType* _pRightOperand)
{
    InternalType *pResult = NULL;

    sub_function sub = pSubfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (sub)
    {
        pResult = sub(_pLeftOperand, _pRightOperand);
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

template<class T, class U, class O>
InternalType* sub_M_M(T *_pL, U *_pR)
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

    sub(_pL->get(), (size_t)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_M_MC(T *_pL, U *_pR)
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

    sub(_pL->get(), (size_t)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    sub(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    sub(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_M_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}


template<class T, class U, class O>
InternalType* sub_MC_M(T *_pL, U *_pR)
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

    sub(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_MC_MC(T *_pL, U *_pR)
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

    sub(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    sub(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    sub(_pL->get(), _pL->getImg(), (size_t)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_MC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}


template<class T, class U, class O>
InternalType* sub_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    sub(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_S_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    sub(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    sub(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    sub(_pL->get(), (size_t)1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_S_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}


template<class T, class U, class O>
InternalType* sub_SC_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    sub(_pL->get(0), _pL->getImg(0), (size_t)_pR->getSize(), _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_SC_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    sub(_pL->get(0), _pL->getImg(0), (size_t)_pR->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_SC_S(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    sub(_pL->get(), _pL->getImg(), (size_t)1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    sub(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_SC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}


template<class T, class U, class O>
InternalType* sub_E_M(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return opposite_M<U, O>(_pR);
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_E_MC(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return opposite_MC<U, O>(_pR);
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_E_E(T * /*_pL*/, U * /*_pR*/)
{
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_I_M(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)opposite_M<U, O>(_pR);
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
        sub(dblLeft, _pR->get(index), pOut->get() + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_I_MC(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)opposite_MC<U, O>(_pR);
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
        sub(dblLeft, pdblRight[index], pdblOut + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_IC_M(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)opposite_M<U, O>(_pR);
    pOut->setComplex(true);
    double* pdblOutR = pOut->get();
    double* pdblOutI = pOut->getImg();
    double* pdblRight = _pR->get();
    double dblLeftR = _pL->get(0);
    double dblLeftI = _pL->getImg(0);
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
        sub(&dblLeftR, &dblLeftI, (size_t)1, pdblRight[index], pdblOutR + index, pdblOutI + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_IC_MC(T *_pL, U *_pR)
{
    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    O* pOut = (O*)opposite_MC<U, O>(_pR);
    double dblLeftR = _pL->get(0);
    double dblLeftI = _pL->getImg(0);
    double* pdblOutR = pOut->get();
    double* pdblOutI = pOut->getImg();
    double* pdblRightR = _pR->get();
    double* pdblRightI = _pR->getImg();
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
        sub(dblLeftR, dblLeftI, pdblRightR[index], pdblRightI[index], pdblOutR + index, pdblOutI + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O>
InternalType* sub_I_S(T *_pL, U *_pR)
{
    return sub_S_S<T, U, O>(_pL, _pR);
}

template<class T, class U, class O>
InternalType* sub_IC_S(T *_pL, U *_pR)
{
    return sub_SC_S<T, U, O>(_pL, _pR);
}

template<class T, class U, class O>
InternalType* sub_I_SC(T *_pL, U *_pR)
{
    return sub_S_SC<T, U, O>(_pL, _pR);
}

template<class T, class U, class O>
InternalType* sub_IC_SC(T *_pL, U *_pR)
{
    return sub_SC_SC<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_M_I(T *_pL, U *_pR)
{
    int iDims = _pL->getDims();
    int* piDims = _pL->getDimsArray();
    O* pOut = (O*)_pL->clone();
    double* pdblOutR = pOut->get();
    double* pdblLeft = _pL->get();
    double dblRight = _pR->get(0);
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

        int index = _pL->getIndex(piIndex);
        sub(pdblLeft[index], (size_t)1, &dblRight, pdblOutR + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O> InternalType* sub_MC_I(T *_pL, U *_pR)
{
    return sub_M_I<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_M_IC(T *_pL, U *_pR)
{
    int iDims = _pL->getDims();
    int* piDims = _pL->getDimsArray();
    O* pOut = (O*)_pL->clone();
    pOut->setComplex(true);
    double* pdblOutR = pOut->get();
    double* pdblOutI = pOut->getImg();
    double* pdblLeft = _pL->get();
    double dblRightR = _pR->get(0);
    double dblRightI = _pR->getImg(0);
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

        int index = _pL->getIndex(piIndex);
        sub(pdblLeft[index], (size_t)1, &dblRightR, &dblRightI, pdblOutR + index, pdblOutI + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O> InternalType* sub_MC_IC(T *_pL, U *_pR)
{
    int iDims = _pL->getDims();
    int* piDims = _pL->getDimsArray();
    O* pOut = (O*)_pL->clone();
    pOut->setComplex(true);
    double* pdblOutR = pOut->get();
    double* pdblOutI = pOut->getImg();
    double* pdblLeftR = _pL->get();
    double* pdblLeftI = _pL->getImg();
    double dblRightR = _pR->get(0);
    double dblRightI = _pR->getImg(0);
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

        int index = _pL->getIndex(piIndex);
        sub(pdblLeftR[index], pdblLeftI[index], (size_t)1, &dblRightR, &dblRightI, pdblOutR + index, pdblOutI + index);
    }

    delete[] piIndex;
    return pOut;
}

template<class T, class U, class O> InternalType* sub_S_I(T *_pL, U *_pR)
{
    return sub_S_S<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_SC_I(T *_pL, U *_pR)
{
    return sub_SC_SC<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_S_IC(T *_pL, U *_pR)
{
    return sub_S_SC<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_SC_IC(T *_pL, U *_pR)
{
    return sub_SC_SC<T, U, O>(_pL, _pR);
}

template<class T, class U, class O> InternalType* sub_I_I(T *_pL, U *_pR)
{
    O* pOut = types::Double::Identity(-1, -1);
    sub(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O> InternalType* sub_I_IC(T *_pL, U *_pR)
{
    O* pOut = types::Double::Identity(-1, -1);
    pOut->setComplex(true);
    sub(_pL->get(), (size_t)1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> InternalType* sub_IC_I(T *_pL, U *_pR)
{
    O* pOut = types::Double::Identity(-1, -1);
    pOut->setComplex(true);
    sub(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> InternalType* sub_IC_IC(T *_pL, U *_pR)
{
    O* pOut = types::Double::Identity(-1, -1);
    pOut->setComplex(true);
    sub(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O> types::InternalType* sub_I_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O> types::InternalType* sub_IC_E(T *_pL, U * /*_pR*/)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return _pL;
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O> types::InternalType* sub_E_I(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return opposite_I<U, O>(_pR);
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<class T, class U, class O> types::InternalType* sub_E_IC(T * /*_pL*/, U *_pR)
{
    if (ConfigVariable::getOldEmptyBehaviour())
    {
        Sciwarning(_("operation -: Warning adding a matrix with the empty matrix old behaviour.\n"));
        return opposite_IC<U, O>(_pR);
    }
    Sciwarning(_("operation -: Warning adding a matrix with the empty matrix will give an empty matrix result.\n"));
    Double* pOut = Double::Empty();
    sub();
    return pOut;
}

template<> InternalType* sub_M_M<Polynom, Polynom, Polynom>(Polynom* _pL, Polynom* _pR)
{
    Polynom* pOut = NULL;
    if (_pL->isScalar())
    {
        SinglePoly* p1Coef  = _pL->get(0);
        int iRank1          = p1Coef->getRank();
        int* pRank2         = new int[_pR->getSize()];
        int* pRankOut       = new int[_pR->getSize()];

        _pR->getRank(pRank2);
        for (int i = 0 ; i < _pR->getSize() ; i++)
        {
            pRankOut[i] = std::max(iRank1, pRank2[i]);
        }

        pOut = new Polynom(_pR->getVariableName(), _pR->getDims(), _pR->getDimsArray(), pRankOut);
        bool isOutComplex = _pL->isComplex() || _pR->isComplex();

        //Result P1(0) - P2(i)
        double* p1R = p1Coef->get();
        if (isOutComplex)
        {
            double* p1I = p1Coef->getImg();
            for (int i = 0 ; i < _pR->getSize() ; i++)
            {
                SinglePoly* p2Coef   = _pR->get(i);
                double* p2R          = p2Coef->get();
                double* p2I          = p2Coef->getImg();

                SinglePoly* pOutCoef = pOut->get(i);
                pOutCoef->setComplex(isOutComplex);
                double* pOutR        = pOutCoef->get();
                double* pOutI        = pOutCoef->getImg();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > iRank1)
                    {
                        pOutR[j] = - p2R[j];
                        pOutI[j] = - (p2I ? p2I[j] : 0);
                    }
                    else if (j > pRank2[i])
                    {
                        pOutR[j] = p1R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0);
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0) - (p2I ? p2I[j] : 0);
                    }
                }
            }
        }
        else
        {
            for (int i = 0 ; i < _pR->getSize() ; i++)
            {
                SinglePoly* p2Coef   = _pR->get(i);
                double* p2R          = p2Coef->get();

                SinglePoly* pOutCoef = pOut->get(i);
                double* pOutR        = pOutCoef->get();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > iRank1)
                    {
                        pOutR[j] = - p2R[j];
                    }
                    else if (j > pRank2[i])
                    {
                        pOutR[j] = p1R[j];
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                    }
                }
            }
        }

        delete[] pRankOut;
        delete[] pRank2;
        pOut->updateRank();
        return pOut;
    }

    if (_pR->isScalar())
    {
        //size(p2) == 1
        int *pRank1     = new int[_pL->getSize()];
        int iRank2      = _pR->get(0)->getRank();
        int *pRankOut   = new int[_pL->getSize()];

        _pL->getRank(pRank1);
        for (int i = 0 ; i < _pL->getSize() ; i++)
        {
            pRankOut[i] = std::max(pRank1[i], iRank2);
        }

        pOut = new Polynom(_pL->getVariableName(), _pL->getDims(), _pL->getDimsArray(), pRankOut);
        bool isOutComplex = _pL->isComplex() || _pR->isComplex();

        //Result P1(i) - P2(0)
        SinglePoly *p2Coef          = _pR->get(0);
        double *p2R                 = p2Coef->get();

        if (isOutComplex)
        {
            double *p2I             = p2Coef->getImg();
            for (int i = 0 ; i < _pL->getSize() ; i++)
            {
                SinglePoly *p1Coef      = _pL->get(i);
                double *p1R             = p1Coef->get();
                double *p1I             = p1Coef->getImg();

                SinglePoly *pOutCoef    = pOut->get(i);
                pOutCoef->setComplex(isOutComplex);
                double *pOutR           = pOutCoef->get();
                double *pOutI           = pOutCoef->getImg();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > pRank1[j])
                    {
                        pOutR[j] = - p2R[j];
                        pOutI[j] = - (p2I ? p2I[j] : 0);
                    }
                    else if (j > iRank2)
                    {
                        pOutR[j] = p1R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0);
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0) - (p2I ? p2I[j] : 0);
                    }
                }
            }
        }
        else
        {
            for (int i = 0 ; i < _pL->getSize() ; i++)
            {
                SinglePoly *p1Coef      = _pL->get(i);
                double *p1R             = p1Coef->get();

                SinglePoly *pOutCoef    = pOut->get(i);
                double *pOutR           = pOutCoef->get();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > pRank1[j])
                    {
                        pOutR[j] = - p2R[j];
                    }
                    else if (j > iRank2)
                    {
                        pOutR[j] = p1R[j];
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                    }
                }
            }
        }

        delete[] pRankOut;
        delete[] pRank1;
        pOut->updateRank();
        return pOut;
    }

    //check dimension compatibilities
    int iDims1  = _pL->getDims();
    int iDims2  = _pR->getDims();

    if (iDims1 != iDims2)
    {
        return nullptr;
    }

    int* piDims1    = _pL->getDimsArray();
    int* piDims2    = _pR->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    int *pRankOut   = new int[_pL->getSize()];
    int *pRank1     = new int[_pL->getSize()];
    int *pRank2     = new int[_pR->getSize()];

    _pL->getRank(pRank1);
    _pR->getRank(pRank2);
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pRankOut[i] = std::max(pRank1[i], pRank2[i]);
    }

    pOut = new Polynom(_pR->getVariableName(), iDims1, piDims1, pRankOut);
    bool isOutComplex =  _pL->isComplex() || _pR->isComplex();

    //Result P1(i) - P2(i)
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        SinglePoly* pOutCoef = pOut->get(i);
        pOutCoef->setComplex(isOutComplex);

        double *p1R     = _pL->get(i)->get();
        double *p2R     = _pR->get(i)->get();
        double *pOutR   = pOutCoef->get();
        int iMin        = std::min(pRank1[i], pRank2[i]);
        int iMax        = std::max(pRank1[i], pRank2[i]);

        for (int j = 0 ; j < iMin + 1 ; j++)
        {
            pOutR[j]    = p1R[j] - p2R[j];
        }

        double *pTemp   = NULL;
        int iCoef       = 1;
        if (pRank1[i] > pRank2[i])
        {
            pTemp       = p1R;
            iCoef       = 1;
        }
        else
        {
            pTemp       = p2R;
            iCoef       = -1;
        }

        for (int j = iMin + 1 ; j < iMax + 1 ; j++)
        {
            pOutR[j]    = pTemp[j] * iCoef;
        }

        if (isOutComplex)
        {
            double *p1I     = _pL->get(i)->getImg();
            double *p2I     = _pR->get(i)->getImg();
            double *pOutI   = pOutCoef->getImg();

            for (int j = 0 ; j < iMin + 1 ; j++)
            {
                pOutI[j]    = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
            }

            for (int j = iMin + 1 ; j < iMax + 1 ; j++)
            {
                pOutI[j]  = pTemp[j] * iCoef;
            }
        }
    }

    delete[] pRankOut;
    delete[] pRank1;
    delete[] pRank2;

    pOut->updateRank();
    return pOut;
}

// P - D
template<> InternalType* sub_M_M<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    Polynom* pOut = NULL;

    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();
    bool isComplexOut = bComplex2 || bComplex1;

    // P - []
    if (_pR->isEmpty())
    {
        return _pL;
    }

    if (_pR->isIdentity())
    {
        double dblRightR = _pR->get(0);
        double dblRightI = 0;
        if (_pR->isComplex())
        {
            dblRightI = _pR->getImg(0);
        }

        int iDims = _pL->getDims();
        int* piDims = _pL->getDimsArray();
        pOut = (Polynom*)_pL->clone();
        pOut->setComplex(isComplexOut);
        SinglePoly** pSPOut = pOut->get();
        SinglePoly** pSPLeft = _pL->get();
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

        if (isComplexOut)
        {
            for (int i = 0 ; i < iLeadDims ; ++i)
            {
                for (int j = 0 ; j < iDims ; ++j)
                {
                    piIndex[j] = i;
                }

                int index = _pL->getIndex(piIndex);
                sub(pSPLeft[index]->get(0), pSPLeft[index]->getImg(0), (size_t)1, &dblRightR, &dblRightI, pSPOut[index]->get(), pSPOut[index]->getImg());
            }
        }
        else
        {
            for (int i = 0 ; i < iLeadDims ; ++i)
            {
                for (int j = 0 ; j < iDims ; ++j)
                {
                    piIndex[j] = i;
                }

                int index = _pL->getIndex(piIndex);
                sub(pSPLeft[index]->get(0), (size_t)1, &dblRightR, pSPOut[index]->get());
            }
        }

        delete[] piIndex;
        return pOut;

    }

    if (_pR->isScalar())
    {
        //subtract same value to all polynoms
        pOut = (Polynom*)_pL->clone();
        pOut->setComplex(isComplexOut);

        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();

        double dblR = _pR->get(0);
        if (isComplexOut)
        {
            double dblI = _pR->getImg(0);
            for (int i = 0 ; i < iSize ; ++i)
            {
                pSP[i]->get()[0] -= dblR;
                pSP[i]->getImg()[0] -= dblI;
            }
        }
        else
        {
            for (int i = 0 ; i < iSize ; ++i)
            {
                pSP[i]->get()[0] -= dblR;
            }
        }

        return pOut;
    }

    if (_pL->isScalar())
    {
        //and _pR is not !
        int iDims = _pR->getDims();
        int* piDims = _pR->getDimsArray();
        int iSize = _pR->getSize();

        pOut = new Polynom(_pL->getVariableName(), iDims, piDims);

        SinglePoly* pSPL = _pL->get(0);
        if (_pR->isComplex())
        {
            double* pdblR = _pR->get();
            double* pdblI = _pR->getImg();

            for (int i = 0 ; i < iSize ; ++i)
            {
                SinglePoly* pSPOut = pSPL->clone();
                //in case of original is not complex
                pSPOut->setComplex(isComplexOut);
                pSPOut->get()[0] -= pdblR[i];
                pSPOut->getImg()[0] -= pdblI[i];
                pOut->set(i, pSPOut);
                delete pSPOut;
            }
        }
        else
        {
            double* pdblR = _pR->get();

            for (int i = 0 ; i < iSize ; ++i)
            {
                SinglePoly* pSPOut = pSPL->clone();
                //update 0th rank value
                pSPOut->get()[0] -= pdblR[i];
                pOut->set(i, pSPOut);
                delete pSPOut;
            }
        }

        return pOut;
    }

    //P - D

    //check dimensions
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
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n").c_str(),  L"-", _pL->DimToString().c_str(), _pR->DimToString().c_str());
            throw ast::InternalError(pMsg);
        }
    }

    double* pInDblR = _pR->get();
    pOut = (Polynom*)_pL->clone();
    if (bComplex1 && bComplex2)
    {
        double* pInDblI = _pR->getImg();
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut   = pOut->get(i);
            double *pOutPolyR    = pSPOut->get();
            double *pOutPolyI    = pSPOut->getImg();

            pOutPolyR[0] -= pInDblR[i];
            pOutPolyI[0] -= pInDblI[i];
        }
    }
    else if (bComplex2)
    {
        double* pInDblI = _pR->getImg();
        pOut->setComplex(true);
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut   = pOut->get(i);
            double *pOutPolyR    = pSPOut->get();
            double *pOutPolyI    = pSPOut->getImg();

            pOutPolyR[0] -= pInDblR[i];
            pOutPolyI[0] = -pInDblI[i];
        }
    }
    else
    {
        for (int i = 0 ; i < pOut->getSize() ; i++)
        {
            SinglePoly *pSPOut = pOut->get(i);
            double *pOutPolyR  = pSPOut->get();

            pOutPolyR[0] -= pInDblR[i];
        }
    }

    return pOut;
}

template<> InternalType* sub_I_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = (Polynom*)opposite_M<Polynom, Polynom>(_pR);
    double dblLeft = _pL->get(0);

    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    SinglePoly** pSP = _pR->get();
    SinglePoly** pSPOut = pOut->get();
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
        sub(dblLeft, pSP[index]->get(0), pSPOut[index]->get());
    }

    delete[] piIndex;
    return pOut;
}

template<> InternalType* sub_I_MC<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = (Polynom*)opposite_MC<Polynom, Polynom>(_pR);
    double dblLeft = _pL->get(0);

    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    SinglePoly** pSP = _pR->get();
    SinglePoly** pSPOut = pOut->get();
    piIndex[0] = 0;

    for (int i = 0 ; i < iDims ; ++i)
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
        sub(dblLeft, pSP[index]->get(0), pSPOut[index]->get());
    }

    delete[] piIndex;
    return pOut;
}

template<> InternalType* sub_IC_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = (Polynom*)opposite_M<Polynom, Polynom>(_pR);
    pOut->setComplex(true);
    double dblLeftR = _pL->get(0);
    double dblLeftI = _pL->getImg(0);

    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    SinglePoly** pSP = _pR->get();
    SinglePoly** pSPOut = pOut->get();
    piIndex[0] = 0;

    for (int i = 0 ; i < iDims ; ++i)
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
        sub(&dblLeftR, &dblLeftI, (size_t)1, pSP[index]->get(0), pSPOut[index]->get(), pSPOut[index]->getImg());
    }

    delete[] piIndex;
    return pOut;
}

template<> InternalType* sub_IC_MC<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = (Polynom*)opposite_MC<Polynom, Polynom>(_pR);
    double dblLeftR = _pL->get(0);
    double dblLeftI = _pL->getImg(0);

    int iDims = _pR->getDims();
    int* piDims = _pR->getDimsArray();
    int iLeadDims = piDims[0];
    int* piIndex = new int[iDims];
    SinglePoly** pSP = _pR->get();
    SinglePoly** pSPOut = pOut->get();
    piIndex[0] = 0;

    for (int i = 0 ; i < iDims ; ++i)
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
        sub(dblLeftR, dblLeftI, pSP[index]->get(0), pSP[index]->getImg(0), pSPOut[index]->get(), pSPOut[index]->getImg());
    }

    delete[] piIndex;
    return pOut;
}

template<> InternalType* sub_M_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = NULL;
    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();

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

            pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

            for (int j = 1 ; j < pInPoly->getSize() ; j++)
            {
                pOutPolyR[j] = -pInPolyR[j];
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

                pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pInPolyI != NULL ? pInPolyI[0] : 0);

                for (int j = 1 ; j < pInPoly->getSize() ; j++)
                {
                    pOutPolyI[j] = (pInPolyI != NULL ? -pInPolyI[j] : 0);
                }
            }
        }

        return pOut;
    }

    if (_pL->isScalar())
    {
        if (bComplex2)
        {
            pOut = (Polynom*)opposite_MC<Polynom, Polynom>(_pR);
        }
        else
        {
            pOut = (Polynom*)opposite_M<Polynom, Polynom>(_pR);
        }

        int iSize = pOut->getSize();

        if (bComplex1 && bComplex2)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                SinglePoly *pSPOut   = pOut->get(i);
                double *pOutPolyR    = pSPOut->get();
                double *pOutPolyI    = pSPOut->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] += pInDblI[0];
            }
        }
        else if (bComplex1)
        {
            pOut->setComplex(true);
            for (int i = 0 ; i < iSize ; i++)
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
            for (int i = 0 ; i < iSize ; i++)
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

    if (bComplex2)
    {
        pOut = (Polynom*)opposite_MC<Polynom, Polynom>(_pR);
    }
    else
    {
        pOut = (Polynom*)opposite_M<Polynom, Polynom>(_pR);
    }

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
    else if (bComplex1)
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

//sp - sp
template<> InternalType* sub_M_M<Sparse, Sparse, Sparse>(Sparse* _pL, Sparse* _pR)
{
    //check scalar hidden in a sparse ;)
    if (_pL->isScalar() || _pR->isScalar())
    {
        // scalar - sp  or  sp - scalar
        // call Overload
        return NULL;
    }

    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        //dimensions not match
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    types::Sparse* pSPOut = _pL->substract(*_pR);
    pSPOut->finalize();
    return pSPOut;
}

//d - sp
template<> InternalType* sub_M_M<Double, Sparse, Double>(Double* _pL, Sparse* _pR)
{
    Double* pOut = NULL;
    int iOne = 1; //fortran
    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();

    if (_pL->isScalar() && _pR->isScalar())
    {
        //d - sp
        pOut = (Double*)_pL->clone();
        pOut->setComplex(bComplex1 || bComplex2);
        if (bComplex2)
        {
            std::complex<double> dbl = _pR->getImg(0, 0);
            pOut->set(0, dbl.real() - pOut->get(0));
            pOut->setImg(0, pOut->getImg(0) - dbl.imag());
        }
        else
        {
            pOut->set(0, pOut->get(0) - _pL->get(0, 0));
        }

        return pOut;
    }

    if (_pL->isScalar())
    {
        //d - SP
        pOut = new Double(_pR->getRows(), _pR->getCols(), bComplex1 || bComplex2);
        int iSize = _pR->getSize();
        double dblVal = _pL->get(0);
        double dblValI = 0;
        C2F(dset)(&iSize, &dblVal, pOut->get(), &iOne);
        if (bComplex1)
        {
            //initialize imag part at 0
            dblValI = _pL->getImg(0);
            C2F(dset)(&iSize, &dblValI, pOut->getImg(), &iOne);
        }
        else if (bComplex2)
        {
            dblValI = 0;
            C2F(dset)(&iSize, &dblValI, pOut->getImg(), &iOne);
        }

        int nonZeros = static_cast<int>(_pR->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pR->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (pOut->isComplex())
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pR->getImg(iRow, iCol);
                pOut->set(iRow, iCol, dblVal - dbl.real());
                pOut->setImg(iRow, iCol, dblValI - dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pOut->set(iRow, iCol, dblVal - _pR->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;

        return pOut;
    }

    if (_pR->isScalar())
    {
        //D - sp
        pOut = (Double*)_pL->clone();
        pOut->setComplex(bComplex1 || bComplex2);

        if (pOut->isComplex())
        {
            double* pReal = pOut->get();
            double* pImg = pOut->getImg();
            int size = pOut->getSize();
            std::complex<double> dbl = _pR->getImg(0, 0);
            for (int i = 0 ; i < size ; i++)
            {
                pReal[i] -= dbl.real();
                pImg[i] -= dbl.imag();
            }
        }
        else
        {
            double* pReal = pOut->get();
            int size = pOut->getSize();
            double dblTmp = _pR->get(0);
            for (int i = 0 ; i < size ; i++)
            {
                pReal[i] -= dblTmp;
            }
        }

        return pOut;
    }

    if (_pL->getDims() > 2)
    {
        return nullptr;
    }

    if (_pL->getRows() == _pR->getRows() && _pL->getCols() == _pR->getCols())
    {
        //D - SP
        pOut = (Double*)_pL->clone();
        pOut->setComplex(bComplex1 || bComplex2);

        int nonZeros = static_cast<int>(_pR->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pR->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1 || bComplex2)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pR->getImg(iRow, iCol);
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) - dbl.real());
                pOut->setImg(iRow, iCol, pOut->getImg(iRow, iCol) - dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pOut->set(iRow, iCol, pOut->get(iRow, iCol) - _pR->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;
        return pOut;
    }
    else
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }
}

//sp - d
template<> InternalType* sub_M_M<Sparse, Double, Double>(Sparse* _pL, Double* _pR)
{
    Double* pOut = NULL;
    int iOne = 1; //fortran
    bool bComplex1 = _pL->isComplex();
    bool bComplex2 = _pR->isComplex();
    bool bComplexOut = bComplex1 || bComplex2;

    if (_pL->isScalar() && _pR->isScalar())
    {
        //sp - d
        pOut = (Double*)_pR->clone();
        pOut->setComplex(bComplexOut);
        if (bComplex1)
        {
            std::complex<double> dbl = _pL->getImg(0, 0);
            pOut->set(0, pOut->get(0) - dbl.real());
            pOut->setImg(0, dbl.imag() - pOut->getImg(0));
        }
        else
        {
            pOut->set(0, _pL->get(0, 0) - pOut->get(0));
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        //SP - d
        pOut = new Double(_pL->getRows(), _pL->getCols(), bComplexOut);
        int iSize = _pL->getSize();
        double dblVal = -_pR->get(0);
        double dblValI = 0;
        C2F(dset)(&iSize, &dblVal, pOut->get(), &iOne);
        if (bComplex2)
        {
            dblValI = -_pR->getImg(0);
            C2F(dset)(&iSize, &dblValI, pOut->getImg(), &iOne);
        }
        else if (bComplex1)
        {
            //initialize imag part at 0
            dblValI = 0;
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
                pOut->set(iRow, iCol, dbl.real() - (-dblVal));
                pOut->setImg(iRow, iCol, dbl.imag() - (-dblValI));
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pOut->set(iRow, iCol, _pL->get(iRow, iCol) - (-dblVal));
            }
        }

        //clear
        delete[] pRows;

        return pOut;
    }

    if (_pL->isScalar())
    {
        //sp - D
        pOut = (Double*)_pR->clone();
        pOut->setComplex(bComplexOut);

        if (bComplex1)
        {
            double* pReal = pOut->get();
            double* pImg = pOut->getImg();
            int size = pOut->getSize();
            for (int i = 0 ; i < size ; i++)
            {
                std::complex<double> dbl = _pL->getImg(0, 0);
                pReal[i] = dbl.real() - pReal[i];
                pImg[i] = dbl.imag() - pImg[i];
            }
        }
        else
        {
            double* pReal = pOut->get();
            int size = pOut->getSize();
            for (int i = 0 ; i < size ; i++)
            {
                pReal[i] = _pL->get(0, 0) - pReal[i];
            }
        }

        return pOut;
    }


    if (_pR->getDims() > 2)
    {
        return nullptr;
    }

    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    //SP - D
    pOut = new types::Double(_pL->getRows(), _pL->getCols(), _pL->isComplex());
    _pL->fill(*pOut);
    int iSize = pOut->getSize();
    pOut->setComplex(bComplexOut);
    double* pdblOutR = pOut->get();
    double* pdblOutI = pOut->getImg(); //can be null for non complex output
    double* pdblRR = _pR->get();
    double* pdblRI = _pR->getImg(); //can be null for non complex output

    if (bComplex1)
    {
        if (bComplex2)
        {
            sub(pdblOutR, pdblOutI, iSize, pdblRR, pdblRI, pdblOutR, pdblOutI);
        }
        else
        {
            sub(pdblOutR, pdblOutI, iSize, pdblRR, pdblOutR, pdblOutI);
        }
    }
    else
    {
        if (bComplex2)
        {
            sub(pdblOutR, iSize, pdblRR, pdblRI, pdblOutR, pdblOutI);
        }
        else
        {
            sub(pdblOutR, iSize, pdblRR, pdblOutR);
        }
    }

    return pOut;
}

//Identity - sp
template<> InternalType* sub_M_M<Double, Sparse, Sparse>(Double* _pL, Sparse* _pR)
{
    Sparse* pOut = NULL;
    if (_pL->isIdentity())
    {
        //convert to _pL
        Sparse* pS = new Sparse(_pR->getRows(), _pR->getCols(), _pL->isComplex());
        int size = std::min(_pR->getRows(), _pR->getCols());
        double dblLeftR = _pL->get(0);


        if (_pL->isComplex())
        {
            std::complex<double> complexLeft(dblLeftR, _pL->getImg(0));
            for (int i = 0 ; i < size ; i++)
            {
                pS->set(i, i, complexLeft);
            }
        }
        else
        {
            for (int i = 0 ; i < size ; i++)
            {
                pS->set(i, i, dblLeftR);
            }
        }
        pS->finalize();


        pOut = pS->substract(*_pR);
        delete pS;
        return pOut;
    }
    else
    {
        // Call overload if the matrix is not identity
        return NULL;
    }
}

//sp - Identity
template<> InternalType* sub_M_M<Sparse, Double, Sparse>(Sparse* _pL, Double* _pR)
{
    Sparse* pOut = NULL;
    if (_pR->isIdentity())
    {
        //convert to _pL
        Sparse* pS = new Sparse(_pL->getRows(), _pL->getCols(), _pR->isComplex());
        int size = std::min(_pL->getRows(), _pL->getCols());
        double dblRightR = _pR->get(0);

        if (_pR->isComplex())
        {
            std::complex<double> complexRight(dblRightR, _pR->getImg(0));
            for (int i = 0 ; i < size ; i++)
            {
                pS->set(i, i, complexRight, false);
            }
        }
        else
        {
            for (int i = 0 ; i < size ; i++)
            {
                pS->set(i, i, dblRightR, false);
            }
        }
        pS->finalize();


        pOut = _pL->substract(*pS);
        delete pS;
        return pOut;
    }
    else
    {
        // Call overload if the matrix is not identity
        return NULL;
    }
}
