/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

extern "C"
{
#include "os_string.h"
}

#include "types_or.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "sparse.hxx"

using namespace types;

//define arrays on operation functions
static or_function pOrfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillOrFunction()
{
#define scilab_fill_or(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pOrfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (or_function)&or_##func<typeIn1, typeIn2, typeOut>

    //Double
    scilab_fill_or(Double, Double, M_M, Double, Double, Bool);
    scilab_fill_or(Double, Bool, M_M, Double, Bool, Bool);

    scilab_fill_or(Double, ScalarDouble, M_S, Double, Double, Bool);
    scilab_fill_or(Double, ScalarBool, M_S, Double, Bool, Bool);

    scilab_fill_or(Double, Empty, M_E, Double, Double, Double);
    scilab_fill_or(Double, Identity, M_S, Double, Double, Bool);

    scilab_fill_or(ScalarDouble, Double, S_M, Double, Double, Bool);
    scilab_fill_or(ScalarDouble, Bool, S_M, Double, Bool, Bool);

    scilab_fill_or(ScalarDouble, ScalarDouble, S_S, Double, Double, Bool);
    scilab_fill_or(ScalarDouble, ScalarBool, S_S, Double, Bool, Bool);

    scilab_fill_or(ScalarDouble, Empty, M_E, Double, Double, Double);
    scilab_fill_or(ScalarDouble, Identity, S_S, Double, Double, Bool);

    //Bool
    scilab_fill_or(Bool, Double, M_M, Bool, Double, Bool);
    scilab_fill_or(Bool, Bool, M_M, Bool, Bool, Bool);

    scilab_fill_or(Bool, ScalarDouble, M_S, Bool, Double, Bool);
    scilab_fill_or(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    scilab_fill_or(Bool, Empty, M_E, Bool, Double, Bool);
    scilab_fill_or(Bool, Identity, M_S, Bool, Double, Bool);

    scilab_fill_or(ScalarBool, Double, S_M, Bool, Double, Bool);
    scilab_fill_or(ScalarBool, Bool, S_M, Bool, Bool, Bool);

    scilab_fill_or(ScalarBool, ScalarDouble, S_S, Bool, Double, Bool);
    scilab_fill_or(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    scilab_fill_or(ScalarBool, Empty, M_E, Bool, Double, Bool);
    scilab_fill_or(ScalarBool, Identity, S_S, Bool, Double, Bool);

    // []
    scilab_fill_or(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_or(Empty, Bool, E_M, Double, Bool, Bool);
    scilab_fill_or(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_or(Empty, ScalarBool, E_M, Double, Bool, Bool);
    scilab_fill_or(Empty, Empty, E_M, Double, Double, Double);
    scilab_fill_or(Empty, Identity, E_M, Double, Double, Double);

    // Identity
    scilab_fill_or(Identity, Double, I_M, Double, Double, Bool);
    scilab_fill_or(Identity, Bool, I_M, Double, Bool, Bool);

    scilab_fill_or(Identity, ScalarDouble, I_S, Double, Double, Bool);
    scilab_fill_or(Identity, ScalarBool, I_S, Double, Bool, Bool);

    scilab_fill_or(Identity, Empty, M_E, Double, Double, Bool);
    scilab_fill_or(Identity, Identity, I_S, Double, Double, Bool);


    //int8
    scilab_fill_or(Int8, Int8, int_M_M, Int8, Int8, Int8);
    scilab_fill_or(Int8, UInt8, int_M_M, Int8, UInt8, UInt8);
    scilab_fill_or(Int8, Int16, int_M_M, Int8, Int16, Int16);
    scilab_fill_or(Int8, UInt16, int_M_M, Int8, UInt16, UInt16);
    scilab_fill_or(Int8, Int32, int_M_M, Int8, Int32, Int32);
    scilab_fill_or(Int8, UInt32, int_M_M, Int8, UInt32, UInt32);
    scilab_fill_or(Int8, Int64, int_M_M, Int8, Int64, Int64);
    scilab_fill_or(Int8, UInt64, int_M_M, Int8, UInt64, UInt64);

    scilab_fill_or(Int8, ScalarInt8, int_M_S, Int8, Int8, Int8);
    scilab_fill_or(Int8, ScalarUInt8, int_M_S, Int8, UInt8, UInt8);
    scilab_fill_or(Int8, ScalarInt16, int_M_S, Int8, Int16, Int16);
    scilab_fill_or(Int8, ScalarUInt16, int_M_S, Int8, UInt16, UInt16);
    scilab_fill_or(Int8, ScalarInt32, int_M_S, Int8, Int32, Int32);
    scilab_fill_or(Int8, ScalarUInt32, int_M_S, Int8, UInt32, UInt32);
    scilab_fill_or(Int8, ScalarInt64, int_M_S, Int8, Int64, Int64);
    scilab_fill_or(Int8, ScalarUInt64, int_M_S, Int8, UInt64, UInt64);

    scilab_fill_or(ScalarInt8, Int8, int_S_M, Int8, Int8, Int8);
    scilab_fill_or(ScalarInt8, UInt8, int_S_M, Int8, UInt8, UInt8);
    scilab_fill_or(ScalarInt8, Int16, int_S_M, Int8, Int16, Int16);
    scilab_fill_or(ScalarInt8, UInt16, int_S_M, Int8, UInt16, UInt16);
    scilab_fill_or(ScalarInt8, Int32, int_S_M, Int8, Int32, Int32);
    scilab_fill_or(ScalarInt8, UInt32, int_S_M, Int8, UInt32, UInt32);
    scilab_fill_or(ScalarInt8, Int64, int_S_M, Int8, Int64, Int64);
    scilab_fill_or(ScalarInt8, UInt64, int_S_M, Int8, UInt64, UInt64);

    scilab_fill_or(ScalarInt8, ScalarInt8, int_S_S, Int8, Int8, Int8);
    scilab_fill_or(ScalarInt8, ScalarUInt8, int_S_S, Int8, UInt8, UInt8);
    scilab_fill_or(ScalarInt8, ScalarInt16, int_S_S, Int8, Int16, Int16);
    scilab_fill_or(ScalarInt8, ScalarUInt16, int_S_S, Int8, UInt16, UInt16);
    scilab_fill_or(ScalarInt8, ScalarInt32, int_S_S, Int8, Int32, Int32);
    scilab_fill_or(ScalarInt8, ScalarUInt32, int_S_S, Int8, UInt32, UInt32);
    scilab_fill_or(ScalarInt8, ScalarInt64, int_S_S, Int8, Int64, Int64);
    scilab_fill_or(ScalarInt8, ScalarUInt64, int_S_S, Int8, UInt64, UInt64);

    //uint8
    scilab_fill_or(UInt8, Int8, int_M_M, UInt8, Int8, UInt8);
    scilab_fill_or(UInt8, UInt8, int_M_M, UInt8, UInt8, UInt8);
    scilab_fill_or(UInt8, Int16, int_M_M, UInt8, Int16, UInt16);
    scilab_fill_or(UInt8, UInt16, int_M_M, UInt8, UInt16, UInt16);
    scilab_fill_or(UInt8, Int32, int_M_M, UInt8, Int32, UInt32);
    scilab_fill_or(UInt8, UInt32, int_M_M, UInt8, UInt32, UInt32);
    scilab_fill_or(UInt8, Int64, int_M_M, UInt8, Int64, UInt64);
    scilab_fill_or(UInt8, UInt64, int_M_M, UInt8, UInt64, UInt64);

    scilab_fill_or(UInt8, ScalarInt8, int_M_S, UInt8, Int8, UInt8);
    scilab_fill_or(UInt8, ScalarUInt8, int_M_S, UInt8, UInt8, UInt8);
    scilab_fill_or(UInt8, ScalarInt16, int_M_S, UInt8, Int16, UInt16);
    scilab_fill_or(UInt8, ScalarUInt16, int_M_S, UInt8, UInt16, UInt16);
    scilab_fill_or(UInt8, ScalarInt32, int_M_S, UInt8, Int32, UInt32);
    scilab_fill_or(UInt8, ScalarUInt32, int_M_S, UInt8, UInt32, UInt32);
    scilab_fill_or(UInt8, ScalarInt64, int_M_S, UInt8, Int64, UInt64);
    scilab_fill_or(UInt8, ScalarUInt64, int_M_S, UInt8, UInt64, UInt64);

    scilab_fill_or(ScalarUInt8, Int8, int_S_M, UInt8, Int8, UInt8);
    scilab_fill_or(ScalarUInt8, UInt8, int_S_M, UInt8, UInt8, UInt8);
    scilab_fill_or(ScalarUInt8, Int16, int_S_M, UInt8, Int16, UInt16);
    scilab_fill_or(ScalarUInt8, UInt16, int_S_M, UInt8, UInt16, UInt16);
    scilab_fill_or(ScalarUInt8, Int32, int_S_M, UInt8, Int32, UInt32);
    scilab_fill_or(ScalarUInt8, UInt32, int_S_M, UInt8, UInt32, UInt32);
    scilab_fill_or(ScalarUInt8, Int64, int_S_M, UInt8, Int64, UInt64);
    scilab_fill_or(ScalarUInt8, UInt64, int_S_M, UInt8, UInt64, UInt64);

    scilab_fill_or(ScalarUInt8, ScalarInt8, int_S_S, UInt8, Int8, UInt8);
    scilab_fill_or(ScalarUInt8, ScalarUInt8, int_S_S, UInt8, UInt8, UInt8);
    scilab_fill_or(ScalarUInt8, ScalarInt16, int_S_S, UInt8, Int16, UInt16);
    scilab_fill_or(ScalarUInt8, ScalarUInt16, int_S_S, UInt8, UInt16, UInt16);
    scilab_fill_or(ScalarUInt8, ScalarInt32, int_S_S, UInt8, Int32, UInt32);
    scilab_fill_or(ScalarUInt8, ScalarUInt32, int_S_S, UInt8, UInt32, UInt32);
    scilab_fill_or(ScalarUInt8, ScalarInt64, int_S_S, UInt8, Int64, UInt64);
    scilab_fill_or(ScalarUInt8, ScalarUInt64, int_S_S, UInt8, UInt64, UInt64);

    //int16
    scilab_fill_or(Int16, Int8, int_M_M, Int16, Int8, Int16);
    scilab_fill_or(Int16, UInt8, int_M_M, Int16, UInt8, UInt16);
    scilab_fill_or(Int16, Int16, int_M_M, Int16, Int16, Int16);
    scilab_fill_or(Int16, UInt16, int_M_M, Int16, UInt16, UInt16);
    scilab_fill_or(Int16, Int32, int_M_M, Int16, Int32, Int32);
    scilab_fill_or(Int16, UInt32, int_M_M, Int16, UInt32, UInt32);
    scilab_fill_or(Int16, Int64, int_M_M, Int16, Int64, Int64);
    scilab_fill_or(Int16, UInt64, int_M_M, Int16, UInt64, UInt64);

    scilab_fill_or(Int16, ScalarInt8, int_M_S, Int16, Int8, Int16);
    scilab_fill_or(Int16, ScalarUInt8, int_M_S, Int16, UInt8, UInt16);
    scilab_fill_or(Int16, ScalarInt16, int_M_S, Int16, Int16, Int16);
    scilab_fill_or(Int16, ScalarUInt16, int_M_S, Int16, UInt16, UInt16);
    scilab_fill_or(Int16, ScalarInt32, int_M_S, Int16, Int32, Int32);
    scilab_fill_or(Int16, ScalarUInt32, int_M_S, Int16, UInt32, UInt32);
    scilab_fill_or(Int16, ScalarInt64, int_M_S, Int16, Int64, Int64);
    scilab_fill_or(Int16, ScalarUInt64, int_M_S, Int16, UInt64, UInt64);

    scilab_fill_or(ScalarInt16, Int8, int_S_M, Int16, Int8, Int16);
    scilab_fill_or(ScalarInt16, UInt8, int_S_M, Int16, UInt8, UInt16);
    scilab_fill_or(ScalarInt16, Int16, int_S_M, Int16, Int16, Int16);
    scilab_fill_or(ScalarInt16, UInt16, int_S_M, Int16, UInt16, UInt16);
    scilab_fill_or(ScalarInt16, Int32, int_S_M, Int16, Int32, Int32);
    scilab_fill_or(ScalarInt16, UInt32, int_S_M, Int16, UInt32, UInt32);
    scilab_fill_or(ScalarInt16, Int64, int_S_M, Int16, Int64, Int64);
    scilab_fill_or(ScalarInt16, UInt64, int_S_M, Int16, UInt64, UInt64);

    scilab_fill_or(ScalarInt16, ScalarInt8, int_S_S, Int16, Int8, Int16);
    scilab_fill_or(ScalarInt16, ScalarUInt8, int_S_S, Int16, UInt8, UInt16);
    scilab_fill_or(ScalarInt16, ScalarInt16, int_S_S, Int16, Int16, Int16);
    scilab_fill_or(ScalarInt16, ScalarUInt16, int_S_S, Int16, UInt16, UInt16);
    scilab_fill_or(ScalarInt16, ScalarInt32, int_S_S, Int16, Int32, Int32);
    scilab_fill_or(ScalarInt16, ScalarUInt32, int_S_S, Int16, UInt32, UInt32);
    scilab_fill_or(ScalarInt16, ScalarInt64, int_S_S, Int16, Int64, Int64);
    scilab_fill_or(ScalarInt16, ScalarUInt64, int_S_S, Int16, UInt64, UInt64);

    //uint16
    scilab_fill_or(UInt16, Int8, int_M_M, UInt16, Int8, UInt16);
    scilab_fill_or(UInt16, UInt8, int_M_M, UInt16, UInt8, UInt16);
    scilab_fill_or(UInt16, Int16, int_M_M, UInt16, Int16, UInt16);
    scilab_fill_or(UInt16, UInt16, int_M_M, UInt16, UInt16, UInt16);
    scilab_fill_or(UInt16, Int32, int_M_M, UInt16, Int32, UInt32);
    scilab_fill_or(UInt16, UInt32, int_M_M, UInt16, UInt32, UInt32);
    scilab_fill_or(UInt16, Int64, int_M_M, UInt16, Int64, UInt64);
    scilab_fill_or(UInt16, UInt64, int_M_M, UInt16, UInt64, UInt64);

    scilab_fill_or(UInt16, ScalarInt8, int_M_S, UInt16, Int8, UInt16);
    scilab_fill_or(UInt16, ScalarUInt8, int_M_S, UInt16, UInt8, UInt16);
    scilab_fill_or(UInt16, ScalarInt16, int_M_S, UInt16, Int16, UInt16);
    scilab_fill_or(UInt16, ScalarUInt16, int_M_S, UInt16, UInt16, UInt16);
    scilab_fill_or(UInt16, ScalarInt32, int_M_S, UInt16, Int32, UInt32);
    scilab_fill_or(UInt16, ScalarUInt32, int_M_S, UInt16, UInt32, UInt32);
    scilab_fill_or(UInt16, ScalarInt64, int_M_S, UInt16, Int64, UInt64);
    scilab_fill_or(UInt16, ScalarUInt64, int_M_S, UInt16, UInt64, UInt64);

    scilab_fill_or(ScalarUInt16, Int8, int_S_M, UInt16, Int8, UInt16);
    scilab_fill_or(ScalarUInt16, UInt8, int_S_M, UInt16, UInt8, UInt16);
    scilab_fill_or(ScalarUInt16, Int16, int_S_M, UInt16, Int16, UInt16);
    scilab_fill_or(ScalarUInt16, UInt16, int_S_M, UInt16, UInt16, UInt16);
    scilab_fill_or(ScalarUInt16, Int32, int_S_M, UInt16, Int32, UInt32);
    scilab_fill_or(ScalarUInt16, UInt32, int_S_M, UInt16, UInt32, UInt32);
    scilab_fill_or(ScalarUInt16, Int64, int_S_M, UInt16, Int64, UInt64);
    scilab_fill_or(ScalarUInt16, UInt64, int_S_M, UInt16, UInt64, UInt64);

    scilab_fill_or(ScalarUInt16, ScalarInt8, int_S_S, UInt16, Int8, UInt16);
    scilab_fill_or(ScalarUInt16, ScalarUInt8, int_S_S, UInt16, UInt8, UInt16);
    scilab_fill_or(ScalarUInt16, ScalarInt16, int_S_S, UInt16, Int16, UInt16);
    scilab_fill_or(ScalarUInt16, ScalarUInt16, int_S_S, UInt16, UInt16, UInt16);
    scilab_fill_or(ScalarUInt16, ScalarInt32, int_S_S, UInt16, Int32, UInt32);
    scilab_fill_or(ScalarUInt16, ScalarUInt32, int_S_S, UInt16, UInt32, UInt32);
    scilab_fill_or(ScalarUInt16, ScalarInt64, int_S_S, UInt16, Int64, UInt64);
    scilab_fill_or(ScalarUInt16, ScalarUInt64, int_S_S, UInt16, UInt64, UInt64);

    //int32
    scilab_fill_or(Int32, Int8, int_M_M, Int32, Int8, Int32);
    scilab_fill_or(Int32, UInt8, int_M_M, Int32, UInt8, UInt32);
    scilab_fill_or(Int32, Int16, int_M_M, Int32, Int16, Int32);
    scilab_fill_or(Int32, UInt16, int_M_M, Int32, UInt16, UInt32);
    scilab_fill_or(Int32, Int32, int_M_M, Int32, Int32, Int32);
    scilab_fill_or(Int32, UInt32, int_M_M, Int32, UInt32, UInt32);
    scilab_fill_or(Int32, Int64, int_M_M, Int32, Int64, Int64);
    scilab_fill_or(Int32, UInt64, int_M_M, Int32, UInt64, UInt64);

    scilab_fill_or(Int32, ScalarInt8, int_M_S, Int32, Int8, Int32);
    scilab_fill_or(Int32, ScalarUInt8, int_M_S, Int32, UInt8, UInt32);
    scilab_fill_or(Int32, ScalarInt16, int_M_S, Int32, Int16, Int32);
    scilab_fill_or(Int32, ScalarUInt16, int_M_S, Int32, UInt16, UInt32);
    scilab_fill_or(Int32, ScalarInt32, int_M_S, Int32, Int32, Int32);
    scilab_fill_or(Int32, ScalarUInt32, int_M_S, Int32, UInt32, UInt32);
    scilab_fill_or(Int32, ScalarInt64, int_M_S, Int32, Int64, Int64);
    scilab_fill_or(Int32, ScalarUInt64, int_M_S, Int32, UInt64, UInt64);

    scilab_fill_or(ScalarInt32, Int8, int_S_M, Int32, Int8, Int32);
    scilab_fill_or(ScalarInt32, UInt8, int_S_M, Int32, UInt8, UInt32);
    scilab_fill_or(ScalarInt32, Int16, int_S_M, Int32, Int16, Int32);
    scilab_fill_or(ScalarInt32, UInt16, int_S_M, Int32, UInt16, UInt32);
    scilab_fill_or(ScalarInt32, Int32, int_S_M, Int32, Int32, Int32);
    scilab_fill_or(ScalarInt32, UInt32, int_S_M, Int32, UInt32, UInt32);
    scilab_fill_or(ScalarInt32, Int64, int_S_M, Int32, Int64, Int64);
    scilab_fill_or(ScalarInt32, UInt64, int_S_M, Int32, UInt64, UInt64);

    scilab_fill_or(ScalarInt32, ScalarInt8, int_S_S, Int32, Int8, Int32);
    scilab_fill_or(ScalarInt32, ScalarUInt8, int_S_S, Int32, UInt8, UInt32);
    scilab_fill_or(ScalarInt32, ScalarInt16, int_S_S, Int32, Int16, Int32);
    scilab_fill_or(ScalarInt32, ScalarUInt16, int_S_S, Int32, UInt16, UInt32);
    scilab_fill_or(ScalarInt32, ScalarInt32, int_S_S, Int32, Int32, Int32);
    scilab_fill_or(ScalarInt32, ScalarUInt32, int_S_S, Int32, UInt32, UInt32);
    scilab_fill_or(ScalarInt32, ScalarInt64, int_S_S, Int32, Int64, Int64);
    scilab_fill_or(ScalarInt32, ScalarUInt64, int_S_S, Int32, UInt64, UInt64);

    //uint32
    scilab_fill_or(UInt32, Int8, int_M_M, UInt32, Int8, UInt32);
    scilab_fill_or(UInt32, UInt8, int_M_M, UInt32, UInt8, UInt32);
    scilab_fill_or(UInt32, Int16, int_M_M, UInt32, Int16, UInt32);
    scilab_fill_or(UInt32, UInt16, int_M_M, UInt32, UInt16, UInt32);
    scilab_fill_or(UInt32, Int32, int_M_M, UInt32, Int32, UInt32);
    scilab_fill_or(UInt32, UInt32, int_M_M, UInt32, UInt32, UInt32);
    scilab_fill_or(UInt32, Int64, int_M_M, UInt32, Int64, UInt64);
    scilab_fill_or(UInt32, UInt64, int_M_M, UInt32, UInt64, UInt64);

    scilab_fill_or(UInt32, ScalarInt8, int_M_S, UInt32, Int8, UInt32);
    scilab_fill_or(UInt32, ScalarUInt8, int_M_S, UInt32, UInt8, UInt32);
    scilab_fill_or(UInt32, ScalarInt16, int_M_S, UInt32, Int16, UInt32);
    scilab_fill_or(UInt32, ScalarUInt16, int_M_S, UInt32, UInt16, UInt32);
    scilab_fill_or(UInt32, ScalarInt32, int_M_S, UInt32, Int32, UInt32);
    scilab_fill_or(UInt32, ScalarUInt32, int_M_S, UInt32, UInt32, UInt32);
    scilab_fill_or(UInt32, ScalarInt64, int_M_S, UInt32, Int64, UInt64);
    scilab_fill_or(UInt32, ScalarUInt64, int_M_S, UInt32, UInt64, UInt64);

    scilab_fill_or(ScalarUInt32, Int8, int_S_M, UInt32, Int8, UInt32);
    scilab_fill_or(ScalarUInt32, UInt8, int_S_M, UInt32, UInt8, UInt32);
    scilab_fill_or(ScalarUInt32, Int16, int_S_M, UInt32, Int16, UInt32);
    scilab_fill_or(ScalarUInt32, UInt16, int_S_M, UInt32, UInt16, UInt32);
    scilab_fill_or(ScalarUInt32, Int32, int_S_M, UInt32, Int32, UInt32);
    scilab_fill_or(ScalarUInt32, UInt32, int_S_M, UInt32, UInt32, UInt32);
    scilab_fill_or(ScalarUInt32, Int64, int_S_M, UInt32, Int64, UInt64);
    scilab_fill_or(ScalarUInt32, UInt64, int_S_M, UInt32, UInt64, UInt64);

    scilab_fill_or(ScalarUInt32, ScalarInt8, int_S_S, UInt32, Int8, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarUInt8, int_S_S, UInt32, UInt8, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarInt16, int_S_S, UInt32, Int16, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarUInt16, int_S_S, UInt32, UInt16, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarInt32, int_S_S, UInt32, Int32, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarUInt32, int_S_S, UInt32, UInt32, UInt32);
    scilab_fill_or(ScalarUInt32, ScalarInt64, int_S_S, UInt32, Int64, UInt64);
    scilab_fill_or(ScalarUInt32, ScalarUInt64, int_S_S, UInt32, UInt64, UInt64);

    //int64
    scilab_fill_or(Int64, Int8, int_M_M, Int64, Int8, Int64);
    scilab_fill_or(Int64, UInt8, int_M_M, Int64, UInt8, UInt64);
    scilab_fill_or(Int64, Int16, int_M_M, Int64, Int16, Int64);
    scilab_fill_or(Int64, UInt16, int_M_M, Int64, UInt16, UInt64);
    scilab_fill_or(Int64, Int32, int_M_M, Int64, Int32, Int64);
    scilab_fill_or(Int64, UInt32, int_M_M, Int64, UInt32, UInt64);
    scilab_fill_or(Int64, Int64, int_M_M, Int64, Int64, Int64);
    scilab_fill_or(Int64, UInt64, int_M_M, Int64, UInt64, UInt64);

    scilab_fill_or(Int64, ScalarInt8, int_M_S, Int64, Int8, Int64);
    scilab_fill_or(Int64, ScalarUInt8, int_M_S, Int64, UInt8, UInt64);
    scilab_fill_or(Int64, ScalarInt16, int_M_S, Int64, Int16, Int64);
    scilab_fill_or(Int64, ScalarUInt16, int_M_S, Int64, UInt16, UInt64);
    scilab_fill_or(Int64, ScalarInt32, int_M_S, Int64, Int32, Int64);
    scilab_fill_or(Int64, ScalarUInt32, int_M_S, Int64, UInt32, UInt64);
    scilab_fill_or(Int64, ScalarInt64, int_M_S, Int64, Int64, Int64);
    scilab_fill_or(Int64, ScalarUInt64, int_M_S, Int64, UInt64, UInt64);

    scilab_fill_or(ScalarInt64, Int8, int_S_M, Int64, Int8, Int64);
    scilab_fill_or(ScalarInt64, UInt8, int_S_M, Int64, UInt8, UInt64);
    scilab_fill_or(ScalarInt64, Int16, int_S_M, Int64, Int16, Int64);
    scilab_fill_or(ScalarInt64, UInt16, int_S_M, Int64, UInt16, UInt64);
    scilab_fill_or(ScalarInt64, Int32, int_S_M, Int64, Int32, Int64);
    scilab_fill_or(ScalarInt64, UInt32, int_S_M, Int64, UInt32, UInt64);
    scilab_fill_or(ScalarInt64, Int64, int_S_M, Int64, Int64, Int64);
    scilab_fill_or(ScalarInt64, UInt64, int_S_M, Int64, UInt64, UInt64);

    scilab_fill_or(ScalarInt64, ScalarInt8, int_S_S, Int64, Int8, Int64);
    scilab_fill_or(ScalarInt64, ScalarUInt8, int_S_S, Int64, UInt8, UInt64);
    scilab_fill_or(ScalarInt64, ScalarInt16, int_S_S, Int64, Int16, Int64);
    scilab_fill_or(ScalarInt64, ScalarUInt16, int_S_S, Int64, UInt16, UInt64);
    scilab_fill_or(ScalarInt64, ScalarInt32, int_S_S, Int64, Int32, Int64);
    scilab_fill_or(ScalarInt64, ScalarUInt32, int_S_S, Int64, UInt32, UInt64);
    scilab_fill_or(ScalarInt64, ScalarInt64, int_S_S, Int64, Int64, Int64);
    scilab_fill_or(ScalarInt64, ScalarUInt64, int_S_S, Int64, UInt64, UInt64);

    //uint64
    scilab_fill_or(UInt64, Int8, int_M_M, UInt64, Int8, UInt64);
    scilab_fill_or(UInt64, UInt8, int_M_M, UInt64, UInt8, UInt64);
    scilab_fill_or(UInt64, Int16, int_M_M, UInt64, Int16, UInt64);
    scilab_fill_or(UInt64, UInt16, int_M_M, UInt64, UInt16, UInt64);
    scilab_fill_or(UInt64, Int32, int_M_M, UInt64, Int32, UInt64);
    scilab_fill_or(UInt64, UInt32, int_M_M, UInt64, UInt32, UInt64);
    scilab_fill_or(UInt64, Int64, int_M_M, UInt64, Int64, UInt64);
    scilab_fill_or(UInt64, UInt64, int_M_M, UInt64, UInt64, UInt64);

    scilab_fill_or(UInt64, ScalarInt8, int_M_S, UInt64, Int8, UInt64);
    scilab_fill_or(UInt64, ScalarUInt8, int_M_S, UInt64, UInt8, UInt64);
    scilab_fill_or(UInt64, ScalarInt16, int_M_S, UInt64, Int16, UInt64);
    scilab_fill_or(UInt64, ScalarUInt16, int_M_S, UInt64, UInt16, UInt64);
    scilab_fill_or(UInt64, ScalarInt32, int_M_S, UInt64, Int32, UInt64);
    scilab_fill_or(UInt64, ScalarUInt32, int_M_S, UInt64, UInt32, UInt64);
    scilab_fill_or(UInt64, ScalarInt64, int_M_S, UInt64, Int64, UInt64);
    scilab_fill_or(UInt64, ScalarUInt64, int_M_S, UInt64, UInt64, UInt64);

    scilab_fill_or(ScalarUInt64, Int8, int_S_M, UInt64, Int8, UInt64);
    scilab_fill_or(ScalarUInt64, UInt8, int_S_M, UInt64, UInt8, UInt64);
    scilab_fill_or(ScalarUInt64, Int16, int_S_M, UInt64, Int16, UInt64);
    scilab_fill_or(ScalarUInt64, UInt16, int_S_M, UInt64, UInt16, UInt64);
    scilab_fill_or(ScalarUInt64, Int32, int_S_M, UInt64, Int32, UInt64);
    scilab_fill_or(ScalarUInt64, UInt32, int_S_M, UInt64, UInt32, UInt64);
    scilab_fill_or(ScalarUInt64, Int64, int_S_M, UInt64, Int64, UInt64);
    scilab_fill_or(ScalarUInt64, UInt64, int_S_M, UInt64, UInt64, UInt64);

    scilab_fill_or(ScalarUInt64, ScalarInt8, int_S_S, UInt64, Int8, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarUInt8, int_S_S, UInt64, UInt8, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarInt16, int_S_S, UInt64, Int16, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarUInt16, int_S_S, UInt64, UInt16, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarInt32, int_S_S, UInt64, Int32, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarUInt32, int_S_S, UInt64, UInt32, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarInt64, int_S_S, UInt64, Int64, UInt64);
    scilab_fill_or(ScalarUInt64, ScalarUInt64, int_S_S, UInt64, UInt64, UInt64);

    //boolean sparse
    scilab_fill_or(SparseBool, SparseBool, M_M, SparseBool, SparseBool, SparseBool);
    scilab_fill_or(Bool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_or(SparseBool, Bool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_or(ScalarBool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_or(SparseBool, ScalarBool, M_M, SparseBool, Bool, SparseBool);

#undef scilab_fill_or
}

// ||
InternalType* GenericShortcutOr(InternalType* _pL)
{
    InternalType* pResult = NULL;
    switch (_pL->getType())
    {
        case InternalType::ScilabBool :
            isValueTrue(_pL->getAs<Bool>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabDouble :
            isValueTrue(_pL->getAs<Double>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt8 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt8 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt16 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt16 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt32 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt32 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt64 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt64 :
            isValueTrue(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabSparse :
            isValueTrue(_pL->getAs<SparseBool>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabSparseBool :
            isValueTrue(_pL->getAs<SparseBool>(), (Bool**)&pResult);
            break;
        default:
            // will return NULL
            break;
    }
    return pResult;
}

template<typename T>
void isValueTrue(T* _pL, types::Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //call non shortcut opearion
            *_pOut = NULL;
            return;
        }
    }

    // All values are different than 0
    *_pOut = new Bool(1); //true || something -> true
    return;
}

template<>
void isValueTrue(Double* _pL, Bool** _pOut)
{
    if (_pL->isEmpty())
    {
        //call non shorcut operation
        *_pOut = NULL;
        return;
    }

    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            if ( !_pL->isComplex() || (_pL->getImg(i) == 0) )
            {
                // Any value is false, call non shortcut operation
                *_pOut = NULL;
                return;
            }
        }
    }

    // All values are True, return True
    *_pOut = new Bool(1);
    return;
}

template<>
void isValueTrue(Sparse* _pL, Bool** _pOut)
{
    if (_pL->nonZeros() == (size_t)_pL->getSize())
    {
        *_pOut = new Bool(1);
        return;
    }

    *_pOut = NULL;
    return;
}

template<>
void isValueTrue(SparseBool* _pL, Bool** _pOut)
{
    SparseBool* pL = _pL->getAs<SparseBool>();
    if (pL->nbTrue() == pL->getSize())
    {
        *_pOut = new Bool(1);
        return;
    }

    *_pOut = NULL;
    return;
}

// |
InternalType* GenericLogicalOr(InternalType* _pL, InternalType* _pR)
{
    InternalType *pResult = NULL;

    or_function bit_or = pOrfunction[_pL->getId()][_pR->getId()];
    if (bit_or)
    {
        pResult = bit_or(_pL, _pR);
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
InternalType* or_M_M(T *_pL, U *_pR)
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

    bit_or(_pL->get(), (long long)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    bit_or(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    bit_or(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    bit_or(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_M_E(T *_pL, U *_pR)
{
    return _pR;
}

template<>
InternalType* or_M_E<Bool, Double, Bool>(Bool* _pL, Double* /* _pR */)
{
    Bool* pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
    pOut->setTrue();
    return pOut;
}

template<class T, class U, class O>
InternalType* or_E_M(T *_pL, U *_pR)
{
    return _pL;
}

template<>
InternalType* or_E_M<Double, Bool, Bool>(Double* /* _pL */, Bool*  _pR)
{
    Bool* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    pOut->setTrue();
    return pOut;
}

template<class T, class U, class O>
InternalType* or_I_S(T *_pL, U *_pR)
{
    return or_S_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* or_I_M(T *_pL, U *_pR)
{
    return or_M_S<U, T, O>(_pR, _pL);
}


template<class T, class U, class O>
InternalType* or_int_M_M(T *_pL, U *_pR)
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

    int_or(_pL->get(), (long long)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_int_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    int_or(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_int_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    int_or(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* or_int_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    int_or(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

//boolean sparse
template<>
InternalType* or_M_M<SparseBool, SparseBool, SparseBool>(SparseBool* _pL, SparseBool* _pR)
{
    SparseBool* pOut = NULL;
    if (_pL->isScalar())
    {
        if (_pL->get(0, 0))
        {
            pOut = new SparseBool(_pR->getRows(), _pR->getCols());
            int iCols = pOut->getCols();
            int iRows = pOut->getRows();
            for (int i = 0 ; i < iRows ; i++)
            {
                for (int j = 0 ; j < iCols ; j++)
                {
                    pOut->set(i, j, true, false);
                }
            }

            pOut->finalize();
        }
        else
        {
            pOut = _pR;
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        if (_pR->get(0, 0))
        {
            pOut = new SparseBool(_pL->getRows(), _pL->getCols());
            int iCols = pOut->getCols();
            int iRows = pOut->getRows();
            for (int i = 0 ; i < iRows ; i++)
            {
                for (int j = 0 ; j < iCols ; j++)
                {
                    pOut->set(i, j, true, false);
                }
            }

            pOut->finalize();
        }
        else
        {
            pOut = _pL;
        }

        return pOut;
    }

    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    return _pL->newLogicalOr(*_pR);
}

template<>
InternalType* or_M_M<SparseBool, Bool, SparseBool>(SparseBool* _pL, Bool* _pR)
{
    SparseBool* pR = new SparseBool(*_pR);
    InternalType* pOut = or_M_M<SparseBool, SparseBool, SparseBool>(_pL, pR);
    delete pR;
    return pOut;
}

template<>
InternalType* or_M_M<Bool, SparseBool, SparseBool>(Bool* _pL, SparseBool* _pR)
{
    SparseBool* pL = new SparseBool(*_pL);
    InternalType* pOut = or_M_M<SparseBool, SparseBool, SparseBool>(pL, _pR);
    delete pL;
    return pOut;
}

