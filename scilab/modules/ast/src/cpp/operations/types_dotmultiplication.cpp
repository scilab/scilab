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

#include "types_dotmultiplication.hxx"
#include "double.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "polynom.hxx"


extern "C"
{
#include "localization.h"
}


using namespace types;
//define arrays on operation functions
static dotmul_function pDotMulfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillDotMulFunction()
{
#define scilab_fill_dotmul(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pDotMulfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (dotmul_function)&dotmul_##func<typeIn1, typeIn2, typeOut>

    //Double
    //Matrix .* Matrix
    scilab_fill_dotmul(Double, Double, M_M, Double, Double, Double);
    scilab_fill_dotmul(Double, Int8, M_M, Double, Int8, Int8);
    scilab_fill_dotmul(Double, UInt8, M_M, Double, UInt8, UInt8);
    scilab_fill_dotmul(Double, Int16, M_M, Double, Int16, Int16);
    scilab_fill_dotmul(Double, UInt16, M_M, Double, UInt16, UInt16);
    scilab_fill_dotmul(Double, Int32, M_M, Double, Int32, Int32);
    scilab_fill_dotmul(Double, UInt32, M_M, Double, UInt32, UInt32);
    scilab_fill_dotmul(Double, Int64, M_M, Double, Int64, Int64);
    scilab_fill_dotmul(Double, UInt64, M_M, Double, UInt64, UInt64);
    scilab_fill_dotmul(Double, Bool, M_M, Double, Bool, Double);
    scilab_fill_dotmul(Double, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Double, Sparse, M_M, Double, Sparse, Sparse);

    //Matrix .* Matrix Complex
    scilab_fill_dotmul(Double, DoubleComplex, M_MC, Double, Double, Double);
    scilab_fill_dotmul(Double, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Double, SparseComplex, M_M, Double, Sparse, Sparse);

    //Matrix .* Scalar
    scilab_fill_dotmul(Double, ScalarDouble, M_S, Double, Double, Double);
    scilab_fill_dotmul(Double, ScalarInt8, M_S, Double, Int8, Int8);
    scilab_fill_dotmul(Double, ScalarUInt8, M_S, Double, UInt8, UInt8);
    scilab_fill_dotmul(Double, ScalarInt16, M_S, Double, Int16, Int16);
    scilab_fill_dotmul(Double, ScalarUInt16, M_S, Double, UInt16, UInt16);
    scilab_fill_dotmul(Double, ScalarInt32, M_S, Double, Int32, Int32);
    scilab_fill_dotmul(Double, ScalarUInt32, M_S, Double, UInt32, UInt32);
    scilab_fill_dotmul(Double, ScalarInt64, M_S, Double, Int64, Int64);
    scilab_fill_dotmul(Double, ScalarUInt64, M_S, Double, UInt64, UInt64);
    scilab_fill_dotmul(Double, ScalarBool, M_S, Double, Bool, Double);
    scilab_fill_dotmul(Double, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Matrix .* Scalar Complex
    scilab_fill_dotmul(Double, ScalarDoubleComplex, M_SC, Double, Double, Double);
    scilab_fill_dotmul(Double, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Matrix .* Empty
    scilab_fill_dotmul(Double, Empty, M_E, Double, Double, Double);


    //Matrix Complex .* Matrix
    scilab_fill_dotmul(DoubleComplex, Double, MC_M, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, ScalarDouble, MC_S, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Empty, M_E, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_dotmul(DoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarDouble, Double, S_M, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, Int8, S_M, Double, Int8, Int8);
    scilab_fill_dotmul(ScalarDouble, UInt8, S_M, Double, UInt8, UInt8);
    scilab_fill_dotmul(ScalarDouble, Int16, S_M, Double, Int16, Int16);
    scilab_fill_dotmul(ScalarDouble, UInt16, S_M, Double, UInt16, UInt16);
    scilab_fill_dotmul(ScalarDouble, Int32, S_M, Double, Int32, Int32);
    scilab_fill_dotmul(ScalarDouble, UInt32, S_M, Double, UInt32, UInt32);
    scilab_fill_dotmul(ScalarDouble, Int64, S_M, Double, Int64, Int64);
    scilab_fill_dotmul(ScalarDouble, UInt64, S_M, Double, UInt64, UInt64);
    scilab_fill_dotmul(ScalarDouble, Bool, S_M, Double, Bool, Double);
    scilab_fill_dotmul(ScalarDouble, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, Sparse, M_M, Double, Sparse, Sparse);

    //Scalar .* Matrix Complex
    scilab_fill_dotmul(ScalarDouble, DoubleComplex, S_MC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarDouble, ScalarDouble, S_S, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, ScalarInt8, S_S, Double, Int8, Int8);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, UInt8);
    scilab_fill_dotmul(ScalarDouble, ScalarInt16, S_S, Double, Int16, Int16);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, UInt16);
    scilab_fill_dotmul(ScalarDouble, ScalarInt32, S_S, Double, Int32, Int32);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, UInt32);
    scilab_fill_dotmul(ScalarDouble, ScalarInt64, S_S, Double, Int64, Int64);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, UInt64);
    scilab_fill_dotmul(ScalarDouble, ScalarBool, S_S, Double, Bool, Double);
    scilab_fill_dotmul(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Scalar .* Scalar Complex
    scilab_fill_dotmul(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);

    //Scalar .* Empty
    scilab_fill_dotmul(ScalarDouble, Empty, M_E, Double, Double, Double);

    //Scalar Complex .* Matrix
    scilab_fill_dotmul(ScalarDoubleComplex, Double, SC_M, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    //Scalar Complex .* Matrix Complex
    scilab_fill_dotmul(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);
    //Scalar Complex .* Scalar
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    //Scalar Complex .* Scalar Complex
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Scalar Complex .* Empty
    scilab_fill_dotmul(ScalarDoubleComplex, Empty, M_E, Double, Double, Double);

    //Empty .* Matrix
    scilab_fill_dotmul(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, Int8, E_M, Double, Int8, Double);
    scilab_fill_dotmul(Empty, UInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotmul(Empty, Int16, E_M, Double, Int16, Double);
    scilab_fill_dotmul(Empty, UInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotmul(Empty, Int32, E_M, Double, Int32, Double);
    scilab_fill_dotmul(Empty, UInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotmul(Empty, Int64, E_M, Double, Int64, Double);
    scilab_fill_dotmul(Empty, UInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotmul(Empty, Polynom, E_M, Double, Polynom, Double);
    scilab_fill_dotmul(Empty, PolynomComplex, E_M, Double, Polynom, Double);
    scilab_fill_dotmul(Empty, Sparse, E_M, Double, Sparse, Double);
    scilab_fill_dotmul(Empty, SparseComplex, E_M, Double, Sparse, Double);

    //Empty .* Matrix Complex
    scilab_fill_dotmul(Empty, DoubleComplex, E_M, Double, Double, Double);
    //Empty .* Scalar
    scilab_fill_dotmul(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, ScalarInt8, E_M, Double, Int8, Double);
    scilab_fill_dotmul(Empty, ScalarUInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotmul(Empty, ScalarInt16, E_M, Double, Int16, Double);
    scilab_fill_dotmul(Empty, ScalarUInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotmul(Empty, ScalarInt32, E_M, Double, Int32, Double);
    scilab_fill_dotmul(Empty, ScalarUInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotmul(Empty, ScalarInt64, E_M, Double, Int64, Double);
    scilab_fill_dotmul(Empty, ScalarUInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotmul(Empty, ScalarBool, E_M, Double, Bool, Double);
    scilab_fill_dotmul(Empty, ScalarPolynom, E_M, Double, Polynom, Double);

    //Empty .* Scalar Complex
    scilab_fill_dotmul(Empty, ScalarDoubleComplex, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Double);
    //Empty .* Empty
    scilab_fill_dotmul(Empty, Empty, E_M, Double, Double, Double);
    //Empty .* eye
    scilab_fill_dotmul(Empty, Identity, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, IdentityComplex, E_M, Double, Double, Double);

    //Matrix .* Identity
    scilab_fill_dotmul(Double, Identity, M_I, Double, Double, Double);
    scilab_fill_dotmul(Double, IdentityComplex, M_IC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Identity, MC_I, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, Identity, S_I, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, IdentityComplex, S_IC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, Identity, SC_I, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Double);

    //Int8
    //Matrix .* Matrix
    scilab_fill_dotmul(Int8, Double, M_M, Int8, Double, Int8);
    scilab_fill_dotmul(Int8, Int8, M_M, Int8, Int8, Int8);
    scilab_fill_dotmul(Int8, UInt8, M_M, Int8, UInt8, UInt8);
    scilab_fill_dotmul(Int8, Int16, M_M, Int8, Int16, Int16);
    scilab_fill_dotmul(Int8, UInt16, M_M, Int8, UInt16, UInt16);
    scilab_fill_dotmul(Int8, Int32, M_M, Int8, Int32, Int32);
    scilab_fill_dotmul(Int8, UInt32, M_M, Int8, UInt32, UInt32);
    scilab_fill_dotmul(Int8, Int64, M_M, Int8, Int64, Int64);
    scilab_fill_dotmul(Int8, UInt64, M_M, Int8, UInt64, UInt64);
    scilab_fill_dotmul(Int8, Bool, M_M, Int8, Bool, Int8);
    scilab_fill_dotmul(Int8, Empty, M_E, Int8, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int8, ScalarDouble, M_S, Int8, Double, Int8);
    scilab_fill_dotmul(Int8, ScalarInt8, M_S, Int8, Int8, Int8);
    scilab_fill_dotmul(Int8, ScalarUInt8, M_S, Int8, UInt8, UInt8);
    scilab_fill_dotmul(Int8, ScalarInt16, M_S, Int8, Int16, Int16);
    scilab_fill_dotmul(Int8, ScalarUInt16, M_S, Int8, UInt16, UInt16);
    scilab_fill_dotmul(Int8, ScalarInt32, M_S, Int8, Int32, Int32);
    scilab_fill_dotmul(Int8, ScalarUInt32, M_S, Int8, UInt32, UInt32);
    scilab_fill_dotmul(Int8, ScalarInt64, M_S, Int8, Int64, Int64);
    scilab_fill_dotmul(Int8, ScalarUInt64, M_S, Int8, UInt64, UInt64);
    scilab_fill_dotmul(Int8, ScalarBool, M_S, Int8, Bool, Int8);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt8, Double, S_M, Int8, Double, Int8);
    scilab_fill_dotmul(ScalarInt8, Int8, S_M, Int8, Int8, Int8);
    scilab_fill_dotmul(ScalarInt8, UInt8, S_M, Int8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarInt8, Int16, S_M, Int8, Int16, Int16);
    scilab_fill_dotmul(ScalarInt8, UInt16, S_M, Int8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt8, Int32, S_M, Int8, Int32, Int32);
    scilab_fill_dotmul(ScalarInt8, UInt32, S_M, Int8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt8, Int64, S_M, Int8, Int64, Int64);
    scilab_fill_dotmul(ScalarInt8, UInt64, S_M, Int8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt8, Bool, S_M, Int8, Bool, Int8);
    scilab_fill_dotmul(ScalarInt8, Empty, M_E, Int8, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt8, ScalarDouble, S_S, Int8, Double, Int8);
    scilab_fill_dotmul(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Int8);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Int16);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Int32);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Int64);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt8, ScalarBool, S_S, Int8, Bool, Int8);

    //UInt8
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt8, Double, M_M, UInt8, Double, UInt8);
    scilab_fill_dotmul(UInt8, Int8, M_M, UInt8, Int8, UInt8);
    scilab_fill_dotmul(UInt8, UInt8, M_M, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(UInt8, Int16, M_M, UInt8, Int16, UInt16);
    scilab_fill_dotmul(UInt8, UInt16, M_M, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(UInt8, Int32, M_M, UInt8, Int32, UInt32);
    scilab_fill_dotmul(UInt8, UInt32, M_M, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(UInt8, Int64, M_M, UInt8, Int64, UInt64);
    scilab_fill_dotmul(UInt8, UInt64, M_M, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(UInt8, Bool, M_M, UInt8, Bool, UInt8);
    scilab_fill_dotmul(UInt8, Empty, M_E, UInt8, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt8, ScalarDouble, M_S, UInt8, Double, UInt8);
    scilab_fill_dotmul(UInt8, ScalarInt8, M_S, UInt8, Int8, UInt8);
    scilab_fill_dotmul(UInt8, ScalarUInt8, M_S, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(UInt8, ScalarInt16, M_S, UInt8, Int16, UInt16);
    scilab_fill_dotmul(UInt8, ScalarUInt16, M_S, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(UInt8, ScalarInt32, M_S, UInt8, Int32, UInt32);
    scilab_fill_dotmul(UInt8, ScalarUInt32, M_S, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(UInt8, ScalarInt64, M_S, UInt8, Int64, UInt64);
    scilab_fill_dotmul(UInt8, ScalarUInt64, M_S, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(UInt8, ScalarBool, M_S, UInt8, Bool, UInt8);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt8, Double, S_M, UInt8, Double, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Int8, S_M, UInt8, Int8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, UInt8, S_M, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Int16, S_M, UInt8, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, UInt16, S_M, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, Int32, S_M, UInt8, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, UInt32, S_M, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, Int64, S_M, UInt8, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, UInt64, S_M, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, Bool, S_M, UInt8, Bool, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Empty, M_E, UInt8, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, UInt8);

    //Int16
    //Matrix .* Matrix
    scilab_fill_dotmul(Int16, Double, M_M, Int16, Double, Int16);
    scilab_fill_dotmul(Int16, Int8, M_M, Int16, Int8, Int16);
    scilab_fill_dotmul(Int16, UInt8, M_M, Int16, UInt8, UInt16);
    scilab_fill_dotmul(Int16, Int16, M_M, Int16, Int16, Int16);
    scilab_fill_dotmul(Int16, UInt16, M_M, Int16, UInt16, UInt16);
    scilab_fill_dotmul(Int16, Int32, M_M, Int16, Int32, Int32);
    scilab_fill_dotmul(Int16, UInt32, M_M, Int16, UInt32, UInt32);
    scilab_fill_dotmul(Int16, Int64, M_M, Int16, Int64, Int64);
    scilab_fill_dotmul(Int16, UInt64, M_M, Int16, UInt64, UInt64);
    scilab_fill_dotmul(Int16, Bool, M_M, Int16, Bool, Int16);
    scilab_fill_dotmul(Int16, Empty, M_E, Int16, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int16, ScalarDouble, M_S, Int16, Double, Int16);
    scilab_fill_dotmul(Int16, ScalarInt8, M_S, Int16, Int8, Int16);
    scilab_fill_dotmul(Int16, ScalarUInt8, M_S, Int16, UInt8, UInt16);
    scilab_fill_dotmul(Int16, ScalarInt16, M_S, Int16, Int16, Int16);
    scilab_fill_dotmul(Int16, ScalarUInt16, M_S, Int16, UInt16, UInt16);
    scilab_fill_dotmul(Int16, ScalarInt32, M_S, Int16, Int32, Int32);
    scilab_fill_dotmul(Int16, ScalarUInt32, M_S, Int16, UInt32, UInt32);
    scilab_fill_dotmul(Int16, ScalarInt64, M_S, Int16, Int64, Int64);
    scilab_fill_dotmul(Int16, ScalarUInt64, M_S, Int16, UInt64, UInt64);
    scilab_fill_dotmul(Int16, ScalarBool, M_E, Int16, Bool, Int16);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt16, Double, S_M, Int16, Double, Int16);
    scilab_fill_dotmul(ScalarInt16, Int8, S_M, Int16, Int8, Int16);
    scilab_fill_dotmul(ScalarInt16, UInt8, S_M, Int16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarInt16, Int16, S_M, Int16, Int16, Int16);
    scilab_fill_dotmul(ScalarInt16, UInt16, S_M, Int16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt16, Int32, S_M, Int16, Int32, Int32);
    scilab_fill_dotmul(ScalarInt16, UInt32, S_M, Int16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt16, Int64, S_M, Int16, Int64, Int64);
    scilab_fill_dotmul(ScalarInt16, UInt64, S_M, Int16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt16, Bool, S_M, Int16, Bool, Int16);
    scilab_fill_dotmul(ScalarInt16, Empty, M_E, Int16, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt16, ScalarDouble, S_S, Int16, Double, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Int32);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Int64);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt16, ScalarBool, S_S, Int16, Bool, Int16);

    //UInt16
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt16, Double, M_M, UInt16, Double, UInt16);
    scilab_fill_dotmul(UInt16, Int8, M_M, UInt16, Int8, UInt16);
    scilab_fill_dotmul(UInt16, UInt8, M_M, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(UInt16, Int16, M_M, UInt16, Int16, UInt16);
    scilab_fill_dotmul(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(UInt16, Int32, M_M, UInt16, Int32, UInt32);
    scilab_fill_dotmul(UInt16, UInt32, M_M, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(UInt16, Int64, M_M, UInt16, Int64, UInt64);
    scilab_fill_dotmul(UInt16, UInt64, M_M, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(UInt16, Bool, M_M, UInt16, Bool, UInt16);
    scilab_fill_dotmul(UInt16, Empty, M_E, UInt16, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt16, ScalarDouble, M_S, UInt16, Double, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt8, M_S, UInt16, Int8, UInt16);
    scilab_fill_dotmul(UInt16, ScalarUInt8, M_S, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt16, M_S, UInt16, Int16, UInt16);
    scilab_fill_dotmul(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt32, M_S, UInt16, Int32, UInt32);
    scilab_fill_dotmul(UInt16, ScalarUInt32, M_S, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(UInt16, ScalarInt64, M_S, UInt16, Int64, UInt64);
    scilab_fill_dotmul(UInt16, ScalarUInt64, M_S, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(UInt16, ScalarBool, M_S, UInt16, Bool, UInt16);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt16, Double, S_M, UInt16, Double, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int8, S_M, UInt16, Int8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, UInt8, S_M, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int16, S_M, UInt16, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int32, S_M, UInt16, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, UInt32, S_M, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, Int64, S_M, UInt16, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, UInt64, S_M, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, Bool, S_M, UInt16, Bool, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Empty, S_M, UInt16, Double, UInt16);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, UInt16);

    //Int32
    //Matrix .* Matrix
    scilab_fill_dotmul(Int32, Double, M_M, Int32, Double, Int32);
    scilab_fill_dotmul(Int32, Int8, M_M, Int32, Int8, Int32);
    scilab_fill_dotmul(Int32, UInt8, M_M, Int32, UInt8, UInt32);
    scilab_fill_dotmul(Int32, Int16, M_M, Int32, Int16, Int32);
    scilab_fill_dotmul(Int32, UInt16, M_M, Int32, UInt16, UInt32);
    scilab_fill_dotmul(Int32, Int32, M_M, Int32, Int32, Int32);
    scilab_fill_dotmul(Int32, UInt32, M_M, Int32, UInt32, UInt32);
    scilab_fill_dotmul(Int32, Int64, M_M, Int32, Int64, Int64);
    scilab_fill_dotmul(Int32, UInt64, M_M, Int32, UInt64, UInt64);
    scilab_fill_dotmul(Int32, Bool, M_M, Int32, Bool, Int32);
    scilab_fill_dotmul(Int32, Empty, M_E, Int32, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int32, ScalarDouble, M_S, Int32, Double, Int32);
    scilab_fill_dotmul(Int32, ScalarInt8, M_S, Int32, Int8, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt8, M_S, Int32, UInt8, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt16, M_S, Int32, Int16, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt16, M_S, Int32, UInt16, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt32, M_S, Int32, Int32, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt32, M_S, Int32, UInt32, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt64, M_S, Int32, Int64, Int64);
    scilab_fill_dotmul(Int32, ScalarUInt64, M_S, Int32, UInt64, UInt64);
    scilab_fill_dotmul(Int32, ScalarBool, M_S, Int32, Bool, Int32);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt32, Double, S_M, Int32, Double, Int32);
    scilab_fill_dotmul(ScalarInt32, Int8, S_M, Int32, Int8, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt8, S_M, Int32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int16, S_M, Int32, Int16, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt16, S_M, Int32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int32, S_M, Int32, Int32, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt32, S_M, Int32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int64, S_M, Int32, Int64, Int64);
    scilab_fill_dotmul(ScalarInt32, UInt64, S_M, Int32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt32, Bool, S_M, Int32, Bool, Int32);
    scilab_fill_dotmul(ScalarInt32, Empty, M_E, Int32, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt32, ScalarDouble, S_S, Int32, Double, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Int64);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt32, ScalarBool, S_S, Int32, Bool, Int32);

    //UInt32
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt32, Double, M_M, UInt32, Double, UInt32);
    scilab_fill_dotmul(UInt32, Int8, M_M, UInt32, Int8, UInt32);
    scilab_fill_dotmul(UInt32, UInt8, M_M, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(UInt32, Int16, M_M, UInt32, Int16, UInt32);
    scilab_fill_dotmul(UInt32, UInt16, M_M, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(UInt32, Int32, M_M, UInt32, Int32, UInt32);
    scilab_fill_dotmul(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(UInt32, Int64, M_M, UInt32, Int64, UInt64);
    scilab_fill_dotmul(UInt32, UInt64, M_M, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(UInt32, Bool, M_M, UInt32, Bool, UInt32);
    scilab_fill_dotmul(UInt32, Empty, M_E, UInt32, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt32, ScalarDouble, M_S, UInt32, Double, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt8, M_S, UInt32, Int8, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt8, M_S, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt16, M_S, UInt32, Int16, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt16, M_S, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt32, M_S, UInt32, Int32, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt64, M_S, UInt32, Int64, UInt64);
    scilab_fill_dotmul(UInt32, ScalarUInt64, M_S, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(UInt32, ScalarBool, M_S, UInt32, Bool, UInt32);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt32, Double, S_M, UInt32, Double, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int8, S_M, UInt32, Int8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt8, S_M, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int16, S_M, UInt32, Int16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt16, S_M, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int32, S_M, UInt32, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int64, S_M, UInt32, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, UInt64, S_M, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, Bool, S_M, UInt32, Bool, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Empty, S_M, UInt32, Double, UInt32);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, UInt32);

    //Int64
    //Matrix .* Matrix
    scilab_fill_dotmul(Int64, Double, M_M, Int64, Double, Int64);
    scilab_fill_dotmul(Int64, Int8, M_M, Int64, Int8, Int64);
    scilab_fill_dotmul(Int64, UInt8, M_M, Int64, UInt8, UInt64);
    scilab_fill_dotmul(Int64, Int16, M_M, Int64, Int16, Int64);
    scilab_fill_dotmul(Int64, UInt16, M_M, Int64, UInt16, UInt64);
    scilab_fill_dotmul(Int64, Int32, M_M, Int64, Int32, Int64);
    scilab_fill_dotmul(Int64, UInt32, M_M, Int64, UInt32, UInt64);
    scilab_fill_dotmul(Int64, Int64, M_M, Int64, Int64, Int64);
    scilab_fill_dotmul(Int64, UInt64, M_M, Int64, UInt64, UInt64);
    scilab_fill_dotmul(Int64, Bool, M_M, Int64, Bool, Int64);
    scilab_fill_dotmul(Int64, Empty, M_E, Int64, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int64, ScalarDouble, M_S, Int64, Double, Int64);
    scilab_fill_dotmul(Int64, ScalarInt8, M_S, Int64, Int8, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt8, M_S, Int64, UInt8, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt16, M_S, Int64, Int16, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt16, M_S, Int64, UInt16, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt32, M_S, Int64, Int32, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt32, M_S, Int64, UInt32, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt64, M_S, Int64, Int64, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt64, M_S, Int64, UInt64, UInt64);
    scilab_fill_dotmul(Int64, ScalarBool, M_S, Int64, Bool, Int64);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt64, Double, S_M, Int64, Double, Int64);
    scilab_fill_dotmul(ScalarInt64, Int8, S_M, Int64, Int8, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt8, S_M, Int64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int16, S_M, Int64, Int16, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt16, S_M, Int64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int32, S_M, Int64, Int32, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt32, S_M, Int64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int64, S_M, Int64, Int64, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt64, S_M, Int64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt64, Bool, S_M, Int64, Bool, Int64);
    scilab_fill_dotmul(ScalarInt64, Empty, M_E, Int64, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt64, ScalarDouble, S_S, Int64, Double, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarBool, S_S, Int64, Bool, Int64);

    //UInt64
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt64, Double, M_M, UInt64, Double, UInt64);
    scilab_fill_dotmul(UInt64, Int8, M_M, UInt64, Int8, UInt64);
    scilab_fill_dotmul(UInt64, UInt8, M_M, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(UInt64, Int16, M_M, UInt64, Int16, UInt64);
    scilab_fill_dotmul(UInt64, UInt16, M_M, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(UInt64, Int32, M_M, UInt64, Int32, UInt64);
    scilab_fill_dotmul(UInt64, UInt32, M_M, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(UInt64, Int64, M_M, UInt64, Int64, UInt64);
    scilab_fill_dotmul(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(UInt64, Bool, M_M, UInt64, Bool, UInt64);
    scilab_fill_dotmul(UInt64, Empty, M_E, UInt64, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt64, ScalarDouble, M_S, UInt64, Double, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt8, M_S, UInt64, Int8, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt8, M_S, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt16, M_S, UInt64, Int16, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt16, M_S, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt32, M_S, UInt64, Int32, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt32, M_S, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt64, M_S, UInt64, Int64, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(UInt64, ScalarBool, M_S, UInt64, Bool, UInt64);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt64, Double, S_M, UInt64, Double, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int8, S_M, UInt64, Int8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt8, S_M, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int16, S_M, UInt64, Int16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt16, S_M, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int32, S_M, UInt64, Int32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt32, S_M, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int64, S_M, UInt64, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Bool, S_M, UInt64, Bool, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Empty, S_M, UInt64, Double, UInt64);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, UInt64);

    //Bool
    //Matrix + Matrix
    scilab_fill_dotmul(Bool, Double, M_M, Bool, Double, Double);
    scilab_fill_dotmul(Bool, Int8, M_M, Bool, Int8, Int8);
    scilab_fill_dotmul(Bool, UInt8, M_M, Bool, UInt8, UInt8);
    scilab_fill_dotmul(Bool, Int16, M_M, Bool, Int16, Int16);
    scilab_fill_dotmul(Bool, UInt16, M_M, Bool, UInt16, UInt16);
    scilab_fill_dotmul(Bool, Int32, M_M, Bool, Int32, Int32);
    scilab_fill_dotmul(Bool, UInt32, M_M, Bool, UInt32, UInt32);
    scilab_fill_dotmul(Bool, Int64, M_M, Bool, Int64, Int64);
    scilab_fill_dotmul(Bool, UInt64, M_M, Bool, UInt64, UInt64);
    scilab_fill_dotmul(Bool, Bool, M_M, Bool, Bool, Double);
    scilab_fill_dotmul(Bool, Empty, M_E, Bool, Double, Double);

    //Matrix + Scalar
    scilab_fill_dotmul(Bool, ScalarDouble, M_S, Bool, Double, Double);
    scilab_fill_dotmul(Bool, ScalarInt8, M_S, Bool, Int8, Int8);
    scilab_fill_dotmul(Bool, ScalarUInt8, M_S, Bool, UInt8, UInt8);
    scilab_fill_dotmul(Bool, ScalarInt16, M_S, Bool, Int16, Int16);
    scilab_fill_dotmul(Bool, ScalarUInt16, M_S, Bool, UInt16, UInt16);
    scilab_fill_dotmul(Bool, ScalarInt32, M_S, Bool, Int32, Int32);
    scilab_fill_dotmul(Bool, ScalarUInt32, M_S, Bool, UInt32, UInt32);
    scilab_fill_dotmul(Bool, ScalarInt64, M_S, Bool, Int64, Int64);
    scilab_fill_dotmul(Bool, ScalarUInt64, M_S, Bool, UInt64, UInt64);
    scilab_fill_dotmul(Bool, ScalarBool, M_S, Bool, Bool, Double);

    //Scalar + Matrix
    scilab_fill_dotmul(ScalarBool, Double, S_M, Bool, Double, Double);
    scilab_fill_dotmul(ScalarBool, Int8, S_M, Bool, Int8, Int8);
    scilab_fill_dotmul(ScalarBool, UInt8, S_M, Bool, UInt8, UInt8);
    scilab_fill_dotmul(ScalarBool, Int16, S_M, Bool, Int16, Int16);
    scilab_fill_dotmul(ScalarBool, UInt16, S_M, Bool, UInt16, UInt16);
    scilab_fill_dotmul(ScalarBool, Int32, S_M, Bool, Int32, Int32);
    scilab_fill_dotmul(ScalarBool, UInt32, S_M, Bool, UInt32, UInt32);
    scilab_fill_dotmul(ScalarBool, Int64, S_M, Bool, Int64, Int64);
    scilab_fill_dotmul(ScalarBool, UInt64, S_M, Bool, UInt64, UInt64);
    scilab_fill_dotmul(ScalarBool, Bool, S_M, Bool, Bool, Double);
    scilab_fill_dotmul(ScalarBool, Empty, M_E, Bool, Double, Double);

    //Scalar + Scalar
    scilab_fill_dotmul(ScalarBool, ScalarDouble, S_S, Bool, Double, Double);
    scilab_fill_dotmul(ScalarBool, ScalarInt8, S_S, Bool, Int8, Int8);
    scilab_fill_dotmul(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, UInt8);
    scilab_fill_dotmul(ScalarBool, ScalarInt16, S_S, Bool, Int16, Int16);
    scilab_fill_dotmul(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, UInt16);
    scilab_fill_dotmul(ScalarBool, ScalarInt32, S_S, Bool, Int32, Int32);
    scilab_fill_dotmul(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, UInt32);
    scilab_fill_dotmul(ScalarBool, ScalarInt64, S_S, Bool, Int64, Int64);
    scilab_fill_dotmul(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, UInt64);
    scilab_fill_dotmul(ScalarBool, ScalarBool, S_S, Bool, Bool, Double);

    //Identity
    scilab_fill_dotmul(Identity, Double, I_M, Double, Double, Double);
    scilab_fill_dotmul(Identity, DoubleComplex, I_MC, Double, Double, Double);
    scilab_fill_dotmul(Identity, ScalarDouble, I_S, Double, Double, Double);
    scilab_fill_dotmul(Identity, ScalarDoubleComplex, I_SC, Double, Double, Double);
    scilab_fill_dotmul(Identity, Identity, I_I, Double, Double, Double);
    scilab_fill_dotmul(Identity, IdentityComplex, I_IC, Double, Double, Double);
    scilab_fill_dotmul(Identity, Empty, M_E, Double, Double, Double);

    scilab_fill_dotmul(Identity, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //scilab_fill_dotmul(Identity, Sparse, M_M, Double, Sparse, Sparse);
    //scilab_fill_dotmul(Identity, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_dotmul(IdentityComplex, Double, IC_M, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, ScalarDouble, IC_S, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, Identity, IC_I, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, Empty, M_E, Double, Double, Double);

    scilab_fill_dotmul(IdentityComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //scilab_fill_dotmul(IdentityComplex, Sparse, M_M, Double, Sparse, Sparse);
    //scilab_fill_dotmul(IdentityComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Polynom

    //poly + poly
    scilab_fill_dotmul(Polynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(Polynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + scalar poly
    scilab_fill_dotmul(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + double
    scilab_fill_dotmul(Polynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + scalar double
    scilab_fill_dotmul(Polynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + []
    scilab_fill_dotmul(Polynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotmul(PolynomComplex, Empty, M_E, Polynom, Double, Double);

    //poly + eye
    scilab_fill_dotmul(Polynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + poly
    scilab_fill_dotmul(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + scalar poly
    scilab_fill_dotmul(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + double
    scilab_fill_dotmul(ScalarPolynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + scalar double
    scilab_fill_dotmul(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + []
    scilab_fill_dotmul(ScalarPolynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotmul(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Double);

    //scalar poly + eye
    scilab_fill_dotmul(ScalarPolynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //Sparse
    scilab_fill_dotmul(Sparse, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(Sparse, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(Sparse, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(Sparse, Empty, M_E, Sparse, Double, Double);
    //scilab_fill_dotmul(Sparse, Identity, M_M, Sparse, Double, Sparse);
    //scilab_fill_dotmul(Sparse, IdentityComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(SparseComplex, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(SparseComplex, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(SparseComplex, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(SparseComplex, Empty, M_E, Sparse, Double, Double);
    //scilab_fill_dotmul(SparseComplex, Identity, M_M, Sparse, Double, Sparse);
    //scilab_fill_dotmul(SparseComplex, IdentityComplex, M_M, Sparse, Double, Sparse);

#undef scilab_fill_dotmul
}

InternalType *GenericDotTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    dotmul_function dotmul = pDotMulfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (dotmul)
    {
        pResult = dotmul(_pLeftOperand, _pRightOperand);
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

//Matrix .* x
template<class T, class U, class O>
InternalType* dotmul_M_M(T *_pL, U *_pR)
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

    dotmul(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_MC(T *_pL, U *_pR)
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

    dotmul(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    dotmul(_pL->get(), (size_t)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_I(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_M_IC(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_M_E(T * /*_pL*/, U *_pR)
{
    return _pR;
}

//Matrix complex .* x
template<class T, class U, class O>
InternalType* dotmul_MC_M(T *_pL, U *_pR)
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

    dotmul(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_MC(T *_pL, U *_pR)
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

    dotmul(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_I(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_MC_IC(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

//Scalar .* x
template<class T, class U, class O>
InternalType* dotmul_S_M(T *_pL, U *_pR)
{
    return dotmul_M_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_S_MC(T *_pL, U *_pR)
{
    return dotmul_MC_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotmul(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Scalar complex .* x
template<class T, class U, class O>
InternalType* dotmul_SC_M(T *_pL, U *_pR)
{
    return dotmul_M_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_MC(T *_pL, U *_pR)
{
    return dotmul_MC_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_S(T *_pL, U *_pR)
{
    return dotmul_S_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_SC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    pOut->setComplex(true);
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_SC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity .* x
template<class T, class U, class O>
InternalType* dotmul_I_M(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_I_MC(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_I_S(T *_pL, U *_pR)
{
    return dotmul_S_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_I_SC(T *_pL, U *_pR)
{
    return dotmul_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_I_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_I_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity complex .* x
template<class T, class U, class O>
InternalType* dotmul_IC_M(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_IC_MC(T * /*_pL*/, U * /*_pR*/)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_IC_S(T *_pL, U *_pR)
{
    return dotmul_S_IC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_IC_SC(T *_pL, U *_pR)
{
    return dotmul_SC_IC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_IC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_IC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Empty .* x
template<class T, class U, class O>
InternalType* dotmul_E_M(T *_pL, U * /*_pR*/)
{
    return _pL;
}

template<>
InternalType* dotmul_M_M<Sparse, Sparse, Sparse>(Sparse* _pL, Sparse* _pR)
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
        pOut = (Sparse*)dotmul_M_M<Double, Sparse, Sparse>(pL, _pR);
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
        pOut = (Sparse*)dotmul_M_M<Sparse, Double, Sparse>(_pL, pR);
        delete pR;
        return pOut;
    }

    //check dimensions
    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    return _pL->dotMultiply(*_pR);
}

//[] .* SP
template<>
InternalType* dotmul_M_M<Double, Sparse, Double>(Double* _pL, Sparse* /*_pR*/)
{
    return _pL;
}

//SP .* []
template<>
InternalType* dotmul_M_M<Sparse, Double, Double>(Sparse* /*_pL*/, Double* _pR)
{
    return _pR;
}

template<>
InternalType* dotmul_M_M<Double, Sparse, Sparse>(Double* _pL, Sparse* _pR)
{
    //D * SP
    if (_pL->isScalar())
    {
        //d * SP -> SP
        if (_pL->isComplex())
        {
            std::complex<double> dbl(_pL->get(0), _pL->getImg(0));
            return _pR->multiply(dbl);
        }
        else
        {
            return _pR->multiply(_pL->get(0));
        }
    }

    if (_pR->isScalar())
    {
        //D * sp -> D .* d
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

        InternalType* pIT = GenericDotTimes(_pL, pD);
        delete pD;
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

                pOut->set(iRow, iCol,  pdblR[index] * pValR[i], false);
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
                c.real(pdblR[index] * pValR[i]);
                c.imag(pdblR[index] * pValI[i]);
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
                c.real(pdblR[index] * pValR[i]);
                c.imag(pdblI[index] * pValR[i]);
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
                c.real(pdblR[index] * pValR[i] - pdblI[index] * pValI[i]);
                c.imag(pdblR[index] * pValI[i] + pdblI[index] * pValR[i]);
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
InternalType* dotmul_M_M<Sparse, Double, Sparse>(Sparse* _pL, Double* _pR)
{
    return dotmul_M_M<Double, Sparse, Sparse>(_pR, _pL);
}

//Polynom
template<>
InternalType* dotmul_M_M<Polynom, Polynom, Polynom>(Polynom* _pL, Polynom* _pR)
{
    //check varname
    if (_pL->getVariableName() != _pR->getVariableName())
    {
        //call overload
        return NULL;
    }

    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();
    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    if (_pL->isScalar())
    {
        Polynom* pOut = new Polynom(_pL->getVariableName(), iDimsR, piDimsR);
        int iSize = pOut->getSize();
        SinglePoly** pSPOut = pOut->get();
        SinglePoly* pSPL = _pL->get(0);
        SinglePoly** pSPR = _pR->get();

        for (int i = 0 ; i < iSize ; ++i)
        {
            pSPOut[i] = *pSPL **pSPR[i];
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        Polynom* pOut = new Polynom(_pL->getVariableName(), iDimsL, piDimsL);
        int iSize = pOut->getSize();
        SinglePoly** pSPOut = pOut->get();
        SinglePoly** pSPL = _pL->get();
        SinglePoly* pSPR = _pR->get(0);

        for (int i = 0 ; i < iSize ; ++i)
        {
            pSPOut[i] = *pSPL[i] **pSPR;
        }

        return pOut;
    }

    //check dims
    if (iDimsL != iDimsR)
    {
        return nullptr;
    }

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    Polynom* pOut = new Polynom(_pL->getVariableName(), iDimsL, piDimsR);
    int iSize = pOut->getSize();
    SinglePoly** pSPOut = pOut->get();
    SinglePoly** pSPL = _pL->get();
    SinglePoly** pSPR = _pR->get();

    for (int i = 0 ; i < iSize ; ++i)
    {
        pSPOut[i] = *pSPL[i] **pSPR[i];
    }

    return pOut;
}

template<>
InternalType* dotmul_M_M<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    return dotmul_M_M<Double, Polynom, Polynom>(_pR, _pL);
}

template<>
InternalType* dotmul_M_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = NULL;

    bool isComplexL = _pL->isComplex();
    bool isComplexR = _pR->isComplex();
    bool isComplexOut = isComplexL || isComplexR;

    if (_pL->isScalar())
    {
        pOut = (Polynom*)_pR->clone();
        SinglePoly** pSPR = _pR->get();
        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();

        double dblR = _pL->get(0);
        if (isComplexL)
        {
            double dblI = _pL->getImg(0);
            pOut->setComplex(true);
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
        }
        else
        {
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), dblR, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                //r .* P
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), dblR, pSP[i]->get());
                }
            }
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        pOut = new Polynom(_pR->getVariableName(), _pL->getDims(), _pL->getDimsArray());
        SinglePoly* pSPL = _pR->get(0);
        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();
        double* pdblLR = _pL->get();
        double* pdblLI = NULL;
        if (isComplexL)
        {
            pdblLI  = _pL->getImg();
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

                    dotmul(pdblRR, pdblRI, (size_t)iSPSize, pdblLR[i], pdblLI[i], pdblOutR, pdblOutI);
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
                    dotmul(pdblRR, (size_t)iSPSize, pdblLR[i], pdblLI[i], pdblOutR, pdblOutI);
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
                    dotmul(pdblRR, pdblRI, (size_t)iSPSize, pdblLR[i], pdblOutR, pdblOutI);
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
                    dotmul(pdblRR, (size_t)iSPSize, pdblLR[i], pdblOutR);
                    pSP[i] = pSPOut;
                }
            }
        }

        return pOut;

    }

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


    pOut = (Polynom*)_pR->clone();
    pOut->setComplex(isComplexOut);
    SinglePoly** pSPR = _pR->get();
    SinglePoly** pSP = pOut->get();
    int iSize = pOut->getSize();

    double* pdblR = _pL->get();
    double* pdblI = NULL;
    if (isComplexL)
    {
        pdblI = _pL->getImg();
    }

    if (isComplexL)
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotmul(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), pdblR[i], pdblI[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
        else
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotmul(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), pdblR[i], pdblI[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
    }
    else
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                dotmul(pSPR[i]->get(), pSPR[i]->getImg(), (size_t)pSPR[i]->getSize(), pdblR[i], pSP[i]->get(), pSP[i]->getImg());
            }
        }
        else
        {
            //r .* P
            for (int i = 0 ; i < iSize ; i++)
            {
                dotmul(pSPR[i]->get(), (size_t)pSPR[i]->getSize(), pdblR[i], pSP[i]->get());
            }
        }
    }

    return pOut;
}
