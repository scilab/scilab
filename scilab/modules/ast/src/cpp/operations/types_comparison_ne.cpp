/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_comparison_ne.hxx"
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

using namespace types;

//define arrays on operation functions
static compnoequal_function pComparisonEqualfunction[types::InternalType::IdLast][types::InternalType::IdLast] = { NULL };


void fillComparisonNoEqualFunction()
{
#define scilab_fill_comparison_no_equal(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pComparisonEqualfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (compnoequal_function)&compnoequal_##func<typeIn1, typeIn2, typeOut>

    //String != String
    scilab_fill_comparison_no_equal(String, String, M_M, String, String, Bool);
    scilab_fill_comparison_no_equal(String, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, String, M_M, String, String, Bool);
    //String != Empty
    scilab_fill_comparison_no_equal(Empty, ScalarString, E_M, String, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Empty, M_E, String, String, Bool);
    scilab_fill_comparison_no_equal(String, Empty, M_E, String, String, Bool);
    scilab_fill_comparison_no_equal(Empty, String, E_M, String, String, Bool);
    //String != Double
    scilab_fill_comparison_no_equal(Double, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Double, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, Double, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(Double, String, M_E, Double, String, Bool);
    //String != ScalarDouble
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, String, M_E, Double, String, Bool);
    //String != Int8
    scilab_fill_comparison_no_equal(Int8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparison_no_equal(String, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparison_no_equal(Int8, String, M_E, Int8, String, Bool);
    //String != UInt8
    scilab_fill_comparison_no_equal(UInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_no_equal(String, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt8, String, M_E, UInt8, String, Bool);
    //String != Int16
    scilab_fill_comparison_no_equal(Int16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparison_no_equal(String, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparison_no_equal(Int16, String, M_E, Int16, String, Bool);
    //String != UInt16
    scilab_fill_comparison_no_equal(UInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_no_equal(String, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt16, String, M_E, UInt16, String, Bool);
    //String != Int32
    scilab_fill_comparison_no_equal(Int32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparison_no_equal(String, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparison_no_equal(Int32, String, M_E, Int32, String, Bool);
    //String != UInt32
    scilab_fill_comparison_no_equal(UInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_no_equal(String, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt32, String, M_E, UInt32, String, Bool);
    //String != Int64
    scilab_fill_comparison_no_equal(Int64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparison_no_equal(String, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparison_no_equal(Int64, String, M_E, Int64, String, Bool);
    //String != UInt64
    scilab_fill_comparison_no_equal(UInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_no_equal(String, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt64, String, M_E, UInt64, String, Bool);
    //String != Bool
    scilab_fill_comparison_no_equal(Bool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparison_no_equal(String, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparison_no_equal(Bool, String, M_E, Bool, String, Bool);
    //String != Polynom
    scilab_fill_comparison_no_equal(Polynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(Polynom, String, M_E, Polynom, String, Bool);
    //String != PolynomComplex
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, String, M_E, Polynom, String, Bool);
    //String != ScalarPolynom
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String != ScalarPolynomComplex
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String != Sparse
    scilab_fill_comparison_no_equal(Sparse, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparison_no_equal(String, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparison_no_equal(Sparse, String, M_E, Sparse, String, Bool);
    //String != SparseBool
    scilab_fill_comparison_no_equal(SparseBool, ScalarString, M_E, SparseBool, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparison_no_equal(String, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, String, M_E, SparseBool, String, Bool);
    //String != ScalarInt8
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparison_no_equal(String, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, String, M_E, Int8, String, Bool);
    //String != ScalarUInt8
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_no_equal(String, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, String, M_E, UInt8, String, Bool);
    //String != ScalarInt16
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparison_no_equal(String, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, String, M_E, Int16, String, Bool);
    //String != ScalarUInt16
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_no_equal(String, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, String, M_E, UInt16, String, Bool);
    //String != ScalarInt32
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparison_no_equal(String, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, String, M_E, Int32, String, Bool);
    //String != ScalarUInt32
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_no_equal(String, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, String, M_E, UInt32, String, Bool);
    //String != ScalarInt64
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparison_no_equal(String, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, String, M_E, Int64, String, Bool);
    //String != ScalarUInt64
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_no_equal(String, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, String, M_E, UInt64, String, Bool);
    //String != ScalarBool
    scilab_fill_comparison_no_equal(ScalarBool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparison_no_equal(String, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, String, M_E, Bool, String, Bool);
    //String != ScalarPolynom
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String != ScalarDoubleComplex
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, String, M_E, Double, String, Bool);
    //String != DoubleComplex
    scilab_fill_comparison_no_equal(DoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, String, M_E, Double, String, Bool);
    //String != ScalarPolynomComplex
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String != SparseComplex
    scilab_fill_comparison_no_equal(SparseComplex, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparison_no_equal(String, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, String, M_E, Sparse, String, Bool);
    //String != Identity
    scilab_fill_comparison_no_equal(Identity, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Identity, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, Identity, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, String, M_E, Double, String, Bool);
    //String != IdentityComplex
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(String, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, String, M_E, Double, String, Bool);

    //Double
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Double, Double, M_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Double, Int8, M_M, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(Double, UInt8, M_M, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(Double, Int16, M_M, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(Double, UInt16, M_M, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(Double, Int32, M_M, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(Double, UInt32, M_M, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(Double, Int64, M_M, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(Double, UInt64, M_M, Double, UInt64, Bool);
    scilab_fill_comparison_no_equal(Double, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(Double, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Double, Sparse, M_SP, Double, Sparse, SparseBool);

    //Matrix != Matrix Complex
    scilab_fill_comparison_no_equal(Double, DoubleComplex, M_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Double, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Double, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Double, ScalarDouble, M_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarInt8, M_S, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarUInt8, M_S, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarInt16, M_S, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarUInt16, M_S, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarInt32, M_S, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarUInt32, M_S, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarInt64, M_S, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarUInt64, M_S, Double, UInt64, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarPolynom, M_P, Double, Polynom, Bool);

    //Matrix != Scalar Complex
    scilab_fill_comparison_no_equal(Double, ScalarDoubleComplex, M_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Double, ScalarPolynomComplex, M_P, Double, Polynom, Bool);

    //Matrix != Empty
    scilab_fill_comparison_no_equal(Double, Empty, M_E, Double, Double, Bool);


    //Matrix Complex != Matrix
    scilab_fill_comparison_no_equal(DoubleComplex, Double, MC_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, ScalarDouble, MC_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, Empty, M_E, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, Sparse, M_SP, Double, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(DoubleComplex, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarDouble, Double, S_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Int8, S_M, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, UInt8, S_M, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Int16, S_M, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, UInt16, S_M, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Int32, S_M, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, UInt32, S_M, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Int64, S_M, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, UInt64, S_M, Double, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Sparse, M_SP, Double, Sparse, SparseBool);

    //Scalar != Matrix Complex
    scilab_fill_comparison_no_equal(ScalarDouble, DoubleComplex, S_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, SparseComplex, M_SP, Double, Sparse, SparseBool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarDouble, S_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarInt8, S_S, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarInt16, S_S, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarInt32, S_S, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarInt64, S_S, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarPolynom, M_P, Double, Polynom, Bool);

    //Scalar != Scalar Complex
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, ScalarPolynomComplex, M_P, Double, Polynom, Bool);

    //Scalar != Empty
    scilab_fill_comparison_no_equal(ScalarDouble, Empty, M_E, Double, Double, Bool);

    //Scalar Complex != Matrix
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Double, SC_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Sparse, M_SP, Double, Sparse, SparseBool);
    //Scalar Complex != Matrix Complex
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, SparseComplex, M_SP, Double, Sparse, SparseBool);
    //Scalar Complex != Scalar
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    //Scalar Complex != Scalar Complex
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    //Scalar Complex != Empty
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Empty, M_E, Double, Double, Bool);

    //Empty != Matrix
    scilab_fill_comparison_no_equal(Empty, Double, E_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, Int8, E_M, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(Empty, UInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(Empty, Int16, E_M, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(Empty, UInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(Empty, Int32, E_M, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(Empty, UInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(Empty, Int64, E_M, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(Empty, UInt64, E_M, Double, UInt64, Bool);

    scilab_fill_comparison_no_equal(Empty, Polynom, E_M, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Empty, PolynomComplex, E_M, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Empty, Sparse, E_M, Double, Sparse, Bool);
    scilab_fill_comparison_no_equal(Empty, SparseComplex, E_M, Double, Sparse, Bool);

    //Empty != Matrix Complex
    scilab_fill_comparison_no_equal(Empty, DoubleComplex, E_M, Double, Double, Bool);
    //Empty != Scalar
    scilab_fill_comparison_no_equal(Empty, ScalarDouble, E_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarInt8, E_M, Double, Int8, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarUInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarInt16, E_M, Double, Int16, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarUInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarInt32, E_M, Double, Int32, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarUInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarInt64, E_M, Double, Int64, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarUInt64, E_M, Double, UInt64, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarBool, E_M, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarPolynom, E_M, Double, Polynom, Bool);

    //Empty != Scalar Complex
    scilab_fill_comparison_no_equal(Empty, ScalarDoubleComplex, E_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Bool);
    //Empty != Empty
    scilab_fill_comparison_no_equal(Empty, Empty, E_E, Double, Double, Bool);
    //Empty != eye
    scilab_fill_comparison_no_equal(Empty, Identity, E_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, IdentityComplex, E_M, Double, Double, Bool);

    //Matrix != Identity
    scilab_fill_comparison_no_equal(Double, Identity, M_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Double, IdentityComplex, M_IC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, Identity, MC_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Identity, S_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, IdentityComplex, S_IC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Identity, SC_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Bool);

    //Int8
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Int8, Double, M_M, Int8, Double, Bool);
    scilab_fill_comparison_no_equal(Int8, Int8, M_M, Int8, Int8, Bool);
    scilab_fill_comparison_no_equal(Int8, UInt8, M_M, Int8, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int8, Int16, M_M, Int8, Int16, Bool);
    scilab_fill_comparison_no_equal(Int8, UInt16, M_M, Int8, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int8, Int32, M_M, Int8, Int32, Bool);
    scilab_fill_comparison_no_equal(Int8, UInt32, M_M, Int8, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int8, Int64, M_M, Int8, Int64, Bool);
    scilab_fill_comparison_no_equal(Int8, UInt64, M_M, Int8, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int8, Bool, M_B, Int8, Bool, Bool);
    scilab_fill_comparison_no_equal(Int8, Empty, M_E, Int8, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Int8, ScalarDouble, M_S, Int8, Double, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarInt8, M_S, Int8, Int8, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarUInt8, M_S, Int8, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarInt16, M_S, Int8, Int16, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarUInt16, M_S, Int8, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarInt32, M_S, Int8, Int32, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarUInt32, M_S, Int8, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarInt64, M_S, Int8, Int64, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarUInt64, M_S, Int8, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarBool, M_B, Int8, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarInt8, Double, S_M, Int8, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Int8, S_M, Int8, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, UInt8, S_M, Int8, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Int16, S_M, Int8, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, UInt16, S_M, Int8, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Int32, S_M, Int8, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, UInt32, S_M, Int8, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Int64, S_M, Int8, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, UInt64, S_M, Int8, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Bool, S_B, Int8, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Empty, M_E, Int8, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarDouble, S_S, Int8, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarBool, M_E, Int8, Bool, Bool);

    //UInt8
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(UInt8, Double, M_M, UInt8, Double, Bool);
    scilab_fill_comparison_no_equal(UInt8, Int8, M_M, UInt8, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt8, UInt8, M_M, UInt8, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt8, Int16, M_M, UInt8, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt8, UInt16, M_M, UInt8, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt8, Int32, M_M, UInt8, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt8, UInt32, M_M, UInt8, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt8, Int64, M_M, UInt8, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt8, UInt64, M_M, UInt8, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt8, Bool, M_B, UInt8, Bool, Bool);
    scilab_fill_comparison_no_equal(UInt8, Empty, M_E, UInt8, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(UInt8, ScalarDouble, M_S, UInt8, Double, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarInt8, M_S, UInt8, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarUInt8, M_S, UInt8, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarInt16, M_S, UInt8, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarUInt16, M_S, UInt8, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarInt32, M_S, UInt8, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarUInt32, M_S, UInt8, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarInt64, M_S, UInt8, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarUInt64, M_S, UInt8, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarBool, M_B, UInt8, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarUInt8, Double, S_M, UInt8, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Int8, S_M, UInt8, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, UInt8, S_M, UInt8, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Int16, S_M, UInt8, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, UInt16, S_M, UInt8, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Int32, S_M, UInt8, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, UInt32, S_M, UInt8, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Int64, S_M, UInt8, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, UInt64, S_M, UInt8, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Bool, S_B, UInt8, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Empty, M_E, UInt8, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarBool, M_E, UInt8, Bool, Bool);

    //Int16
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Int16, Double, M_M, Int16, Double, Bool);
    scilab_fill_comparison_no_equal(Int16, Int8, M_M, Int16, Int8, Bool);
    scilab_fill_comparison_no_equal(Int16, UInt8, M_M, Int16, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int16, Int16, M_M, Int16, Int16, Bool);
    scilab_fill_comparison_no_equal(Int16, UInt16, M_M, Int16, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int16, Int32, M_M, Int16, Int32, Bool);
    scilab_fill_comparison_no_equal(Int16, UInt32, M_M, Int16, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int16, Int64, M_M, Int16, Int64, Bool);
    scilab_fill_comparison_no_equal(Int16, UInt64, M_M, Int16, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int16, Bool, M_B, Int16, Bool, Bool);
    scilab_fill_comparison_no_equal(Int16, Empty, M_E, Int16, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Int16, ScalarDouble, M_S, Int16, Double, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarInt8, M_S, Int16, Int8, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarUInt8, M_S, Int16, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarInt16, M_S, Int16, Int16, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarUInt16, M_S, Int16, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarInt32, M_S, Int16, Int32, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarUInt32, M_S, Int16, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarInt64, M_S, Int16, Int64, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarUInt64, M_S, Int16, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarBool, M_B, Int16, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarInt16, Double, S_M, Int16, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Int8, S_M, Int16, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, UInt8, S_M, Int16, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Int16, S_M, Int16, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, UInt16, S_M, Int16, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Int32, S_M, Int16, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, UInt32, S_M, Int16, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Int64, S_M, Int16, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, UInt64, S_M, Int16, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Bool, S_B, Int16, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Empty, M_E, Int16, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarDouble, S_S, Int16, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarBool, M_E, Int16, Bool, Bool);

    //UInt16
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(UInt16, Double, M_M, UInt16, Double, Bool);
    scilab_fill_comparison_no_equal(UInt16, Int8, M_M, UInt16, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt16, UInt8, M_M, UInt16, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt16, Int16, M_M, UInt16, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt16, UInt16, M_M, UInt16, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt16, Int32, M_M, UInt16, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt16, UInt32, M_M, UInt16, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt16, Int64, M_M, UInt16, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt16, UInt64, M_M, UInt16, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt16, Bool, M_B, UInt16, Bool, Bool);
    scilab_fill_comparison_no_equal(UInt16, Empty, M_E, UInt16, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(UInt16, ScalarDouble, M_S, UInt16, Double, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarInt8, M_S, UInt16, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarUInt8, M_S, UInt16, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarInt16, M_S, UInt16, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarUInt16, M_S, UInt16, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarInt32, M_S, UInt16, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarUInt32, M_S, UInt16, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarInt64, M_S, UInt16, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarUInt64, M_S, UInt16, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarBool, M_B, UInt16, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarUInt16, Double, S_M, UInt16, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Int8, S_M, UInt16, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, UInt8, S_M, UInt16, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Int16, S_M, UInt16, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, UInt16, S_M, UInt16, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Int32, S_M, UInt16, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, UInt32, S_M, UInt16, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Int64, S_M, UInt16, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, UInt64, S_M, UInt16, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Bool, S_B, UInt16, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Empty, M_E, UInt16, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarBool, M_E, UInt16, Bool, Bool);

    //Int32
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Int32, Double, M_M, Int32, Double, Bool);
    scilab_fill_comparison_no_equal(Int32, Int8, M_M, Int32, Int8, Bool);
    scilab_fill_comparison_no_equal(Int32, UInt8, M_M, Int32, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int32, Int16, M_M, Int32, Int16, Bool);
    scilab_fill_comparison_no_equal(Int32, UInt16, M_M, Int32, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int32, Int32, M_M, Int32, Int32, Bool);
    scilab_fill_comparison_no_equal(Int32, UInt32, M_M, Int32, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int32, Int64, M_M, Int32, Int64, Bool);
    scilab_fill_comparison_no_equal(Int32, UInt64, M_M, Int32, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int32, Bool, M_B, Int32, Bool, Bool);
    scilab_fill_comparison_no_equal(Int32, Empty, M_E, Int32, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Int32, ScalarDouble, M_S, Int32, Double, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarInt8, M_S, Int32, Int8, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarUInt8, M_S, Int32, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarInt16, M_S, Int32, Int16, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarUInt16, M_S, Int32, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarInt32, M_S, Int32, Int32, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarUInt32, M_S, Int32, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarInt64, M_S, Int32, Int64, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarUInt64, M_S, Int32, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarBool, M_B, Int32, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarInt32, Double, S_M, Int32, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Int8, S_M, Int32, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, UInt8, S_M, Int32, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Int16, S_M, Int32, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, UInt16, S_M, Int32, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Int32, S_M, Int32, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, UInt32, S_M, Int32, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Int64, S_M, Int32, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, UInt64, S_M, Int32, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Bool, S_B, Int32, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Empty, M_E, Int32, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarDouble, S_S, Int32, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarBool, M_E, Int32, Bool, Bool);

    //UInt32
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(UInt32, Double, M_M, UInt32, Double, Bool);
    scilab_fill_comparison_no_equal(UInt32, Int8, M_M, UInt32, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt32, UInt8, M_M, UInt32, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt32, Int16, M_M, UInt32, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt32, UInt16, M_M, UInt32, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt32, Int32, M_M, UInt32, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt32, UInt32, M_M, UInt32, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt32, Int64, M_M, UInt32, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt32, UInt64, M_M, UInt32, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt32, Bool, M_B, UInt32, Bool, Bool);
    scilab_fill_comparison_no_equal(UInt32, Empty, M_E, UInt32, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(UInt32, ScalarDouble, M_S, UInt32, Double, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarInt8, M_S, UInt32, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarUInt8, M_S, UInt32, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarInt16, M_S, UInt32, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarUInt16, M_S, UInt32, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarInt32, M_S, UInt32, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarUInt32, M_S, UInt32, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarInt64, M_S, UInt32, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarUInt64, M_S, UInt32, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarBool, M_B, UInt32, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarUInt32, Double, S_M, UInt32, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Int8, S_M, UInt32, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, UInt8, S_M, UInt32, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Int16, S_M, UInt32, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, UInt16, S_M, UInt32, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Int32, S_M, UInt32, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, UInt32, S_M, UInt32, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Int64, S_M, UInt32, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, UInt64, S_M, UInt32, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Bool, S_B, UInt32, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Empty, M_E, UInt32, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarBool, M_E, UInt32, Bool, Bool);

    //Int64
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Int64, Double, M_M, Int64, Double, Bool);
    scilab_fill_comparison_no_equal(Int64, Int8, M_M, Int64, Int8, Bool);
    scilab_fill_comparison_no_equal(Int64, UInt8, M_M, Int64, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int64, Int16, M_M, Int64, Int16, Bool);
    scilab_fill_comparison_no_equal(Int64, UInt16, M_M, Int64, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int64, Int32, M_M, Int64, Int32, Bool);
    scilab_fill_comparison_no_equal(Int64, UInt32, M_M, Int64, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int64, Int64, M_M, Int64, Int64, Bool);
    scilab_fill_comparison_no_equal(Int64, UInt64, M_M, Int64, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int64, Bool, M_B, Int64, Bool, Bool);
    scilab_fill_comparison_no_equal(Int64, Empty, M_E, Int64, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Int64, ScalarDouble, M_S, Int64, Double, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarInt8, M_S, Int64, Int8, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarUInt8, M_S, Int64, UInt8, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarInt16, M_S, Int64, Int16, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarUInt16, M_S, Int64, UInt16, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarInt32, M_S, Int64, Int32, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarUInt32, M_S, Int64, UInt32, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarInt64, M_S, Int64, Int64, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarUInt64, M_S, Int64, UInt64, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarBool, M_B, Int64, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarInt64, Double, S_M, Int64, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Int8, S_M, Int64, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, UInt8, S_M, Int64, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Int16, S_M, Int64, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, UInt16, S_M, Int64, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Int32, S_M, Int64, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, UInt32, S_M, Int64, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Int64, S_M, Int64, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, UInt64, S_M, Int64, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Bool, S_B, Int64, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Empty, M_E, Int64, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarDouble, S_S, Int64, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarBool, M_E, Int64, Bool, Bool);

    //UInt64
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(UInt64, Double, M_M, UInt64, Double, Bool);
    scilab_fill_comparison_no_equal(UInt64, Int8, M_M, UInt64, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt64, UInt8, M_M, UInt64, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt64, Int16, M_M, UInt64, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt64, UInt16, M_M, UInt64, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt64, Int32, M_M, UInt64, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt64, UInt32, M_M, UInt64, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt64, Int64, M_M, UInt64, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt64, UInt64, M_M, UInt64, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt64, Bool, M_B, UInt64, Bool, Bool);
    scilab_fill_comparison_no_equal(UInt64, Empty, M_E, UInt64, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(UInt64, ScalarDouble, M_S, UInt64, Double, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarInt8, M_S, UInt64, Int8, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarUInt8, M_S, UInt64, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarInt16, M_S, UInt64, Int16, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarUInt16, M_S, UInt64, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarInt32, M_S, UInt64, Int32, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarUInt32, M_S, UInt64, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarInt64, M_S, UInt64, Int64, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarUInt64, M_S, UInt64, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarBool, M_B, UInt64, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarUInt64, Double, S_M, UInt64, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Int8, S_M, UInt64, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, UInt8, S_M, UInt64, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Int16, S_M, UInt64, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, UInt16, S_M, UInt64, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Int32, S_M, UInt64, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, UInt32, S_M, UInt64, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Int64, S_M, UInt64, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, UInt64, S_M, UInt64, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Bool, S_B, UInt64, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Empty, M_E, UInt64, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarBool, M_E, UInt64, Bool, Bool);

    //Bool
    //Matrix != Matrix
    scilab_fill_comparison_no_equal(Bool, Double, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(Bool, Int8, B_M, Bool, Int8, Bool);
    scilab_fill_comparison_no_equal(Bool, UInt8, B_M, Bool, UInt8, Bool);
    scilab_fill_comparison_no_equal(Bool, Int16, B_M, Bool, Int16, Bool);
    scilab_fill_comparison_no_equal(Bool, UInt16, B_M, Bool, UInt16, Bool);
    scilab_fill_comparison_no_equal(Bool, Int32, B_M, Bool, Int32, Bool);
    scilab_fill_comparison_no_equal(Bool, UInt32, B_M, Bool, UInt32, Bool);
    scilab_fill_comparison_no_equal(Bool, Int64, B_M, Bool, Int64, Bool);
    scilab_fill_comparison_no_equal(Bool, UInt64, B_M, Bool, UInt64, Bool);
    scilab_fill_comparison_no_equal(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_comparison_no_equal(Bool, Empty, M_E, Bool, Double, Bool);

    //Matrix != Scalar
    scilab_fill_comparison_no_equal(Bool, ScalarDouble, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarInt8, B_S, Bool, Int8, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarUInt8, B_S, Bool, UInt8, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarInt16, B_S, Bool, Int16, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarUInt16, B_S, Bool, UInt16, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarInt32, B_S, Bool, Int32, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarUInt32, B_S, Bool, UInt32, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarInt64, B_S, Bool, Int64, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarUInt64, B_S, Bool, UInt64, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar != Matrix
    scilab_fill_comparison_no_equal(ScalarBool, Double, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Int8, B_M, Bool, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, UInt8, B_M, Bool, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Int16, B_M, Bool, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, UInt16, B_M, Bool, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Int32, B_M, Bool, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, UInt32, B_M, Bool, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Int64, B_M, Bool, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, UInt64, B_M, Bool, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Empty, M_E, Bool, Double, Bool);

    //Scalar != Scalar
    scilab_fill_comparison_no_equal(ScalarBool, ScalarDouble, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarInt8, M_E, Bool, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarUInt8, M_E, Bool, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarInt16, M_E, Bool, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarUInt16, M_E, Bool, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarInt32, M_E, Bool, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarUInt32, M_E, Bool, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarInt64, M_E, Bool, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarUInt64, M_E, Bool, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Bool != Identity
    scilab_fill_comparison_no_equal(Bool, Identity, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(Bool, IdentityComplex, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Identity, M_E, Bool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, IdentityComplex, M_E, Bool, Double, Bool);

    //Bool != Polynom
    scilab_fill_comparison_no_equal(Bool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);

    //Bool != Sparse
    scilab_fill_comparison_no_equal(Bool, Sparse, M_E, Bool, Sparse, Bool);
    scilab_fill_comparison_no_equal(Bool, SparseComplex, M_E, Bool, Sparse, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, Sparse, M_E, Bool, Sparse, Bool);
    scilab_fill_comparison_no_equal(ScalarBool, SparseComplex, M_E, Bool, Sparse, Bool);

    //Identity
    scilab_fill_comparison_no_equal(Identity, Double, I_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, DoubleComplex, I_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, ScalarDouble, I_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, ScalarDoubleComplex, I_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, Identity, I_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, IdentityComplex, I_IC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparison_no_equal(Identity, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Identity, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Identity, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Identity, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(Identity, Sparse, M_E, Double, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(Identity, SparseComplex, M_E, Double, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(IdentityComplex, Double, IC_M, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarDouble, IC_S, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, Identity, IC_I, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparison_no_equal(IdentityComplex, Polynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, PolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarPolynom, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarPolynomComplex, M_P, Double, Polynom, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, Sparse, M_E, Double, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(IdentityComplex, SparseComplex, M_E, Double, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Identity, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, Bool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(Identity, ScalarBool, M_E, Double, Bool, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, ScalarBool, M_E, Double, Bool, Bool);

    //Polynom

    //poly != poly
    scilab_fill_comparison_no_equal(Polynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(Polynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly != scalar poly
    scilab_fill_comparison_no_equal(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly != double
    scilab_fill_comparison_no_equal(Polynom, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(Polynom, DoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, DoubleComplex, P_M, Polynom, Double, Bool);

    //poly != scalar double
    scilab_fill_comparison_no_equal(Polynom, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarDoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarDoubleComplex, P_M, Polynom, Double, Bool);

    //poly != Bool
    scilab_fill_comparison_no_equal(Polynom, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarBool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarBool, E_M, Polynom, Bool, Bool);

    //poly != []
    scilab_fill_comparison_no_equal(Polynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //poly != eye
    scilab_fill_comparison_no_equal(Polynom, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(Polynom, IdentityComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, IdentityComplex, P_M, Polynom, Double, Bool);

    //scalar poly != poly
    scilab_fill_comparison_no_equal(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly != scalar poly
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly != double
    scilab_fill_comparison_no_equal(ScalarPolynom, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, DoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Double, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, DoubleComplex, P_M, Polynom, Double, Bool);

    //scalar poly != scalar double
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarDoubleComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarDouble, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarDoubleComplex, P_M, Polynom, Double, Bool);

    //poly != Bool
    scilab_fill_comparison_no_equal(ScalarPolynom, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarBool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Bool, E_M, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarBool, E_M, Polynom, Bool, Bool);

    //scalar poly != []
    scilab_fill_comparison_no_equal(ScalarPolynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //scalar poly != eye
    scilab_fill_comparison_no_equal(ScalarPolynom, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, IdentityComplex, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Identity, P_M, Polynom, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, IdentityComplex, P_M, Polynom, Double, Bool);

    //poly != Int8
    scilab_fill_comparison_no_equal(Polynom, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(Polynom, UInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, UInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparison_no_equal(Int8, Polynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt8, Polynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int8, PolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt8, PolynomComplex, M_P, UInt8, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, UInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Int8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, UInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparison_no_equal(Int8, ScalarPolynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarPolynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int8, ScalarPolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt8, ScalarPolynomComplex, M_P, UInt8, Polynom, Bool);

    //poly != Int16
    scilab_fill_comparison_no_equal(Polynom, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(Polynom, UInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, UInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparison_no_equal(Int16, Polynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt16, Polynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int16, PolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt16, PolynomComplex, M_P, UInt16, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, UInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Int16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, UInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparison_no_equal(Int16, ScalarPolynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarPolynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int16, ScalarPolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt16, ScalarPolynomComplex, M_P, UInt16, Polynom, Bool);

    //poly != Int32
    scilab_fill_comparison_no_equal(Polynom, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(Polynom, UInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, UInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparison_no_equal(Int32, Polynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt32, Polynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int32, PolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt32, PolynomComplex, M_P, UInt32, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, UInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Int32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, UInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparison_no_equal(Int32, ScalarPolynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarPolynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int32, ScalarPolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt32, ScalarPolynomComplex, M_P, UInt32, Polynom, Bool);

    //poly != Int64
    scilab_fill_comparison_no_equal(Polynom, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(Polynom, UInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, UInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparison_no_equal(Int64, Polynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt64, Polynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int64, PolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt64, PolynomComplex, M_P, UInt64, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, UInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Int64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, UInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparison_no_equal(Int64, ScalarPolynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarPolynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparison_no_equal(Int64, ScalarPolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(UInt64, ScalarPolynomComplex, M_P, UInt64, Polynom, Bool);

    //poly != Int8
    scilab_fill_comparison_no_equal(Polynom, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarUInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarUInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparison_no_equal(ScalarInt8, Polynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Polynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, PolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, PolynomComplex, M_P, UInt8, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarUInt8, P_M, Polynom, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarInt8, P_M, Polynom, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarUInt8, P_M, Polynom, UInt8, Bool);

    scilab_fill_comparison_no_equal(ScalarInt8, ScalarPolynom, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarPolynom, M_P, UInt8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, ScalarPolynomComplex, M_P, Int8, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, ScalarPolynomComplex, M_P, UInt8, Polynom, Bool);

    //poly != Int16
    scilab_fill_comparison_no_equal(Polynom, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarUInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarUInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparison_no_equal(ScalarInt16, Polynom, M_P, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Polynom, M_P, Polynom, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, PolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, PolynomComplex, M_P, UInt16, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarUInt16, P_M, Polynom, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarInt16, P_M, Polynom, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarUInt16, P_M, Polynom, UInt16, Bool);

    scilab_fill_comparison_no_equal(ScalarInt16, ScalarPolynom, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarPolynom, M_P, UInt16, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, ScalarPolynomComplex, M_P, Int16, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, ScalarPolynomComplex, M_P, UInt16, Polynom, Bool);

    //poly != Int32
    scilab_fill_comparison_no_equal(Polynom, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarUInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarUInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparison_no_equal(ScalarInt32, Polynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Polynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, PolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, PolynomComplex, M_P, UInt32, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarUInt32, P_M, Polynom, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarInt32, P_M, Polynom, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarUInt32, P_M, Polynom, UInt32, Bool);

    scilab_fill_comparison_no_equal(ScalarInt32, ScalarPolynom, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarPolynom, M_P, UInt32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, ScalarPolynomComplex, M_P, Int32, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, ScalarPolynomComplex, M_P, UInt32, Polynom, Bool);

    //poly != Int64
    scilab_fill_comparison_no_equal(Polynom, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(Polynom, ScalarUInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, ScalarUInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparison_no_equal(ScalarInt64, Polynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Polynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, PolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, PolynomComplex, M_P, UInt64, Polynom, Bool);

    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, ScalarUInt64, P_M, Polynom, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarInt64, P_M, Polynom, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, ScalarUInt64, P_M, Polynom, UInt64, Bool);

    scilab_fill_comparison_no_equal(ScalarInt64, ScalarPolynom, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarPolynom, M_P, UInt64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, ScalarPolynomComplex, M_P, Int64, Polynom, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, ScalarPolynomComplex, M_P, UInt64, Polynom, Bool);

    //poly != bool
    scilab_fill_comparison_no_equal(Polynom, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(PolynomComplex, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynom, Bool, M_E, Polynom, Bool, Bool);
    scilab_fill_comparison_no_equal(ScalarPolynomComplex, Bool, M_E, Polynom, Bool, Bool);

    scilab_fill_comparison_no_equal(Bool, Polynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, PolynomComplex, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarPolynom, M_E, Bool, Polynom, Bool);
    scilab_fill_comparison_no_equal(Bool, ScalarPolynomComplex, M_E, Bool, Polynom, Bool);

    //Sparse
    scilab_fill_comparison_no_equal(Sparse, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, Double, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, DoubleComplex, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, ScalarDouble, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, ScalarDoubleComplex, SP_M, Sparse, Double, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparison_no_equal(Sparse, Identity, M_E, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(Sparse, IdentityComplex, M_E, Sparse, Double, SparseBool);

    scilab_fill_comparison_no_equal(SparseComplex, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, Double, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, DoubleComplex, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarDouble, SP_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarDoubleComplex, SP_M, Sparse, Double, SparseBool);

    scilab_fill_comparison_no_equal(SparseComplex, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, Identity, M_E, Sparse, Double, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, IdentityComplex, M_E, Sparse, Double, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, Bool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparison_no_equal(Sparse, ScalarBool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, Bool, M_E, Sparse, Bool, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarBool, M_E, Sparse, Bool, Bool);

    scilab_fill_comparison_no_equal(Sparse, Int8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparison_no_equal(Int8, Sparse, M_SP, Int8, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, Int8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparison_no_equal(Int8, SparseComplex, M_SP, Int8, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, UInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparison_no_equal(UInt8, Sparse, M_SP, UInt8, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, UInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparison_no_equal(UInt8, SparseComplex, M_SP, UInt8, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, Int16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparison_no_equal(Int16, Sparse, M_SP, Int16, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, Int16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparison_no_equal(Int16, SparseComplex, M_SP, Int16, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, UInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparison_no_equal(UInt16, Sparse, M_SP, UInt16, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, UInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparison_no_equal(UInt16, SparseComplex, M_SP, UInt16, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, Int32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparison_no_equal(Int32, Sparse, M_SP, Int32, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, Int32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparison_no_equal(Int32, SparseComplex, M_SP, Int32, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, UInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparison_no_equal(UInt32, Sparse, M_SP, UInt32, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, UInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparison_no_equal(UInt32, SparseComplex, M_SP, UInt32, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, Int64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparison_no_equal(Int64, Sparse, M_SP, Int64, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, Int64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparison_no_equal(Int64, SparseComplex, M_SP, Int64, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, UInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparison_no_equal(UInt64, Sparse, M_SP, UInt64, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, UInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparison_no_equal(UInt64, SparseComplex, M_SP, UInt64, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarInt8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt8, Sparse, M_SP, Int8, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarInt8, SP_M, Sparse, Int8, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt8, SparseComplex, M_SP, Int8, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarUInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Sparse, M_SP, UInt8, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarUInt8, SP_M, Sparse, UInt8, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt8, SparseComplex, M_SP, UInt8, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarInt16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt16, Sparse, M_SP, Int16, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarInt16, SP_M, Sparse, Int16, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt16, SparseComplex, M_SP, Int16, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarUInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Sparse, M_SP, UInt16, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarUInt16, SP_M, Sparse, UInt16, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt16, SparseComplex, M_SP, UInt16, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarInt32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt32, Sparse, M_SP, Int32, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarInt32, SP_M, Sparse, Int32, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt32, SparseComplex, M_SP, Int32, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarUInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Sparse, M_SP, UInt32, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarUInt32, SP_M, Sparse, UInt32, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt32, SparseComplex, M_SP, UInt32, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarInt64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt64, Sparse, M_SP, Int64, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarInt64, SP_M, Sparse, Int64, SparseBool);
    scilab_fill_comparison_no_equal(ScalarInt64, SparseComplex, M_SP, Int64, Sparse, SparseBool);

    scilab_fill_comparison_no_equal(Sparse, ScalarUInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Sparse, M_SP, UInt64, Sparse, SparseBool);
    scilab_fill_comparison_no_equal(SparseComplex, ScalarUInt64, SP_M, Sparse, UInt64, SparseBool);
    scilab_fill_comparison_no_equal(ScalarUInt64, SparseComplex, M_SP, UInt64, Sparse, SparseBool);


    //SparseBool
    scilab_fill_comparison_no_equal(SparseBool, SparseBool, M_M, SparseBool, SparseBool, SparseBool);
    scilab_fill_comparison_no_equal(SparseBool, Bool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_comparison_no_equal(Bool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarBool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_comparison_no_equal(ScalarBool, SparseBool, M_M, Bool, SparseBool, SparseBool);

    scilab_fill_comparison_no_equal(SparseBool, Double, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(Double, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, DoubleComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, ScalarDouble, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarDoubleComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Empty, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, Identity, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, SparseBool, M_E, Double, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, IdentityComplex, M_E, SparseBool, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, SparseBool, M_E, Double, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Int8, M_E, SparseBool, Int8, Bool);
    scilab_fill_comparison_no_equal(Int8, SparseBool, M_E, Int8, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, UInt8, M_E, SparseBool, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt8, SparseBool, M_E, UInt8, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Int16, M_E, SparseBool, Int16, Bool);
    scilab_fill_comparison_no_equal(Int16, SparseBool, M_E, Int16, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, UInt16, M_E, SparseBool, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt16, SparseBool, M_E, UInt16, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Int32, M_E, SparseBool, Int32, Bool);
    scilab_fill_comparison_no_equal(Int32, SparseBool, M_E, Int32, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, UInt32, M_E, SparseBool, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt32, SparseBool, M_E, UInt32, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Int64, M_E, SparseBool, Int64, Bool);
    scilab_fill_comparison_no_equal(Int64, SparseBool, M_E, Int64, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, UInt64, M_E, SparseBool, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt64, SparseBool, M_E, UInt64, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, ScalarInt8, M_E, SparseBool, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, SparseBool, M_E, Int8, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarUInt8, M_E, SparseBool, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, SparseBool, M_E, UInt8, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, ScalarInt16, M_E, SparseBool, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, SparseBool, M_E, Int16, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarUInt16, M_E, SparseBool, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, SparseBool, M_E, UInt16, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, ScalarInt32, M_E, SparseBool, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, SparseBool, M_E, Int32, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarUInt32, M_E, SparseBool, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, SparseBool, M_E, UInt32, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, ScalarInt64, M_E, SparseBool, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, SparseBool, M_E, Int64, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarUInt64, M_E, SparseBool, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, SparseBool, M_E, UInt64, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, String, M_E, SparseBool, String, Bool);
    scilab_fill_comparison_no_equal(String, SparseBool, M_E, String, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, ScalarString, M_E, SparseBool, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, SparseBool, M_E, String, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, SparseComplex, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, SparseBool, M_E, Sparse, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Sparse, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparison_no_equal(Sparse, SparseBool, M_E, Sparse, SparseBool, Bool);

    scilab_fill_comparison_no_equal(SparseBool, Sparse, M_E, SparseBool, Sparse, Bool);
    scilab_fill_comparison_no_equal(Sparse, SparseBool, M_E, Sparse, SparseBool, Bool);

    //struct
    scilab_fill_comparison_no_equal(Struct, Struct, M_M, Struct, Struct, Bool);

    /*scilab_fill_comparison_no_equal(Struct, Double, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(Double, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, DoubleComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(DoubleComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, ScalarDouble, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDouble, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarDoubleComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(ScalarDoubleComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Empty, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(Empty, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, Identity, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(Identity, Struct, M_E, Double, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, IdentityComplex, M_E, Struct, Double, Bool);
    scilab_fill_comparison_no_equal(IdentityComplex, Struct, M_E, Double, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Int8, M_E, Struct, Int8, Bool);
    scilab_fill_comparison_no_equal(Int8, Struct, M_E, Int8, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, UInt8, M_E, Struct, UInt8, Bool);
    scilab_fill_comparison_no_equal(UInt8, Struct, M_E, UInt8, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Int16, M_E, Struct, Int16, Bool);
    scilab_fill_comparison_no_equal(Int16, Struct, M_E, Int16, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, UInt16, M_E, Struct, UInt16, Bool);
    scilab_fill_comparison_no_equal(UInt16, Struct, M_E, UInt16, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Int32, M_E, Struct, Int32, Bool);
    scilab_fill_comparison_no_equal(Int32, Struct, M_E, Int32, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, UInt32, M_E, Struct, UInt32, Bool);
    scilab_fill_comparison_no_equal(UInt32, Struct, M_E, UInt32, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Int64, M_E, Struct, Int64, Bool);
    scilab_fill_comparison_no_equal(Int64, Struct, M_E, Int64, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, UInt64, M_E, Struct, UInt64, Bool);
    scilab_fill_comparison_no_equal(UInt64, Struct, M_E, UInt64, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, ScalarInt8, M_E, Struct, Int8, Bool);
    scilab_fill_comparison_no_equal(ScalarInt8, Struct, M_E, Int8, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarUInt8, M_E, Struct, UInt8, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt8, Struct, M_E, UInt8, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, ScalarInt16, M_E, Struct, Int16, Bool);
    scilab_fill_comparison_no_equal(ScalarInt16, Struct, M_E, Int16, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarUInt16, M_E, Struct, UInt16, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt16, Struct, M_E, UInt16, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, ScalarInt32, M_E, Struct, Int32, Bool);
    scilab_fill_comparison_no_equal(ScalarInt32, Struct, M_E, Int32, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarUInt32, M_E, Struct, UInt32, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt32, Struct, M_E, UInt32, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, ScalarInt64, M_E, Struct, Int64, Bool);
    scilab_fill_comparison_no_equal(ScalarInt64, Struct, M_E, Int64, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarUInt64, M_E, Struct, UInt64, Bool);
    scilab_fill_comparison_no_equal(ScalarUInt64, Struct, M_E, UInt64, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, String, M_E, Struct, String, Bool);
    scilab_fill_comparison_no_equal(String, Struct, M_E, String, Struct, Bool);
    scilab_fill_comparison_no_equal(Struct, ScalarString, M_E, Struct, String, Bool);
    scilab_fill_comparison_no_equal(ScalarString, Struct, M_E, String, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, SparseComplex, M_E, Struct, Sparse, Bool);
    scilab_fill_comparison_no_equal(SparseComplex, Struct, M_E, Sparse, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, Sparse, M_E, Struct, Sparse, Bool);
    scilab_fill_comparison_no_equal(Sparse, Struct, M_E, Sparse, Struct, Bool);

    scilab_fill_comparison_no_equal(Struct, SparseBool, M_E, Struct, SparseBool, Bool);
    scilab_fill_comparison_no_equal(SparseBool, Struct, M_E, SparseBool, Struct, Bool);*/

    //List, TList, MList
    scilab_fill_comparison_no_equal(List, List, LT_LT, List, List, Bool);
    scilab_fill_comparison_no_equal(MList, MList, LT_LT, MList, MList, Bool);
    scilab_fill_comparison_no_equal(TList, TList, LT_LT, TList, TList, Bool);

    scilab_fill_comparison_no_equal(List, TList, LT_LT, List, TList, Bool);
    scilab_fill_comparison_no_equal(TList, List, LT_LT, TList, List, Bool);
    scilab_fill_comparison_no_equal(List, MList, LT_LT, List, MList, Bool);
    scilab_fill_comparison_no_equal(MList, List, LT_LT, MList, List, Bool);

    scilab_fill_comparison_no_equal(TList, MList, LT_LT, TList, MList, Bool);
    scilab_fill_comparison_no_equal(MList, TList, LT_LT, MList, TList, Bool);

    //Cell
    scilab_fill_comparison_no_equal(Cell, Cell, M_M, Cell, Cell, Bool);

    //Handle
    scilab_fill_comparison_no_equal(Handle, Handle, M_M, GraphicHandle, GraphicHandle, Bool);

    //Macro
    scilab_fill_comparison_no_equal(Macro, Macro, MCR_MCR, Macro, Macro, Bool);
    scilab_fill_comparison_no_equal(MacroFile, MacroFile, MCR_MCR, MacroFile, MacroFile, Bool);
    scilab_fill_comparison_no_equal(Macro, MacroFile, MCR_MCR, Macro, MacroFile, Bool);
    scilab_fill_comparison_no_equal(MacroFile, Macro, MCR_MCR, MacroFile, Macro, Bool);


#undef scilab_fill_comparison_no_equal

}

InternalType *GenericComparisonNonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    compnoequal_function compnoequal = pComparisonEqualfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (compnoequal)
    {
        pResult = compnoequal(_pLeftOperand, _pRightOperand);
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


//Matrix != x
template<class T, class U, class O>
InternalType* compnoequal_M_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(true);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(true);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), (size_t)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compnoequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compnoequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_E(T *_pL, U *_pR)
{
    return new Bool(true);
}

template<class T, class U, class O>
InternalType* compnoequal_E_M(T *_pL, U *_pR)
{
    return new Bool(true);
}

template<class T, class U, class O>
InternalType* compnoequal_E_E(T *_pL, U *_pR)
{
    return new Bool(false);
}

//B != x
template<class T, class U, class O>
InternalType* compnoequal_B_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();
    int* piDimsR = _pR->getDimsArray();

    if (_pL->isScalar() == false)
    {
        if (iDimsL != iDimsR)
        {
            return new Bool(true);
        }

        int* piDimsL = _pL->getDimsArray();

        for (int i = 0; i < iDimsL; ++i)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                return new Bool(true);
            }
        }
    }

    O* pOut = new O(iDimsR, piDimsR);

    pOut->setTrue();

    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_B(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();
    int* piDimsL = _pL->getDimsArray();

    if (_pR->isScalar() == false)
    {
        if (iDimsL != iDimsR)
        {
            return new Bool(true);
        }

        int* piDimsR = _pR->getDimsArray();

        for (int i = 0; i < iDimsL; ++i)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                return new Bool(true);
            }
        }
    }

    O* pOut = new O(iDimsL, piDimsL);

    pOut->setTrue();

    return pOut;
}

//B != x
template<class T, class U, class O>
InternalType* compnoequal_B_S(T *_pL, U *_pR)
{

    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());

    pOut->setTrue();

    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_B(T *_pL, U *_pR)
{

    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());

    pOut->setTrue();

    return pOut;
}



//Matrix complex != x
template<class T, class U, class O>
InternalType* compnoequal_MC_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(true);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return new Bool(true);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compnoequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compnoequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

//Scalar == x
template<class T, class U, class O>
InternalType* compnoequal_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), (size_t)pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_I(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_IC(T *_pL, U *_pR)
{

    O* pOut = new  Bool(true);
    compnoequal(_pL->get(0), 1, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

//Scalar complex != x
template<class T, class U, class O>
InternalType* compnoequal_SC_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_S(T *_pL, U *_pR)
{
    O* pOut = new O(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(false);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

//Identity != x
template<class T, class U, class O>
InternalType* compnoequal_I_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compnoequal(pIdentity->get(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compnoequal(pIdentity->get(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_SC(T *_pL, U *_pR)
{
    return compnoequal_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* compnoequal_I_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pR->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

//Identity complex != x
template<class T, class U, class O>
InternalType* compnoequal_IC_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compnoequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compnoequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_SC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}


template<>
InternalType* compnoequal_M_M<SparseBool, SparseBool, SparseBool>(SparseBool* _pL, SparseBool* _pR)
{
    //pending changes

    if (((_pL->getRows() != _pR->getRows()) || (_pL->getCols() != _pR->getCols())) && (_pL->getSize() != 1) && (_pR->getSize() != 1))
    {
        return new Bool(true);
    }

    return _pR->newNotEqualTo(*_pL);
}

template<>
InternalType* compnoequal_M_M<Sparse, Sparse, SparseBool>(Sparse* _pL, Sparse* _pR)
{
    //pending changes

    if (((_pL->getRows() != _pR->getRows()) || (_pL->getCols() != _pR->getCols())) && (_pL->getSize() != 1) && (_pR->getSize() != 1))
    {
        return new Bool(true);
    }

    return _pR->newNotEqualTo(*_pL);
}

//[] != SP
template<>
InternalType* compnoequal_M_M<Double, Sparse, Bool>(Double* _pL, Sparse* _pR)
{
    return new Bool(true);
}

//SP != []
template<>
InternalType* compnoequal_M_M<Sparse, Double, Bool>(Sparse* _pL, Double* _pR)
{
    return new Bool(true);
}

//
template<class T, class U, class O>
InternalType* compnoequal_M_SP(T* _pL, U* _pR)
{
    //pending changes

    //D -> SP != SP
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
                pspConvert->set(i, stComplex);
            }
        }
        else
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pL->get(0));
            }
        }
    }
    else
    {
        //check dimensions
        if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
        {
            return new Bool(true);
        }

        int iSizeOut = _pR->getSize();
        if (_pL->isComplex())
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), true);

            for (int i = 0; i < iSizeOut; i++)
            {
                std::complex<double> stComplex((double)_pL->get(i), (double)_pL->getImg(i));
                pspConvert->set(i, stComplex);
            }
        }
        else
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pL->get(i));
            }
        }
    }

    pOut = _pR->newNotEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;
}


template<class T, class U, class O>
InternalType* compnoequal_SP_M(T* _pL, U* _pR)
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
                pspConvert->set(i, stComplex);
            }
        }
        else
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                pspConvert->set(i, (double)_pR->get(0));
            }
        }
    }
    else
    {
        //check dimensions
        if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
        {
            return new Bool(true);
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
                pspConvert->set(i, (double)_pR->get(i));
            }
        }
    }

    pOut = _pL->newNotEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;

}

//sparsebool
template<>
InternalType* compnoequal_M_M<Bool, SparseBool, SparseBool>(Bool* _pL, SparseBool* _pR)
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
            pspConvert->set(i, _pL->get(0) == 1);
        }

    }
    else
    {
        //check dimensions
        if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
        {
            return new Bool(true);
        }

        int iSizeOut = _pR->getSize();

        pspConvert = new SparseBool(_pR->getRows(), _pR->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pL->get(i) == 1);
        }
    }

    pOut = _pR->newNotEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;
}

template<>
InternalType* compnoequal_M_M<SparseBool, Bool, SparseBool>(SparseBool* _pL, Bool* _pR)
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
            pspConvert->set(i, _pR->get(0) == 1);
        }
    }
    else
    {
        //check dimensions
        if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
        {
            return new Bool(true);
        }

        int iSizeOut = _pL->getSize();

        pspConvert = new SparseBool(_pL->getRows(), _pL->getCols());
        for (int i = 0; i < iSizeOut; i++)
        {
            pspConvert->set(i, _pR->get(i) == 1);
        }
    }

    pOut = _pL->newNotEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;

}

//Polynom
template<>
InternalType* compnoequal_M_M<Polynom, Polynom, Bool>(Polynom* _pL, Polynom* _pR)
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[0]->get(j), (double)0, pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPL[i]->get(j), &bPoise);
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
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), pSPR[i]->getImg(j), &bPoise);
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
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
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
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == true); j++)
                            {
                                compnoequal(pSPL[0]->get(j), (double)0, pSPR[i]->get(j), pSPR[i]->get(j), &bPoise);
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
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = true;
                            for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPR[i]->get(j), &bPoise);
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
            return new Bool(true);
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                return new Bool(true);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPL[i]->get(j), pSPL[i]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
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
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = true;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == true); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPL[i]->get(j), &bPoise);
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
InternalType* compnoequal_P_M(T *_pL, U *_pR)
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)pdblEye->get(i), (double)pdblEye->getImg(i), &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)pdblEye->get(i), (double)0, &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)0, (double)pdblEye->get(i), (double)pdblEye->getImg(i), &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pdblEye->get(i), &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(0), (double)_pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(0), (double)0, &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)0, (double)_pR->get(0), (double)_pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)_pR->get(0), &(pbOut->get()[i]));
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
                        compnoequal((double)pSPR[0]->get(0), (double)pSPR[0]->getImg(0), (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal((double)pSPR[0]->get(0), (double)pSPR[0]->getImg(0), (double)_pR->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal((double)pSPR[0]->get(0), (double)0, (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal((double)pSPR[0]->get(0), (double)_pR->get(i), &(pbOut->get()[i]));
                    }
                }

            }
        }
        else
        {
            pbOut->setTrue();
        }

    }
    else
    {
        //check dimensions
        if (_pL->getDims() != _pR->getDims())
        {
            return new Bool(true);
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                return new Bool(true);
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)pSPR[i]->getImg(0), (double)_pR->get(i), (double)0, &(pbOut->get()[i]));
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
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)0, (double)_pR->get(i), (double)_pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal((double)pSPR[i]->get(0), (double)_pR->get(i), &(pbOut->get()[i]));
                    }
                }
            }
        }
    }

    return pbOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_P(T *_pL, U *_pR)
{
    return GenericComparisonNonEqual(_pR, _pL);
}

template<>
InternalType* compnoequal_M_M<String, String, Bool>(String* _pL, String* _pR)
{
    if (_pL->isScalar())
    {
        Bool*  pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        for (int i = 0; i < _pR->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(0), _pR->get(i)) != 0);
        }
        return pOut;
    }

    if (_pR->isScalar())
    {
        Bool*  pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(0)) != 0);
        }
        return pOut;
    }

    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    if (iDimsL != iDimsR)
    {
        return new Bool(true);
    }


    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }


    if (_pL->getSize() == _pR->getSize())
    {
        Bool*  pOut = new Bool(iDimsL, piDimsL);

        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(i)) != 0);
        }
        return pOut;
    }


    return NULL;
}

template<>
types::InternalType* compnoequal_M_M<Struct, Struct, Bool>(types::Struct* _pL, types::Struct* _pR)
{

    if (_pL->getSize() != _pR->getSize())
    {
        return new Bool(true);
    }

    String* pStringFielNamesPL = _pL->getFieldNames();
    String* pStringFielNamesPR = _pR->getFieldNames();
    wchar_t** ppwchPR = pStringFielNamesPR->get();
    wchar_t** ppwchPL = pStringFielNamesPL->get();

    Bool* pOut = new Bool(1, 2 * pStringFielNamesPL->getSize());
    pOut->setTrue();
    int* piPOut = pOut->get();

    for (int iComptSStruct = 0; iComptSStruct < _pL->getSize(); iComptSStruct++)
    {
        for (int i = 0; i < pStringFielNamesPL->getSize(); i++)
        {
            piPOut[i * 2] = (wcscmp(ppwchPR[i], ppwchPL[i]) != 0) && (piPOut[i * 2] == 1);


            if (_pL->get(iComptSStruct)->get(ppwchPL[i])->getType() == _pR->get(iComptSStruct)->get(ppwchPR[i])->getType())
            {
                if (_pL->get(iComptSStruct)->get(ppwchPL[i])->getType() == types::InternalType::ScilabStruct)
                {
                    piPOut[i * 2 + 1] = _pL->get(iComptSStruct)->get(ppwchPL[i]) != _pR->get(iComptSStruct)->get(ppwchPR[i]) && (piPOut[i * 2] == 1);
                }
                else
                {
                    types::InternalType* pResolution = GenericComparisonNonEqual(_pL->get(iComptSStruct)->get(ppwchPL[i]), _pR->get(iComptSStruct)->get(ppwchPR[i]));
                    bool bValeur = true;
                    if (pResolution->getType() == types::InternalType::ScilabBool)
                    {
                        Bool* pBoolTempo = pResolution->getAs<Bool>();
                        for (int j = 0; j < pBoolTempo->getSize() && bValeur == true; j++)
                        {
                            if (pBoolTempo->get(j) != 1)
                            {
                                bValeur = false;
                                break;
                            }
                        }
                    }
                    else
                    {
                        SparseBool* pSparseBoolTempo = pResolution->getAs<SparseBool>();
                        for (int j = 0; j < pSparseBoolTempo->getSize() && bValeur == true; j++)
                        {
                            if (pSparseBoolTempo->get(j) != 1)
                            {
                                bValeur = false;
                                break;
                            }
                        }
                    }
                    piPOut[i * 2 + 1] = bValeur && (piPOut[i * 2 + 1] == 1);
                }
            }
            else
            {
                piPOut[i * 2 + 1] = true;
            }
        }
    }

    return pOut;
}

//List, TList, MList
template<class T, class U, class O>
InternalType* compnoequal_LT_LT(T *_pL, U *_pR)
{
    if (_pL->getSize() != _pR->getSize())
    {
        return new Bool(true);
    }

    if (_pL->getSize() == 0 && _pR->getSize() == 0)
    {
        return new Bool(false);
    }

    Bool* pB = new Bool(1, _pL->getSize());
    for (int i = 0; i < _pL->getSize(); i++)
    {
        pB->set(i, !((*_pL->get(i) == *_pR->get(i)) && (_pL->get(i)->getType() != types::InternalType::ScilabVoid)));
    }

    return pB;
}

template<>
types::InternalType* compnoequal_M_M<Cell, Cell, Bool>(types::Cell* _pL, types::Cell* _pR)
{
    /* check dimension*/
    if (_pL->getDims() != _pR->getDims())
    {
        return new Bool(true);
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return new Bool(true);
        }
    }

    if (_pL->getSize() == 0)
    {
        //{} == {} -> return true
        return new Bool(false);
    }

    Bool *pB = new Bool(_pL->getDims(), piDimsL);
    for (int i = 0; i < _pL->getSize(); i++)
    {
        pB->set(i, !(*_pL->get(i) == *_pR->get(i)));
    }

    return pB;
}

template<>
types::InternalType* compnoequal_M_M<GraphicHandle, GraphicHandle, Bool>(GraphicHandle* _pL, GraphicHandle* _pR)
{

    /* check dimension*/
    if (_pL->getDims() != _pR->getDims())
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0; i < _pL->getDims(); i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    Bool* pOut = new Bool(_pL->getDims(), piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MCR_MCR(T *_pL, U *_pR)
{
    bool ret = false;
    if (_pL->getType() == GenericType::ScilabMacroFile)
    {
        types::InternalType* pIT = ((InternalType *) _pL);
        types::MacroFile* pL = pIT->getAs<types::MacroFile>();
        ret = *pL != *_pR;
    }
    else if (_pL->getType() == GenericType::ScilabMacro)
    {
        types::InternalType* pIT = ((InternalType *) _pL);
        types::Macro* pL = pIT->getAs<types::Macro>();
        if (_pR->getType() == GenericType::ScilabMacroFile)
        {
            types::InternalType* pIT = ((InternalType *) _pR);
            types::MacroFile* pR = pIT->getAs<types::MacroFile>();
            ret = *pR != *pL;
        }
        else
        {
            ret = *pL != *_pR;
        }
    }

    return new Bool(ret);
}
