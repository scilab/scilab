/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
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

#include "types_dotdivide.hxx"
#include "double.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "polynom.hxx"


extern "C"
{
#include "localization.h"
#include "sciprint.h"
}

using namespace types;
//define arrays on operation functions
static dotdiv_function pDotDivfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillDotDivFunction()
{
#define scilab_fill_dotdiv(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pDotDivfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (dotdiv_function)&dotdiv_##func<typeIn1, typeIn2, typeOut>

    //Double
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Double, Double, M_M, Double, Double, Double);
    scilab_fill_dotdiv(Double, Int8, M_M, Double, Int8, Int8);
    scilab_fill_dotdiv(Double, UInt8, M_M, Double, UInt8, UInt8);
    scilab_fill_dotdiv(Double, Int16, M_M, Double, Int16, Int16);
    scilab_fill_dotdiv(Double, UInt16, M_M, Double, UInt16, UInt16);
    scilab_fill_dotdiv(Double, Int32, M_M, Double, Int32, Int32);
    scilab_fill_dotdiv(Double, UInt32, M_M, Double, UInt32, UInt32);
    scilab_fill_dotdiv(Double, Int64, M_M, Double, Int64, Int64);
    scilab_fill_dotdiv(Double, UInt64, M_M, Double, UInt64, UInt64);
    scilab_fill_dotdiv(Double, Bool, M_M, Double, Bool, Double);
    scilab_fill_dotdiv(Double, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Double, Sparse, M_M, Double, Sparse, Sparse);

    //Matrix ./ Matrix Complex
    scilab_fill_dotdiv(Double, DoubleComplex, M_MC, Double, Double, Double);
    scilab_fill_dotdiv(Double, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Double, SparseComplex, M_M, Double, Sparse, Sparse);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Double, ScalarDouble, M_S, Double, Double, Double);
    scilab_fill_dotdiv(Double, ScalarInt8, M_S, Double, Int8, Int8);
    scilab_fill_dotdiv(Double, ScalarUInt8, M_S, Double, UInt8, UInt8);
    scilab_fill_dotdiv(Double, ScalarInt16, M_S, Double, Int16, Int16);
    scilab_fill_dotdiv(Double, ScalarUInt16, M_S, Double, UInt16, UInt16);
    scilab_fill_dotdiv(Double, ScalarInt32, M_S, Double, Int32, Int32);
    scilab_fill_dotdiv(Double, ScalarUInt32, M_S, Double, UInt32, UInt32);
    scilab_fill_dotdiv(Double, ScalarInt64, M_S, Double, Int64, Int64);
    scilab_fill_dotdiv(Double, ScalarUInt64, M_S, Double, UInt64, UInt64);
    scilab_fill_dotdiv(Double, ScalarBool, M_S, Double, Bool, Double);
    scilab_fill_dotdiv(Double, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Matrix ./ Scalar Complex
    scilab_fill_dotdiv(Double, ScalarDoubleComplex, M_SC, Double, Double, Double);
    scilab_fill_dotdiv(Double, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Matrix ./ Empty
    scilab_fill_dotdiv(Double, Empty, M_E, Double, Double, Double);


    //Matrix Complex ./ Matrix
    scilab_fill_dotdiv(DoubleComplex, Double, MC_M, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, ScalarDouble, MC_S, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, Empty, M_E, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(DoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_dotdiv(DoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarDouble, Double, S_M, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, Int8, S_M, Double, Int8, Int8);
    scilab_fill_dotdiv(ScalarDouble, UInt8, S_M, Double, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarDouble, Int16, S_M, Double, Int16, Int16);
    scilab_fill_dotdiv(ScalarDouble, UInt16, S_M, Double, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarDouble, Int32, S_M, Double, Int32, Int32);
    scilab_fill_dotdiv(ScalarDouble, UInt32, S_M, Double, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarDouble, Int64, S_M, Double, Int64, Int64);
    scilab_fill_dotdiv(ScalarDouble, UInt64, S_M, Double, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarDouble, Bool, S_M, Double, Bool, Double);
    scilab_fill_dotdiv(ScalarDouble, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarDouble, Sparse, M_M, Double, Sparse, Sparse);

    //Scalar ./ Matrix Complex
    scilab_fill_dotdiv(ScalarDouble, DoubleComplex, S_MC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarDouble, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarDouble, ScalarDouble, S_S, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, ScalarInt8, S_S, Double, Int8, Int8);
    scilab_fill_dotdiv(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarDouble, ScalarInt16, S_S, Double, Int16, Int16);
    scilab_fill_dotdiv(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarDouble, ScalarInt32, S_S, Double, Int32, Int32);
    scilab_fill_dotdiv(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarDouble, ScalarInt64, S_S, Double, Int64, Int64);
    scilab_fill_dotdiv(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarDouble, ScalarBool, S_S, Double, Bool, Double);
    scilab_fill_dotdiv(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Scalar ./ Scalar Complex
    scilab_fill_dotdiv(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);

    //Scalar ./ Empty
    scilab_fill_dotdiv(ScalarDouble, Empty, M_E, Double, Double, Double);

    //Scalar Complex ./ Matrix
    scilab_fill_dotdiv(ScalarDoubleComplex, Double, SC_M, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    //Scalar Complex ./ Matrix Complex
    scilab_fill_dotdiv(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);
    //Scalar Complex ./ Scalar
    scilab_fill_dotdiv(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    //Scalar Complex ./ Scalar Complex
    scilab_fill_dotdiv(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Scalar Complex ./ Empty
    scilab_fill_dotdiv(ScalarDoubleComplex, Empty, M_E, Double, Double, Double);

    //Empty ./ Matrix
    scilab_fill_dotdiv(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_dotdiv(Empty, Int8, E_M, Double, Int8, Double);
    scilab_fill_dotdiv(Empty, UInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotdiv(Empty, Int16, E_M, Double, Int16, Double);
    scilab_fill_dotdiv(Empty, UInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotdiv(Empty, Int32, E_M, Double, Int32, Double);
    scilab_fill_dotdiv(Empty, UInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotdiv(Empty, Int64, E_M, Double, Int64, Double);
    scilab_fill_dotdiv(Empty, UInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotdiv(Empty, Polynom, E_M, Double, Polynom, Double);
    scilab_fill_dotdiv(Empty, PolynomComplex, E_M, Double, Polynom, Double);
    scilab_fill_dotdiv(Empty, Sparse, E_M, Double, Sparse, Double);
    scilab_fill_dotdiv(Empty, SparseComplex, E_M, Double, Sparse, Double);

    //Empty ./ Matrix Complex
    scilab_fill_dotdiv(Empty, DoubleComplex, E_M, Double, Double, Double);
    //Empty ./ Scalar
    scilab_fill_dotdiv(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_dotdiv(Empty, ScalarInt8, E_M, Double, Int8, Double);
    scilab_fill_dotdiv(Empty, ScalarUInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotdiv(Empty, ScalarInt16, E_M, Double, Int16, Double);
    scilab_fill_dotdiv(Empty, ScalarUInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotdiv(Empty, ScalarInt32, E_M, Double, Int32, Double);
    scilab_fill_dotdiv(Empty, ScalarUInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotdiv(Empty, ScalarInt64, E_M, Double, Int64, Double);
    scilab_fill_dotdiv(Empty, ScalarUInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotdiv(Empty, ScalarBool, E_M, Double, Bool, Double);
    scilab_fill_dotdiv(Empty, ScalarPolynom, E_M, Double, Polynom, Double);

    //Empty ./ Scalar Complex
    scilab_fill_dotdiv(Empty, ScalarDoubleComplex, E_M, Double, Double, Double);
    scilab_fill_dotdiv(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Double);
    //Empty ./ Empty
    scilab_fill_dotdiv(Empty, Empty, E_M, Double, Double, Double);
    //Empty ./ eye
    scilab_fill_dotdiv(Empty, Identity, E_M, Double, Double, Double);
    scilab_fill_dotdiv(Empty, IdentityComplex, E_M, Double, Double, Double);

    //Matrix ./ Identity
    scilab_fill_dotdiv(Double, Identity, M_I, Double, Double, Double);
    scilab_fill_dotdiv(Double, IdentityComplex, M_IC, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, Identity, MC_I, Double, Double, Double);
    scilab_fill_dotdiv(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, Identity, S_I, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDouble, IdentityComplex, S_IC, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, Identity, SC_I, Double, Double, Double);
    scilab_fill_dotdiv(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Double);

    //Int8
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Int8, Double, M_M, Int8, Double, Int8);
    scilab_fill_dotdiv(Int8, Int8, M_M, Int8, Int8, Int8);
    scilab_fill_dotdiv(Int8, UInt8, M_M, Int8, UInt8, UInt8);
    scilab_fill_dotdiv(Int8, Int16, M_M, Int8, Int16, Int16);
    scilab_fill_dotdiv(Int8, UInt16, M_M, Int8, UInt16, UInt16);
    scilab_fill_dotdiv(Int8, Int32, M_M, Int8, Int32, Int32);
    scilab_fill_dotdiv(Int8, UInt32, M_M, Int8, UInt32, UInt32);
    scilab_fill_dotdiv(Int8, Int64, M_M, Int8, Int64, Int64);
    scilab_fill_dotdiv(Int8, UInt64, M_M, Int8, UInt64, UInt64);
    scilab_fill_dotdiv(Int8, Bool, M_M, Int8, Bool, Int8);
    scilab_fill_dotdiv(Int8, Empty, M_E, Int8, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Int8, ScalarDouble, M_S, Int8, Double, Int8);
    scilab_fill_dotdiv(Int8, ScalarInt8, M_S, Int8, Int8, Int8);
    scilab_fill_dotdiv(Int8, ScalarUInt8, M_S, Int8, UInt8, UInt8);
    scilab_fill_dotdiv(Int8, ScalarInt16, M_S, Int8, Int16, Int16);
    scilab_fill_dotdiv(Int8, ScalarUInt16, M_S, Int8, UInt16, UInt16);
    scilab_fill_dotdiv(Int8, ScalarInt32, M_S, Int8, Int32, Int32);
    scilab_fill_dotdiv(Int8, ScalarUInt32, M_S, Int8, UInt32, UInt32);
    scilab_fill_dotdiv(Int8, ScalarInt64, M_S, Int8, Int64, Int64);
    scilab_fill_dotdiv(Int8, ScalarUInt64, M_S, Int8, UInt64, UInt64);
    scilab_fill_dotdiv(Int8, ScalarBool, M_S, Int8, Bool, Int8);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarInt8, Double, S_M, Int8, Double, Int8);
    scilab_fill_dotdiv(ScalarInt8, Int8, S_M, Int8, Int8, Int8);
    scilab_fill_dotdiv(ScalarInt8, UInt8, S_M, Int8, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarInt8, Int16, S_M, Int8, Int16, Int16);
    scilab_fill_dotdiv(ScalarInt8, UInt16, S_M, Int8, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarInt8, Int32, S_M, Int8, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt8, UInt32, S_M, Int8, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt8, Int64, S_M, Int8, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt8, UInt64, S_M, Int8, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt8, Bool, S_M, Int8, Bool, Int8);
    scilab_fill_dotdiv(ScalarInt8, Empty, M_E, Int8, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarInt8, ScalarDouble, S_S, Int8, Double, Int8);
    scilab_fill_dotdiv(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Int8);
    scilab_fill_dotdiv(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Int16);
    scilab_fill_dotdiv(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt8, ScalarBool, S_S, Int8, Bool, Int8);

    //UInt8
    //Matrix ./ Matrix
    scilab_fill_dotdiv(UInt8, Double, M_M, UInt8, Double, UInt8);
    scilab_fill_dotdiv(UInt8, Int8, M_M, UInt8, Int8, UInt8);
    scilab_fill_dotdiv(UInt8, UInt8, M_M, UInt8, UInt8, UInt8);
    scilab_fill_dotdiv(UInt8, Int16, M_M, UInt8, Int16, UInt16);
    scilab_fill_dotdiv(UInt8, UInt16, M_M, UInt8, UInt16, UInt16);
    scilab_fill_dotdiv(UInt8, Int32, M_M, UInt8, Int32, UInt32);
    scilab_fill_dotdiv(UInt8, UInt32, M_M, UInt8, UInt32, UInt32);
    scilab_fill_dotdiv(UInt8, Int64, M_M, UInt8, Int64, UInt64);
    scilab_fill_dotdiv(UInt8, UInt64, M_M, UInt8, UInt64, UInt64);
    scilab_fill_dotdiv(UInt8, Bool, M_M, UInt8, Bool, UInt8);
    scilab_fill_dotdiv(UInt8, Empty, M_E, UInt8, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(UInt8, ScalarDouble, M_S, UInt8, Double, UInt8);
    scilab_fill_dotdiv(UInt8, ScalarInt8, M_S, UInt8, Int8, UInt8);
    scilab_fill_dotdiv(UInt8, ScalarUInt8, M_S, UInt8, UInt8, UInt8);
    scilab_fill_dotdiv(UInt8, ScalarInt16, M_S, UInt8, Int16, UInt16);
    scilab_fill_dotdiv(UInt8, ScalarUInt16, M_S, UInt8, UInt16, UInt16);
    scilab_fill_dotdiv(UInt8, ScalarInt32, M_S, UInt8, Int32, UInt32);
    scilab_fill_dotdiv(UInt8, ScalarUInt32, M_S, UInt8, UInt32, UInt32);
    scilab_fill_dotdiv(UInt8, ScalarInt64, M_S, UInt8, Int64, UInt64);
    scilab_fill_dotdiv(UInt8, ScalarUInt64, M_S, UInt8, UInt64, UInt64);
    scilab_fill_dotdiv(UInt8, ScalarBool, M_S, UInt8, Bool, UInt8);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarUInt8, Double, S_M, UInt8, Double, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, Int8, S_M, UInt8, Int8, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, UInt8, S_M, UInt8, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, Int16, S_M, UInt8, Int16, UInt16);
    scilab_fill_dotdiv(ScalarUInt8, UInt16, S_M, UInt8, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarUInt8, Int32, S_M, UInt8, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt8, UInt32, S_M, UInt8, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt8, Int64, S_M, UInt8, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt8, UInt64, S_M, UInt8, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt8, Bool, S_M, UInt8, Bool, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, Empty, M_E, UInt8, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, UInt16);
    scilab_fill_dotdiv(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, UInt8);

    //Int16
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Int16, Double, M_M, Int16, Double, Int16);
    scilab_fill_dotdiv(Int16, Int8, M_M, Int16, Int8, Int16);
    scilab_fill_dotdiv(Int16, UInt8, M_M, Int16, UInt8, UInt16);
    scilab_fill_dotdiv(Int16, Int16, M_M, Int16, Int16, Int16);
    scilab_fill_dotdiv(Int16, UInt16, M_M, Int16, UInt16, UInt16);
    scilab_fill_dotdiv(Int16, Int32, M_M, Int16, Int32, Int32);
    scilab_fill_dotdiv(Int16, UInt32, M_M, Int16, UInt32, UInt32);
    scilab_fill_dotdiv(Int16, Int64, M_M, Int16, Int64, Int64);
    scilab_fill_dotdiv(Int16, UInt64, M_M, Int16, UInt64, UInt64);
    scilab_fill_dotdiv(Int16, Bool, M_M, Int16, Bool, Int16);
    scilab_fill_dotdiv(Int16, Empty, M_E, Int16, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Int16, ScalarDouble, M_S, Int16, Double, Int16);
    scilab_fill_dotdiv(Int16, ScalarInt8, M_S, Int16, Int8, Int16);
    scilab_fill_dotdiv(Int16, ScalarUInt8, M_S, Int16, UInt8, UInt16);
    scilab_fill_dotdiv(Int16, ScalarInt16, M_S, Int16, Int16, Int16);
    scilab_fill_dotdiv(Int16, ScalarUInt16, M_S, Int16, UInt16, UInt16);
    scilab_fill_dotdiv(Int16, ScalarInt32, M_S, Int16, Int32, Int32);
    scilab_fill_dotdiv(Int16, ScalarUInt32, M_S, Int16, UInt32, UInt32);
    scilab_fill_dotdiv(Int16, ScalarInt64, M_S, Int16, Int64, Int64);
    scilab_fill_dotdiv(Int16, ScalarUInt64, M_S, Int16, UInt64, UInt64);
    scilab_fill_dotdiv(Int16, ScalarBool, M_E, Int16, Bool, Int16);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarInt16, Double, S_M, Int16, Double, Int16);
    scilab_fill_dotdiv(ScalarInt16, Int8, S_M, Int16, Int8, Int16);
    scilab_fill_dotdiv(ScalarInt16, UInt8, S_M, Int16, UInt8, UInt16);
    scilab_fill_dotdiv(ScalarInt16, Int16, S_M, Int16, Int16, Int16);
    scilab_fill_dotdiv(ScalarInt16, UInt16, S_M, Int16, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarInt16, Int32, S_M, Int16, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt16, UInt32, S_M, Int16, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt16, Int64, S_M, Int16, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt16, UInt64, S_M, Int16, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt16, Bool, S_M, Int16, Bool, Int16);
    scilab_fill_dotdiv(ScalarInt16, Empty, M_E, Int16, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarInt16, ScalarDouble, S_S, Int16, Double, Int16);
    scilab_fill_dotdiv(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Int16);
    scilab_fill_dotdiv(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, UInt16);
    scilab_fill_dotdiv(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Int16);
    scilab_fill_dotdiv(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt16, ScalarBool, S_S, Int16, Bool, Int16);

    //UInt16
    //Matrix ./ Matrix
    scilab_fill_dotdiv(UInt16, Double, M_M, UInt16, Double, UInt16);
    scilab_fill_dotdiv(UInt16, Int8, M_M, UInt16, Int8, UInt16);
    scilab_fill_dotdiv(UInt16, UInt8, M_M, UInt16, UInt8, UInt16);
    scilab_fill_dotdiv(UInt16, Int16, M_M, UInt16, Int16, UInt16);
    scilab_fill_dotdiv(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_dotdiv(UInt16, Int32, M_M, UInt16, Int32, UInt32);
    scilab_fill_dotdiv(UInt16, UInt32, M_M, UInt16, UInt32, UInt32);
    scilab_fill_dotdiv(UInt16, Int64, M_M, UInt16, Int64, UInt64);
    scilab_fill_dotdiv(UInt16, UInt64, M_M, UInt16, UInt64, UInt64);
    scilab_fill_dotdiv(UInt16, Bool, M_M, UInt16, Bool, UInt16);
    scilab_fill_dotdiv(UInt16, Empty, M_E, UInt16, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(UInt16, ScalarDouble, M_S, UInt16, Double, UInt16);
    scilab_fill_dotdiv(UInt16, ScalarInt8, M_S, UInt16, Int8, UInt16);
    scilab_fill_dotdiv(UInt16, ScalarUInt8, M_S, UInt16, UInt8, UInt16);
    scilab_fill_dotdiv(UInt16, ScalarInt16, M_S, UInt16, Int16, UInt16);
    scilab_fill_dotdiv(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_dotdiv(UInt16, ScalarInt32, M_S, UInt16, Int32, UInt32);
    scilab_fill_dotdiv(UInt16, ScalarUInt32, M_S, UInt16, UInt32, UInt32);
    scilab_fill_dotdiv(UInt16, ScalarInt64, M_S, UInt16, Int64, UInt64);
    scilab_fill_dotdiv(UInt16, ScalarUInt64, M_S, UInt16, UInt64, UInt64);
    scilab_fill_dotdiv(UInt16, ScalarBool, M_S, UInt16, Bool, UInt16);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarUInt16, Double, S_M, UInt16, Double, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, Int8, S_M, UInt16, Int8, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, UInt8, S_M, UInt16, UInt8, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, Int16, S_M, UInt16, Int16, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, Int32, S_M, UInt16, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt16, UInt32, S_M, UInt16, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt16, Int64, S_M, UInt16, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt16, UInt64, S_M, UInt16, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt16, Bool, S_M, UInt16, Bool, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, Empty, S_M, UInt16, Double, UInt16);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, UInt16);

    //Int32
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Int32, Double, M_M, Int32, Double, Int32);
    scilab_fill_dotdiv(Int32, Int8, M_M, Int32, Int8, Int32);
    scilab_fill_dotdiv(Int32, UInt8, M_M, Int32, UInt8, UInt32);
    scilab_fill_dotdiv(Int32, Int16, M_M, Int32, Int16, Int32);
    scilab_fill_dotdiv(Int32, UInt16, M_M, Int32, UInt16, UInt32);
    scilab_fill_dotdiv(Int32, Int32, M_M, Int32, Int32, Int32);
    scilab_fill_dotdiv(Int32, UInt32, M_M, Int32, UInt32, UInt32);
    scilab_fill_dotdiv(Int32, Int64, M_M, Int32, Int64, Int64);
    scilab_fill_dotdiv(Int32, UInt64, M_M, Int32, UInt64, UInt64);
    scilab_fill_dotdiv(Int32, Bool, M_M, Int32, Bool, Int32);
    scilab_fill_dotdiv(Int32, Empty, M_E, Int32, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Int32, ScalarDouble, M_S, Int32, Double, Int32);
    scilab_fill_dotdiv(Int32, ScalarInt8, M_S, Int32, Int8, Int32);
    scilab_fill_dotdiv(Int32, ScalarUInt8, M_S, Int32, UInt8, UInt32);
    scilab_fill_dotdiv(Int32, ScalarInt16, M_S, Int32, Int16, Int32);
    scilab_fill_dotdiv(Int32, ScalarUInt16, M_S, Int32, UInt16, UInt32);
    scilab_fill_dotdiv(Int32, ScalarInt32, M_S, Int32, Int32, Int32);
    scilab_fill_dotdiv(Int32, ScalarUInt32, M_S, Int32, UInt32, UInt32);
    scilab_fill_dotdiv(Int32, ScalarInt64, M_S, Int32, Int64, Int64);
    scilab_fill_dotdiv(Int32, ScalarUInt64, M_S, Int32, UInt64, UInt64);
    scilab_fill_dotdiv(Int32, ScalarBool, M_S, Int32, Bool, Int32);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarInt32, Double, S_M, Int32, Double, Int32);
    scilab_fill_dotdiv(ScalarInt32, Int8, S_M, Int32, Int8, Int32);
    scilab_fill_dotdiv(ScalarInt32, UInt8, S_M, Int32, UInt8, UInt32);
    scilab_fill_dotdiv(ScalarInt32, Int16, S_M, Int32, Int16, Int32);
    scilab_fill_dotdiv(ScalarInt32, UInt16, S_M, Int32, UInt16, UInt32);
    scilab_fill_dotdiv(ScalarInt32, Int32, S_M, Int32, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt32, UInt32, S_M, Int32, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt32, Int64, S_M, Int32, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt32, UInt64, S_M, Int32, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt32, Bool, S_M, Int32, Bool, Int32);
    scilab_fill_dotdiv(ScalarInt32, Empty, M_E, Int32, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarInt32, ScalarDouble, S_S, Int32, Double, Int32);
    scilab_fill_dotdiv(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Int32);
    scilab_fill_dotdiv(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, UInt32);
    scilab_fill_dotdiv(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Int32);
    scilab_fill_dotdiv(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, UInt32);
    scilab_fill_dotdiv(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Int32);
    scilab_fill_dotdiv(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt32, ScalarBool, S_S, Int32, Bool, Int32);

    //UInt32
    //Matrix ./ Matrix
    scilab_fill_dotdiv(UInt32, Double, M_M, UInt32, Double, UInt32);
    scilab_fill_dotdiv(UInt32, Int8, M_M, UInt32, Int8, UInt32);
    scilab_fill_dotdiv(UInt32, UInt8, M_M, UInt32, UInt8, UInt32);
    scilab_fill_dotdiv(UInt32, Int16, M_M, UInt32, Int16, UInt32);
    scilab_fill_dotdiv(UInt32, UInt16, M_M, UInt32, UInt16, UInt32);
    scilab_fill_dotdiv(UInt32, Int32, M_M, UInt32, Int32, UInt32);
    scilab_fill_dotdiv(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_dotdiv(UInt32, Int64, M_M, UInt32, Int64, UInt64);
    scilab_fill_dotdiv(UInt32, UInt64, M_M, UInt32, UInt64, UInt64);
    scilab_fill_dotdiv(UInt32, Bool, M_M, UInt32, Bool, UInt32);
    scilab_fill_dotdiv(UInt32, Empty, M_E, UInt32, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(UInt32, ScalarDouble, M_S, UInt32, Double, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarInt8, M_S, UInt32, Int8, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarUInt8, M_S, UInt32, UInt8, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarInt16, M_S, UInt32, Int16, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarUInt16, M_S, UInt32, UInt16, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarInt32, M_S, UInt32, Int32, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_dotdiv(UInt32, ScalarInt64, M_S, UInt32, Int64, UInt64);
    scilab_fill_dotdiv(UInt32, ScalarUInt64, M_S, UInt32, UInt64, UInt64);
    scilab_fill_dotdiv(UInt32, ScalarBool, M_S, UInt32, Bool, UInt32);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarUInt32, Double, S_M, UInt32, Double, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, Int8, S_M, UInt32, Int8, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, UInt8, S_M, UInt32, UInt8, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, Int16, S_M, UInt32, Int16, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, UInt16, S_M, UInt32, UInt16, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, Int32, S_M, UInt32, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, Int64, S_M, UInt32, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt32, UInt64, S_M, UInt32, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt32, Bool, S_M, UInt32, Bool, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, Empty, S_M, UInt32, Double, UInt32);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, UInt32);

    //Int64
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Int64, Double, M_M, Int64, Double, Int64);
    scilab_fill_dotdiv(Int64, Int8, M_M, Int64, Int8, Int64);
    scilab_fill_dotdiv(Int64, UInt8, M_M, Int64, UInt8, UInt64);
    scilab_fill_dotdiv(Int64, Int16, M_M, Int64, Int16, Int64);
    scilab_fill_dotdiv(Int64, UInt16, M_M, Int64, UInt16, UInt64);
    scilab_fill_dotdiv(Int64, Int32, M_M, Int64, Int32, Int64);
    scilab_fill_dotdiv(Int64, UInt32, M_M, Int64, UInt32, UInt64);
    scilab_fill_dotdiv(Int64, Int64, M_M, Int64, Int64, Int64);
    scilab_fill_dotdiv(Int64, UInt64, M_M, Int64, UInt64, UInt64);
    scilab_fill_dotdiv(Int64, Bool, M_M, Int64, Bool, Int64);
    scilab_fill_dotdiv(Int64, Empty, M_E, Int64, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Int64, ScalarDouble, M_S, Int64, Double, Int64);
    scilab_fill_dotdiv(Int64, ScalarInt8, M_S, Int64, Int8, Int64);
    scilab_fill_dotdiv(Int64, ScalarUInt8, M_S, Int64, UInt8, UInt64);
    scilab_fill_dotdiv(Int64, ScalarInt16, M_S, Int64, Int16, Int64);
    scilab_fill_dotdiv(Int64, ScalarUInt16, M_S, Int64, UInt16, UInt64);
    scilab_fill_dotdiv(Int64, ScalarInt32, M_S, Int64, Int32, Int64);
    scilab_fill_dotdiv(Int64, ScalarUInt32, M_S, Int64, UInt32, UInt64);
    scilab_fill_dotdiv(Int64, ScalarInt64, M_S, Int64, Int64, Int64);
    scilab_fill_dotdiv(Int64, ScalarUInt64, M_S, Int64, UInt64, UInt64);
    scilab_fill_dotdiv(Int64, ScalarBool, M_S, Int64, Bool, Int64);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarInt64, Double, S_M, Int64, Double, Int64);
    scilab_fill_dotdiv(ScalarInt64, Int8, S_M, Int64, Int8, Int64);
    scilab_fill_dotdiv(ScalarInt64, UInt8, S_M, Int64, UInt8, UInt64);
    scilab_fill_dotdiv(ScalarInt64, Int16, S_M, Int64, Int16, Int64);
    scilab_fill_dotdiv(ScalarInt64, UInt16, S_M, Int64, UInt16, UInt64);
    scilab_fill_dotdiv(ScalarInt64, Int32, S_M, Int64, Int32, Int64);
    scilab_fill_dotdiv(ScalarInt64, UInt32, S_M, Int64, UInt32, UInt64);
    scilab_fill_dotdiv(ScalarInt64, Int64, S_M, Int64, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt64, UInt64, S_M, Int64, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt64, Bool, S_M, Int64, Bool, Int64);
    scilab_fill_dotdiv(ScalarInt64, Empty, M_E, Int64, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarInt64, ScalarDouble, S_S, Int64, Double, Int64);
    scilab_fill_dotdiv(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Int64);
    scilab_fill_dotdiv(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, UInt64);
    scilab_fill_dotdiv(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Int64);
    scilab_fill_dotdiv(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, UInt64);
    scilab_fill_dotdiv(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Int64);
    scilab_fill_dotdiv(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, UInt64);
    scilab_fill_dotdiv(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Int64);
    scilab_fill_dotdiv(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarInt64, ScalarBool, S_S, Int64, Bool, Int64);

    //UInt64
    //Matrix ./ Matrix
    scilab_fill_dotdiv(UInt64, Double, M_M, UInt64, Double, UInt64);
    scilab_fill_dotdiv(UInt64, Int8, M_M, UInt64, Int8, UInt64);
    scilab_fill_dotdiv(UInt64, UInt8, M_M, UInt64, UInt8, UInt64);
    scilab_fill_dotdiv(UInt64, Int16, M_M, UInt64, Int16, UInt64);
    scilab_fill_dotdiv(UInt64, UInt16, M_M, UInt64, UInt16, UInt64);
    scilab_fill_dotdiv(UInt64, Int32, M_M, UInt64, Int32, UInt64);
    scilab_fill_dotdiv(UInt64, UInt32, M_M, UInt64, UInt32, UInt64);
    scilab_fill_dotdiv(UInt64, Int64, M_M, UInt64, Int64, UInt64);
    scilab_fill_dotdiv(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);
    scilab_fill_dotdiv(UInt64, Bool, M_M, UInt64, Bool, UInt64);
    scilab_fill_dotdiv(UInt64, Empty, M_E, UInt64, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(UInt64, ScalarDouble, M_S, UInt64, Double, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarInt8, M_S, UInt64, Int8, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarUInt8, M_S, UInt64, UInt8, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarInt16, M_S, UInt64, Int16, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarUInt16, M_S, UInt64, UInt16, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarInt32, M_S, UInt64, Int32, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarUInt32, M_S, UInt64, UInt32, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarInt64, M_S, UInt64, Int64, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);
    scilab_fill_dotdiv(UInt64, ScalarBool, M_S, UInt64, Bool, UInt64);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarUInt64, Double, S_M, UInt64, Double, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Int8, S_M, UInt64, Int8, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, UInt8, S_M, UInt64, UInt8, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Int16, S_M, UInt64, Int16, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, UInt16, S_M, UInt64, UInt16, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Int32, S_M, UInt64, Int32, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, UInt32, S_M, UInt64, UInt32, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Int64, S_M, UInt64, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Bool, S_M, UInt64, Bool, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, Empty, S_M, UInt64, Double, UInt64);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, UInt64);

    //Bool
    //Matrix ./ Matrix
    scilab_fill_dotdiv(Bool, Double, M_M, Bool, Double, Double);
    scilab_fill_dotdiv(Bool, Int8, M_M, Bool, Int8, Int8);
    scilab_fill_dotdiv(Bool, UInt8, M_M, Bool, UInt8, UInt8);
    scilab_fill_dotdiv(Bool, Int16, M_M, Bool, Int16, Int16);
    scilab_fill_dotdiv(Bool, UInt16, M_M, Bool, UInt16, UInt16);
    scilab_fill_dotdiv(Bool, Int32, M_M, Bool, Int32, Int32);
    scilab_fill_dotdiv(Bool, UInt32, M_M, Bool, UInt32, UInt32);
    scilab_fill_dotdiv(Bool, Int64, M_M, Bool, Int64, Int64);
    scilab_fill_dotdiv(Bool, UInt64, M_M, Bool, UInt64, UInt64);
    scilab_fill_dotdiv(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_dotdiv(Bool, Empty, M_E, Bool, Double, Double);

    //Matrix ./ Scalar
    scilab_fill_dotdiv(Bool, ScalarDouble, M_S, Bool, Double, Double);
    scilab_fill_dotdiv(Bool, ScalarInt8, M_S, Bool, Int8, Int8);
    scilab_fill_dotdiv(Bool, ScalarUInt8, M_S, Bool, UInt8, UInt8);
    scilab_fill_dotdiv(Bool, ScalarInt16, M_S, Bool, Int16, Int16);
    scilab_fill_dotdiv(Bool, ScalarUInt16, M_S, Bool, UInt16, UInt16);
    scilab_fill_dotdiv(Bool, ScalarInt32, M_S, Bool, Int32, Int32);
    scilab_fill_dotdiv(Bool, ScalarUInt32, M_S, Bool, UInt32, UInt32);
    scilab_fill_dotdiv(Bool, ScalarInt64, M_S, Bool, Int64, Int64);
    scilab_fill_dotdiv(Bool, ScalarUInt64, M_S, Bool, UInt64, UInt64);
    scilab_fill_dotdiv(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar ./ Matrix
    scilab_fill_dotdiv(ScalarBool, Double, S_M, Bool, Double, Double);
    scilab_fill_dotdiv(ScalarBool, Int8, S_M, Bool, Int8, Int8);
    scilab_fill_dotdiv(ScalarBool, UInt8, S_M, Bool, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarBool, Int16, S_M, Bool, Int16, Int16);
    scilab_fill_dotdiv(ScalarBool, UInt16, S_M, Bool, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarBool, Int32, S_M, Bool, Int32, Int32);
    scilab_fill_dotdiv(ScalarBool, UInt32, S_M, Bool, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarBool, Int64, S_M, Bool, Int64, Int64);
    scilab_fill_dotdiv(ScalarBool, UInt64, S_M, Bool, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_dotdiv(ScalarBool, Empty, M_E, Bool, Double, Double);

    //Scalar ./ Scalar
    scilab_fill_dotdiv(ScalarBool, ScalarDouble, S_S, Bool, Double, Double);
    scilab_fill_dotdiv(ScalarBool, ScalarInt8, S_S, Bool, Int8, Int8);
    scilab_fill_dotdiv(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, UInt8);
    scilab_fill_dotdiv(ScalarBool, ScalarInt16, S_S, Bool, Int16, Int16);
    scilab_fill_dotdiv(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, UInt16);
    scilab_fill_dotdiv(ScalarBool, ScalarInt32, S_S, Bool, Int32, Int32);
    scilab_fill_dotdiv(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, UInt32);
    scilab_fill_dotdiv(ScalarBool, ScalarInt64, S_S, Bool, Int64, Int64);
    scilab_fill_dotdiv(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, UInt64);
    scilab_fill_dotdiv(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Identity
    scilab_fill_dotdiv(Identity, Double, I_M, Double, Double, Double);
    scilab_fill_dotdiv(Identity, DoubleComplex, I_MC, Double, Double, Double);
    scilab_fill_dotdiv(Identity, ScalarDouble, I_S, Double, Double, Double);
    scilab_fill_dotdiv(Identity, ScalarDoubleComplex, I_SC, Double, Double, Double);
    scilab_fill_dotdiv(Identity, Identity, I_I, Double, Double, Double);
    scilab_fill_dotdiv(Identity, IdentityComplex, I_IC, Double, Double, Double);
    scilab_fill_dotdiv(Identity, Empty, M_E, Double, Double, Double);

    scilab_fill_dotdiv(Identity, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Identity, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Identity, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Identity, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(Identity, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_dotdiv(Identity, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_dotdiv(IdentityComplex, Double, IC_M, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, ScalarDouble, IC_S, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, Identity, IC_I, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Double);
    scilab_fill_dotdiv(IdentityComplex, Empty, M_E, Double, Double, Double);

    scilab_fill_dotdiv(IdentityComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(IdentityComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(IdentityComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(IdentityComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotdiv(IdentityComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_dotdiv(IdentityComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Polynom

    //poly ./ poly
    scilab_fill_dotdiv(Polynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(Polynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(PolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly ./ scalar poly
    scilab_fill_dotdiv(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly ./ double
    scilab_fill_dotdiv(Polynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(Polynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //poly ./ scalar double
    scilab_fill_dotdiv(Polynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //poly ./ []
    scilab_fill_dotdiv(Polynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotdiv(PolynomComplex, Empty, M_E, Polynom, Double, Double);

    //poly ./ eye
    scilab_fill_dotdiv(Polynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(Polynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(PolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //scalar poly ./ poly
    scilab_fill_dotdiv(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly ./ scalar poly
    scilab_fill_dotdiv(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly ./ double
    scilab_fill_dotdiv(ScalarPolynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly ./ scalar double
    scilab_fill_dotdiv(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly ./ []
    scilab_fill_dotdiv(ScalarPolynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotdiv(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Double);

    //scalar poly ./ eye
    scilab_fill_dotdiv(ScalarPolynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotdiv(ScalarPolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //Sparse
    scilab_fill_dotdiv(Sparse, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotdiv(Sparse, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotdiv(Sparse, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(Sparse, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(Sparse, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotdiv(Sparse, Empty, M_E, Sparse, Double, Double);
    scilab_fill_dotdiv(Sparse, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(Sparse, IdentityComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotdiv(SparseComplex, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotdiv(SparseComplex, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotdiv(SparseComplex, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(SparseComplex, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(SparseComplex, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotdiv(SparseComplex, Empty, M_E, Sparse, Double, Double);
    scilab_fill_dotdiv(SparseComplex, Identity, M_M, Sparse, Double, Sparse);
    scilab_fill_dotdiv(SparseComplex, IdentityComplex, M_M, Sparse, Double, Sparse);

#undef scilab_fill_dotdiv

}

InternalType *GenericDotLDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    dotdiv_function dotdiv = pDotDivfunction[_pRightOperand->getId()][_pLeftOperand->getId()];
    if (dotdiv)
    {
        pResult = dotdiv(_pRightOperand, _pLeftOperand);
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

InternalType *GenericDotRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    dotdiv_function dotdiv = pDotDivfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (dotdiv)
    {
        ConfigVariable::setDivideByZero(false);
        pResult = dotdiv(_pLeftOperand, _pRightOperand);

        if (pResult)
        {
            bool iszero = ConfigVariable::isDivideByZero();
            ConfigVariable::setDivideByZero(false);

            if (iszero)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    pResult->killMe();
                    throw ast::InternalError(_("Division by zero...\n"));
                }

                if (ConfigVariable::getIeee() == 1)
                {
                    sciprint(_("Warning : division by zero...\n"));
                }
            }

            return pResult;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}




//Matrix ./ x
template<class T, class U, class O>
InternalType* dotdiv_M_M(T *_pL, U *_pR)
{
    //check dims
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
    int iSize = pOut->getSize();

    dotdiv(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_M_MC(T *_pL, U *_pR)
{
    //check dims
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
    int iSize = pOut->getSize();

    dotdiv(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    dotdiv(_pL->get(), (size_t)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotdiv(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_M_I(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_M_IC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_M_E(T *_pL, U *_pR)
{
    return _pR;
}

template<class T, class U, class O>
InternalType* dotdiv_E_M(T *_pL, U *_pR)
{
    return _pL;
}

//Matrix complex ./ x
template<class T, class U, class O>
InternalType* dotdiv_MC_M(T *_pL, U *_pR)
{
    //check dims
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
    int iSize = pOut->getSize();

    dotdiv(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_MC_MC(T *_pL, U *_pR)
{
    //check dims
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
    int iSize = pOut->getSize();

    dotdiv(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotdiv(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotdiv(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_MC_I(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_MC_IC(T *_pL, U *_pR)
{
    return NULL;
}

//Scalar ./ x
template<class T, class U, class O>
InternalType* dotdiv_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    dotdiv(_pL->get(0), (size_t)pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_S_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    dotdiv(_pL->get(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    dotdiv(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotdiv(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_S_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotdiv(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_S_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotdiv(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}
//Scalar complex ./ x
template<class T, class U, class O>
InternalType* dotdiv_SC_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    dotdiv(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_SC_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray(), true);
    dotdiv(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_SC_S(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotdiv(_pL->get(0), _pL->getImg(0), 1 , _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_SC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    pOut->setComplex(true);
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_SC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity ./ x
template<class T, class U, class O>
InternalType* dotdiv_I_M(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_I_MC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_I_S(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_I_SC(T *_pL, U *_pR)
{
    return dotdiv_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotdiv_I_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_I_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotdiv(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity complex ./ x
template<class T, class U, class O>
InternalType* dotdiv_IC_M(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_IC_MC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotdiv_IC_S(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_IC_SC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_IC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotdiv_IC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotdiv(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}


template<>
InternalType* dotdiv_M_M<Sparse, Sparse, Sparse>(Sparse* _pL, Sparse* _pR)
{
    //check dims
    if (_pL->isScalar())
    {
        Sparse* pOut = NULL;
        std::complex<double> c(_pL->getImg(0, 0));
        Double* pL = NULL;
        if (c.imag())
        {
            pL = new Double(c.real(), c.imag());
        }
        else
        {
            pL = new Double(c.real());
        }
        pOut = (Sparse*)dotdiv_M_M<Double, Sparse, Sparse>(pL, _pR);
        delete pL;
        return pOut;
    }

    if (_pR->isScalar())
    {
        Sparse* pOut = NULL;
        std::complex<double> c(_pR->getImg(0, 0));
        Double* pR = NULL;
        if (c.imag())
        {
            pR = new Double(c.real(), c.imag());
        }
        else
        {
            pR = new Double(c.real());
        }
        pOut = (Sparse*)dotdiv_M_M<Sparse, Double, Sparse>(_pL, pR);
        delete pR;
        return pOut;
    }

    //check dimensions
    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    Sparse* pSparseOut = _pL->dotDivide(*_pR);
    pSparseOut->finalize();
    return pSparseOut;
}

//[] ./ SP
template<>
InternalType* dotdiv_M_M<Double, Sparse, Double>(Double* _pL, Sparse* _pR)
{
    return _pL;
}

//SP ./ []
template<>
InternalType* dotdiv_M_M<Sparse, Double, Double>(Sparse* _pL, Double* _pR)
{
    return _pR;
}
//
template<>
InternalType* dotdiv_M_M<Double, Sparse, Sparse>(Double* _pL, Sparse* _pR)
{
    //D -> SP / SP
    if (_pL->isScalar())
    {
        Sparse* pTemp = NULL;
        int iSizeOut = _pR->getSize();
        if (_pL->isComplex())
        {
            pTemp = new Sparse(_pR->getRows(), _pR->getCols(), true);
            std::complex<double> stComplex(_pL->get(0), _pL->getImg(0));
            for (int i = 0 ; i < iSizeOut ; i++)
            {
                if (_pR->get(i) != 0)
                {
                    pTemp->set(i, stComplex);
                }
            }
        }
        else
        {
            pTemp = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0 ; i < iSizeOut ; i++)
            {
                if (_pR->get(i) != 0)
                {
                    pTemp->set(i, _pL->get(0), false);
                }
            }
        }

        pTemp->finalize();
        Sparse* pOut = pTemp->dotDivide(*_pR);
        delete pTemp;
        return pOut;
    }

    if (_pR->isScalar())
    {
        //D / sp -> D ./ d
        Double* pD = NULL;

        if (_pR->isComplex())
        {
            std::complex<double> dbl(_pR->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pR->get(0, 0));
        }

        InternalType* pIT = GenericDotRDivide(_pL, pD);
        delete pD;
        return pIT;
    }

    if (_pL->isIdentity())
    {
        Sparse* pSPTemp = new Sparse(_pR->getRows(), _pR->getCols(), _pL->isComplex());
        int size = std::min(_pR->getRows(), _pR->getCols());
        double dblLeftR = _pL->get(0);
        if (_pL->isComplex())
        {
            std::complex<double> complexLeft(dblLeftR, _pL->getImg(0));
            for (int i = 0; i < size; i++)
            {
                pSPTemp->set(i, i, complexLeft, false);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pSPTemp->set(i, i, dblLeftR, false);
            }
        }
        pSPTemp->finalize();
        InternalType* pIT = GenericDotRDivide(pSPTemp, _pR);
        delete pSPTemp;
        return pIT;
    }


    //check dimensions
    if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        return nullptr;
    }

    //get some information
    int iNonZeros = static_cast<int>(_pR->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pR->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pR->outputValues(pValR, pValI);
    double* pdblR = _pL->get();
    int iRows = _pL->getRows();

    Sparse* pOut = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex() || _pR->isComplex());

    if (_pL->isComplex() == false)
    {
        if (_pR->isComplex() == false)
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                pOut->set(iRow, iCol,  pdblR[index] / pValR[i], false);
            }
        }
        else
        {
            double dDenum = 0;
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                dDenum = ( pValR[index] * pValR[index] + pValI[index] * pValI[index]);
                c.real((pdblR[index] * pValR[i]) / dDenum);
                c.imag(-(pdblR[index] * pValI[i]) / dDenum);
                pOut->set(iRow, iCol,  c, false);
            }
        }
    }
    else
    {
        double* pdblI = _pL->getImg();
        if (_pR->isComplex() == false)
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                c.real(pdblR[index] / pValR[i]);
                c.imag(pdblI[index] / pValR[i]);
                pOut->set(iRow, iCol,  c, false);
            }
        }
        else
        {
            double dDenum = 0;
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                dDenum = ( pValR[index] * pValR[index] + pValI[index] * pValI[index]);
                c.real((pdblR[index] * pValR[i] + pdblI[index] * pValI[i]) / dDenum);
                c.imag((pdblI[index] * pValR[i] - pdblR[index] * pValI[i]) / dDenum);
                pOut->set(iRow, iCol,  c, false);
            }
        }
    }

    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    pOut->finalize();
    return pOut;
}

template<>
InternalType* dotdiv_M_M<Sparse, Double, Sparse>(Sparse* _pL, Double* _pR)
{

    if (_pR->isScalar())
    {
        // SP / d-> SP
        Sparse* pTemp = NULL;
        int iSizeOut = _pL->getSize();
        if (_pR->isComplex())
        {
            pTemp = new Sparse(_pL->getRows(), _pL->getCols(), true);
            std::complex<double> stComplex(_pR->get(0), _pR->getImg(0));
            for (int i = 0 ; i < iSizeOut ; i++)
            {
                if (_pL->get(i) != 0)
                {
                    pTemp->set(i, stComplex, false);
                }
            }
        }
        else
        {
            pTemp = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex());
            std::complex<double> stComplex(_pR->get(0), _pR->getImg(0));
            for (int i = 0 ; i < iSizeOut ; i++)
            {
                if (_pL->get(i) != 0)
                {
                    pTemp->set(i, _pR->get(0), false);
                }
            }
        }

        pTemp->finalize();
        Sparse* pOut = _pL->dotDivide(*pTemp);
        delete pTemp;
        return pOut;
    }

    if (_pL->isScalar())
    {
        //D / sp -> D ./ d
        Double* pD = NULL;

        if (_pL->isComplex())
        {
            std::complex<double> dbl(_pL->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pL->get(0, 0));
        }

        InternalType* pIT = GenericDotLDivide(_pR, pD);
        delete pD;
        return pIT;
    }

    if (_pR->isIdentity())
    {
        Sparse* pSPTemp = new Sparse(_pL->getRows(), _pL->getCols(), _pR->isComplex());
        int size = std::min(_pL->getRows(), _pL->getCols());
        double dblRightR = _pR->get(0);
        if (_pR->isComplex())
        {
            std::complex<double> complexRight(dblRightR, _pR->getImg(0));
            for (int i = 0; i < size; i++)
            {
                pSPTemp->set(i, i, complexRight, false);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pSPTemp->set(i, i, dblRightR, false);
            }
        }
        pSPTemp->finalize();
        InternalType* pIT = GenericDotRDivide(_pL, pSPTemp);
        delete pSPTemp;
        return pIT;
    }

    //check dimensions
    if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
    {
        return nullptr;
    }

    //get some information
    int iNonZeros = static_cast<int>(_pL->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pL->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pL->outputValues(pValR, pValI);
    double* pdblR = _pR->get();
    int iRows = _pR->getRows();

    Sparse* pOut = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex() || _pL->isComplex());

    if (_pR->isComplex() == false)
    {
        if (_pL->isComplex() == false)
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                pOut->set(iRow, iCol, pValR[i] / pdblR[index], false);
            }
        }
        else
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                c.real(pValR[i] / pdblR[index]);
                c.imag(pValR[i] / pdblR[index]);
                pOut->set(iRow, iCol, c, false);
            }
        }
    }
    else
    {
        double* pdblI = _pR->getImg();
        double dDenum = 0;
        if (_pL->isComplex() == false)
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                dDenum = ( pdblR[index] * pdblR[index] + pdblI[index] * pdblI[index]);
                c.real((pValR[i]*pdblR[index]) / dDenum );
                c.imag(-(pdblI[index]*pValR[i]) / dDenum );
                pOut->set(iRow, iCol,  c, false);
            }
        }
        else
        {
            for (int i = 0 ; i < iNonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                int index = iCol * iRows + iRow;

                std::complex<double> c;
                dDenum = ( pdblR[index] * pdblR[index] + pdblI[index] * pdblI[index]);
                c.real((pdblR[index] * pValR[i] + pdblI[index] * pValI[i]) / dDenum);
                c.imag((pdblR[index] * pValI[i] - pdblI[index] * pValR[i]) / dDenum);
                pOut->set(iRow, iCol,  c, false);
            }
        }
    }

    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    pOut->finalize();
    return pOut;
}

//Polynom
template<>
InternalType* dotdiv_M_M<Polynom, Polynom, Polynom>(Polynom* _pL, Polynom* _pR)
{
    return NULL;
}

template<>
InternalType* dotdiv_M_M<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    Polynom* pOut = NULL;

    bool isComplexL = _pR->isComplex();
    bool isComplexR = _pL->isComplex();
    bool isComplexOut = isComplexL || isComplexR;

    if (_pR->isScalar())
    {
        pOut = (Polynom*)_pL->clone();
        SinglePoly** pSPR = _pL->get();
        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();

        double dblR = _pR->get(0);
        if (isComplexL)
        {
            double dblI = _pR->getImg(0);
            pOut->setComplex(true);
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotdiv(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotdiv(pSP[i]->get(), (size_t)pSP[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
        }
        else
        {
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotdiv(pSP[i]->get(), pSP[i]->getImg(), (size_t)pSP[i]->getSize(), dblR, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                //r ./ P
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotdiv(pSP[i]->get(), (size_t)pSP[i]->getSize(), dblR, pSP[i]->get());
                }
            }
        }

        return pOut;
    }

    if (_pL->isScalar())
    {
        pOut = new Polynom(_pL->getVariableName(), _pR->getDims(), _pR->getDimsArray());
        SinglePoly* pSPL = _pL->get(0);
        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();
        double* pdblLR = _pR->get();
        double* pdblLI = NULL;
        if (isComplexL)
        {
            pdblLI  = _pR->getImg();
        }

        double* pdblRR = pSPL->get();
        double* pdblRI = NULL;
        if (isComplexR)
        {
            pdblRI = pSPL->getImg();
        }

        if (isComplexL)
        {
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; ++i)
                {
                    SinglePoly* pSPOut = (SinglePoly*)pSPL->clone();
                    int iSPSize = pSPOut->getSize();
                    pSPOut->setComplex(isComplexOut);

                    double* pdblOutR = pSPOut->get();
                    double* pdblOutI = pSPOut->getImg();

                    dotdiv(pdblRR, pdblRI, (size_t)iSPSize, pdblLR[i], pdblLI[i], pdblOutR, pdblOutI);
                    pSP[i] = pSPOut;
                }
            }
            else
            {
                for (int i = 0 ; i < iSize ; ++i)
                {
                    SinglePoly* pSPOut = (SinglePoly*)pSPL->clone();
                    int iSPSize = pSPOut->getSize();
                    pSPOut->setComplex(isComplexOut);

                    double* pdblOutR = pSPOut->get();
                    double* pdblOutI = pSPOut->getImg();
                    dotdiv(pdblRR, (size_t)iSPSize, pdblLR[i], pdblLI[i], pdblOutR, pdblOutI);
                    pSP[i] = pSPOut;
                }
            }
        }
        else
        {
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; ++i)
                {
                    SinglePoly* pSPOut = (SinglePoly*)pSPL->clone();
                    int iSPSize = pSPOut->getSize();
                    pSPOut->setComplex(isComplexOut);

                    double* pdblOutR = pSPOut->get();
                    double* pdblOutI = pSPOut->getImg();
                    dotdiv(pdblRR, pdblRI, (size_t)iSPSize, pdblLR[i], pdblOutR, pdblOutI);
                    pSP[i] = pSPOut;
                }
            }
            else
            {
                for (int i = 0 ; i < iSize ; ++i)
                {
                    SinglePoly* pSPOut = (SinglePoly*)pSPL->clone();
                    int iSPSize = pSPOut->getSize();
                    pSPOut->setComplex(isComplexOut);

                    double* pdblOutR = pSPOut->get();
                    dotdiv(pdblRR, (size_t)iSPSize, pdblLR[i], pdblOutR);
                    pSP[i] = pSPOut;
                }
            }
        }

        return pOut;

    }

    //check dims
    int iDimsL = _pR->getDims();
    int iDimsR = _pL->getDims();

    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    int* piDimsL = _pR->getDimsArray();
    int* piDimsR = _pL->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }


    pOut = (Polynom*)_pL->clone();
    pOut->setComplex(isComplexOut);
    SinglePoly** pSPR = _pL->get();
    SinglePoly** pSP = pOut->get();
    int iSize = pOut->getSize();

    double* pdblR = _pR->get();
    double* pdblI = NULL;
    if (isComplexL)
    {
        pdblI = _pR->getImg();
    }

    if (isComplexL)
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotdiv(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), pdblR[i], pdblI[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
        else
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotdiv(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), pdblR[i], pdblI[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
    }
    else
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotdiv(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), pdblR[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
        else
        {
            //r ./ P
            for (int i = 0 ; i < iSize ; i++)
            {
                dotdiv(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), pdblR[i], pSP[i]->get());
            }
        }
    }

    return pOut;
}

template<>
InternalType* dotdiv_M_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    return NULL;
}
