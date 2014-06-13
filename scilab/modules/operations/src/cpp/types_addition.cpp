/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_addition.hxx"
#include "operations.hxx"
#include "double.hxx"
#include "int.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include <stdio.h>
#include "core_math.h"
#include "matrix_addition.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_swprintf.h"
#include "elem_common.h" //dset
}

using namespace types;

//define arrays on operation functions
static add_function pAddfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

add_function getAddFunction(types::InternalType::ScilabId leftId, types::InternalType::ScilabId rightId)
{
    return pAddfunction[leftId][rightId];
}

void fillAddFunction()
{
    //Double
    add_function* pD = pAddfunction[types::InternalType::IdDouble];
    //Matrix + Matrix
    pD[types::InternalType::IdDouble] = (add_function)&add_M_M<Double, Double, Double>;
    pD[types::InternalType::IdInt8] = (add_function)&add_M_M<Double, Int8, Int8>;
    pD[types::InternalType::IdUInt8] = (add_function)&add_M_M<Double, UInt8, UInt8>;
    pD[types::InternalType::IdInt16] = (add_function)&add_M_M<Double, Int16, Int16>;
    pD[types::InternalType::IdUInt16] = (add_function)&add_M_M<Double, UInt16, UInt16>;
    pD[types::InternalType::IdInt32] = (add_function)&add_M_M<Double, Int32, Int32>;
    pD[types::InternalType::IdUInt32] = (add_function)&add_M_M<Double, UInt32, UInt32>;
    pD[types::InternalType::IdInt64] = (add_function)&add_M_M<Double, Int64, Int64>;
    pD[types::InternalType::IdUInt64] = (add_function)&add_M_M<Double, UInt64, UInt64>;
    pD[types::InternalType::IdBool] = (add_function)&add_M_M<Double, Bool, Double>;

    //Matrix + Matrix Complex
    pD[types::InternalType::IdDoubleComplex] = (add_function)&add_M_MC<Double, Double, Double>;

    //Matrix + Scalar
    pD[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Double, Double, Double>;
    pD[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Double, Int8, Int8>;
    pD[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Double, UInt8, UInt8>;
    pD[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Double, Int16, Int16>;
    pD[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Double, UInt16, UInt16>;
    pD[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Double, Int32, Int32>;
    pD[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Double, UInt32, UInt32>;
    pD[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Double, Int64, Int64>;
    pD[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Double, UInt64, UInt64>;
    pD[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Double, Bool, Double>;

    //Matrix + Scalar Complex
    pD[types::InternalType::IdScalarDoubleComplex] = (add_function)&add_M_SC<Double, Double, Double>;
    //Matrix + Empty
    pD[types::InternalType::IdEmpty] = (add_function)&add_M_E<Double, Double, Double>;


    add_function* pDC = pAddfunction[types::InternalType::IdDoubleComplex];
    //Scalar Complex + Matrix
    pDC[types::InternalType::IdDouble] = (add_function)&add_MC_M<Double, Double, Double>;
    pDC[types::InternalType::IdDoubleComplex] = (add_function)&add_MC_MC<Double, Double, Double>;
    pDC[types::InternalType::IdScalarDouble] = (add_function)&add_MC_S<Double, Double, Double>;
    pDC[types::InternalType::IdScalarDoubleComplex] = (add_function)&add_MC_SC<Double, Double, Double>;
    pDC[types::InternalType::IdEmpty] = (add_function)&add_MC_E<Double, Double, Double>;

    //Scalar + Matrix
    add_function* pD1 = pAddfunction[types::InternalType::IdScalarDouble];
    pD1[types::InternalType::IdDouble] = (add_function)&add_S_M<Double, Double, Double>;
    pD1[types::InternalType::IdInt8] = (add_function)&add_S_M<Double, Int8, Int8>;
    pD1[types::InternalType::IdUInt8] = (add_function)&add_S_M<Double, UInt8, UInt8>;
    pD1[types::InternalType::IdInt16] = (add_function)&add_S_M<Double, Int16, Int16>;
    pD1[types::InternalType::IdUInt16] = (add_function)&add_S_M<Double, UInt16, UInt16>;
    pD1[types::InternalType::IdInt32] = (add_function)&add_S_M<Double, Int32, Int32>;
    pD1[types::InternalType::IdUInt32] = (add_function)&add_S_M<Double, UInt32, UInt32>;
    pD1[types::InternalType::IdInt64] = (add_function)&add_S_M<Double, Int64, Int64>;
    pD1[types::InternalType::IdUInt64] = (add_function)&add_S_M<Double, UInt64, UInt64>;
    pD1[types::InternalType::IdBool] = (add_function)&add_S_M<Double, Bool, Double>;

    //Scalar + Matrix Complex
    pD1[types::InternalType::IdDoubleComplex] = (add_function)&add_S_MC<Double, Double, Double>;

    //Scalar + Scalar
    pD1[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Double, Double, Double>;
    pD1[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Double, Int8, Int8>;
    pD1[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Double, UInt8, UInt8>;
    pD1[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Double, Int16, Int16>;
    pD1[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Double, UInt16, UInt16>;
    pD1[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Double, Int32, Int32>;
    pD1[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Double, UInt32, UInt32>;
    pD1[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Double, Int64, Int64>;
    pD1[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Double, UInt64, UInt64>;
    pD1[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Double, Bool, Double>;

    //Scalar + Scalar Complex
    pD1[types::InternalType::IdScalarDoubleComplex] = (add_function)&add_S_SC<Double, Double, Double>;

    //Scalar + Empty
    pD1[types::InternalType::IdEmpty] = (add_function)&add_S_E<Double, Double, Double>;

    add_function* pD1C = pAddfunction[types::InternalType::IdScalarDoubleComplex];
    pD1C[types::InternalType::IdDouble] = (add_function)&add_SC_M<Double, Double, Double>;
    pD1C[types::InternalType::IdDoubleComplex] = (add_function)&add_SC_MC<Double, Double, Double>;
    pD1C[types::InternalType::IdScalarDouble] = (add_function)&add_SC_S<Double, Double, Double>;
    pD1C[types::InternalType::IdScalarDoubleComplex] = (add_function)&add_SC_SC<Double, Double, Double>;
    pD1C[types::InternalType::IdEmpty] = (add_function)&add_SC_E<Double, Double, Double>;

    add_function* pE = pAddfunction[types::InternalType::IdEmpty];
    //Empty + Matrix
    pE[types::InternalType::IdDouble] = (add_function)&add_E_M<Double, Double, Double>;
    pE[types::InternalType::IdInt8] = (add_function)&add_E_M<Double, Int8, Int8>;
    pE[types::InternalType::IdUInt8] = (add_function)&add_E_M<Double, UInt8, UInt8>;
    pE[types::InternalType::IdInt16] = (add_function)&add_E_M<Double, Int16, Int16>;
    pE[types::InternalType::IdUInt16] = (add_function)&add_E_M<Double, UInt16, UInt16>;
    //pE[types::InternalType::IdInt32] = (add_function)&add_E_M<Double, Int32, Int32>;
    pE[types::InternalType::IdUInt32] = (add_function)&add_E_M<Double, UInt32, UInt32>;
    pE[types::InternalType::IdInt64] = (add_function)&add_E_M<Double, Int64, Int64>;
    pE[types::InternalType::IdUInt64] = (add_function)&add_E_M<Double, UInt64, UInt64>;
    //pE[types::InternalType::IdBool] = (add_function)&add_E_M<Double, Bool, Double>;
    //Empty + Matrix Complex
    pE[types::InternalType::IdDoubleComplex] = (add_function)&add_E_MC<Double, Double, Double>;
    //Empty + Scalar
    pE[types::InternalType::IdScalarDouble] = (add_function)&add_E_S<Double, Double, Double>;
    pE[types::InternalType::IdScalarInt8] = (add_function)&add_E_S<Double, Int8, Int8>;
    pE[types::InternalType::IdScalarUInt8] = (add_function)&add_E_S<Double, UInt8, UInt8>;
    pE[types::InternalType::IdScalarInt16] = (add_function)&add_E_S<Double, Int16, Int16>;
    pE[types::InternalType::IdScalarUInt16] = (add_function)&add_E_S<Double, UInt16, UInt16>;
    pE[types::InternalType::IdScalarInt32] = (add_function)&add_E_S<Double, Int32, Int32>;
    pE[types::InternalType::IdScalarUInt32] = (add_function)&add_E_S<Double, UInt32, UInt32>;
    pE[types::InternalType::IdScalarInt64] = (add_function)&add_E_S<Double, Int64, Int64>;
    pE[types::InternalType::IdScalarUInt64] = (add_function)&add_E_S<Double, UInt64, UInt64>;
    pE[types::InternalType::IdScalarBool] = (add_function)&add_E_S<Double, Bool, Double>;

    //Empty + Scalar Complex
    pE[types::InternalType::IdScalarDoubleComplex] = (add_function)&add_E_SC<Double, Double, Double>;
    //Empty + Empty
    pE[types::InternalType::IdEmpty] = (add_function)&add_E_E<Double, Double, Double>;

    //Int8
    add_function* pI8 = pAddfunction[types::InternalType::IdInt8];
    //Matrix + Matrix
    pI8[types::InternalType::IdDouble] = (add_function)&add_M_M<Int8, Double, Int8>;
    pI8[types::InternalType::IdInt8] = (add_function)&add_M_M<Int8, Int8, Int8>;
    pI8[types::InternalType::IdUInt8] = (add_function)&add_M_M<Int8, UInt8, UInt8>;
    pI8[types::InternalType::IdInt16] = (add_function)&add_M_M<Int8, Int16, Int16>;
    pI8[types::InternalType::IdUInt16] = (add_function)&add_M_M<Int8, UInt16, UInt16>;
    pI8[types::InternalType::IdInt32] = (add_function)&add_M_M<Int8, Int32, Int32>;
    pI8[types::InternalType::IdUInt32] = (add_function)&add_M_M<Int8, UInt32, UInt32>;
    pI8[types::InternalType::IdInt64] = (add_function)&add_M_M<Int8, Int64, Int64>;
    pI8[types::InternalType::IdUInt64] = (add_function)&add_M_M<Int8, UInt64, UInt64>;
    pI8[types::InternalType::IdBool] = (add_function)&add_M_M<Int8, Bool, Int8>;
    pI8[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int8, Double, Int8>;

    //Matrix + Scalar
    pI8[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Int8, Double, Int8>;
    pI8[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Int8, Int8, Int8>;
    pI8[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Int8, UInt8, UInt8>;
    pI8[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Int8, Int16, Int16>;
    pI8[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Int8, UInt16, UInt16>;
    pI8[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Int8, Int32, Int32>;
    pI8[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Int8, UInt32, UInt32>;
    pI8[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Int8, Int64, Int64>;
    pI8[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Int8, UInt64, UInt64>;
    pI8[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Int8, Bool, Int8>;

    add_function* pI81 = pAddfunction[types::InternalType::IdScalarInt8];
    //Scalar + Matrix
    pI81[types::InternalType::IdDouble] = (add_function)&add_S_M<Int8, Double, Int8>;
    pI81[types::InternalType::IdInt8] = (add_function)&add_S_M<Int8, Int8, Int8>;
    pI81[types::InternalType::IdUInt8] = (add_function)&add_S_M<Int8, UInt8, UInt8>;
    pI81[types::InternalType::IdInt16] = (add_function)&add_S_M<Int8, Int16, Int16>;
    pI81[types::InternalType::IdUInt16] = (add_function)&add_S_M<Int8, UInt16, UInt16>;
    pI81[types::InternalType::IdInt32] = (add_function)&add_S_M<Int8, Int32, Int32>;
    pI81[types::InternalType::IdUInt32] = (add_function)&add_S_M<Int8, UInt32, UInt32>;
    pI81[types::InternalType::IdInt64] = (add_function)&add_S_M<Int8, Int64, Int64>;
    pI81[types::InternalType::IdUInt64] = (add_function)&add_S_M<Int8, UInt64, UInt64>;
    pI81[types::InternalType::IdBool] = (add_function)&add_S_M<Int8, Bool, Int8>;
    pI81[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int8, Double, Int8>;

    //Scalar + Scalar
    pI81[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Int8, Double, Int8>;
    pI81[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Int8, Int8, Int8>;
    pI81[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Int8, UInt8, UInt8>;
    pI81[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Int8, Int16, Int16>;
    pI81[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Int8, UInt16, UInt16>;
    pI81[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Int8, Int32, Int32>;
    pI81[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Int8, UInt32, UInt32>;
    pI81[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Int8, Int64, Int64>;
    pI81[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Int8, UInt64, UInt64>;
    pI81[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Int8, Bool, Int8>;

    //UInt8
    add_function* pUI8 = pAddfunction[types::InternalType::IdUInt8];
    //Matrix + Matrix
    pUI8[types::InternalType::IdDouble] = (add_function)&add_M_M<UInt8, Double, UInt8>;
    pUI8[types::InternalType::IdInt8] = (add_function)&add_M_M<UInt8, Int8, UInt8>;
    pUI8[types::InternalType::IdUInt8] = (add_function)&add_M_M<UInt8, UInt8, UInt8>;
    pUI8[types::InternalType::IdInt16] = (add_function)&add_M_M<UInt8, Int16, UInt16>;
    pUI8[types::InternalType::IdUInt16] = (add_function)&add_M_M<UInt8, UInt16, UInt16>;
    pUI8[types::InternalType::IdInt32] = (add_function)&add_M_M<UInt8, Int32, UInt32>;
    pUI8[types::InternalType::IdUInt32] = (add_function)&add_M_M<UInt8, UInt32, UInt32>;
    pUI8[types::InternalType::IdInt64] = (add_function)&add_M_M<UInt8, Int64, UInt64>;
    pUI8[types::InternalType::IdUInt64] = (add_function)&add_M_M<UInt8, UInt64, UInt64>;
    pUI8[types::InternalType::IdBool] = (add_function)&add_M_M<UInt8, Bool, UInt8>;
    pUI8[types::InternalType::IdEmpty] = (add_function)&add_M_M<UInt8, Double, UInt8>;

    //Matrix + Scalar
    pUI8[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<UInt8, Double, UInt8>;
    pUI8[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<UInt8, Int8, UInt8>;
    pUI8[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<UInt8, UInt8, UInt8>;
    pUI8[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<UInt8, Int16, UInt16>;
    pUI8[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<UInt8, UInt16, UInt16>;
    pUI8[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<UInt8, Int32, UInt32>;
    pUI8[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<UInt8, UInt32, UInt32>;
    pUI8[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<UInt8, Int64, UInt64>;
    pUI8[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<UInt8, UInt64, UInt64>;
    pUI8[types::InternalType::IdScalarBool] = (add_function)&add_M_S<UInt8, Bool, UInt8>;

    add_function* pUI81 = pAddfunction[types::InternalType::IdScalarUInt8];
    //Scalar + Matrix
    pUI81[types::InternalType::IdDouble] = (add_function)&add_S_M<UInt8, Double, UInt8>;
    pUI81[types::InternalType::IdInt8] = (add_function)&add_S_M<UInt8, Int8, UInt8>;
    pUI81[types::InternalType::IdUInt8] = (add_function)&add_S_M<UInt8, UInt8, UInt8>;
    pUI81[types::InternalType::IdInt16] = (add_function)&add_S_M<UInt8, Int16, UInt16>;
    pUI81[types::InternalType::IdUInt16] = (add_function)&add_S_M<UInt8, UInt16, UInt16>;
    pUI81[types::InternalType::IdInt32] = (add_function)&add_S_M<UInt8, Int32, UInt32>;
    pUI81[types::InternalType::IdUInt32] = (add_function)&add_S_M<UInt8, UInt32, UInt32>;
    pUI81[types::InternalType::IdInt64] = (add_function)&add_S_M<UInt8, Int64, UInt64>;
    pUI81[types::InternalType::IdUInt64] = (add_function)&add_S_M<UInt8, UInt64, UInt64>;
    pUI81[types::InternalType::IdBool] = (add_function)&add_S_M<UInt8, Bool, UInt8>;
    pUI81[types::InternalType::IdEmpty] = (add_function)&add_S_M<UInt8, Double, UInt8>;

    //Scalar + Scalar
    pUI81[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<UInt8, Double, UInt8>;
    pUI81[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<UInt8, Int8, UInt8>;
    pUI81[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<UInt8, UInt8, UInt8>;
    pUI81[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<UInt8, Int16, UInt16>;
    pUI81[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<UInt8, UInt16, UInt16>;
    pUI81[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<UInt8, Int32, UInt32>;
    pUI81[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<UInt8, UInt32, UInt32>;
    pUI81[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<UInt8, Int64, UInt64>;
    pUI81[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<UInt8, UInt64, UInt64>;
    pUI81[types::InternalType::IdScalarBool] = (add_function)&add_S_S<UInt8, Bool, UInt8>;

    //Int16
    add_function* pI16 = pAddfunction[types::InternalType::IdInt16];
    //Matrix + Matrix
    pI16[types::InternalType::IdDouble] = (add_function)&add_M_M<Int16, Double, Int16>;
    pI16[types::InternalType::IdInt8] = (add_function)&add_M_M<Int16, Int8, Int16>;
    pI16[types::InternalType::IdUInt8] = (add_function)&add_M_M<Int16, UInt8, UInt16>;
    pI16[types::InternalType::IdInt16] = (add_function)&add_M_M<Int16, Int16, Int16>;
    pI16[types::InternalType::IdUInt16] = (add_function)&add_M_M<Int16, UInt16, UInt16>;
    pI16[types::InternalType::IdInt32] = (add_function)&add_M_M<Int16, Int32, Int32>;
    pI16[types::InternalType::IdUInt32] = (add_function)&add_M_M<Int16, UInt32, UInt32>;
    pI16[types::InternalType::IdInt64] = (add_function)&add_M_M<Int16, Int64, Int64>;
    pI16[types::InternalType::IdUInt64] = (add_function)&add_M_M<Int16, UInt64, UInt64>;
    pI16[types::InternalType::IdBool] = (add_function)&add_M_M<Int16, Bool, Int16>;
    pI16[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int16, Double, Int16>;

    //Matrix + Scalar
    pI16[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Int16, Double, Int16>;
    pI16[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Int16, Int8, Int16>;
    pI16[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Int16, UInt8, UInt16>;
    pI16[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Int16, Int16, Int16>;
    pI16[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Int16, UInt16, UInt16>;
    pI16[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Int16, Int32, Int32>;
    pI16[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Int16, UInt32, UInt32>;
    pI16[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Int16, Int64, Int64>;
    pI16[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Int16, UInt64, UInt64>;
    pI16[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Int16, Bool, Int16>;

    add_function* pI161 = pAddfunction[types::InternalType::IdScalarInt16];
    //Scalar + Matrix
    pI161[types::InternalType::IdDouble] = (add_function)&add_S_M<Int16, Double, Int16>;
    pI161[types::InternalType::IdInt8] = (add_function)&add_S_M<Int16, Int8, Int16>;
    pI161[types::InternalType::IdUInt8] = (add_function)&add_S_M<Int16, UInt8, UInt16>;
    pI161[types::InternalType::IdInt16] = (add_function)&add_S_M<Int16, Int16, Int16>;
    pI161[types::InternalType::IdUInt16] = (add_function)&add_S_M<Int16, UInt16, UInt16>;
    pI161[types::InternalType::IdInt32] = (add_function)&add_S_M<Int16, Int32, Int32>;
    pI161[types::InternalType::IdUInt32] = (add_function)&add_S_M<Int16, UInt32, UInt32>;
    pI161[types::InternalType::IdInt64] = (add_function)&add_S_M<Int16, Int64, Int64>;
    pI161[types::InternalType::IdUInt64] = (add_function)&add_S_M<Int16, UInt64, UInt64>;
    pI161[types::InternalType::IdBool] = (add_function)&add_S_M<Int16, Bool, Int16>;
    pI161[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int16, Double, Int16>;

    //Scalar + Scalar
    pI161[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Int16, Double, Int16>;
    pI161[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Int16, Int8, Int16>;
    pI161[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Int16, UInt8, UInt16>;
    pI161[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Int16, Int16, Int16>;
    pI161[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Int16, UInt16, UInt16>;
    pI161[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Int16, Int32, Int32>;
    pI161[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Int16, UInt32, UInt32>;
    pI161[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Int16, Int64, Int64>;
    pI161[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Int16, UInt64, UInt64>;
    pI161[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Int16, Bool, Int16>;

    //UInt16
    add_function* pUI16 = pAddfunction[types::InternalType::IdUInt16];
    //Matrix + Matrix
    pUI16[types::InternalType::IdDouble] = (add_function)&add_M_M<UInt16, Double, UInt16>;
    pUI16[types::InternalType::IdInt8] = (add_function)&add_M_M<UInt16, Int8, UInt16>;
    pUI16[types::InternalType::IdUInt8] = (add_function)&add_M_M<UInt16, UInt8, UInt16>;
    pUI16[types::InternalType::IdInt16] = (add_function)&add_M_M<UInt16, Int16, UInt16>;
    pUI16[types::InternalType::IdUInt16] = (add_function)&add_M_M<UInt16, UInt16, UInt16>;
    pUI16[types::InternalType::IdInt32] = (add_function)&add_M_M<UInt16, Int32, UInt32>;
    pUI16[types::InternalType::IdUInt32] = (add_function)&add_M_M<UInt16, UInt32, UInt32>;
    pUI16[types::InternalType::IdInt64] = (add_function)&add_M_M<UInt16, Int64, UInt64>;
    pUI16[types::InternalType::IdUInt64] = (add_function)&add_M_M<UInt16, UInt64, UInt64>;
    pUI16[types::InternalType::IdBool] = (add_function)&add_M_M<UInt16, Bool, UInt16>;
    pUI16[types::InternalType::IdEmpty] = (add_function)&add_M_M<UInt16, Double, UInt16>;

    //Matrix + Scalar
    pUI16[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<UInt16, Double, UInt16>;
    pUI16[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<UInt16, Int8, UInt16>;
    pUI16[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<UInt16, UInt8, UInt16>;
    pUI16[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<UInt16, Int16, UInt16>;
    pUI16[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<UInt16, UInt16, UInt16>;
    pUI16[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<UInt16, Int32, UInt32>;
    pUI16[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<UInt16, UInt32, UInt32>;
    pUI16[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<UInt16, Int64, UInt64>;
    pUI16[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<UInt16, UInt64, UInt64>;
    pUI16[types::InternalType::IdScalarBool] = (add_function)&add_M_S<UInt16, Bool, UInt16>;

    add_function* pUI161 = pAddfunction[types::InternalType::IdScalarUInt16];
    //Scalar + Matrix
    pUI161[types::InternalType::IdDouble] = (add_function)&add_S_M<UInt16, Double, UInt16>;
    pUI161[types::InternalType::IdInt8] = (add_function)&add_S_M<UInt16, Int8, UInt16>;
    pUI161[types::InternalType::IdUInt8] = (add_function)&add_S_M<UInt16, UInt8, UInt16>;
    pUI161[types::InternalType::IdInt16] = (add_function)&add_S_M<UInt16, Int16, UInt16>;
    pUI161[types::InternalType::IdUInt16] = (add_function)&add_S_M<UInt16, UInt16, UInt16>;
    pUI161[types::InternalType::IdInt32] = (add_function)&add_S_M<UInt16, Int32, UInt32>;
    pUI161[types::InternalType::IdUInt32] = (add_function)&add_S_M<UInt16, UInt32, UInt32>;
    pUI161[types::InternalType::IdInt64] = (add_function)&add_S_M<UInt16, Int64, UInt64>;
    pUI161[types::InternalType::IdUInt64] = (add_function)&add_S_M<UInt16, UInt64, UInt64>;
    pUI161[types::InternalType::IdBool] = (add_function)&add_S_M<UInt16, Bool, UInt16>;
    pUI161[types::InternalType::IdEmpty] = (add_function)&add_S_M<UInt16, Double, UInt16>;

    //Scalar + Scalar
    pUI161[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<UInt16, Double, UInt16>;
    pUI161[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<UInt16, Int8, UInt16>;
    pUI161[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<UInt16, UInt8, UInt16>;
    pUI161[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<UInt16, Int16, UInt16>;
    pUI161[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<UInt16, UInt16, UInt16>;
    pUI161[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<UInt16, Int32, UInt32>;
    pUI161[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<UInt16, UInt32, UInt32>;
    pUI161[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<UInt16, Int64, UInt64>;
    pUI161[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<UInt16, UInt64, UInt64>;
    pUI161[types::InternalType::IdScalarBool] = (add_function)&add_S_S<UInt16, Bool, UInt16>;

    //Int32
    add_function* pI32 = pAddfunction[types::InternalType::IdInt32];
    //Matrix + Matrix
    pI32[types::InternalType::IdDouble] = (add_function)&add_M_M<Int32, Double, Int32>;
    pI32[types::InternalType::IdInt8] = (add_function)&add_M_M<Int32, Int8, Int32>;
    pI32[types::InternalType::IdUInt8] = (add_function)&add_M_M<Int32, UInt8, UInt32>;
    pI32[types::InternalType::IdInt16] = (add_function)&add_M_M<Int32, Int16, Int32>;
    pI32[types::InternalType::IdUInt16] = (add_function)&add_M_M<Int32, UInt16, UInt32>;
    pI32[types::InternalType::IdInt32] = (add_function)&add_M_M<Int32, Int32, Int32>;
    pI32[types::InternalType::IdUInt32] = (add_function)&add_M_M<Int32, UInt32, UInt32>;
    pI32[types::InternalType::IdInt64] = (add_function)&add_M_M<Int32, Int64, Int64>;
    pI32[types::InternalType::IdUInt64] = (add_function)&add_M_M<Int32, UInt64, UInt64>;
    pI32[types::InternalType::IdBool] = (add_function)&add_M_M<Int32, Bool, Int32>;
    pI32[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int32, Double, Int32>;

    //Matrix + Scalar
    pI32[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Int32, Double, Int32>;
    pI32[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Int32, Int8, Int32>;
    pI32[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Int32, UInt8, UInt32>;
    pI32[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Int32, Int16, Int32>;
    pI32[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Int32, UInt16, UInt32>;
    pI32[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Int32, Int32, Int32>;
    pI32[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Int32, UInt32, UInt32>;
    pI32[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Int32, Int64, Int64>;
    pI32[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Int32, UInt64, UInt64>;
    pI32[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Int32, Bool, Int32>;

    add_function* pI321 = pAddfunction[types::InternalType::IdScalarInt32];
    //Scalar + Matrix
    pI321[types::InternalType::IdDouble] = (add_function)&add_S_M<Int32, Double, Int32>;
    pI321[types::InternalType::IdInt8] = (add_function)&add_S_M<Int32, Int8, Int32>;
    pI321[types::InternalType::IdUInt8] = (add_function)&add_S_M<Int32, UInt8, UInt32>;
    pI321[types::InternalType::IdInt16] = (add_function)&add_S_M<Int32, Int16, Int32>;
    pI321[types::InternalType::IdUInt16] = (add_function)&add_S_M<Int32, UInt16, UInt32>;
    pI321[types::InternalType::IdInt32] = (add_function)&add_S_M<Int32, Int32, Int32>;
    pI321[types::InternalType::IdUInt32] = (add_function)&add_S_M<Int32, UInt32, UInt32>;
    pI321[types::InternalType::IdInt64] = (add_function)&add_S_M<Int32, Int64, Int64>;
    pI321[types::InternalType::IdUInt64] = (add_function)&add_S_M<Int32, UInt64, UInt64>;
    pI321[types::InternalType::IdBool] = (add_function)&add_S_M<Int32, Bool, Int32>;
    pI321[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int32, Double, Int32>;

    //Scalar + Scalar
    pI321[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Int32, Double, Int32>;
    pI321[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Int32, Int8, Int32>;
    pI321[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Int32, UInt8, UInt32>;
    pI321[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Int32, Int16, Int32>;
    pI321[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Int32, UInt16, UInt32>;
    pI321[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Int32, Int32, Int32>;
    pI321[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Int32, UInt32, UInt32>;
    pI321[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Int32, Int64, Int64>;
    pI321[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Int32, UInt64, UInt64>;
    pI321[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Int32, Bool, Int32>;

    //UInt32
    add_function* pUI32 = pAddfunction[types::InternalType::IdUInt32];
    //Matrix + Matrix
    pUI32[types::InternalType::IdDouble] = (add_function)&add_M_M<UInt32, Double, UInt32>;
    pUI32[types::InternalType::IdInt8] = (add_function)&add_M_M<UInt32, Int8, UInt32>;
    pUI32[types::InternalType::IdUInt8] = (add_function)&add_M_M<UInt32, UInt8, UInt32>;
    pUI32[types::InternalType::IdInt16] = (add_function)&add_M_M<UInt32, Int16, UInt32>;
    pUI32[types::InternalType::IdUInt16] = (add_function)&add_M_M<UInt32, UInt16, UInt32>;
    pUI32[types::InternalType::IdInt32] = (add_function)&add_M_M<UInt32, Int32, UInt32>;
    pUI32[types::InternalType::IdUInt32] = (add_function)&add_M_M<UInt32, UInt32, UInt32>;
    pUI32[types::InternalType::IdInt64] = (add_function)&add_M_M<UInt32, Int64, UInt64>;
    pUI32[types::InternalType::IdUInt64] = (add_function)&add_M_M<UInt32, UInt64, UInt64>;
    pUI32[types::InternalType::IdBool] = (add_function)&add_M_M<UInt32, Bool, UInt32>;
    pUI32[types::InternalType::IdEmpty] = (add_function)&add_M_M<UInt32, Double, UInt32>;

    //Matrix + Scalar
    pUI32[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<UInt32, Double, UInt32>;
    pUI32[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<UInt32, Int8, UInt32>;
    pUI32[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<UInt32, UInt8, UInt32>;
    pUI32[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<UInt32, Int16, UInt32>;
    pUI32[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<UInt32, UInt16, UInt32>;
    pUI32[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<UInt32, Int32, UInt32>;
    pUI32[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<UInt32, UInt32, UInt32>;
    pUI32[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<UInt32, Int64, UInt64>;
    pUI32[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<UInt32, UInt64, UInt64>;
    pUI32[types::InternalType::IdScalarBool] = (add_function)&add_M_S<UInt32, Bool, UInt32>;

    add_function* pUI321 = pAddfunction[types::InternalType::IdScalarUInt32];
    //Scalar + Matrix
    pUI321[types::InternalType::IdDouble] = (add_function)&add_S_M<UInt32, Double, UInt32>;
    pUI321[types::InternalType::IdInt8] = (add_function)&add_S_M<UInt32, Int8, UInt32>;
    pUI321[types::InternalType::IdUInt8] = (add_function)&add_S_M<UInt32, UInt8, UInt32>;
    pUI321[types::InternalType::IdInt16] = (add_function)&add_S_M<UInt32, Int16, UInt32>;
    pUI321[types::InternalType::IdUInt16] = (add_function)&add_S_M<UInt32, UInt16, UInt32>;
    pUI321[types::InternalType::IdInt32] = (add_function)&add_S_M<UInt32, Int32, UInt32>;
    pUI321[types::InternalType::IdUInt32] = (add_function)&add_S_M<UInt32, UInt32, UInt32>;
    pUI321[types::InternalType::IdInt64] = (add_function)&add_S_M<UInt32, Int64, UInt64>;
    pUI321[types::InternalType::IdUInt64] = (add_function)&add_S_M<UInt32, UInt64, UInt64>;
    pUI321[types::InternalType::IdBool] = (add_function)&add_S_M<UInt32, Bool, UInt32>;
    pUI321[types::InternalType::IdEmpty] = (add_function)&add_S_M<UInt32, Double, UInt32>;

    //Scalar + Scalar
    pUI321[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<UInt32, Double, UInt32>;
    pUI321[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<UInt32, Int8, UInt32>;
    pUI321[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<UInt32, UInt8, UInt32>;
    pUI321[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<UInt32, Int16, UInt32>;
    pUI321[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<UInt32, UInt16, UInt32>;
    pUI321[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<UInt32, Int32, UInt32>;
    pUI321[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<UInt32, UInt32, UInt32>;
    pUI321[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<UInt32, Int64, UInt64>;
    pUI321[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<UInt32, UInt64, UInt64>;
    pUI321[types::InternalType::IdScalarBool] = (add_function)&add_S_S<UInt32, Bool, UInt32>;

    //Int64
    add_function* pI64 = pAddfunction[types::InternalType::IdInt64];
    //Matrix + Matrix
    pI64[types::InternalType::IdDouble] = (add_function)&add_M_M<Int64, Double, Int64>;
    pI64[types::InternalType::IdInt8] = (add_function)&add_M_M<Int64, Int8, Int64>;
    pI64[types::InternalType::IdUInt8] = (add_function)&add_M_M<Int64, UInt8, UInt64>;
    pI64[types::InternalType::IdInt16] = (add_function)&add_M_M<Int64, Int16, Int64>;
    pI64[types::InternalType::IdUInt16] = (add_function)&add_M_M<Int64, UInt16, UInt64>;
    pI64[types::InternalType::IdInt32] = (add_function)&add_M_M<Int64, Int32, Int64>;
    pI64[types::InternalType::IdUInt32] = (add_function)&add_M_M<Int64, UInt32, UInt64>;
    pI64[types::InternalType::IdInt64] = (add_function)&add_M_M<Int64, Int64, Int64>;
    pI64[types::InternalType::IdUInt64] = (add_function)&add_M_M<Int64, UInt64, UInt64>;
    pI64[types::InternalType::IdBool] = (add_function)&add_M_M<Int64, Bool, Int64>;
    pI64[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int64, Double, Int64>;

    //Matrix + Scalar
    pI64[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Int64, Double, Int64>;
    pI64[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Int64, Int8, Int64>;
    pI64[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Int64, UInt8, UInt64>;
    pI64[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Int64, Int16, Int64>;
    pI64[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Int64, UInt16, UInt64>;
    pI64[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Int64, Int32, Int64>;
    pI64[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Int64, UInt32, UInt64>;
    pI64[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Int64, Int64, Int64>;
    pI64[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Int64, UInt64, UInt64>;
    pI64[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Int64, Bool, Int64>;

    add_function* pI641 = pAddfunction[types::InternalType::IdScalarInt64];
    //Scalar + Matrix
    pI641[types::InternalType::IdDouble] = (add_function)&add_S_M<Int64, Double, Int64>;
    pI641[types::InternalType::IdInt8] = (add_function)&add_S_M<Int64, Int8, Int64>;
    pI641[types::InternalType::IdUInt8] = (add_function)&add_S_M<Int64, UInt8, UInt64>;
    pI641[types::InternalType::IdInt16] = (add_function)&add_S_M<Int64, Int16, Int64>;
    pI641[types::InternalType::IdUInt16] = (add_function)&add_S_M<Int64, UInt16, UInt64>;
    pI641[types::InternalType::IdInt32] = (add_function)&add_S_M<Int64, Int32, Int64>;
    pI641[types::InternalType::IdUInt32] = (add_function)&add_S_M<Int64, UInt32, UInt64>;
    pI641[types::InternalType::IdInt64] = (add_function)&add_S_M<Int64, Int64, Int64>;
    pI641[types::InternalType::IdUInt64] = (add_function)&add_S_M<Int64, UInt64, UInt64>;
    pI641[types::InternalType::IdBool] = (add_function)&add_S_M<Int64, Bool, Int64>;
    pI641[types::InternalType::IdEmpty] = (add_function)&add_M_M<Int64, Double, Int64>;

    //Scalar + Scalar
    pI641[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Int64, Double, Int64>;
    pI641[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Int64, Int8, Int64>;
    pI641[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Int64, UInt8, UInt64>;
    pI641[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Int64, Int16, Int64>;
    pI641[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Int64, UInt16, UInt64>;
    pI641[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Int64, Int32, Int64>;
    pI641[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Int64, UInt32, UInt64>;
    pI641[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Int64, Int64, Int64>;
    pI641[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Int64, UInt64, UInt64>;
    pI641[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Int64, Bool, Int64>;

    //UInt64
    add_function* pUI64 = pAddfunction[types::InternalType::IdUInt64];
    //Matrix + Matrix
    pUI64[types::InternalType::IdDouble] = (add_function)&add_M_M<UInt64, Double, UInt64>;
    pUI64[types::InternalType::IdInt8] = (add_function)&add_M_M<UInt64, Int8, UInt64>;
    pUI64[types::InternalType::IdUInt8] = (add_function)&add_M_M<UInt64, UInt8, UInt64>;
    pUI64[types::InternalType::IdInt16] = (add_function)&add_M_M<UInt64, Int16, UInt64>;
    pUI64[types::InternalType::IdUInt16] = (add_function)&add_M_M<UInt64, UInt16, UInt64>;
    pUI64[types::InternalType::IdInt32] = (add_function)&add_M_M<UInt64, Int32, UInt64>;
    pUI64[types::InternalType::IdUInt32] = (add_function)&add_M_M<UInt64, UInt32, UInt64>;
    pUI64[types::InternalType::IdInt64] = (add_function)&add_M_M<UInt64, Int64, UInt64>;
    pUI64[types::InternalType::IdUInt64] = (add_function)&add_M_M<UInt64, UInt64, UInt64>;
    pUI64[types::InternalType::IdBool] = (add_function)&add_M_M<UInt64, Bool, UInt64>;
    pUI64[types::InternalType::IdEmpty] = (add_function)&add_M_M<UInt64, Double, UInt64>;

    //Matrix + Scalar
    pUI64[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<UInt64, Double, UInt64>;
    pUI64[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<UInt64, Int8, UInt64>;
    pUI64[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<UInt64, UInt8, UInt64>;
    pUI64[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<UInt64, Int16, UInt64>;
    pUI64[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<UInt64, UInt16, UInt64>;
    pUI64[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<UInt64, Int32, UInt64>;
    pUI64[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<UInt64, UInt32, UInt64>;
    pUI64[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<UInt64, Int64, UInt64>;
    pUI64[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<UInt64, UInt64, UInt64>;
    pUI64[types::InternalType::IdScalarBool] = (add_function)&add_M_S<UInt64, Bool, UInt64>;

    add_function* pUI641 = pAddfunction[types::InternalType::IdScalarUInt64];
    //Scalar + Matrix
    pUI641[types::InternalType::IdDouble] = (add_function)&add_S_M<UInt64, Double, UInt64>;
    pUI641[types::InternalType::IdInt8] = (add_function)&add_S_M<UInt64, Int8, UInt64>;
    pUI641[types::InternalType::IdUInt8] = (add_function)&add_S_M<UInt64, UInt8, UInt64>;
    pUI641[types::InternalType::IdInt16] = (add_function)&add_S_M<UInt64, Int16, UInt64>;
    pUI641[types::InternalType::IdUInt16] = (add_function)&add_S_M<UInt64, UInt16, UInt64>;
    pUI641[types::InternalType::IdInt32] = (add_function)&add_S_M<UInt64, Int32, UInt64>;
    pUI641[types::InternalType::IdUInt32] = (add_function)&add_S_M<UInt64, UInt32, UInt64>;
    pUI641[types::InternalType::IdInt64] = (add_function)&add_S_M<UInt64, Int64, UInt64>;
    pUI641[types::InternalType::IdUInt64] = (add_function)&add_S_M<UInt64, UInt64, UInt64>;
    pUI641[types::InternalType::IdBool] = (add_function)&add_S_M<UInt64, Bool, UInt64>;
    pUI641[types::InternalType::IdEmpty] = (add_function)&add_S_M<UInt64, Double, UInt64>;

    //Scalar + Scalar
    pUI641[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<UInt64, Double, UInt64>;
    pUI641[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<UInt64, Int8, UInt64>;
    pUI641[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<UInt64, UInt8, UInt64>;
    pUI641[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<UInt64, Int16, UInt64>;
    pUI641[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<UInt64, UInt16, UInt64>;
    pUI641[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<UInt64, Int32, UInt64>;
    pUI641[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<UInt64, UInt32, UInt64>;
    pUI641[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<UInt64, Int64, UInt64>;
    pUI641[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<UInt64, UInt64, UInt64>;
    pUI641[types::InternalType::IdScalarBool] = (add_function)&add_S_S<UInt64, Bool, UInt64>;

    //Bool
    add_function* pB = pAddfunction[types::InternalType::IdBool];
    //Matrix + Matrix
    pB[types::InternalType::IdDouble] = (add_function)&add_M_M<Bool, Double, Double>;
    pB[types::InternalType::IdInt8] = (add_function)&add_M_M<Bool, Int8, Int8>;
    pB[types::InternalType::IdUInt8] = (add_function)&add_M_M<Bool, UInt8, UInt8>;
    pB[types::InternalType::IdInt16] = (add_function)&add_M_M<Bool, Int16, Int16>;
    pB[types::InternalType::IdUInt16] = (add_function)&add_M_M<Bool, UInt16, UInt16>;
    pB[types::InternalType::IdInt32] = (add_function)&add_M_M<Bool, Int32, Int32>;
    pB[types::InternalType::IdUInt32] = (add_function)&add_M_M<Bool, UInt32, UInt32>;
    pB[types::InternalType::IdInt64] = (add_function)&add_M_M<Bool, Int64, Int64>;
    pB[types::InternalType::IdUInt64] = (add_function)&add_M_M<Bool, UInt64, UInt64>;
    pB[types::InternalType::IdBool] = (add_function)&add_M_M<Bool, Bool, Bool>;
    pB[types::InternalType::IdEmpty] = (add_function)&add_M_M<Bool, Double, Double>;

    //Matrix + Scalar
    pB[types::InternalType::IdScalarDouble] = (add_function)&add_M_S<Bool, Double, Double>;
    pB[types::InternalType::IdScalarInt8] = (add_function)&add_M_S<Bool, Int8, Int8>;
    pB[types::InternalType::IdScalarUInt8] = (add_function)&add_M_S<Bool, UInt8, UInt8>;
    pB[types::InternalType::IdScalarInt16] = (add_function)&add_M_S<Bool, Int16, Int16>;
    pB[types::InternalType::IdScalarUInt16] = (add_function)&add_M_S<Bool, UInt16, UInt16>;
    pB[types::InternalType::IdScalarInt32] = (add_function)&add_M_S<Bool, Int32, Int32>;
    pB[types::InternalType::IdScalarUInt32] = (add_function)&add_M_S<Bool, UInt32, UInt32>;
    pB[types::InternalType::IdScalarInt64] = (add_function)&add_M_S<Bool, Int64, Int64>;
    pB[types::InternalType::IdScalarUInt64] = (add_function)&add_M_S<Bool, UInt64, UInt64>;
    pB[types::InternalType::IdScalarBool] = (add_function)&add_M_S<Bool, Bool, Bool>;

    add_function* pB1 = pAddfunction[types::InternalType::IdScalarBool];
    //Scalar + Matrix
    pB1[types::InternalType::IdDouble] = (add_function)&add_S_M<Bool, Double, Double>;
    pB1[types::InternalType::IdInt8] = (add_function)&add_S_M<Bool, Int8, Int8>;
    pB1[types::InternalType::IdUInt8] = (add_function)&add_S_M<Bool, UInt8, UInt8>;
    pB1[types::InternalType::IdInt16] = (add_function)&add_S_M<Bool, Int16, Int16>;
    pB1[types::InternalType::IdUInt16] = (add_function)&add_S_M<Bool, UInt16, UInt16>;
    pB1[types::InternalType::IdInt32] = (add_function)&add_S_M<Bool, Int32, Int32>;
    pB1[types::InternalType::IdUInt32] = (add_function)&add_S_M<Bool, UInt32, UInt32>;
    pB1[types::InternalType::IdInt64] = (add_function)&add_S_M<Bool, Int64, Int64>;
    pB1[types::InternalType::IdUInt64] = (add_function)&add_S_M<Bool, UInt64, UInt64>;
    pB1[types::InternalType::IdBool] = (add_function)&add_S_M<Bool, Bool, Bool>;
    pB1[types::InternalType::IdEmpty] = (add_function)&add_M_M<Bool, Double, Double>;

    //Scalar + Scalar
    pB1[types::InternalType::IdScalarDouble] = (add_function)&add_S_S<Bool, Double, Double>;
    pB1[types::InternalType::IdScalarInt8] = (add_function)&add_S_S<Bool, Int8, Int8>;
    pB1[types::InternalType::IdScalarUInt8] = (add_function)&add_S_S<Bool, UInt8, UInt8>;
    pB1[types::InternalType::IdScalarInt16] = (add_function)&add_S_S<Bool, Int16, Int16>;
    pB1[types::InternalType::IdScalarUInt16] = (add_function)&add_S_S<Bool, UInt16, UInt16>;
    pB1[types::InternalType::IdScalarInt32] = (add_function)&add_S_S<Bool, Int32, Int32>;
    pB1[types::InternalType::IdScalarUInt32] = (add_function)&add_S_S<Bool, UInt32, UInt32>;
    pB1[types::InternalType::IdScalarInt64] = (add_function)&add_S_S<Bool, Int64, Int64>;
    pB1[types::InternalType::IdScalarUInt64] = (add_function)&add_S_S<Bool, UInt64, UInt64>;
    pB1[types::InternalType::IdScalarBool] = (add_function)&add_S_S<Bool, Bool, Bool>;
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
    ** STRING + STRING
    */
    if (_pLeftOperand->isString() && _pRightOperand->isString())
    {
        String *pL = _pLeftOperand->getAs<String>();
        String *pR = _pRightOperand->getAs<String>();

        int iResult = AddStringToString(pL, pR, (String**)&pResult);

        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    // FIXME: Overload or dedicated function.
    //    else if(TypeL == GenericType::ScilabInt && TypeR == GenericType::ScilabInt)
    //    {
    //    }

    /*
    ** DOUBLE + POLY
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isPoly())
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Polynom *pR = _pRightOperand->getAs<Polynom>();

        int iResult = AddDoubleToPoly(pR, pL, (Polynom**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + DOUBLE
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isDouble())
    {
        Double *pR  = _pRightOperand->getAs<Double>();
        Polynom *pL = _pLeftOperand->getAs<Polynom>();

        int iResult = AddDoubleToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + POLY
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isPoly())
    {
        Polynom *pL = _pLeftOperand->getAs<Polynom>();
        Polynom *pR = _pRightOperand->getAs<Polynom>();

        int iResult = AddPolyToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            if (iResult == 1)
            {
                wchar_t pMsg[bsiz];
                os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
                throw ast::ScilabError(pMsg);
            }
            else if (iResult == 2)
            {
                std::wostringstream os;
                os << _W("variables don't have the same formal variable");
                //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                throw ast::ScilabError(os.str());
            }
        }

        return pResult;
    }

    /*
    ** SPARSE + SPARSE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = AddSparseToSparse(pL, pR, (Sparse**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE + DOUBLE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = AddDoubleToSparse(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE + SPARSE
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = AddSparseToDouble(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE + STRING
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isString())
    {
        if (_pLeftOperand->getAs<Double>()->getSize() == 0)
        {
            //[] + "" -> ""
            return _pRightOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition : Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** STRING + DOUBLE
    */
    else if (_pLeftOperand->isString() && _pRightOperand->isDouble())
    {
        if (_pRightOperand->getAs<Double>()->getSize() == 0)
        {
            //"text" + [] -> ""
            return _pLeftOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition :  Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}


int AddDoubleToPoly(Polynom *_pPoly, Double *_pDouble, Polynom ** _pPolyOut)
{
    bool bComplex1 = _pPoly->isComplex();
    bool bComplex2 = _pDouble->isComplex();

    double *pInDblR = _pDouble->getReal();
    double *pInDblI = _pDouble->getImg();

    if (_pPoly->isScalar())
    {
        int *piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pInPoly  = _pPoly->get(0);
            SinglePoly *pOutPoly = (*_pPolyOut)->get(i);
            double *pInPolyR     = pInPoly->getCoef()->getReal();
            double *pOutPolyR    = pOutPoly->getCoef()->getReal();

            pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

            for (int j = 1 ; j < pInPoly->getRank() ; j++)
            {
                pOutPolyR[j] = pInPolyR[j];
            }
        }

        if ((*_pPolyOut)->isComplex())
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pInPoly  = _pPoly->get(0);
                SinglePoly *pOutPoly = (*_pPolyOut)->get(i);
                double *pInPolyI     = pInPoly->getCoef()->getImg();
                double *pOutPolyI    = pOutPoly->getCoef()->getImg();

                pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

                for (int j = 1 ; j < pInPoly->getRank() ; j++)
                {
                    pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
                }
            }
        }

        return 0;
    }

    if (_pDouble->isScalar())
    {
        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();

        if (bComplex1 && bComplex2)
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
                double *pOutPolyR    = pSPOut->getCoef()->getReal();
                double *pOutPolyI    = pSPOut->getCoef()->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] += pInDblI[0];
            }
        }
        else if (bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
                double *pOutPolyR    = pSPOut->getCoef()->getReal();
                double *pOutPolyI    = pSPOut->getCoef()->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] = pInDblI[0];
            }
        }
        else
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut = (*_pPolyOut)->get(i);
                double *pOutPolyR  = pSPOut->getCoef()->getReal();

                pOutPolyR[0] += pInDblR[0];
            }
        }

        return 0;
    }

    int iDims1 = _pPoly->getDims();
    int iDims2 = _pDouble->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pPoly->getDimsArray();
    int* piDims2 = _pDouble->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();
    if (bComplex1 && bComplex2)
    {
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
            double *pOutPolyR    = pSPOut->getCoef()->getReal();
            double *pOutPolyI    = pSPOut->getCoef()->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] += pInDblI[i];
        }
    }
    else if (bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
            double *pOutPolyR    = pSPOut->getCoef()->getReal();
            double *pOutPolyI    = pSPOut->getCoef()->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] = pInDblI[i];
        }
    }
    else
    {
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut = (*_pPolyOut)->get(i);
            double *pOutPolyR  = pSPOut->getCoef()->getReal();

            pOutPolyR[0] += pInDblR[i];
        }
    }

    return 0;
}

int AddPolyToPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom ** _pPolyOut)
{
    if (_pPoly1->getVariableName() != _pPoly2->getVariableName())
    {
        return 2;
    }

    if (_pPoly1->isScalar())
    {
        int *pRank = new int[_pPoly2->getSize()];
        int *pRank1 = new int[_pPoly2->getSize()];
        int *pRank2 = new int[_pPoly2->getSize()];
        bool bComplex1 = _pPoly1->isComplex();
        bool bComplex2 = _pPoly2->isComplex();

        memset(pRank1, 0x00, _pPoly2->getSize() * sizeof(int));

        _pPoly1->getRank(pRank1);
        _pPoly2->getRank(pRank2);
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            pRank[i] = Max(pRank1[0], pRank2[i]);
        }

        (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        //Result P1(0) + P2(i)
        Double *pCoef1 = _pPoly1->get(0)->getCoef();
        double *p1R   = pCoef1->getReal();
        double *p1I   = pCoef1->getImg();
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            Double *pCoef2 = _pPoly2->get(i)->getCoef();
            double *p2R   = pCoef2->getReal();
            double *p2I   = pCoef2->getImg();

            Double *pCoefR = (*_pPolyOut)->get(i)->getCoef();
            double *pRR   = pCoefR->getReal();
            double *pRI   = pCoefR->getImg();

            for (int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[0] > pRank2[i] ? p1R : p2R);
            for (int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]) ; j++)
            {
                pRR[j] = pTemp[j];
            }

            if ((*_pPolyOut)->isComplex())
            {
                for (int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[0] > pRank2[i] ? p1I : p2I);
                for (int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]); j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        int *pRank = new int[_pPoly1->getSize()];
        int *pRank1 = new int[_pPoly1->getSize()];
        int *pRank2 = new int[_pPoly1->getSize()];
        bool bComplex1 = _pPoly1->isComplex();
        bool bComplex2 = _pPoly2->isComplex();

        memset(pRank2, 0x00, _pPoly1->getSize() * sizeof(int));

        _pPoly1->getRank(pRank1);
        _pPoly2->getRank(pRank2);
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            pRank[i] = Max(pRank1[i], pRank2[0]);
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        //Result P1(i) + P2(0)
        Double *pCoef2 = _pPoly2->get(0)->getCoef();
        double *p2R   = pCoef2->getReal();
        double *p2I   = pCoef2->getImg();

        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            Double *pCoef1 = _pPoly1->get(i)->getCoef();
            double *p1R   = pCoef1->getReal();
            double *p1I   = pCoef1->getImg();

            Double *pCoefR = (*_pPolyOut)->get(i)->getCoef();
            double *pRR   = pCoefR->getReal();
            double *pRI   = pCoefR->getImg();

            for (int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[i] > pRank2[0] ? p1R : p2R);
            for (int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]) ; j++)
            {
                pRR[j] = pTemp[j];
            }

            if ((*_pPolyOut)->isComplex())
            {
                for (int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[i] > pRank2[0] ? p1I : p2I);
                for (int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]); j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;
        return 0;
    }

    int iDims1 = _pPoly1->getDims();
    int iDims2 = _pPoly2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pPoly1->getDimsArray();
    int* piDims2 = _pPoly2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    int *pRank = new int[_pPoly1->getSize()];
    int *pRank1 = new int[_pPoly1->getSize()];
    int *pRank2 = new int[_pPoly2->getSize()];
    bool bComplex1 = _pPoly1->isComplex();
    bool bComplex2 = _pPoly2->isComplex();

    _pPoly1->getRank(pRank1);
    _pPoly2->getRank(pRank2);
    for (int i = 0 ; i < _pPoly1->getSize() ; i++)
    {
        pRank[i] = Max(pRank1[i], pRank2[i]);
    }

    (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), pRank);
    if (_pPoly1->isComplex() || _pPoly2->isComplex())
    {
        (*_pPolyOut)->setComplex(true);
    }

    if (bComplex1 == false && bComplex2 == false)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddScilabPolynomToScilabPolynom(
                _pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), pRank[i]);
        }
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddScilabPolynomToComplexPoly(
                _pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), _pPoly2->get(i)->getCoef()->getImg(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddScilabPolynomToComplexPoly(
                _pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
                _pPoly1->get(i)->getCoef()->getReal(), _pPoly1->get(i)->getCoef()->getImg(), pRank1[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddComplexPolyToComplexPoly(
                _pPoly1->get(i)->getCoef()->getReal(), _pPoly1->get(i)->getCoef()->getImg(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), _pPoly2->get(i)->getCoef()->getImg(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }

    delete[] pRank;
    delete[] pRank1;
    delete[] pRank2;

    if ((*_pPolyOut) != NULL)
    {
        (*_pPolyOut)->updateRank();
    }

    //if pResult == NULL -> incompatible dimensions
    return 0;
}

int AddStringToString(String *_pString1, String *_pString2, String **_pStringOut)
{
    if (_pString1->isScalar())
    {
        //concat pL with each element of pR
        (*_pStringOut)  = new String(_pString2->getDims(), _pString2->getDimsArray());

        int iCommonLen = (int)wcslen(_pString1->get(0));

        for (int i = 0 ; i < _pString2->getSize() ; i++)
        {
            int iLen = (int)wcslen(_pString2->get(i));
            wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
            memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));
            memcpy(psz, _pString1->get(0), iCommonLen * sizeof(wchar_t));
            memcpy(psz + iCommonLen, _pString2->get(i), iLen * sizeof(wchar_t));
            (*_pStringOut)->set(i, psz);
            delete[] psz;
        }
        return 0;
    }

    if (_pString2->isScalar())
    {
        //concat each element of pL with pR
        (*_pStringOut)  = new String(_pString1->getDims(), _pString1->getDimsArray());
        int iCommonLen = (int)wcslen(_pString2->get(0));

        for (int i = 0 ; i < _pString1->getSize() ; i++)
        {
            int iLen = (int)wcslen(_pString1->get(i));
            wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
            memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));

            memcpy(psz, _pString1->get(i), iLen * sizeof(wchar_t));
            memcpy(psz + iLen, _pString2->get(0), iCommonLen * sizeof(wchar_t));

            (*_pStringOut)->set(i, psz);
            delete[] psz;
        }

        return 0;
    }

    int iDims1 = _pString1->getDims();
    int iDims2 = _pString2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pString1->getDimsArray();
    int* piDims2 = _pString2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    (*_pStringOut) = new String(_pString1->getDims(), _pString1->getDimsArray());
    for (int i = 0 ; i < _pString1->getSize() ; i++)
    {
        int iLenL = (int)wcslen(_pString1->get(i));
        int iLenR = (int)wcslen(_pString2->get(i));
        wchar_t* psz = new wchar_t[iLenL + iLenR + 1];
        memset(psz, 0x00, (iLenL + iLenR + 1) * sizeof(wchar_t));

        memcpy(psz          , _pString1->get(i), iLenL * sizeof(wchar_t));
        memcpy(psz + iLenL  , _pString2->get(i), iLenR * sizeof(wchar_t));

        (*_pStringOut)->set(i, psz);
        delete[] psz;
    }

    return 0;
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
            for (int i = 0 ; i < Min(sp->getRows() , sp->getCols()) ; i++)
            {
                pS->set(i, i, std::complex<double>(d->get(0), d->getImg(0)));
            }
        }
        else
        {
            for (int i = 0 ; i < Min(sp->getRows() , sp->getCols()) ; i++)
            {
                pS->set(i, i, d->get(0));
            }
        }

        AddSparseToSparse(sp, pS, (Sparse**)pDRes);
        delete pS;
        return 0;
    }

    if (sp->isScalar() && d->isScalar())
    {
        //sp + d
        Double* pRes = d->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);
        if (bComplex1)
        {
            std::complex<double> dbl = sp->getImg(0, 0);
            pRes->set(0, pRes->get(0) + dbl.real());
            pRes->setImg(0, pRes->getImg(0) + dbl.imag());
        }
        else
        {
            pRes->set(0, pRes->get(0) + sp->get(0, 0));
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
        Double* pRes = d->clone()->getAs<Double>();
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
        Double* pRes = d->clone()->getAs<Double>();
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
types::InternalType* add_M_M(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return NULL;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return NULL;
        }
    }

    O* pOut = new O(iDimsL, piDimsL);

    add(_pL->get(), (long long)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_M_MC(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return NULL;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return NULL;
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);

    add(_pL->get(), (long long)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    add(_pL->get(), (long long)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), (long long)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_M_E(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    add(_pL->get(), (long long)_pL->getSize(), pOut->get());
    return pOut;
}


template<class T, class U, class O>
types::InternalType* add_MC_M(T *_pL, U *_pR)
{
    return add_M_MC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_MC_MC(T *_pL, U *_pR)
{
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        return NULL;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return NULL;
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);

    add(_pL->get(), _pL->getImg(), (long long)_pL->getSize(), _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), _pL->getImg(), (long long)_pL->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), _pL->getImg(), (long long)_pL->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_MC_E(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    add(_pL->get(), _pL->getImg(), (long long)_pL->getSize(), pOut->get(), pOut->getImg());
    return pOut;
}


template<class T, class U, class O>
types::InternalType* add_S_M(T *_pL, U *_pR)
{
    return add_M_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_S_MC(T *_pL, U *_pR)
{
    return add_MC_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    add(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_S_E(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    add(_pL->get(0), pOut->get());
    return pOut;
}


template<class T, class U, class O>
types::InternalType* add_SC_M(T *_pL, U *_pR)
{
    return add_M_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_SC_MC(T *_pL, U *_pR)
{
    return add_MC_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_SC_S(T *_pL, U *_pR)
{
    return add_S_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
types::InternalType* add_SC_E(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    add(_pL->get(0), _pL->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}


template<class T, class U, class O>
types::InternalType* add_E_M(T *_pL, U *_pR)
{
    return add_M_E<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_E_MC(T *_pL, U *_pR)
{
    return add_MC_E<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_E_S(T *_pL, U *_pR)
{
    return add_S_E<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_E_SC(T *_pL, U *_pR)
{
    return add_SC_E<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
types::InternalType* add_E_E(T *_pL, U *_pR)
{
    Double* pOut = Double::Empty();
    add();
    return pOut;
}

