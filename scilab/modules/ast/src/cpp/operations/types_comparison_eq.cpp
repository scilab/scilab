/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *  Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
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

#include "types_comparison_eq.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "sparse.hxx"
#include "int.hxx"
#include "graphichandle.hxx"
#include "mlist.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "overload.hxx"
#include "user.hxx"
#include "opexp.hxx"

using namespace types;

//define arrays on operation functions
static compequal_function pComparisonEqualfunction[types::InternalType::IdLast][types::InternalType::IdLast] = { NULL };


void fillComparisonEqualFunction()
{
#define scilab_fill_comparisonequal(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pComparisonEqualfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (compequal_function)&compequal_##func<typeIn1, typeIn2, typeOut>

    //String == String
    scilab_fill_comparisonequal(String, String, M_M, String, String, Bool);
    scilab_fill_comparisonequal(String, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparisonequal(ScalarString, String, M_M, String, String, Bool);
    //String == Empty
    scilab_fill_comparisonequal(Empty, ScalarString, E_M, String, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Empty, M_E, String, String, Bool);
    scilab_fill_comparisonequal(String, Empty, M_E, String, String, Bool);
    scilab_fill_comparisonequal(Empty, String, E_M, String, String, Bool);
    //String == Double
    scilab_fill_comparisonequal(Double, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Double, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, Double, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(Double, String, M_E, Double, String, Bool);
    //String == ScalarDouble
    scilab_fill_comparisonequal(ScalarDouble, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, String, M_E, Double, String, Bool);
    //String == Int8
    scilab_fill_comparisonequal(Int8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparisonequal(String, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparisonequal(Int8, String, M_E, Int8, String, Bool);
    //String == UInt8
    scilab_fill_comparisonequal(UInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparisonequal(ScalarString, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparisonequal(String, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, String, M_E, UInt8, String, Bool);
    //String == Int16
    scilab_fill_comparisonequal(Int16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparisonequal(String, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparisonequal(Int16, String, M_E, Int16, String, Bool);
    //String == UInt16
    scilab_fill_comparisonequal(UInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparisonequal(ScalarString, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparisonequal(String, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, String, M_E, UInt16, String, Bool);
    //String == Int32
    scilab_fill_comparisonequal(Int32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparisonequal(String, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparisonequal(Int32, String, M_E, Int32, String, Bool);
    //String == UInt32
    scilab_fill_comparisonequal(UInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparisonequal(ScalarString, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparisonequal(String, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, String, M_E, UInt32, String, Bool);
    //String == Int64
    scilab_fill_comparisonequal(Int64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparisonequal(String, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparisonequal(Int64, String, M_E, Int64, String, Bool);
    //String == UInt64
    scilab_fill_comparisonequal(UInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparisonequal(ScalarString, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparisonequal(String, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, String, M_E, UInt64, String, Bool);
    //String == Bool
    scilab_fill_comparisonequal(Bool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparisonequal(String, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparisonequal(Bool, String, M_E, Bool, String, Bool);
    //String == Polynom
    scilab_fill_comparisonequal(Polynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(Polynom, String, M_E, Polynom, String, Bool);
    //String == PolynomComplex
    scilab_fill_comparisonequal(PolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(PolynomComplex, String, M_E, Polynom, String, Bool);
    //String == ScalarPolynom
    scilab_fill_comparisonequal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String == ScalarPolynomComplex
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String == Sparse
    scilab_fill_comparisonequal(Sparse, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparisonequal(String, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, String, M_E, Sparse, String, Bool);
    //String == SparseBool
    scilab_fill_comparisonequal(SparseBool, ScalarString, M_E, SparseBool, String, Bool);
    scilab_fill_comparisonequal(ScalarString, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparisonequal(String, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, String, M_E, SparseBool, String, Bool);
    //String == ScalarInt8
    scilab_fill_comparisonequal(ScalarInt8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparisonequal(String, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, String, M_E, Int8, String, Bool);
    //String == ScalarUInt8
    scilab_fill_comparisonequal(ScalarUInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparisonequal(String, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, String, M_E, UInt8, String, Bool);
    //String == ScalarInt16
    scilab_fill_comparisonequal(ScalarInt16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparisonequal(String, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, String, M_E, Int16, String, Bool);
    //String == ScalarUInt16
    scilab_fill_comparisonequal(ScalarUInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparisonequal(String, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, String, M_E, UInt16, String, Bool);
    //String == ScalarInt32
    scilab_fill_comparisonequal(ScalarInt32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparisonequal(String, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, String, M_E, Int32, String, Bool);
    //String == ScalarUInt32
    scilab_fill_comparisonequal(ScalarUInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparisonequal(String, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, String, M_E, UInt32, String, Bool);
    //String == ScalarInt64
    scilab_fill_comparisonequal(ScalarInt64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparisonequal(String, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, String, M_E, Int64, String, Bool);
    //String == ScalarUInt64
    scilab_fill_comparisonequal(ScalarUInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparisonequal(String, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, String, M_E, UInt64, String, Bool);
    //String == ScalarBool
    scilab_fill_comparisonequal(ScalarBool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparisonequal(String, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparisonequal(ScalarBool, String, M_E, Bool, String, Bool);
    //String == ScalarPolynom
    scilab_fill_comparisonequal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String == ScalarDoubleComplex
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, String, M_E, Double, String, Bool);
    //String == DoubleComplex
    scilab_fill_comparisonequal(DoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, String, M_E, Double, String, Bool);
    //String == ScalarPolynomComplex
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String == SparseComplex
    scilab_fill_comparisonequal(SparseComplex, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparisonequal(ScalarString, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparisonequal(String, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, String, M_E, Sparse, String, Bool);
    //String == Identity
    scilab_fill_comparisonequal(Identity, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Identity, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, Identity, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(Identity, String, M_E, Double, String, Bool);
    //String == IdentityComplex
    scilab_fill_comparisonequal(IdentityComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparisonequal(ScalarString, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(String, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, String, M_E, Double, String, Bool);

    //Double
    //Matrix == Matrix
    scilab_fill_comparisonequal(Double, Double, M_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Double, Int8, M_M, Double, Int8, Bool);
    scilab_fill_comparisonequal(Double, UInt8, M_M, Double, UInt8, Bool);
    scilab_fill_comparisonequal(Double, Int16, M_M, Double, Int16, Bool);
    scilab_fill_comparisonequal(Double, UInt16, M_M, Double, UInt16, Bool);
    scilab_fill_comparisonequal(Double, Int32, M_M, Double, Int32, Bool);
    scilab_fill_comparisonequal(Double, UInt32, M_M, Double, UInt32, Bool);
    scilab_fill_comparisonequal(Double, Int64, M_M, Double, Int64, Bool);
    scilab_fill_comparisonequal(Double, UInt64, M_M, Double, UInt64, Bool);
    scilab_fill_comparisonequal(Double, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(Double, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Double, Sparse, M_SP, Double, Sparse, SparseBool);

    //Matrix == Matrix Complex
    scilab_fill_comparisonequal(Double, DoubleComplex, M_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(Double, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Double, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Double, ScalarDouble, M_S, Double, Double, Bool);
    scilab_fill_comparisonequal(Double, ScalarInt8, M_S, Double, Int8, Bool);
    scilab_fill_comparisonequal(Double, ScalarUInt8, M_S, Double, UInt8, Bool);
    scilab_fill_comparisonequal(Double, ScalarInt16, M_S, Double, Int16, Bool);
    scilab_fill_comparisonequal(Double, ScalarUInt16, M_S, Double, UInt16, Bool);
    scilab_fill_comparisonequal(Double, ScalarInt32, M_S, Double, Int32, Bool);
    scilab_fill_comparisonequal(Double, ScalarUInt32, M_S, Double, UInt32, Bool);
    scilab_fill_comparisonequal(Double, ScalarInt64, M_S, Double, Int64, Bool);
    scilab_fill_comparisonequal(Double, ScalarUInt64, M_S, Double, UInt64, Bool);
    scilab_fill_comparisonequal(Double, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(Double, ScalarPolynom, M_P, Double, Polynom, Bool);

    //Matrix == Scalar Complex
    scilab_fill_comparisonequal(Double, ScalarDoubleComplex, M_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(Double, ScalarPolynomComplex, M_P, Double, Polynom, Bool);

    //Matrix == Empty
    scilab_fill_comparisonequal(Double, Empty, M_E, Double, Double, Bool);


    //Matrix Complex == Matrix
    scilab_fill_comparisonequal(DoubleComplex, Double, MC_M, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, ScalarDouble, MC_S, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Empty, M_E, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(DoubleComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(DoubleComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(DoubleComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Sparse, M_SP, Double, Sparse, SparseBool);
    scilab_fill_comparisonequal(DoubleComplex, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarDouble, Double, S_M, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Int8, S_M, Double, Int8, Bool);
    scilab_fill_comparisonequal(ScalarDouble, UInt8, S_M, Double, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Int16, S_M, Double, Int16, Bool);
    scilab_fill_comparisonequal(ScalarDouble, UInt16, S_M, Double, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Int32, S_M, Double, Int32, Bool);
    scilab_fill_comparisonequal(ScalarDouble, UInt32, S_M, Double, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Int64, S_M, Double, Int64, Bool);
    scilab_fill_comparisonequal(ScalarDouble, UInt64, S_M, Double, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Sparse, M_SP, Double, Sparse, SparseBool);

    //Scalar == Matrix Complex
    scilab_fill_comparisonequal(ScalarDouble, DoubleComplex, S_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarDouble, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarDouble, ScalarDouble, S_S, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarInt8, S_S, Double, Int8, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarInt16, S_S, Double, Int16, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarInt32, S_S, Double, Int32, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarInt64, S_S, Double, Int64, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarPolynom, M_P, Double, Polynom, Bool);

    //Scalar == Scalar Complex
    scilab_fill_comparisonequal(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarPolynomComplex, M_P, Double, Polynom, Bool);

    //Scalar == Empty
    scilab_fill_comparisonequal(ScalarDouble, Empty, M_E, Double, Double, Bool);

    //Scalar Complex == Matrix
    scilab_fill_comparisonequal(ScalarDoubleComplex, Double, SC_M, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Sparse, M_SP, Double, Sparse, SparseBool);
    //Scalar Complex == Matrix Complex
    scilab_fill_comparisonequal(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, SparseComplex, M_SP, Double, Sparse, SparseBool);
    //Scalar Complex == Scalar
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    //Scalar Complex == Scalar Complex
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    //Scalar Complex == Empty
    scilab_fill_comparisonequal(ScalarDoubleComplex, Empty, M_E, Double, Double, Bool);

    //Empty == Matrix
    scilab_fill_comparisonequal(Empty, Double, E_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Empty, Int8, E_M, Double, Int8, Bool);
    scilab_fill_comparisonequal(Empty, UInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparisonequal(Empty, Int16, E_M, Double, Int16, Bool);
    scilab_fill_comparisonequal(Empty, UInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparisonequal(Empty, Int32, E_M, Double, Int32, Bool);
    scilab_fill_comparisonequal(Empty, UInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparisonequal(Empty, Int64, E_M, Double, Int64, Bool);
    scilab_fill_comparisonequal(Empty, UInt64, E_M, Double, UInt64, Bool);

    scilab_fill_comparisonequal(Empty, Polynom, E_M, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Empty, PolynomComplex, E_M, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Empty, Sparse, E_M, Double, Sparse, Bool);
    scilab_fill_comparisonequal(Empty, SparseComplex, E_M, Double, Sparse, Bool);

    //Empty == Matrix Complex
    scilab_fill_comparisonequal(Empty, DoubleComplex, E_M, Double, Double, Bool);
    //Empty == Scalar
    scilab_fill_comparisonequal(Empty, ScalarDouble, E_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Empty, ScalarInt8, E_M, Double, Int8, Bool);
    scilab_fill_comparisonequal(Empty, ScalarUInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparisonequal(Empty, ScalarInt16, E_M, Double, Int16, Bool);
    scilab_fill_comparisonequal(Empty, ScalarUInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparisonequal(Empty, ScalarInt32, E_M, Double, Int32, Bool);
    scilab_fill_comparisonequal(Empty, ScalarUInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparisonequal(Empty, ScalarInt64, E_M, Double, Int64, Bool);
    scilab_fill_comparisonequal(Empty, ScalarUInt64, E_M, Double, UInt64, Bool);
    scilab_fill_comparisonequal(Empty, ScalarBool, E_M, Double, Bool, Bool);
    scilab_fill_comparisonequal(Empty, ScalarPolynom, E_M, Double, Polynom, Bool);

    //Empty == Scalar Complex
    scilab_fill_comparisonequal(Empty, ScalarDoubleComplex, E_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Bool);
    //Empty == Empty
    scilab_fill_comparisonequal(Empty, Empty, E_E, Double, Double, Bool);
    //Empty == eye
    scilab_fill_comparisonequal(Empty, Identity, E_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Empty, IdentityComplex, E_M, Double, Double, Bool);

    //Matrix == Identity
    scilab_fill_comparisonequal(Double, Identity, M_I, Double, Double, Bool);
    scilab_fill_comparisonequal(Double, IdentityComplex, M_IC, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Identity, MC_I, Double, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Identity, S_I, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, IdentityComplex, S_IC, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Identity, SC_I, Double, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Bool);

    //Int8
    //Matrix == Matrix
    scilab_fill_comparisonequal(Int8, Double, M_M, Int8, Double, Bool);
    scilab_fill_comparisonequal(Int8, Int8, M_M, Int8, Int8, Bool);
    scilab_fill_comparisonequal(Int8, UInt8, M_M, Int8, UInt8, Bool);
    scilab_fill_comparisonequal(Int8, Int16, M_M, Int8, Int16, Bool);
    scilab_fill_comparisonequal(Int8, UInt16, M_M, Int8, UInt16, Bool);
    scilab_fill_comparisonequal(Int8, Int32, M_M, Int8, Int32, Bool);
    scilab_fill_comparisonequal(Int8, UInt32, M_M, Int8, UInt32, Bool);
    scilab_fill_comparisonequal(Int8, Int64, M_M, Int8, Int64, Bool);
    scilab_fill_comparisonequal(Int8, UInt64, M_M, Int8, UInt64, Bool);
    scilab_fill_comparisonequal(Int8, Bool, M_B, Int8, Bool, Bool);
    scilab_fill_comparisonequal(Int8, Empty, M_E, Int8, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Int8, ScalarDouble, M_S, Int8, Double, Bool);
    scilab_fill_comparisonequal(Int8, ScalarInt8, M_S, Int8, Int8, Bool);
    scilab_fill_comparisonequal(Int8, ScalarUInt8, M_S, Int8, UInt8, Bool);
    scilab_fill_comparisonequal(Int8, ScalarInt16, M_S, Int8, Int16, Bool);
    scilab_fill_comparisonequal(Int8, ScalarUInt16, M_S, Int8, UInt16, Bool);
    scilab_fill_comparisonequal(Int8, ScalarInt32, M_S, Int8, Int32, Bool);
    scilab_fill_comparisonequal(Int8, ScalarUInt32, M_S, Int8, UInt32, Bool);
    scilab_fill_comparisonequal(Int8, ScalarInt64, M_S, Int8, Int64, Bool);
    scilab_fill_comparisonequal(Int8, ScalarUInt64, M_S, Int8, UInt64, Bool);
    scilab_fill_comparisonequal(Int8, ScalarBool, M_B, Int8, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarInt8, Double, S_M, Int8, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Int8, S_M, Int8, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, UInt8, S_M, Int8, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Int16, S_M, Int8, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt8, UInt16, S_M, Int8, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Int32, S_M, Int8, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt8, UInt32, S_M, Int8, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Int64, S_M, Int8, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt8, UInt64, S_M, Int8, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Bool, S_B, Int8, Bool, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Empty, M_E, Int8, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarInt8, ScalarDouble, S_S, Int8, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarBool, M_E, Int8, Bool, Bool);

    //UInt8
    //Matrix == Matrix
    scilab_fill_comparisonequal(UInt8, Double, M_M, UInt8, Double, Bool);
    scilab_fill_comparisonequal(UInt8, Int8, M_M, UInt8, Int8, Bool);
    scilab_fill_comparisonequal(UInt8, UInt8, M_M, UInt8, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, Int16, M_M, UInt8, Int16, Bool);
    scilab_fill_comparisonequal(UInt8, UInt16, M_M, UInt8, UInt16, Bool);
    scilab_fill_comparisonequal(UInt8, Int32, M_M, UInt8, Int32, Bool);
    scilab_fill_comparisonequal(UInt8, UInt32, M_M, UInt8, UInt32, Bool);
    scilab_fill_comparisonequal(UInt8, Int64, M_M, UInt8, Int64, Bool);
    scilab_fill_comparisonequal(UInt8, UInt64, M_M, UInt8, UInt64, Bool);
    scilab_fill_comparisonequal(UInt8, Bool, M_B, UInt8, Bool, Bool);
    scilab_fill_comparisonequal(UInt8, Empty, M_E, UInt8, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(UInt8, ScalarDouble, M_S, UInt8, Double, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarInt8, M_S, UInt8, Int8, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarUInt8, M_S, UInt8, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarInt16, M_S, UInt8, Int16, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarUInt16, M_S, UInt8, UInt16, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarInt32, M_S, UInt8, Int32, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarUInt32, M_S, UInt8, UInt32, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarInt64, M_S, UInt8, Int64, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarUInt64, M_S, UInt8, UInt64, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarBool, M_B, UInt8, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarUInt8, Double, S_M, UInt8, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Int8, S_M, UInt8, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, UInt8, S_M, UInt8, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Int16, S_M, UInt8, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, UInt16, S_M, UInt8, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Int32, S_M, UInt8, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, UInt32, S_M, UInt8, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Int64, S_M, UInt8, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, UInt64, S_M, UInt8, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Bool, S_B, UInt8, Bool, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Empty, M_E, UInt8, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarBool, M_E, UInt8, Bool, Bool);

    //Int16
    //Matrix == Matrix
    scilab_fill_comparisonequal(Int16, Double, M_M, Int16, Double, Bool);
    scilab_fill_comparisonequal(Int16, Int8, M_M, Int16, Int8, Bool);
    scilab_fill_comparisonequal(Int16, UInt8, M_M, Int16, UInt8, Bool);
    scilab_fill_comparisonequal(Int16, Int16, M_M, Int16, Int16, Bool);
    scilab_fill_comparisonequal(Int16, UInt16, M_M, Int16, UInt16, Bool);
    scilab_fill_comparisonequal(Int16, Int32, M_M, Int16, Int32, Bool);
    scilab_fill_comparisonequal(Int16, UInt32, M_M, Int16, UInt32, Bool);
    scilab_fill_comparisonequal(Int16, Int64, M_M, Int16, Int64, Bool);
    scilab_fill_comparisonequal(Int16, UInt64, M_M, Int16, UInt64, Bool);
    scilab_fill_comparisonequal(Int16, Bool, M_B, Int16, Bool, Bool);
    scilab_fill_comparisonequal(Int16, Empty, M_E, Int16, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Int16, ScalarDouble, M_S, Int16, Double, Bool);
    scilab_fill_comparisonequal(Int16, ScalarInt8, M_S, Int16, Int8, Bool);
    scilab_fill_comparisonequal(Int16, ScalarUInt8, M_S, Int16, UInt8, Bool);
    scilab_fill_comparisonequal(Int16, ScalarInt16, M_S, Int16, Int16, Bool);
    scilab_fill_comparisonequal(Int16, ScalarUInt16, M_S, Int16, UInt16, Bool);
    scilab_fill_comparisonequal(Int16, ScalarInt32, M_S, Int16, Int32, Bool);
    scilab_fill_comparisonequal(Int16, ScalarUInt32, M_S, Int16, UInt32, Bool);
    scilab_fill_comparisonequal(Int16, ScalarInt64, M_S, Int16, Int64, Bool);
    scilab_fill_comparisonequal(Int16, ScalarUInt64, M_S, Int16, UInt64, Bool);
    scilab_fill_comparisonequal(Int16, ScalarBool, M_B, Int16, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarInt16, Double, S_M, Int16, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Int8, S_M, Int16, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt16, UInt8, S_M, Int16, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Int16, S_M, Int16, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, UInt16, S_M, Int16, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Int32, S_M, Int16, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt16, UInt32, S_M, Int16, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Int64, S_M, Int16, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt16, UInt64, S_M, Int16, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Bool, S_B, Int16, Bool, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Empty, M_E, Int16, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarInt16, ScalarDouble, S_S, Int16, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarBool, M_E, Int16, Bool, Bool);

    //UInt16
    //Matrix == Matrix
    scilab_fill_comparisonequal(UInt16, Double, M_M, UInt16, Double, Bool);
    scilab_fill_comparisonequal(UInt16, Int8, M_M, UInt16, Int8, Bool);
    scilab_fill_comparisonequal(UInt16, UInt8, M_M, UInt16, UInt8, Bool);
    scilab_fill_comparisonequal(UInt16, Int16, M_M, UInt16, Int16, Bool);
    scilab_fill_comparisonequal(UInt16, UInt16, M_M, UInt16, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, Int32, M_M, UInt16, Int32, Bool);
    scilab_fill_comparisonequal(UInt16, UInt32, M_M, UInt16, UInt32, Bool);
    scilab_fill_comparisonequal(UInt16, Int64, M_M, UInt16, Int64, Bool);
    scilab_fill_comparisonequal(UInt16, UInt64, M_M, UInt16, UInt64, Bool);
    scilab_fill_comparisonequal(UInt16, Bool, M_B, UInt16, Bool, Bool);
    scilab_fill_comparisonequal(UInt16, Empty, M_E, UInt16, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(UInt16, ScalarDouble, M_S, UInt16, Double, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarInt8, M_S, UInt16, Int8, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarUInt8, M_S, UInt16, UInt8, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarInt16, M_S, UInt16, Int16, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarUInt16, M_S, UInt16, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarInt32, M_S, UInt16, Int32, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarUInt32, M_S, UInt16, UInt32, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarInt64, M_S, UInt16, Int64, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarUInt64, M_S, UInt16, UInt64, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarBool, M_B, UInt16, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarUInt16, Double, S_M, UInt16, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Int8, S_M, UInt16, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, UInt8, S_M, UInt16, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Int16, S_M, UInt16, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, UInt16, S_M, UInt16, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Int32, S_M, UInt16, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, UInt32, S_M, UInt16, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Int64, S_M, UInt16, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, UInt64, S_M, UInt16, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Bool, S_B, UInt16, Bool, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Empty, M_E, UInt16, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarBool, M_E, UInt16, Bool, Bool);

    //Int32
    //Matrix == Matrix
    scilab_fill_comparisonequal(Int32, Double, M_M, Int32, Double, Bool);
    scilab_fill_comparisonequal(Int32, Int8, M_M, Int32, Int8, Bool);
    scilab_fill_comparisonequal(Int32, UInt8, M_M, Int32, UInt8, Bool);
    scilab_fill_comparisonequal(Int32, Int16, M_M, Int32, Int16, Bool);
    scilab_fill_comparisonequal(Int32, UInt16, M_M, Int32, UInt16, Bool);
    scilab_fill_comparisonequal(Int32, Int32, M_M, Int32, Int32, Bool);
    scilab_fill_comparisonequal(Int32, UInt32, M_M, Int32, UInt32, Bool);
    scilab_fill_comparisonequal(Int32, Int64, M_M, Int32, Int64, Bool);
    scilab_fill_comparisonequal(Int32, UInt64, M_M, Int32, UInt64, Bool);
    scilab_fill_comparisonequal(Int32, Bool, M_B, Int32, Bool, Bool);
    scilab_fill_comparisonequal(Int32, Empty, M_E, Int32, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Int32, ScalarDouble, M_S, Int32, Double, Bool);
    scilab_fill_comparisonequal(Int32, ScalarInt8, M_S, Int32, Int8, Bool);
    scilab_fill_comparisonequal(Int32, ScalarUInt8, M_S, Int32, UInt8, Bool);
    scilab_fill_comparisonequal(Int32, ScalarInt16, M_S, Int32, Int16, Bool);
    scilab_fill_comparisonequal(Int32, ScalarUInt16, M_S, Int32, UInt16, Bool);
    scilab_fill_comparisonequal(Int32, ScalarInt32, M_S, Int32, Int32, Bool);
    scilab_fill_comparisonequal(Int32, ScalarUInt32, M_S, Int32, UInt32, Bool);
    scilab_fill_comparisonequal(Int32, ScalarInt64, M_S, Int32, Int64, Bool);
    scilab_fill_comparisonequal(Int32, ScalarUInt64, M_S, Int32, UInt64, Bool);
    scilab_fill_comparisonequal(Int32, ScalarBool, M_B, Int32, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarInt32, Double, S_M, Int32, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Int8, S_M, Int32, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt32, UInt8, S_M, Int32, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Int16, S_M, Int32, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt32, UInt16, S_M, Int32, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Int32, S_M, Int32, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, UInt32, S_M, Int32, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Int64, S_M, Int32, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt32, UInt64, S_M, Int32, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Bool, S_B, Int32, Bool, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Empty, M_E, Int32, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarInt32, ScalarDouble, S_S, Int32, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarBool, M_E, Int32, Bool, Bool);

    //UInt32
    //Matrix == Matrix
    scilab_fill_comparisonequal(UInt32, Double, M_M, UInt32, Double, Bool);
    scilab_fill_comparisonequal(UInt32, Int8, M_M, UInt32, Int8, Bool);
    scilab_fill_comparisonequal(UInt32, UInt8, M_M, UInt32, UInt8, Bool);
    scilab_fill_comparisonequal(UInt32, Int16, M_M, UInt32, Int16, Bool);
    scilab_fill_comparisonequal(UInt32, UInt16, M_M, UInt32, UInt16, Bool);
    scilab_fill_comparisonequal(UInt32, Int32, M_M, UInt32, Int32, Bool);
    scilab_fill_comparisonequal(UInt32, UInt32, M_M, UInt32, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, Int64, M_M, UInt32, Int64, Bool);
    scilab_fill_comparisonequal(UInt32, UInt64, M_M, UInt32, UInt64, Bool);
    scilab_fill_comparisonequal(UInt32, Bool, M_B, UInt32, Bool, Bool);
    scilab_fill_comparisonequal(UInt32, Empty, M_E, UInt32, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(UInt32, ScalarDouble, M_S, UInt32, Double, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarInt8, M_S, UInt32, Int8, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarUInt8, M_S, UInt32, UInt8, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarInt16, M_S, UInt32, Int16, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarUInt16, M_S, UInt32, UInt16, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarInt32, M_S, UInt32, Int32, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarUInt32, M_S, UInt32, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarInt64, M_S, UInt32, Int64, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarUInt64, M_S, UInt32, UInt64, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarBool, M_B, UInt32, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarUInt32, Double, S_M, UInt32, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Int8, S_M, UInt32, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, UInt8, S_M, UInt32, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Int16, S_M, UInt32, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, UInt16, S_M, UInt32, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Int32, S_M, UInt32, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, UInt32, S_M, UInt32, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Int64, S_M, UInt32, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, UInt64, S_M, UInt32, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Bool, S_B, UInt32, Bool, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Empty, M_E, UInt32, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarBool, M_E, UInt32, Bool, Bool);

    //Int64
    //Matrix == Matrix
    scilab_fill_comparisonequal(Int64, Double, M_M, Int64, Double, Bool);
    scilab_fill_comparisonequal(Int64, Int8, M_M, Int64, Int8, Bool);
    scilab_fill_comparisonequal(Int64, UInt8, M_M, Int64, UInt8, Bool);
    scilab_fill_comparisonequal(Int64, Int16, M_M, Int64, Int16, Bool);
    scilab_fill_comparisonequal(Int64, UInt16, M_M, Int64, UInt16, Bool);
    scilab_fill_comparisonequal(Int64, Int32, M_M, Int64, Int32, Bool);
    scilab_fill_comparisonequal(Int64, UInt32, M_M, Int64, UInt32, Bool);
    scilab_fill_comparisonequal(Int64, Int64, M_M, Int64, Int64, Bool);
    scilab_fill_comparisonequal(Int64, UInt64, M_M, Int64, UInt64, Bool);
    scilab_fill_comparisonequal(Int64, Bool, M_B, Int64, Bool, Bool);
    scilab_fill_comparisonequal(Int64, Empty, M_E, Int64, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Int64, ScalarDouble, M_S, Int64, Double, Bool);
    scilab_fill_comparisonequal(Int64, ScalarInt8, M_S, Int64, Int8, Bool);
    scilab_fill_comparisonequal(Int64, ScalarUInt8, M_S, Int64, UInt8, Bool);
    scilab_fill_comparisonequal(Int64, ScalarInt16, M_S, Int64, Int16, Bool);
    scilab_fill_comparisonequal(Int64, ScalarUInt16, M_S, Int64, UInt16, Bool);
    scilab_fill_comparisonequal(Int64, ScalarInt32, M_S, Int64, Int32, Bool);
    scilab_fill_comparisonequal(Int64, ScalarUInt32, M_S, Int64, UInt32, Bool);
    scilab_fill_comparisonequal(Int64, ScalarInt64, M_S, Int64, Int64, Bool);
    scilab_fill_comparisonequal(Int64, ScalarUInt64, M_S, Int64, UInt64, Bool);
    scilab_fill_comparisonequal(Int64, ScalarBool, M_B, Int64, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarInt64, Double, S_M, Int64, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Int8, S_M, Int64, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt64, UInt8, S_M, Int64, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Int16, S_M, Int64, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt64, UInt16, S_M, Int64, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Int32, S_M, Int64, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt64, UInt32, S_M, Int64, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Int64, S_M, Int64, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, UInt64, S_M, Int64, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Bool, S_B, Int64, Bool, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Empty, M_E, Int64, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarInt64, ScalarDouble, S_S, Int64, Double, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarBool, M_E, Int64, Bool, Bool);

    //UInt64
    //Matrix == Matrix
    scilab_fill_comparisonequal(UInt64, Double, M_M, UInt64, Double, Bool);
    scilab_fill_comparisonequal(UInt64, Int8, M_M, UInt64, Int8, Bool);
    scilab_fill_comparisonequal(UInt64, UInt8, M_M, UInt64, UInt8, Bool);
    scilab_fill_comparisonequal(UInt64, Int16, M_M, UInt64, Int16, Bool);
    scilab_fill_comparisonequal(UInt64, UInt16, M_M, UInt64, UInt16, Bool);
    scilab_fill_comparisonequal(UInt64, Int32, M_M, UInt64, Int32, Bool);
    scilab_fill_comparisonequal(UInt64, UInt32, M_M, UInt64, UInt32, Bool);
    scilab_fill_comparisonequal(UInt64, Int64, M_M, UInt64, Int64, Bool);
    scilab_fill_comparisonequal(UInt64, UInt64, M_M, UInt64, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, Bool, M_B, UInt64, Bool, Bool);
    scilab_fill_comparisonequal(UInt64, Empty, M_E, UInt64, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(UInt64, ScalarDouble, M_S, UInt64, Double, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarInt8, M_S, UInt64, Int8, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarUInt8, M_S, UInt64, UInt8, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarInt16, M_S, UInt64, Int16, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarUInt16, M_S, UInt64, UInt16, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarInt32, M_S, UInt64, Int32, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarUInt32, M_S, UInt64, UInt32, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarInt64, M_S, UInt64, Int64, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarUInt64, M_S, UInt64, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarBool, M_B, UInt64, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarUInt64, Double, S_M, UInt64, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Int8, S_M, UInt64, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, UInt8, S_M, UInt64, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Int16, S_M, UInt64, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, UInt16, S_M, UInt64, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Int32, S_M, UInt64, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, UInt32, S_M, UInt64, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Int64, S_M, UInt64, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, UInt64, S_M, UInt64, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Bool, S_B, UInt64, Bool, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Empty, M_E, UInt64, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarBool, M_E, UInt64, Bool, Bool);

    //Bool
    //Matrix == Matrix
    scilab_fill_comparisonequal(Bool, Double, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(Bool, Int8, B_M, Bool, Int8, Bool);
    scilab_fill_comparisonequal(Bool, UInt8, B_M, Bool, UInt8, Bool);
    scilab_fill_comparisonequal(Bool, Int16, B_M, Bool, Int16, Bool);
    scilab_fill_comparisonequal(Bool, UInt16, B_M, Bool, UInt16, Bool);
    scilab_fill_comparisonequal(Bool, Int32, B_M, Bool, Int32, Bool);
    scilab_fill_comparisonequal(Bool, UInt32, B_M, Bool, UInt32, Bool);
    scilab_fill_comparisonequal(Bool, Int64, B_M, Bool, Int64, Bool);
    scilab_fill_comparisonequal(Bool, UInt64, B_M, Bool, UInt64, Bool);
    scilab_fill_comparisonequal(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_comparisonequal(Bool, Empty, M_E, Bool, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparisonequal(Bool, ScalarDouble, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(Bool, ScalarInt8, B_S, Bool, Int8, Bool);
    scilab_fill_comparisonequal(Bool, ScalarUInt8, B_S, Bool, UInt8, Bool);
    scilab_fill_comparisonequal(Bool, ScalarInt16, B_S, Bool, Int16, Bool);
    scilab_fill_comparisonequal(Bool, ScalarUInt16, B_S, Bool, UInt16, Bool);
    scilab_fill_comparisonequal(Bool, ScalarInt32, B_S, Bool, Int32, Bool);
    scilab_fill_comparisonequal(Bool, ScalarUInt32, B_S, Bool, UInt32, Bool);
    scilab_fill_comparisonequal(Bool, ScalarInt64, B_S, Bool, Int64, Bool);
    scilab_fill_comparisonequal(Bool, ScalarUInt64, B_S, Bool, UInt64, Bool);
    scilab_fill_comparisonequal(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparisonequal(ScalarBool, Double, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(ScalarBool, Int8, B_M, Bool, Int8, Bool);
    scilab_fill_comparisonequal(ScalarBool, UInt8, B_M, Bool, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarBool, Int16, B_M, Bool, Int16, Bool);
    scilab_fill_comparisonequal(ScalarBool, UInt16, B_M, Bool, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarBool, Int32, B_M, Bool, Int32, Bool);
    scilab_fill_comparisonequal(ScalarBool, UInt32, B_M, Bool, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarBool, Int64, B_M, Bool, Int64, Bool);
    scilab_fill_comparisonequal(ScalarBool, UInt64, B_M, Bool, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_comparisonequal(ScalarBool, Empty, M_E, Bool, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparisonequal(ScalarBool, ScalarDouble, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarInt8, M_E, Bool, Int8, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarUInt8, M_E, Bool, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarInt16, M_E, Bool, Int16, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarUInt16, M_E, Bool, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarInt32, M_E, Bool, Int32, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarUInt32, M_E, Bool, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarInt64, M_E, Bool, Int64, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarUInt64, M_E, Bool, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Bool == Identity
    scilab_fill_comparisonequal(Bool, Identity, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(Bool, IdentityComplex, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(ScalarBool, Identity, M_E, Bool, Double, Bool);
    scilab_fill_comparisonequal(ScalarBool, IdentityComplex, M_E, Bool, Double, Bool);

    //Bool == Polynom
    scilab_fill_comparisonequal(Bool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarBool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarBool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarBool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);

    //Bool == Sparse
    scilab_fill_comparisonequal(Bool, Sparse, M_E, Bool, Sparse, Bool);
    scilab_fill_comparisonequal(Bool, SparseComplex, M_E, Bool, Sparse, Bool);
    scilab_fill_comparisonequal(ScalarBool, Sparse, M_E, Bool, Sparse, Bool);
    scilab_fill_comparisonequal(ScalarBool, SparseComplex, M_E, Bool, Sparse, Bool);

    //Identity
    scilab_fill_comparisonequal(Identity, Double, I_M, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, DoubleComplex, I_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, ScalarDouble, I_S, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, ScalarDoubleComplex, I_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, Identity, I_I, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, IdentityComplex, I_IC, Double, Double, Bool);
    scilab_fill_comparisonequal(Identity, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparisonequal(Identity, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Identity, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Identity, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Identity, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(Identity, Sparse, M_E, Double, Sparse, SparseBool);
    scilab_fill_comparisonequal(Identity, SparseComplex, M_E, Double, Sparse, SparseBool);

    scilab_fill_comparisonequal(IdentityComplex, Double, IC_M, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarDouble, IC_S, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Identity, IC_I, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparisonequal(IdentityComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(IdentityComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Sparse, M_E, Double, Sparse, SparseBool);
    scilab_fill_comparisonequal(IdentityComplex, SparseComplex, M_E, Double, Sparse, SparseBool);

    scilab_fill_comparisonequal(Identity, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(Identity, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarBool, M_E, Double, Bool, Bool);

    //Polynom

    //poly == poly
    scilab_fill_comparisonequal(Polynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(Polynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly == scalar poly
    scilab_fill_comparisonequal(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly == double
    scilab_fill_comparisonequal(Polynom, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(Polynom, DoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, DoubleComplex, P_M, Polynom, Double, Bool);

    //poly == scalar double
    scilab_fill_comparisonequal(Polynom, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarDoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarDoubleComplex, P_M, Polynom, Double, Bool);

    //poly == Bool
    scilab_fill_comparisonequal(Polynom, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarBool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarBool, E_M, Polynom, Bool, Bool);

    //poly == []
    scilab_fill_comparisonequal(Polynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //poly == eye
    scilab_fill_comparisonequal(Polynom, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(Polynom, IdentityComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(PolynomComplex, IdentityComplex, P_M, Polynom, Double, Bool);

    //scalar poly == poly
    scilab_fill_comparisonequal(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly == scalar poly
    scilab_fill_comparisonequal(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly == double
    scilab_fill_comparisonequal(ScalarPolynom, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, DoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, DoubleComplex, P_M, Polynom, Double, Bool);

    //scalar poly == scalar double
    scilab_fill_comparisonequal(ScalarPolynom, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarDoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarDoubleComplex, P_M, Polynom, Double, Bool);

    //poly == Bool
    scilab_fill_comparisonequal(ScalarPolynom, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarBool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarBool, E_M, Polynom, Bool, Bool);

    //scalar poly == []
    scilab_fill_comparisonequal(ScalarPolynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //scalar poly == eye
    scilab_fill_comparisonequal(ScalarPolynom, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, IdentityComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, IdentityComplex, P_M, Polynom, Double, Bool);

    //poly == Int8
    scilab_fill_comparisonequal(Polynom, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(Polynom, UInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(PolynomComplex, UInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparisonequal(Int8, Polynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(UInt8, Polynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparisonequal(Int8, PolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(UInt8, PolynomComplex, M_P, UInt8, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, UInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, UInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparisonequal(Int8, ScalarPolynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarPolynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparisonequal(Int8, ScalarPolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarPolynomComplex, M_P, UInt8, Polynom, Bool);

    //poly == Int16
    scilab_fill_comparisonequal(Polynom, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(Polynom, UInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(PolynomComplex, UInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparisonequal(Int16, Polynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(UInt16, Polynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparisonequal(Int16, PolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(UInt16, PolynomComplex, M_P, UInt16, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, UInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, UInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparisonequal(Int16, ScalarPolynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarPolynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparisonequal(Int16, ScalarPolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarPolynomComplex, M_P, UInt16, Polynom, Bool);

    //poly == Int32
    scilab_fill_comparisonequal(Polynom, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(Polynom, UInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(PolynomComplex, UInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparisonequal(Int32, Polynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(UInt32, Polynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparisonequal(Int32, PolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(UInt32, PolynomComplex, M_P, UInt32, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, UInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, UInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparisonequal(Int32, ScalarPolynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarPolynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparisonequal(Int32, ScalarPolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarPolynomComplex, M_P, UInt32, Polynom, Bool);

    //poly == Int64
    scilab_fill_comparisonequal(Polynom, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(Polynom, UInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(PolynomComplex, UInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparisonequal(Int64, Polynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(UInt64, Polynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparisonequal(Int64, PolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(UInt64, PolynomComplex, M_P, UInt64, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, UInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, UInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparisonequal(Int64, ScalarPolynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarPolynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparisonequal(Int64, ScalarPolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarPolynomComplex, M_P, UInt64, Polynom, Bool);

    //poly == Int8
    scilab_fill_comparisonequal(Polynom, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarUInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarUInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparisonequal(ScalarInt8, Polynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Polynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt8, PolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, PolynomComplex, M_P, UInt8, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarUInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarUInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparisonequal(ScalarInt8, ScalarPolynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarPolynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarPolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarPolynomComplex, M_P, UInt8, Polynom, Bool);

    //poly == Int16
    scilab_fill_comparisonequal(Polynom, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarUInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarUInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparisonequal(ScalarInt16, Polynom, M_P, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Polynom, M_P, Polynom, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, PolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, PolynomComplex, M_P, UInt16, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarUInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarUInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparisonequal(ScalarInt16, ScalarPolynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarPolynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarPolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarPolynomComplex, M_P, UInt16, Polynom, Bool);

    //poly == Int32
    scilab_fill_comparisonequal(Polynom, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarUInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarUInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparisonequal(ScalarInt32, Polynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Polynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt32, PolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, PolynomComplex, M_P, UInt32, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarUInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarUInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparisonequal(ScalarInt32, ScalarPolynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarPolynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarPolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarPolynomComplex, M_P, UInt32, Polynom, Bool);

    //poly == Int64
    scilab_fill_comparisonequal(Polynom, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(Polynom, ScalarUInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(PolynomComplex, ScalarUInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparisonequal(ScalarInt64, Polynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Polynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt64, PolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, PolynomComplex, M_P, UInt64, Polynom, Bool);

    scilab_fill_comparisonequal(ScalarPolynom, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, ScalarUInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, ScalarUInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparisonequal(ScalarInt64, ScalarPolynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarPolynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarPolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarPolynomComplex, M_P, UInt64, Polynom, Bool);

    //poly == bool
    scilab_fill_comparisonequal(Polynom, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(PolynomComplex, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(ScalarPolynom, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparisonequal(ScalarPolynomComplex, Bool, M_E, Polynom, Bool, Bool);

    scilab_fill_comparisonequal(Bool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparisonequal(Bool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);

    //Sparse
    scilab_fill_comparisonequal(Sparse, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparisonequal(Sparse, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparisonequal(Sparse, Double, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(Sparse, DoubleComplex, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(Sparse, ScalarDouble, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(Sparse, ScalarDoubleComplex, SP_M, Sparse, Double, SparseBool);

    scilab_fill_comparisonequal(Sparse, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparisonequal(Sparse, Identity, M_E, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(Sparse, IdentityComplex, M_E, Sparse, Double, SparseBool);

    scilab_fill_comparisonequal(SparseComplex, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, Double, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, DoubleComplex, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarDouble, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarDoubleComplex, SP_M, Sparse, Double, SparseBool);

    scilab_fill_comparisonequal(SparseComplex, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparisonequal(SparseComplex, Identity, M_E, Sparse, Double, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, IdentityComplex, M_E, Sparse, Double, SparseBool);

    scilab_fill_comparisonequal(Sparse, Bool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparisonequal(Sparse, ScalarBool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparisonequal(SparseComplex, Bool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparisonequal(SparseComplex, ScalarBool, M_E, Sparse, Bool, Bool);

    scilab_fill_comparisonequal(Sparse, Int8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparisonequal(Int8, Sparse, M_SP, Int8, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, Int8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparisonequal(Int8, SparseComplex, M_SP, Int8, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, UInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparisonequal(UInt8, Sparse, M_SP, UInt8, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, UInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparisonequal(UInt8, SparseComplex, M_SP, UInt8, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, Int16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparisonequal(Int16, Sparse, M_SP, Int16, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, Int16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparisonequal(Int16, SparseComplex, M_SP, Int16, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, UInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparisonequal(UInt16, Sparse, M_SP, UInt16, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, UInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparisonequal(UInt16, SparseComplex, M_SP, UInt16, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, Int32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparisonequal(Int32, Sparse, M_SP, Int32, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, Int32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparisonequal(Int32, SparseComplex, M_SP, Int32, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, UInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparisonequal(UInt32, Sparse, M_SP, UInt32, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, UInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparisonequal(UInt32, SparseComplex, M_SP, UInt32, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, Int64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparisonequal(Int64, Sparse, M_SP, Int64, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, Int64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparisonequal(Int64, SparseComplex, M_SP, Int64, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, UInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparisonequal(UInt64, Sparse, M_SP, UInt64, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, UInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparisonequal(UInt64, SparseComplex, M_SP, UInt64, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarInt8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparisonequal(ScalarInt8, Sparse, M_SP, Int8, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarInt8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparisonequal(ScalarInt8, SparseComplex, M_SP, Int8, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarUInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt8, Sparse, M_SP, UInt8, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarUInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt8, SparseComplex, M_SP, UInt8, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarInt16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparisonequal(ScalarInt16, Sparse, M_SP, Int16, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarInt16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparisonequal(ScalarInt16, SparseComplex, M_SP, Int16, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarUInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt16, Sparse, M_SP, UInt16, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarUInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt16, SparseComplex, M_SP, UInt16, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarInt32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparisonequal(ScalarInt32, Sparse, M_SP, Int32, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarInt32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparisonequal(ScalarInt32, SparseComplex, M_SP, Int32, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarUInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt32, Sparse, M_SP, UInt32, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarUInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt32, SparseComplex, M_SP, UInt32, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarInt64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparisonequal(ScalarInt64, Sparse, M_SP, Int64, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarInt64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparisonequal(ScalarInt64, SparseComplex, M_SP, Int64, Sparse, SparseBool);

    scilab_fill_comparisonequal(Sparse, ScalarUInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt64, Sparse, M_SP, UInt64, Sparse, SparseBool);
    scilab_fill_comparisonequal(SparseComplex, ScalarUInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparisonequal(ScalarUInt64, SparseComplex, M_SP, UInt64, Sparse, SparseBool);


    //SparseBool
    scilab_fill_comparisonequal(SparseBool, SparseBool, M_M, SparseBool, SparseBool, SparseBool);
    scilab_fill_comparisonequal(SparseBool, Bool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_comparisonequal(Bool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_comparisonequal(SparseBool, ScalarBool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_comparisonequal(ScalarBool, SparseBool, M_M, Bool, SparseBool, SparseBool);

    scilab_fill_comparisonequal(SparseBool, Double, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(Double, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, DoubleComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, ScalarDouble, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarDoubleComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Empty, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(Empty, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, Identity, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(Identity, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, IdentityComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Int8, M_E, SparseBool, Int8, Bool);
    scilab_fill_comparisonequal(Int8, SparseBool, M_E, Int8, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, UInt8, M_E, SparseBool, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, SparseBool, M_E, UInt8, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Int16, M_E, SparseBool, Int16, Bool);
    scilab_fill_comparisonequal(Int16, SparseBool, M_E, Int16, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, UInt16, M_E, SparseBool, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, SparseBool, M_E, UInt16, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Int32, M_E, SparseBool, Int32, Bool);
    scilab_fill_comparisonequal(Int32, SparseBool, M_E, Int32, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, UInt32, M_E, SparseBool, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, SparseBool, M_E, UInt32, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Int64, M_E, SparseBool, Int64, Bool);
    scilab_fill_comparisonequal(Int64, SparseBool, M_E, Int64, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, UInt64, M_E, SparseBool, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, SparseBool, M_E, UInt64, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, ScalarInt8, M_E, SparseBool, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, SparseBool, M_E, Int8, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarUInt8, M_E, SparseBool, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, SparseBool, M_E, UInt8, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, ScalarInt16, M_E, SparseBool, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, SparseBool, M_E, Int16, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarUInt16, M_E, SparseBool, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, SparseBool, M_E, UInt16, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, ScalarInt32, M_E, SparseBool, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, SparseBool, M_E, Int32, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarUInt32, M_E, SparseBool, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, SparseBool, M_E, UInt32, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, ScalarInt64, M_E, SparseBool, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, SparseBool, M_E, Int64, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarUInt64, M_E, SparseBool, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, SparseBool, M_E, UInt64, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, String, M_E, SparseBool, String, Bool);
    scilab_fill_comparisonequal(String, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarString, M_E, SparseBool, String, Bool);
    scilab_fill_comparisonequal(ScalarString, SparseBool, M_E, String, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, SparseComplex, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, SparseBool, M_E, Sparse, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Sparse, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, SparseBool, M_E, Sparse, SparseBool, Bool);

    scilab_fill_comparisonequal(SparseBool, Sparse, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, SparseBool, M_E, Sparse, SparseBool, Bool);

    //struct
    scilab_fill_comparisonequal(Struct, Struct, M_M, Struct, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Empty, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(Empty, Struct, E_M, Double, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Double, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(Double, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparisonequal(Struct, DoubleComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparisonequal(Struct, ScalarDouble, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarDoubleComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Identity, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(Identity, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparisonequal(Struct, IdentityComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Int8, M_E, Struct, Int8, Bool);
    scilab_fill_comparisonequal(Int8, Struct, M_E, Int8, Struct, Bool);
    scilab_fill_comparisonequal(Struct, UInt8, M_E, Struct, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, Struct, M_E, UInt8, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Int16, M_E, Struct, Int16, Bool);
    scilab_fill_comparisonequal(Int16, Struct, M_E, Int16, Struct, Bool);
    scilab_fill_comparisonequal(Struct, UInt16, M_E, Struct, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, Struct, M_E, UInt16, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Int32, M_E, Struct, Int32, Bool);
    scilab_fill_comparisonequal(Int32, Struct, M_E, Int32, Struct, Bool);
    scilab_fill_comparisonequal(Struct, UInt32, M_E, Struct, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, Struct, M_E, UInt32, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Int64, M_E, Struct, Int64, Bool);
    scilab_fill_comparisonequal(Int64, Struct, M_E, Int64, Struct, Bool);
    scilab_fill_comparisonequal(Struct, UInt64, M_E, Struct, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, Struct, M_E, UInt64, Struct, Bool);

    scilab_fill_comparisonequal(Struct, ScalarInt8, M_E, Struct, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Struct, M_E, Int8, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarUInt8, M_E, Struct, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Struct, M_E, UInt8, Struct, Bool);

    scilab_fill_comparisonequal(Struct, ScalarInt16, M_E, Struct, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Struct, M_E, Int16, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarUInt16, M_E, Struct, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Struct, M_E, UInt16, Struct, Bool);

    scilab_fill_comparisonequal(Struct, ScalarInt32, M_E, Struct, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Struct, M_E, Int32, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarUInt32, M_E, Struct, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Struct, M_E, UInt32, Struct, Bool);

    scilab_fill_comparisonequal(Struct, ScalarInt64, M_E, Struct, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Struct, M_E, Int64, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarUInt64, M_E, Struct, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Struct, M_E, UInt64, Struct, Bool);

    scilab_fill_comparisonequal(Struct, String, M_E, Struct, String, Bool);
    scilab_fill_comparisonequal(String, Struct, M_E, String, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarString, M_E, Struct, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Struct, M_E, String, Struct, Bool);

    scilab_fill_comparisonequal(Struct, SparseComplex, M_E, Struct, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, Struct, M_E, Sparse, Struct, Bool);

    scilab_fill_comparisonequal(Struct, Sparse, M_E, Struct, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, Struct, M_E, Sparse, Struct, Bool);

    scilab_fill_comparisonequal(Struct, SparseBool, M_E, Struct, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, Struct, M_E, SparseBool, Struct, Bool);

    //List, TList, MList
    scilab_fill_comparisonequal(List, List, LT_LT, List, List, Bool);
    scilab_fill_comparisonequal(MList, MList, LT_LT, MList, MList, Bool);
    scilab_fill_comparisonequal(TList, TList, LT_LT, TList, TList, Bool);

    scilab_fill_comparisonequal(List, TList, LT_LT, List, TList, Bool);
    scilab_fill_comparisonequal(TList, List, LT_LT, TList, List, Bool);
    scilab_fill_comparisonequal(List, MList, LT_LT, List, MList, Bool);
    scilab_fill_comparisonequal(MList, List, LT_LT, MList, List, Bool);

    scilab_fill_comparisonequal(TList, MList, LT_LT, TList, MList, Bool);
    scilab_fill_comparisonequal(MList, TList, LT_LT, MList, TList, Bool);

    scilab_fill_comparisonequal(List, Empty, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(TList, Empty, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(MList, Empty, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(Empty, List, E_M, Double, List, Bool);
    scilab_fill_comparisonequal(Empty, TList, E_M, Double, TList, Bool);
    scilab_fill_comparisonequal(Empty, MList, E_M, Double, MList, Bool);

    scilab_fill_comparisonequal(List, Double, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(Double, List, M_E, Double, List, Bool);
    scilab_fill_comparisonequal(List, DoubleComplex, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, List, M_E, Double, List, Bool);

    scilab_fill_comparisonequal(List, ScalarDouble, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, List, M_E, Double, List, Bool);
    scilab_fill_comparisonequal(List, ScalarDoubleComplex, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, List, M_E, Double, List, Bool);

    scilab_fill_comparisonequal(List, Identity, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(Identity, List, M_E, Double, List, Bool);
    scilab_fill_comparisonequal(List, IdentityComplex, M_E, List, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, List, M_E, Double, List, Bool);

    scilab_fill_comparisonequal(List, Int8, M_E, List, Int8, Bool);
    scilab_fill_comparisonequal(Int8, List, M_E, Int8, List, Bool);
    scilab_fill_comparisonequal(List, UInt8, M_E, List, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, List, M_E, UInt8, List, Bool);

    scilab_fill_comparisonequal(List, Int16, M_E, List, Int16, Bool);
    scilab_fill_comparisonequal(Int16, List, M_E, Int16, List, Bool);
    scilab_fill_comparisonequal(List, UInt16, M_E, List, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, List, M_E, UInt16, List, Bool);

    scilab_fill_comparisonequal(List, Int32, M_E, List, Int32, Bool);
    scilab_fill_comparisonequal(Int32, List, M_E, Int32, List, Bool);
    scilab_fill_comparisonequal(List, UInt32, M_E, List, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, List, M_E, UInt32, List, Bool);

    scilab_fill_comparisonequal(List, Int64, M_E, List, Int64, Bool);
    scilab_fill_comparisonequal(Int64, List, M_E, Int64, List, Bool);
    scilab_fill_comparisonequal(List, UInt64, M_E, List, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, List, M_E, UInt64, List, Bool);

    scilab_fill_comparisonequal(List, ScalarInt8, M_E, List, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, List, M_E, Int8, List, Bool);
    scilab_fill_comparisonequal(List, ScalarUInt8, M_E, List, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, List, M_E, UInt8, List, Bool);

    scilab_fill_comparisonequal(List, ScalarInt16, M_E, List, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, List, M_E, Int16, List, Bool);
    scilab_fill_comparisonequal(List, ScalarUInt16, M_E, List, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, List, M_E, UInt16, List, Bool);

    scilab_fill_comparisonequal(List, ScalarInt32, M_E, List, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, List, M_E, Int32, List, Bool);
    scilab_fill_comparisonequal(List, ScalarUInt32, M_E, List, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, List, M_E, UInt32, List, Bool);

    scilab_fill_comparisonequal(List, ScalarInt64, M_E, List, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, List, M_E, Int64, List, Bool);
    scilab_fill_comparisonequal(List, ScalarUInt64, M_E, List, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, List, M_E, UInt64, List, Bool);

    scilab_fill_comparisonequal(List, String, M_E, List, String, Bool);
    scilab_fill_comparisonequal(String, List, M_E, String, List, Bool);
    scilab_fill_comparisonequal(List, ScalarString, M_E, List, String, Bool);
    scilab_fill_comparisonequal(ScalarString, List, M_E, String, List, Bool);

    scilab_fill_comparisonequal(List, SparseComplex, M_E, List, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, List, M_E, Sparse, List, Bool);

    scilab_fill_comparisonequal(List, Sparse, M_E, List, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, List, M_E, Sparse, List, Bool);

    scilab_fill_comparisonequal(List, SparseBool, M_E, List, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, List, M_E, SparseBool, List, Bool);

    scilab_fill_comparisonequal(List, Struct, M_E, List, Struct, Bool);
    scilab_fill_comparisonequal(Struct, List, M_E, Struct, List, Bool);

    scilab_fill_comparisonequal(TList, Double, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(Double, TList, M_E, Double, TList, Bool);
    scilab_fill_comparisonequal(TList, DoubleComplex, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, TList, M_E, Double, TList, Bool);

    scilab_fill_comparisonequal(TList, ScalarDouble, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, TList, M_E, Double, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarDoubleComplex, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, TList, M_E, Double, TList, Bool);

    scilab_fill_comparisonequal(TList, Identity, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(Identity, TList, M_E, Double, TList, Bool);
    scilab_fill_comparisonequal(TList, IdentityComplex, M_E, TList, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, TList, M_E, Double, TList, Bool);

    scilab_fill_comparisonequal(TList, Int8, M_E, TList, Int8, Bool);
    scilab_fill_comparisonequal(Int8, TList, M_E, Int8, TList, Bool);
    scilab_fill_comparisonequal(TList, UInt8, M_E, TList, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, TList, M_E, UInt8, TList, Bool);

    scilab_fill_comparisonequal(TList, Int16, M_E, TList, Int16, Bool);
    scilab_fill_comparisonequal(Int16, TList, M_E, Int16, TList, Bool);
    scilab_fill_comparisonequal(TList, UInt16, M_E, TList, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, TList, M_E, UInt16, TList, Bool);

    scilab_fill_comparisonequal(TList, Int32, M_E, TList, Int32, Bool);
    scilab_fill_comparisonequal(Int32, TList, M_E, Int32, TList, Bool);
    scilab_fill_comparisonequal(TList, UInt32, M_E, TList, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, TList, M_E, UInt32, TList, Bool);

    scilab_fill_comparisonequal(TList, Int64, M_E, TList, Int64, Bool);
    scilab_fill_comparisonequal(Int64, TList, M_E, Int64, TList, Bool);
    scilab_fill_comparisonequal(TList, UInt64, M_E, TList, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, TList, M_E, UInt64, TList, Bool);

    scilab_fill_comparisonequal(TList, ScalarInt8, M_E, TList, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, TList, M_E, Int8, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarUInt8, M_E, TList, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, TList, M_E, UInt8, TList, Bool);

    scilab_fill_comparisonequal(TList, ScalarInt16, M_E, TList, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, TList, M_E, Int16, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarUInt16, M_E, TList, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, TList, M_E, UInt16, TList, Bool);

    scilab_fill_comparisonequal(TList, ScalarInt32, M_E, TList, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, TList, M_E, Int32, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarUInt32, M_E, TList, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, TList, M_E, UInt32, TList, Bool);

    scilab_fill_comparisonequal(TList, ScalarInt64, M_E, TList, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, TList, M_E, Int64, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarUInt64, M_E, TList, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, TList, M_E, UInt64, TList, Bool);

    scilab_fill_comparisonequal(TList, String, M_E, TList, String, Bool);
    scilab_fill_comparisonequal(String, TList, M_E, String, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarString, M_E, TList, String, Bool);
    scilab_fill_comparisonequal(ScalarString, TList, M_E, String, TList, Bool);

    scilab_fill_comparisonequal(TList, SparseComplex, M_E, TList, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, TList, M_E, Sparse, TList, Bool);

    scilab_fill_comparisonequal(TList, Sparse, M_E, TList, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, TList, M_E, Sparse, TList, Bool);

    scilab_fill_comparisonequal(TList, SparseBool, M_E, TList, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, TList, M_E, SparseBool, TList, Bool);

    scilab_fill_comparisonequal(TList, Struct, M_E, TList, Struct, Bool);
    scilab_fill_comparisonequal(Struct, TList, M_E, Struct, TList, Bool);

    scilab_fill_comparisonequal(MList, Double, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(Double, MList, M_E, Double, MList, Bool);
    scilab_fill_comparisonequal(MList, DoubleComplex, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, MList, M_E, Double, MList, Bool);

    scilab_fill_comparisonequal(MList, ScalarDouble, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, MList, M_E, Double, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarDoubleComplex, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, MList, M_E, Double, MList, Bool);

    scilab_fill_comparisonequal(MList, Identity, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(Identity, MList, M_E, Double, MList, Bool);
    scilab_fill_comparisonequal(MList, IdentityComplex, M_E, MList, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, MList, M_E, Double, MList, Bool);

    scilab_fill_comparisonequal(MList, Int8, M_E, MList, Int8, Bool);
    scilab_fill_comparisonequal(Int8, MList, M_E, Int8, MList, Bool);
    scilab_fill_comparisonequal(MList, UInt8, M_E, MList, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, MList, M_E, UInt8, MList, Bool);

    scilab_fill_comparisonequal(MList, Int16, M_E, MList, Int16, Bool);
    scilab_fill_comparisonequal(Int16, MList, M_E, Int16, MList, Bool);
    scilab_fill_comparisonequal(MList, UInt16, M_E, MList, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, MList, M_E, UInt16, MList, Bool);

    scilab_fill_comparisonequal(MList, Int32, M_E, MList, Int32, Bool);
    scilab_fill_comparisonequal(Int32, MList, M_E, Int32, MList, Bool);
    scilab_fill_comparisonequal(MList, UInt32, M_E, MList, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, MList, M_E, UInt32, MList, Bool);

    scilab_fill_comparisonequal(MList, Int64, M_E, MList, Int64, Bool);
    scilab_fill_comparisonequal(Int64, MList, M_E, Int64, MList, Bool);
    scilab_fill_comparisonequal(MList, UInt64, M_E, MList, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, MList, M_E, UInt64, MList, Bool);

    scilab_fill_comparisonequal(MList, ScalarInt8, M_E, MList, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, MList, M_E, Int8, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarUInt8, M_E, MList, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, MList, M_E, UInt8, MList, Bool);

    scilab_fill_comparisonequal(MList, ScalarInt16, M_E, MList, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, MList, M_E, Int16, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarUInt16, M_E, MList, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, MList, M_E, UInt16, MList, Bool);

    scilab_fill_comparisonequal(MList, ScalarInt32, M_E, MList, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, MList, M_E, Int32, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarUInt32, M_E, MList, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, MList, M_E, UInt32, MList, Bool);

    scilab_fill_comparisonequal(MList, ScalarInt64, M_E, MList, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, MList, M_E, Int64, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarUInt64, M_E, MList, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, MList, M_E, UInt64, MList, Bool);

    scilab_fill_comparisonequal(MList, String, M_E, MList, String, Bool);
    scilab_fill_comparisonequal(String, MList, M_E, String, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarString, M_E, MList, String, Bool);
    scilab_fill_comparisonequal(ScalarString, MList, M_E, String, MList, Bool);

    scilab_fill_comparisonequal(MList, SparseComplex, M_E, MList, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, MList, M_E, Sparse, MList, Bool);

    scilab_fill_comparisonequal(MList, Sparse, M_E, MList, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, MList, M_E, Sparse, MList, Bool);

    scilab_fill_comparisonequal(MList, SparseBool, M_E, MList, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, MList, M_E, SparseBool, MList, Bool);

    scilab_fill_comparisonequal(MList, Struct, M_E, MList, Struct, Bool);
    scilab_fill_comparisonequal(Struct, MList, M_E, Struct, MList, Bool);

    //Cell
    scilab_fill_comparisonequal(Cell, Cell, M_M, Cell, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Empty, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(Empty, Cell, E_M, Double, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Double, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(Double, Cell, M_E, Double, Cell, Bool);
    scilab_fill_comparisonequal(Cell, DoubleComplex, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Cell, M_E, Double, Cell, Bool);

    scilab_fill_comparisonequal(Cell, ScalarDouble, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Cell, M_E, Double, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarDoubleComplex, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Cell, M_E, Double, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Empty, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(Empty, Cell, M_E, Double, Cell, Bool);
    scilab_fill_comparisonequal(Cell, Identity, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(Identity, Cell, M_E, Double, Cell, Bool);
    scilab_fill_comparisonequal(Cell, IdentityComplex, M_E, Cell, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Cell, M_E, Double, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Int8, M_E, Cell, Int8, Bool);
    scilab_fill_comparisonequal(Int8, Cell, M_E, Int8, Cell, Bool);
    scilab_fill_comparisonequal(Cell, UInt8, M_E, Cell, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, Cell, M_E, UInt8, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Int16, M_E, Cell, Int16, Bool);
    scilab_fill_comparisonequal(Int16, Cell, M_E, Int16, Cell, Bool);
    scilab_fill_comparisonequal(Cell, UInt16, M_E, Cell, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, Cell, M_E, UInt16, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Int32, M_E, Cell, Int32, Bool);
    scilab_fill_comparisonequal(Int32, Cell, M_E, Int32, Cell, Bool);
    scilab_fill_comparisonequal(Cell, UInt32, M_E, Cell, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, Cell, M_E, UInt32, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Int64, M_E, Cell, Int64, Bool);
    scilab_fill_comparisonequal(Int64, Cell, M_E, Int64, Cell, Bool);
    scilab_fill_comparisonequal(Cell, UInt64, M_E, Cell, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, Cell, M_E, UInt64, Cell, Bool);

    scilab_fill_comparisonequal(Cell, ScalarInt8, M_E, Cell, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Cell, M_E, Int8, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarUInt8, M_E, Cell, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Cell, M_E, UInt8, Cell, Bool);

    scilab_fill_comparisonequal(Cell, ScalarInt16, M_E, Cell, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Cell, M_E, Int16, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarUInt16, M_E, Cell, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Cell, M_E, UInt16, Cell, Bool);

    scilab_fill_comparisonequal(Cell, ScalarInt32, M_E, Cell, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Cell, M_E, Int32, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarUInt32, M_E, Cell, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Cell, M_E, UInt32, Cell, Bool);

    scilab_fill_comparisonequal(Cell, ScalarInt64, M_E, Cell, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Cell, M_E, Int64, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarUInt64, M_E, Cell, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Cell, M_E, UInt64, Cell, Bool);

    scilab_fill_comparisonequal(Cell, String, M_E, Cell, String, Bool);
    scilab_fill_comparisonequal(String, Cell, M_E, String, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarString, M_E, Cell, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Cell, M_E, String, Cell, Bool);

    scilab_fill_comparisonequal(Cell, SparseComplex, M_E, Cell, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, Cell, M_E, Sparse, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Sparse, M_E, Cell, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, Cell, M_E, Sparse, Cell, Bool);

    scilab_fill_comparisonequal(Cell, SparseBool, M_E, Cell, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, Cell, M_E, SparseBool, Cell, Bool);

    scilab_fill_comparisonequal(Cell, Struct, M_E, Cell, Struct, Bool);
    scilab_fill_comparisonequal(Struct, Cell, M_E, Struct, Cell, Bool);

    scilab_fill_comparisonequal(Cell, List, M_E, Cell, List, Bool);
    scilab_fill_comparisonequal(List, Cell, M_E, List, Cell, Bool);

    scilab_fill_comparisonequal(Cell, TList, M_E, Cell, TList, Bool);
    scilab_fill_comparisonequal(TList, Cell, M_E, TList, Cell, Bool);

    scilab_fill_comparisonequal(Cell, MList, M_E, Cell, MList, Bool);
    scilab_fill_comparisonequal(MList, Cell, M_E, MList, Cell, Bool);

    //Handle
    scilab_fill_comparisonequal(Handle, Handle, M_M, GraphicHandle, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Empty, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Empty, Handle, E_M, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Double, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Double, Handle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, DoubleComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Handle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, ScalarDouble, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Handle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarDoubleComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Handle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Empty, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Empty, Handle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, Identity, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Identity, Handle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, IdentityComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Handle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Int8, M_E, GraphicHandle, Int8, Bool);
    scilab_fill_comparisonequal(Int8, Handle, M_E, Int8, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, UInt8, M_E, GraphicHandle, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, Handle, M_E, UInt8, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Int16, M_E, GraphicHandle, Int16, Bool);
    scilab_fill_comparisonequal(Int16, Handle, M_E, Int16, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, UInt16, M_E, GraphicHandle, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, Handle, M_E, UInt16, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Int32, M_E, GraphicHandle, Int32, Bool);
    scilab_fill_comparisonequal(Int32, Handle, M_E, Int32, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, UInt32, M_E, GraphicHandle, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, Handle, M_E, UInt32, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Int64, M_E, GraphicHandle, Int64, Bool);
    scilab_fill_comparisonequal(Int64, Handle, M_E, Int64, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, UInt64, M_E, GraphicHandle, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, Handle, M_E, UInt64, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, ScalarInt8, M_E, GraphicHandle, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Handle, M_E, Int8, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarUInt8, M_E, GraphicHandle, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Handle, M_E, UInt8, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, ScalarInt16, M_E, GraphicHandle, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Handle, M_E, Int16, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarUInt16, M_E, GraphicHandle, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Handle, M_E, UInt16, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, ScalarInt32, M_E, GraphicHandle, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Handle, M_E, Int32, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarUInt32, M_E, GraphicHandle, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Handle, M_E, UInt32, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, ScalarInt64, M_E, GraphicHandle, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Handle, M_E, Int64, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarUInt64, M_E, GraphicHandle, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Handle, M_E, UInt64, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, String, M_E, GraphicHandle, String, Bool);
    scilab_fill_comparisonequal(String, Handle, M_E, String, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarString, M_E, GraphicHandle, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Handle, M_E, String, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, SparseComplex, M_E, GraphicHandle, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, Handle, M_E, Sparse, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Sparse, M_E, GraphicHandle, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, Handle, M_E, Sparse, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, SparseBool, M_E, GraphicHandle, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, Handle, M_E, SparseBool, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Struct, M_E, GraphicHandle, Struct, Bool);
    scilab_fill_comparisonequal(Struct, Handle, M_E, Struct, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, List, M_E, GraphicHandle, List, Bool);
    scilab_fill_comparisonequal(List, Handle, M_E, List, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, TList, M_E, GraphicHandle, TList, Bool);
    scilab_fill_comparisonequal(TList, Handle, M_E, TList, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, MList, M_E, GraphicHandle, MList, Bool);
    scilab_fill_comparisonequal(MList, Handle, M_E, MList, GraphicHandle, Bool);

    scilab_fill_comparisonequal(Handle, Cell, M_E, GraphicHandle, Cell, Bool);
    scilab_fill_comparisonequal(Cell, Handle, M_E, Cell, GraphicHandle, Bool);

    //Macro
    scilab_fill_comparisonequal(Macro, Macro, MCR_MCR, Macro, Macro, Bool);
    scilab_fill_comparisonequal(MacroFile, MacroFile, MCR_MCR, MacroFile, MacroFile, Bool);
    scilab_fill_comparisonequal(Macro, MacroFile, MCR_MCR, Macro, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, Macro, MCR_MCR, MacroFile, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Empty, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(Empty, Macro, E_M, Double, Macro, Bool);
    scilab_fill_comparisonequal(MacroFile, Empty, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(Empty, MacroFile, E_M, Double, MacroFile, Bool);

    scilab_fill_comparisonequal(Macro, Double, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(Double, Macro, M_E, Double, Macro, Bool);
    scilab_fill_comparisonequal(Macro, DoubleComplex, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, Macro, M_E, Double, Macro, Bool);

    scilab_fill_comparisonequal(Macro, ScalarDouble, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, Macro, M_E, Double, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarDoubleComplex, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, Macro, M_E, Double, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Empty, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(Empty, Macro, M_E, Double, Macro, Bool);
    scilab_fill_comparisonequal(Macro, Identity, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(Identity, Macro, M_E, Double, Macro, Bool);
    scilab_fill_comparisonequal(Macro, IdentityComplex, M_E, Macro, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, Macro, M_E, Double, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Int8, M_E, Macro, Int8, Bool);
    scilab_fill_comparisonequal(Int8, Macro, M_E, Int8, Macro, Bool);
    scilab_fill_comparisonequal(Macro, UInt8, M_E, Macro, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, Macro, M_E, UInt8, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Int16, M_E, Macro, Int16, Bool);
    scilab_fill_comparisonequal(Int16, Macro, M_E, Int16, Macro, Bool);
    scilab_fill_comparisonequal(Macro, UInt16, M_E, Macro, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, Macro, M_E, UInt16, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Int32, M_E, Macro, Int32, Bool);
    scilab_fill_comparisonequal(Int32, Macro, M_E, Int32, Macro, Bool);
    scilab_fill_comparisonequal(Macro, UInt32, M_E, Macro, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, Macro, M_E, UInt32, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Int64, M_E, Macro, Int64, Bool);
    scilab_fill_comparisonequal(Int64, Macro, M_E, Int64, Macro, Bool);
    scilab_fill_comparisonequal(Macro, UInt64, M_E, Macro, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, Macro, M_E, UInt64, Macro, Bool);

    scilab_fill_comparisonequal(Macro, ScalarInt8, M_E, Macro, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, Macro, M_E, Int8, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarUInt8, M_E, Macro, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, Macro, M_E, UInt8, Macro, Bool);

    scilab_fill_comparisonequal(Macro, ScalarInt16, M_E, Macro, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, Macro, M_E, Int16, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarUInt16, M_E, Macro, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, Macro, M_E, UInt16, Macro, Bool);

    scilab_fill_comparisonequal(Macro, ScalarInt32, M_E, Macro, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, Macro, M_E, Int32, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarUInt32, M_E, Macro, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, Macro, M_E, UInt32, Macro, Bool);

    scilab_fill_comparisonequal(Macro, ScalarInt64, M_E, Macro, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, Macro, M_E, Int64, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarUInt64, M_E, Macro, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, Macro, M_E, UInt64, Macro, Bool);

    scilab_fill_comparisonequal(Macro, String, M_E, Macro, String, Bool);
    scilab_fill_comparisonequal(String, Macro, M_E, String, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarString, M_E, Macro, String, Bool);
    scilab_fill_comparisonequal(ScalarString, Macro, M_E, String, Macro, Bool);

    scilab_fill_comparisonequal(Macro, SparseComplex, M_E, Macro, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, Macro, M_E, Sparse, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Sparse, M_E, Macro, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, Macro, M_E, Sparse, Macro, Bool);

    scilab_fill_comparisonequal(Macro, SparseBool, M_E, Macro, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, Macro, M_E, SparseBool, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Struct, M_E, Macro, Struct, Bool);
    scilab_fill_comparisonequal(Struct, Macro, M_E, Struct, Macro, Bool);

    scilab_fill_comparisonequal(Macro, List, M_E, Macro, List, Bool);
    scilab_fill_comparisonequal(List, Macro, M_E, List, Macro, Bool);

    scilab_fill_comparisonequal(Macro, TList, M_E, Macro, TList, Bool);
    scilab_fill_comparisonequal(TList, Macro, M_E, TList, Macro, Bool);

    scilab_fill_comparisonequal(Macro, MList, M_E, Macro, MList, Bool);
    scilab_fill_comparisonequal(MList, Macro, M_E, MList, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Cell, M_E, Macro, Cell, Bool);
    scilab_fill_comparisonequal(Cell, Macro, M_E, Cell, Macro, Bool);

    scilab_fill_comparisonequal(Macro, Handle, M_E, Macro, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, Macro, M_E, GraphicHandle, Macro, Bool);

    scilab_fill_comparisonequal(MacroFile, Double, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(Double, MacroFile, M_E, Double, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, DoubleComplex, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, MacroFile, M_E, Double, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, ScalarDouble, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, MacroFile, M_E, Double, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarDoubleComplex, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, MacroFile, M_E, Double, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Empty, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(Empty, MacroFile, M_E, Double, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, Identity, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(Identity, MacroFile, M_E, Double, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, IdentityComplex, M_E, MacroFile, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, MacroFile, M_E, Double, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Int8, M_E, MacroFile, Int8, Bool);
    scilab_fill_comparisonequal(Int8, MacroFile, M_E, Int8, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, UInt8, M_E, MacroFile, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, MacroFile, M_E, UInt8, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Int16, M_E, MacroFile, Int16, Bool);
    scilab_fill_comparisonequal(Int16, MacroFile, M_E, Int16, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, UInt16, M_E, MacroFile, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, MacroFile, M_E, UInt16, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Int32, M_E, MacroFile, Int32, Bool);
    scilab_fill_comparisonequal(Int32, MacroFile, M_E, Int32, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, UInt32, M_E, MacroFile, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, MacroFile, M_E, UInt32, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Int64, M_E, MacroFile, Int64, Bool);
    scilab_fill_comparisonequal(Int64, MacroFile, M_E, Int64, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, UInt64, M_E, MacroFile, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, MacroFile, M_E, UInt64, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, ScalarInt8, M_E, MacroFile, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, MacroFile, M_E, Int8, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarUInt8, M_E, MacroFile, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, MacroFile, M_E, UInt8, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, ScalarInt16, M_E, MacroFile, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, MacroFile, M_E, Int16, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarUInt16, M_E, MacroFile, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, MacroFile, M_E, UInt16, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, ScalarInt32, M_E, MacroFile, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, MacroFile, M_E, Int32, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarUInt32, M_E, MacroFile, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, MacroFile, M_E, UInt32, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, ScalarInt64, M_E, MacroFile, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, MacroFile, M_E, Int64, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarUInt64, M_E, MacroFile, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, MacroFile, M_E, UInt64, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, String, M_E, MacroFile, String, Bool);
    scilab_fill_comparisonequal(String, MacroFile, M_E, String, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarString, M_E, MacroFile, String, Bool);
    scilab_fill_comparisonequal(ScalarString, MacroFile, M_E, String, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, SparseComplex, M_E, MacroFile, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, MacroFile, M_E, Sparse, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Sparse, M_E, MacroFile, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, MacroFile, M_E, Sparse, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, SparseBool, M_E, MacroFile, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, MacroFile, M_E, SparseBool, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Struct, M_E, MacroFile, Struct, Bool);
    scilab_fill_comparisonequal(Struct, MacroFile, M_E, Struct, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, List, M_E, MacroFile, List, Bool);
    scilab_fill_comparisonequal(List, MacroFile, M_E, List, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, TList, M_E, MacroFile, TList, Bool);
    scilab_fill_comparisonequal(TList, MacroFile, M_E, TList, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, MList, M_E, MacroFile, MList, Bool);
    scilab_fill_comparisonequal(MList, MacroFile, M_E, MList, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Cell, M_E, MacroFile, Cell, Bool);
    scilab_fill_comparisonequal(Cell, MacroFile, M_E, Cell, MacroFile, Bool);

    scilab_fill_comparisonequal(MacroFile, Handle, M_E, MacroFile, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, MacroFile, M_E, GraphicHandle, MacroFile, Bool);

    //ScalarHandle
    scilab_fill_comparisonequal(ScalarHandle, ScalarHandle, M_M, GraphicHandle, GraphicHandle, Bool);
    scilab_fill_comparisonequal(Handle, ScalarHandle, M_M, GraphicHandle, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, Handle, M_M, GraphicHandle, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Empty, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Empty, ScalarHandle, E_M, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Double, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Double, ScalarHandle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, DoubleComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(DoubleComplex, ScalarHandle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, ScalarDouble, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(ScalarDouble, ScalarHandle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarDoubleComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(ScalarDoubleComplex, ScalarHandle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Empty, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Empty, ScalarHandle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, Identity, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(Identity, ScalarHandle, M_E, Double, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, IdentityComplex, M_E, GraphicHandle, Double, Bool);
    scilab_fill_comparisonequal(IdentityComplex, ScalarHandle, M_E, Double, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Int8, M_E, GraphicHandle, Int8, Bool);
    scilab_fill_comparisonequal(Int8, ScalarHandle, M_E, Int8, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, UInt8, M_E, GraphicHandle, UInt8, Bool);
    scilab_fill_comparisonequal(UInt8, ScalarHandle, M_E, UInt8, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Int16, M_E, GraphicHandle, Int16, Bool);
    scilab_fill_comparisonequal(Int16, ScalarHandle, M_E, Int16, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, UInt16, M_E, GraphicHandle, UInt16, Bool);
    scilab_fill_comparisonequal(UInt16, ScalarHandle, M_E, UInt16, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Int32, M_E, GraphicHandle, Int32, Bool);
    scilab_fill_comparisonequal(Int32, ScalarHandle, M_E, Int32, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, UInt32, M_E, GraphicHandle, UInt32, Bool);
    scilab_fill_comparisonequal(UInt32, ScalarHandle, M_E, UInt32, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Int64, M_E, GraphicHandle, Int64, Bool);
    scilab_fill_comparisonequal(Int64, ScalarHandle, M_E, Int64, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, UInt64, M_E, GraphicHandle, UInt64, Bool);
    scilab_fill_comparisonequal(UInt64, ScalarHandle, M_E, UInt64, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, ScalarInt8, M_E, GraphicHandle, Int8, Bool);
    scilab_fill_comparisonequal(ScalarInt8, ScalarHandle, M_E, Int8, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarUInt8, M_E, GraphicHandle, UInt8, Bool);
    scilab_fill_comparisonequal(ScalarUInt8, ScalarHandle, M_E, UInt8, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, ScalarInt16, M_E, GraphicHandle, Int16, Bool);
    scilab_fill_comparisonequal(ScalarInt16, ScalarHandle, M_E, Int16, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarUInt16, M_E, GraphicHandle, UInt16, Bool);
    scilab_fill_comparisonequal(ScalarUInt16, ScalarHandle, M_E, UInt16, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, ScalarInt32, M_E, GraphicHandle, Int32, Bool);
    scilab_fill_comparisonequal(ScalarInt32, ScalarHandle, M_E, Int32, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarUInt32, M_E, GraphicHandle, UInt32, Bool);
    scilab_fill_comparisonequal(ScalarUInt32, ScalarHandle, M_E, UInt32, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, ScalarInt64, M_E, GraphicHandle, Int64, Bool);
    scilab_fill_comparisonequal(ScalarInt64, ScalarHandle, M_E, Int64, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarUInt64, M_E, GraphicHandle, UInt64, Bool);
    scilab_fill_comparisonequal(ScalarUInt64, ScalarHandle, M_E, UInt64, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, String, M_E, GraphicHandle, String, Bool);
    scilab_fill_comparisonequal(String, ScalarHandle, M_E, String, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, ScalarString, M_E, GraphicHandle, String, Bool);
    scilab_fill_comparisonequal(ScalarString, ScalarHandle, M_E, String, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, SparseComplex, M_E, GraphicHandle, Sparse, Bool);
    scilab_fill_comparisonequal(SparseComplex, ScalarHandle, M_E, Sparse, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Sparse, M_E, GraphicHandle, Sparse, Bool);
    scilab_fill_comparisonequal(Sparse, ScalarHandle, M_E, Sparse, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, SparseBool, M_E, GraphicHandle, SparseBool, Bool);
    scilab_fill_comparisonequal(SparseBool, ScalarHandle, M_E, SparseBool, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Struct, M_E, GraphicHandle, Struct, Bool);
    scilab_fill_comparisonequal(Struct, ScalarHandle, M_E, Struct, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, List, M_E, GraphicHandle, List, Bool);
    scilab_fill_comparisonequal(List, ScalarHandle, M_E, List, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, TList, M_E, GraphicHandle, TList, Bool);
    scilab_fill_comparisonequal(TList, ScalarHandle, M_E, TList, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, MList, M_E, GraphicHandle, MList, Bool);
    scilab_fill_comparisonequal(MList, ScalarHandle, M_E, MList, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Cell, M_E, GraphicHandle, Cell, Bool);
    scilab_fill_comparisonequal(Cell, ScalarHandle, M_E, Cell, GraphicHandle, Bool);

    scilab_fill_comparisonequal(ScalarHandle, Macro, M_E, GraphicHandle, Macro, Bool);
    scilab_fill_comparisonequal(Macro, ScalarHandle, M_E, Macro, GraphicHandle, Bool);
    scilab_fill_comparisonequal(ScalarHandle, MacroFile, M_E, GraphicHandle, MacroFile, Bool);
    scilab_fill_comparisonequal(MacroFile, ScalarHandle, M_E, MacroFile, GraphicHandle, Bool);

    //UserType
    scilab_fill_comparisonequal(UserType, UserType, UT_UT, UserType, UserType, Bool);

#undef scilab_fill_comparisonequal

}

InternalType *GenericComparisonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    compequal_function compequal = pComparisonEqualfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (compequal)
    {
        pResult = compequal(_pLeftOperand, _pRightOperand);
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


//Matrix == x
template<class T, class U, class O>
InternalType* compequal_M_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compequal(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compequal(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compequal(_pL->get(), (size_t)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compequal(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_E(T *_pL, U *_pR)
{
    // Try to find an overload and call it if exists
    types::typed_list tmp;
    tmp.push_back(_pL);
    tmp.push_back(_pR);
    std::wstring overloadName(Overload::buildOverloadName(Overload::getNameFromOper(ast::OpExp::eq), tmp, 1, true));
    types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(overloadName));
    if (pIT)
    {
        return NULL;
    }
    else
    {
        return new Bool(false);
    }
}

template<class T, class U, class O>
InternalType* compequal_E_M(T *_pL, U *_pR)
{
    return compequal_M_E<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* compequal_E_E(T *_pL, U *_pR)
{
    return new Bool(true);
}

//B == x
template<class T, class U, class O>
InternalType* compequal_B_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();
    int* piDimsR = _pR->getDimsArray();

    if (_pL->isScalar() == false)
    {
        if (iDimsL != iDimsR)
        {
            return new Bool(false);
        }

        int* piDimsL = _pL->getDimsArray();

        for (int i = 0; i < iDimsL; ++i)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                return new Bool(false);
            }
        }
    }

    O* pOut = new O(iDimsR, piDimsR);

    pOut->setFalse();

    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_M_B(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();
    int* piDimsL = _pL->getDimsArray();

    if (_pR->isScalar() == false)
    {
        if (iDimsL != iDimsR)
        {
            return new Bool(false);
        }

        int* piDimsR = _pR->getDimsArray();

        for (int i = 0; i < iDimsL; ++i)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                return new Bool(false);
            }
        }
    }

    O* pOut = new O(iDimsL, piDimsL);

    pOut->setFalse();

    return pOut;
}

//B == x
template<class T, class U, class O>
InternalType* compequal_B_S(T *_pL, U *_pR)
{

    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());

    pOut->setFalse();

    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_B(T *_pL, U *_pR)
{

    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());

    pOut->setFalse();

    return pOut;
}



//Matrix complex == x
template<class T, class U, class O>
InternalType* compequal_MC_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MC_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MC_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MC_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

//Scalar == x
template<class T, class U, class O>
InternalType* compequal_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compequal(_pL->get(0), (size_t)pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compequal(_pL->get(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compequal(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_I(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_S_IC(T *_pL, U *_pR)
{

    O* pOut = new  Bool(false);
    compequal(_pL->get(0), 1, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}
//Scalar complex == x
template<class T, class U, class O>
InternalType* compequal_SC_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SC_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SC_S(T *_pL, U *_pR)
{
    O* pOut = new O(false);
    if (_pL->getImg(0) == 0)
    {
        compequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(true);
    compequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(false);
    if (_pL->getImg(0) == 0)
    {
        compequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(false);
    compequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

//Identity == x
template<class T, class U, class O>
InternalType* compequal_I_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compequal(pIdentity->get(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_I_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compequal(pIdentity->get(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_I_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_I_SC(T *_pL, U *_pR)
{
    return compequal_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* compequal_I_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_I_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(false);
    if (_pR->getImg(0) == 0)
    {
        compequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

//Identity complex == x
template<class T, class U, class O>
InternalType* compequal_IC_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_IC_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_IC_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(false);
    if (_pL->getImg(0) == 0)
    {
        compequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_IC_SC(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_IC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(false);
    if (_pL->getImg(0) == 0)
    {
        compequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_IC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<>
InternalType* compequal_M_M<SparseBool, SparseBool, SparseBool>(SparseBool* _pL, SparseBool* _pR)
{
    //pending changes
    if (((_pL->getRows() != _pR->getRows()) || (_pL->getCols() != _pR->getCols())) && (_pL->getSize() != 1) && (_pR->getSize() != 1))
    {
        return new Bool(false);
    }

    return _pR->newEqualTo(*_pL);
}

template<>
InternalType* compequal_M_M<Sparse, Sparse, SparseBool>(Sparse* _pL, Sparse* _pR)
{
    //pending changes
    if (((_pL->getRows() != _pR->getRows()) || (_pL->getCols() != _pR->getCols())) && (_pL->getSize() != 1) && (_pR->getSize() != 1))
    {
        return new Bool(false);
    }

    return _pR->newEqualTo(*_pL);
}

//[] == SP
template<>
InternalType* compequal_M_M<Double, Sparse, Bool>(Double* _pL, Sparse* _pR)
{
    return new Bool(false);
}

//SP == []
template<>
InternalType* compequal_M_M<Sparse, Double, Bool>(Sparse* _pL, Double* _pR)
{
    return new Bool(false);
}

//sparse
template<class T, class U, class O>
InternalType* compequal_M_SP(T* _pL, U* _pR)
{
    //pending changes

    //D -> SP == SP
    Sparse* pspConvert = NULL;
    types::SparseBool* pOut = NULL;


    if (_pL->isScalar())
    {
        int iSizeOut = _pR->getSize();
        if (_pL->isComplex())
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), true);
            std::complex<double> stComplex((double)_pL->get(0), (double)_pL->getImg(0));
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, stComplex, false);
            }
        }
        else
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pL->get(0), false);
            }
        }
    }
    else
    {
        //check dimensions
        if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
        {
            return new Bool(false);
        }

        int iSizeOut = _pR->getSize();
        if (_pL->isComplex())
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), true);

            for (int i = 0; i < iSizeOut; i++)
            {
                std::complex<double> stComplex((double)_pL->get(i), (double)_pL->getImg(i));
                pspConvert->set(i, stComplex, false);
            }
        }
        else
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pL->get(i), false);
            }
        }
    }

    pspConvert->finalize();
    pOut = _pR->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_SP_M(T* _pL, U* _pR)
{
    //pending changes

    Sparse* pspConvert = NULL;
    types::SparseBool* pOut = NULL;

    if (_pR->isScalar())
    {
        int iSizeOut = _pL->getSize();
        if (_pR->isComplex())
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), true);
            std::complex<double> stComplex((double)_pR->get(0), (double)_pR->getImg(0));
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, stComplex, false);
            }
        }
        else
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pR->get(0), false);
            }
        }
    }
    else
    {
        //check dimensions
        if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
        {
            return new Bool(false);
        }

        int iSizeOut = _pL->getSize();
        if (_pR->isComplex())
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), true);

            for (int i = 0; i < iSizeOut; i++)
            {
                std::complex<double> stComplex((double)_pR->get(i), (double)_pR->getImg(i));
                pspConvert->set(i, stComplex);
            }
        }
        else
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pR->get(i), false);

            }
        }
    }

    pspConvert->finalize();
    pOut = _pL->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;

}

//sparsebool
template<>
InternalType* compequal_M_M<Bool, SparseBool, SparseBool>(Bool* _pL, SparseBool* _pR)
{
    //pending changes

    //D -> SP == SP
    SparseBool* pspConvert = NULL;
    types::SparseBool* pOut = NULL;


    if (_pL->isScalar())
    {
        int iSizeOut = _pR->getSize();

        pspConvert = new SparseBool(_pR->getRows(), _pR->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pL->get(0) == 1, false);
        }

    }
    else
    {
        //check dimensions
        if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
        {
            return new Bool(false);
        }

        int iSizeOut = _pR->getSize();

        pspConvert = new SparseBool(_pR->getRows(), _pR->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pL->get(i) == 1, false);
        }
    }

    pspConvert->finalize();
    pOut = _pR->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;
}

template<>
InternalType* compequal_M_M<SparseBool, Bool, SparseBool>(SparseBool* _pL, Bool* _pR)
{
    //pending changes

    SparseBool* pspConvert = NULL;
    types::SparseBool* pOut = NULL;

    if (_pR->isScalar())
    {
        int iSizeOut = _pL->getSize();

        pspConvert = new SparseBool(_pL->getRows(), _pL->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pR->get(0) == 1, false);
        }

    }
    else
    {
        //check dimensions
        if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
        {
            return new Bool(false);
        }

        int iSizeOut = _pL->getSize();

        pspConvert = new SparseBool(_pL->getRows(), _pL->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pR->get(i) == 1, false);
        }
    }

    pspConvert->finalize();
    pOut = _pL->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;

}

//Polynom
template<>
InternalType* compequal_M_M<Polynom, Polynom, Bool>(Polynom* _pL, Polynom* _pR)
{
    Bool* pbOut = NULL;
    bool bPoise = true;

    SinglePoly** pSPL = _pL->get();
    SinglePoly** pSPR = _pR->get();

    if (_pR->isScalar())
    {
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);

                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[0]->get(j), (double)0, pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[0]->get(j), pSPL[i]->get(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
    }
    else if (_pL->isScalar())
    {
        pbOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        int iSize = pbOut->getSize();

        if (pSPR[0]->getSize() != 1)
        {
            if (_pL->isComplex())
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = false;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), pSPR[i]->getImg(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = false;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
            }
            else
            {

                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = false;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compequal(pSPL[0]->get(j), (double)0, pSPR[i]->get(j), pSPR[i]->get(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = false;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                            {
                                compequal(pSPL[0]->get(j), pSPR[i]->get(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }

            }
        }
    }
    else
    {
        //check dimensions
        if (_pL->getDims() != _pR->getDims())
        {
            return new Bool(false);
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                return new Bool(false);
            }
        }

        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPL[i]->get(j), pSPL[i]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = false;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compequal(pSPR[i]->get(j), pSPL[i]->get(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
    }

    return pbOut;
}

template<class T, class U, class O>
InternalType* compequal_P_M(T *_pL, U *_pR)
{
    Bool* pbOut = NULL;

    SinglePoly** pSPR = _pL->get();

    if (_pR->isIdentity())
    {
        Double* pdblEye = NULL;
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), (double)_pR->get(0), (double)_pR->getImg(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)pdblEye->get(i), (double)pdblEye->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), (double)_pR->get(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)pdblEye->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), (double)_pR->get(0), (double)_pR->getImg(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)0, (double)pdblEye->get(i), (double)pdblEye->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), (double)_pR->get(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pdblEye->get(i), &(pbOut->get()[i]));
                    }
                }
            }
        }
        delete pdblEye;
    }
    else if (_pR->isScalar())
    {
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(0), (double)_pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(0), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)0, (double)_pR->get(0), (double)_pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)_pR->get(0), &(pbOut->get()[i]));
                    }
                }
            }
        }
    }
    else if (_pL->isScalar())
    {
        pbOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        int iSize = pbOut->getSize();

        if (pSPR[0]->getSize() == 1)
        {
            if (_pL->isComplex())
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compequal((double)pSPR[0]->get(0), (double)pSPR[0]->getImg(0), (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compequal((double)pSPR[0]->get(0), (double)pSPR[0]->getImg(0), (double)_pR->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compequal((double)pSPR[0]->get(0), (double)0, (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compequal((double)pSPR[0]->get(0), (double)_pR->get(i), &(pbOut->get()[i]));
                    }
                }

            }
        }
        else
        {
            pbOut->setFalse();
        }

    }
    else
    {
        //check dimensions
        if (_pL->getDims() != _pR->getDims())
        {
            return new Bool(false);
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                return new Bool(false);
            }
        }

        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)0, (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, false);
                    }
                    else
                    {
                        compequal((double)pSPR[i]->get(0), (double)_pR->get(i), &(pbOut->get()[i]));
                    }
                }
            }
        }
    }

    return pbOut;
}

template<class T, class U, class O>
InternalType* compequal_M_P(T *_pL, U *_pR)
{
    return GenericComparisonEqual(_pR, _pL);
}

template<>
InternalType* compequal_M_M<String, String, Bool>(String* _pL, String* _pR)
{

    if (_pL->isScalar())
    {
        Bool*  pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        for (int i = 0; i < _pR->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(0), _pR->get(i)) == 0);
        }
        return pOut;
    }

    if (_pR->isScalar())
    {
        Bool*  pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(0)) == 0);
        }
        return pOut;
    }

    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();


    if (iDimsL != iDimsR)
    {
        return new Bool(false);
    }


    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }


    if (_pL->getSize() == _pR->getSize())
    {
        Bool*  pOut = new Bool(iDimsL, piDimsL);

        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(i)) == 0);
        }
        return pOut;
    }
    return NULL;
}

template<>
types::InternalType* compequal_M_M<Struct, Struct, Bool>(types::Struct* _pL, types::Struct* _pR)
{
    /* check dimension*/
    if (_pL->getDims() != _pR->getDims())
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    if (_pL->getSize() == 0)
    {
        return new Bool(true);
    }

    Bool *pOut = new Bool(_pL->getDims(), piDimsL);
    for (int i = 0; i < _pL->getSize(); i++)
    {
        pOut->set(i, *_pL->get(i) == *_pR->get(i));
    }
    return pOut;
}

//List, TList, MList
template<class T, class U, class O>
InternalType* compequal_LT_LT(T *_pL, U *_pR)
{
    if (_pL->getType() != GenericType::ScilabList || _pR->getType() != GenericType::ScilabList)
    {
        //try to find overload function, if symbol exist, return NULL to let opexep to call it.
        //otherwise do a "binary" comparison
        types::typed_list in;
        in.push_back(_pL);
        in.push_back(_pR);
        std::wstring overloadName(Overload::buildOverloadName(Overload::getNameFromOper(ast::OpExp::eq), in, 1, true));
        types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(overloadName));
        if (pIT)
        {
            return NULL;
        }
    }

    if (_pL->getSize() != _pR->getSize())
    {
        return new Bool(false);
    }

    if (_pL->getSize() == 0 && _pR->getSize() == 0)
    {
        return new Bool(true);
    }

    Bool* pB = new Bool(1, _pL->getSize());
    for (int i = 0; i < _pL->getSize(); i++)
    {
        pB->set(i, ((*_pL->get(i) == *_pR->get(i)) && (_pL->get(i)->getType() != types::InternalType::ScilabVoid)));
    }

    return pB;
}

template<>
types::InternalType* compequal_M_M<Cell, Cell, Bool>(types::Cell* _pL, types::Cell* _pR)
{
    /* check dimension*/
    if (_pL->getDims() != _pR->getDims())
    {
        return new Bool(false);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(false);
        }
    }

    if (_pL->getSize() == 0)
    {
        //{} == {} -> return true
        return new Bool(true);
    }

    Bool *pB = new Bool(_pL->getDims(), piDimsL);
    for (int i = 0; i < _pL->getSize(); i++)
    {
        pB->set(i, *_pL->get(i) == *_pR->get(i));
    }

    return pB;
}

template<>
types::InternalType* compequal_M_M<GraphicHandle, GraphicHandle, Bool>(GraphicHandle* _pL, GraphicHandle* _pR)
{
    if (_pL->isScalar())
    {
        Bool* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        compequal(_pL->get(0), _pR->getSize(), _pR->get(), pOut->get());
        return pOut;
    }

    if (_pR->isScalar())
    {
        Bool* pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        compequal(_pR->get(0), _pL->getSize(), _pL->get(), pOut->get());
        return pOut;
    }

    /* check dimension*/
    if (_pL->getDims() != _pR->getDims())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    Bool* pOut = new Bool(_pL->getDims(), piDimsL);
    int iSize = pOut->getSize();

    compequal(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compequal_MCR_MCR(T *_pL, U *_pR)
{
    bool ret = false;
    if (_pL->getType() == GenericType::ScilabMacroFile)
    {
        types::InternalType* pIT = ((InternalType *) _pL);
        types::MacroFile* pL = pIT->getAs<types::MacroFile>();
        ret = *pL == *_pR;
    }
    else if (_pL->getType() == GenericType::ScilabMacro)
    {
        types::InternalType* pIT = ((InternalType *) _pL);
        types::Macro* pL = pIT->getAs<types::Macro>();
        if (_pR->getType() == GenericType::ScilabMacroFile)
        {
            types::InternalType* pIT = ((InternalType *) _pR);
            types::MacroFile* pR = pIT->getAs<types::MacroFile>();
            ret = *pR == *pL;
        }
        else
        {
            ret = *pL == *_pR;
        }
    }

    return new Bool(ret);
}

//UserType
template<class T, class U, class O>
InternalType* compequal_UT_UT(T *_pL, U *_pR)
{
    return _pL->equal(_pR);
}
