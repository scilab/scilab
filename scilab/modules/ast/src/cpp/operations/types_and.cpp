/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "types_and.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "sparse.hxx"

using namespace types;

//define arrays on operation functions
static and_function pAndfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillAndFunction()
{
#define scilab_fill_and(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pAndfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (and_function)&and_##func<typeIn1, typeIn2, typeOut>

    //Double
    scilab_fill_and(Double, Double, M_M, Double, Double, Bool);
    scilab_fill_and(Double, Bool, M_M, Double, Bool, Bool);

    scilab_fill_and(Double, ScalarDouble, M_S, Double, Double, Bool);
    scilab_fill_and(Double, ScalarBool, M_S, Double, Bool, Bool);

    scilab_fill_and(Double, Empty, M_E, Double, Double, Double);
    scilab_fill_and(Double, Identity, M_S, Double, Double, Bool);

    scilab_fill_and(ScalarDouble, Double, S_M, Double, Double, Bool);
    scilab_fill_and(ScalarDouble, Bool, S_M, Double, Bool, Bool);

    scilab_fill_and(ScalarDouble, ScalarDouble, S_S, Double, Double, Bool);
    scilab_fill_and(ScalarDouble, ScalarBool, S_S, Double, Bool, Bool);

    scilab_fill_and(ScalarDouble, Empty, M_E, Double, Double, Double);
    scilab_fill_and(ScalarDouble, Identity, S_S, Double, Double, Bool);

    //Bool
    scilab_fill_and(Bool, Double, M_M, Bool, Double, Bool);
    scilab_fill_and(Bool, Bool, M_M, Bool, Bool, Bool);

    scilab_fill_and(Bool, ScalarDouble, M_S, Bool, Double, Bool);
    scilab_fill_and(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    scilab_fill_and(Bool, Empty, M_E, Bool, Double, Bool);
    scilab_fill_and(Bool, Identity, M_S, Bool, Double, Bool);

    scilab_fill_and(ScalarBool, Double, S_M, Bool, Double, Bool);
    scilab_fill_and(ScalarBool, Bool, S_M, Bool, Bool, Bool);

    scilab_fill_and(ScalarBool, ScalarDouble, S_S, Bool, Double, Bool);
    scilab_fill_and(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    scilab_fill_and(ScalarBool, Empty, M_E, Bool, Double, Bool);
    scilab_fill_and(ScalarBool, Identity, S_S, Bool, Double, Bool);

    // []
    scilab_fill_and(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_and(Empty, Bool, E_M, Double, Bool, Bool);
    scilab_fill_and(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_and(Empty, ScalarBool, E_M, Double, Bool, Bool);
    scilab_fill_and(Empty, Empty, E_M, Double, Double, Double);
    scilab_fill_and(Empty, Identity, E_M, Double, Double, Double);

    // Identity
    scilab_fill_and(Identity, Double, I_M, Double, Double, Bool);
    scilab_fill_and(Identity, Bool, I_M, Double, Bool, Bool);

    scilab_fill_and(Identity, ScalarDouble, I_S, Double, Double, Bool);
    scilab_fill_and(Identity, ScalarBool, I_S, Double, Bool, Bool);

    scilab_fill_and(Identity, Empty, M_E, Double, Double, Bool);
    scilab_fill_and(Identity, Identity, I_S, Double, Double, Bool);


    //int8
    scilab_fill_and(Int8, Int8, int_M_M, Int8, Int8, Int8);
    scilab_fill_and(Int8, UInt8, int_M_M, Int8, UInt8, UInt8);
    scilab_fill_and(Int8, Int16, int_M_M, Int8, Int16, Int16);
    scilab_fill_and(Int8, UInt16, int_M_M, Int8, UInt16, UInt16);
    scilab_fill_and(Int8, Int32, int_M_M, Int8, Int32, Int32);
    scilab_fill_and(Int8, UInt32, int_M_M, Int8, UInt32, UInt32);
    scilab_fill_and(Int8, Int64, int_M_M, Int8, Int64, Int64);
    scilab_fill_and(Int8, UInt64, int_M_M, Int8, UInt64, UInt64);

    scilab_fill_and(Int8, ScalarInt8, int_M_S, Int8, Int8, Int8);
    scilab_fill_and(Int8, ScalarUInt8, int_M_S, Int8, UInt8, UInt8);
    scilab_fill_and(Int8, ScalarInt16, int_M_S, Int8, Int16, Int16);
    scilab_fill_and(Int8, ScalarUInt16, int_M_S, Int8, UInt16, UInt16);
    scilab_fill_and(Int8, ScalarInt32, int_M_S, Int8, Int32, Int32);
    scilab_fill_and(Int8, ScalarUInt32, int_M_S, Int8, UInt32, UInt32);
    scilab_fill_and(Int8, ScalarInt64, int_M_S, Int8, Int64, Int64);
    scilab_fill_and(Int8, ScalarUInt64, int_M_S, Int8, UInt64, UInt64);

    scilab_fill_and(ScalarInt8, Int8, int_S_M, Int8, Int8, Int8);
    scilab_fill_and(ScalarInt8, UInt8, int_S_M, Int8, UInt8, UInt8);
    scilab_fill_and(ScalarInt8, Int16, int_S_M, Int8, Int16, Int16);
    scilab_fill_and(ScalarInt8, UInt16, int_S_M, Int8, UInt16, UInt16);
    scilab_fill_and(ScalarInt8, Int32, int_S_M, Int8, Int32, Int32);
    scilab_fill_and(ScalarInt8, UInt32, int_S_M, Int8, UInt32, UInt32);
    scilab_fill_and(ScalarInt8, Int64, int_S_M, Int8, Int64, Int64);
    scilab_fill_and(ScalarInt8, UInt64, int_S_M, Int8, UInt64, UInt64);

    scilab_fill_and(ScalarInt8, ScalarInt8, int_S_S, Int8, Int8, Int8);
    scilab_fill_and(ScalarInt8, ScalarUInt8, int_S_S, Int8, UInt8, UInt8);
    scilab_fill_and(ScalarInt8, ScalarInt16, int_S_S, Int8, Int16, Int16);
    scilab_fill_and(ScalarInt8, ScalarUInt16, int_S_S, Int8, UInt16, UInt16);
    scilab_fill_and(ScalarInt8, ScalarInt32, int_S_S, Int8, Int32, Int32);
    scilab_fill_and(ScalarInt8, ScalarUInt32, int_S_S, Int8, UInt32, UInt32);
    scilab_fill_and(ScalarInt8, ScalarInt64, int_S_S, Int8, Int64, Int64);
    scilab_fill_and(ScalarInt8, ScalarUInt64, int_S_S, Int8, UInt64, UInt64);

    //uint8
    scilab_fill_and(UInt8, Int8, int_M_M, UInt8, Int8, UInt8);
    scilab_fill_and(UInt8, UInt8, int_M_M, UInt8, UInt8, UInt8);
    scilab_fill_and(UInt8, Int16, int_M_M, UInt8, Int16, UInt16);
    scilab_fill_and(UInt8, UInt16, int_M_M, UInt8, UInt16, UInt16);
    scilab_fill_and(UInt8, Int32, int_M_M, UInt8, Int32, UInt32);
    scilab_fill_and(UInt8, UInt32, int_M_M, UInt8, UInt32, UInt32);
    scilab_fill_and(UInt8, Int64, int_M_M, UInt8, Int64, UInt64);
    scilab_fill_and(UInt8, UInt64, int_M_M, UInt8, UInt64, UInt64);

    scilab_fill_and(UInt8, ScalarInt8, int_M_S, UInt8, Int8, UInt8);
    scilab_fill_and(UInt8, ScalarUInt8, int_M_S, UInt8, UInt8, UInt8);
    scilab_fill_and(UInt8, ScalarInt16, int_M_S, UInt8, Int16, UInt16);
    scilab_fill_and(UInt8, ScalarUInt16, int_M_S, UInt8, UInt16, UInt16);
    scilab_fill_and(UInt8, ScalarInt32, int_M_S, UInt8, Int32, UInt32);
    scilab_fill_and(UInt8, ScalarUInt32, int_M_S, UInt8, UInt32, UInt32);
    scilab_fill_and(UInt8, ScalarInt64, int_M_S, UInt8, Int64, UInt64);
    scilab_fill_and(UInt8, ScalarUInt64, int_M_S, UInt8, UInt64, UInt64);

    scilab_fill_and(ScalarUInt8, Int8, int_S_M, UInt8, Int8, UInt8);
    scilab_fill_and(ScalarUInt8, UInt8, int_S_M, UInt8, UInt8, UInt8);
    scilab_fill_and(ScalarUInt8, Int16, int_S_M, UInt8, Int16, UInt16);
    scilab_fill_and(ScalarUInt8, UInt16, int_S_M, UInt8, UInt16, UInt16);
    scilab_fill_and(ScalarUInt8, Int32, int_S_M, UInt8, Int32, UInt32);
    scilab_fill_and(ScalarUInt8, UInt32, int_S_M, UInt8, UInt32, UInt32);
    scilab_fill_and(ScalarUInt8, Int64, int_S_M, UInt8, Int64, UInt64);
    scilab_fill_and(ScalarUInt8, UInt64, int_S_M, UInt8, UInt64, UInt64);

    scilab_fill_and(ScalarUInt8, ScalarInt8, int_S_S, UInt8, Int8, UInt8);
    scilab_fill_and(ScalarUInt8, ScalarUInt8, int_S_S, UInt8, UInt8, UInt8);
    scilab_fill_and(ScalarUInt8, ScalarInt16, int_S_S, UInt8, Int16, UInt16);
    scilab_fill_and(ScalarUInt8, ScalarUInt16, int_S_S, UInt8, UInt16, UInt16);
    scilab_fill_and(ScalarUInt8, ScalarInt32, int_S_S, UInt8, Int32, UInt32);
    scilab_fill_and(ScalarUInt8, ScalarUInt32, int_S_S, UInt8, UInt32, UInt32);
    scilab_fill_and(ScalarUInt8, ScalarInt64, int_S_S, UInt8, Int64, UInt64);
    scilab_fill_and(ScalarUInt8, ScalarUInt64, int_S_S, UInt8, UInt64, UInt64);

    //int16
    scilab_fill_and(Int16, Int8, int_M_M, Int16, Int8, Int16);
    scilab_fill_and(Int16, UInt8, int_M_M, Int16, UInt8, UInt16);
    scilab_fill_and(Int16, Int16, int_M_M, Int16, Int16, Int16);
    scilab_fill_and(Int16, UInt16, int_M_M, Int16, UInt16, UInt16);
    scilab_fill_and(Int16, Int32, int_M_M, Int16, Int32, Int32);
    scilab_fill_and(Int16, UInt32, int_M_M, Int16, UInt32, UInt32);
    scilab_fill_and(Int16, Int64, int_M_M, Int16, Int64, Int64);
    scilab_fill_and(Int16, UInt64, int_M_M, Int16, UInt64, UInt64);

    scilab_fill_and(Int16, ScalarInt8, int_M_S, Int16, Int8, Int16);
    scilab_fill_and(Int16, ScalarUInt8, int_M_S, Int16, UInt8, UInt16);
    scilab_fill_and(Int16, ScalarInt16, int_M_S, Int16, Int16, Int16);
    scilab_fill_and(Int16, ScalarUInt16, int_M_S, Int16, UInt16, UInt16);
    scilab_fill_and(Int16, ScalarInt32, int_M_S, Int16, Int32, Int32);
    scilab_fill_and(Int16, ScalarUInt32, int_M_S, Int16, UInt32, UInt32);
    scilab_fill_and(Int16, ScalarInt64, int_M_S, Int16, Int64, Int64);
    scilab_fill_and(Int16, ScalarUInt64, int_M_S, Int16, UInt64, UInt64);

    scilab_fill_and(ScalarInt16, Int8, int_S_M, Int16, Int8, Int16);
    scilab_fill_and(ScalarInt16, UInt8, int_S_M, Int16, UInt8, UInt16);
    scilab_fill_and(ScalarInt16, Int16, int_S_M, Int16, Int16, Int16);
    scilab_fill_and(ScalarInt16, UInt16, int_S_M, Int16, UInt16, UInt16);
    scilab_fill_and(ScalarInt16, Int32, int_S_M, Int16, Int32, Int32);
    scilab_fill_and(ScalarInt16, UInt32, int_S_M, Int16, UInt32, UInt32);
    scilab_fill_and(ScalarInt16, Int64, int_S_M, Int16, Int64, Int64);
    scilab_fill_and(ScalarInt16, UInt64, int_S_M, Int16, UInt64, UInt64);

    scilab_fill_and(ScalarInt16, ScalarInt8, int_S_S, Int16, Int8, Int16);
    scilab_fill_and(ScalarInt16, ScalarUInt8, int_S_S, Int16, UInt8, UInt16);
    scilab_fill_and(ScalarInt16, ScalarInt16, int_S_S, Int16, Int16, Int16);
    scilab_fill_and(ScalarInt16, ScalarUInt16, int_S_S, Int16, UInt16, UInt16);
    scilab_fill_and(ScalarInt16, ScalarInt32, int_S_S, Int16, Int32, Int32);
    scilab_fill_and(ScalarInt16, ScalarUInt32, int_S_S, Int16, UInt32, UInt32);
    scilab_fill_and(ScalarInt16, ScalarInt64, int_S_S, Int16, Int64, Int64);
    scilab_fill_and(ScalarInt16, ScalarUInt64, int_S_S, Int16, UInt64, UInt64);

    //uint16
    scilab_fill_and(UInt16, Int8, int_M_M, UInt16, Int8, UInt16);
    scilab_fill_and(UInt16, UInt8, int_M_M, UInt16, UInt8, UInt16);
    scilab_fill_and(UInt16, Int16, int_M_M, UInt16, Int16, UInt16);
    scilab_fill_and(UInt16, UInt16, int_M_M, UInt16, UInt16, UInt16);
    scilab_fill_and(UInt16, Int32, int_M_M, UInt16, Int32, UInt32);
    scilab_fill_and(UInt16, UInt32, int_M_M, UInt16, UInt32, UInt32);
    scilab_fill_and(UInt16, Int64, int_M_M, UInt16, Int64, UInt64);
    scilab_fill_and(UInt16, UInt64, int_M_M, UInt16, UInt64, UInt64);

    scilab_fill_and(UInt16, ScalarInt8, int_M_S, UInt16, Int8, UInt16);
    scilab_fill_and(UInt16, ScalarUInt8, int_M_S, UInt16, UInt8, UInt16);
    scilab_fill_and(UInt16, ScalarInt16, int_M_S, UInt16, Int16, UInt16);
    scilab_fill_and(UInt16, ScalarUInt16, int_M_S, UInt16, UInt16, UInt16);
    scilab_fill_and(UInt16, ScalarInt32, int_M_S, UInt16, Int32, UInt32);
    scilab_fill_and(UInt16, ScalarUInt32, int_M_S, UInt16, UInt32, UInt32);
    scilab_fill_and(UInt16, ScalarInt64, int_M_S, UInt16, Int64, UInt64);
    scilab_fill_and(UInt16, ScalarUInt64, int_M_S, UInt16, UInt64, UInt64);

    scilab_fill_and(ScalarUInt16, Int8, int_S_M, UInt16, Int8, UInt16);
    scilab_fill_and(ScalarUInt16, UInt8, int_S_M, UInt16, UInt8, UInt16);
    scilab_fill_and(ScalarUInt16, Int16, int_S_M, UInt16, Int16, UInt16);
    scilab_fill_and(ScalarUInt16, UInt16, int_S_M, UInt16, UInt16, UInt16);
    scilab_fill_and(ScalarUInt16, Int32, int_S_M, UInt16, Int32, UInt32);
    scilab_fill_and(ScalarUInt16, UInt32, int_S_M, UInt16, UInt32, UInt32);
    scilab_fill_and(ScalarUInt16, Int64, int_S_M, UInt16, Int64, UInt64);
    scilab_fill_and(ScalarUInt16, UInt64, int_S_M, UInt16, UInt64, UInt64);

    scilab_fill_and(ScalarUInt16, ScalarInt8, int_S_S, UInt16, Int8, UInt16);
    scilab_fill_and(ScalarUInt16, ScalarUInt8, int_S_S, UInt16, UInt8, UInt16);
    scilab_fill_and(ScalarUInt16, ScalarInt16, int_S_S, UInt16, Int16, UInt16);
    scilab_fill_and(ScalarUInt16, ScalarUInt16, int_S_S, UInt16, UInt16, UInt16);
    scilab_fill_and(ScalarUInt16, ScalarInt32, int_S_S, UInt16, Int32, UInt32);
    scilab_fill_and(ScalarUInt16, ScalarUInt32, int_S_S, UInt16, UInt32, UInt32);
    scilab_fill_and(ScalarUInt16, ScalarInt64, int_S_S, UInt16, Int64, UInt64);
    scilab_fill_and(ScalarUInt16, ScalarUInt64, int_S_S, UInt16, UInt64, UInt64);

    //int32
    scilab_fill_and(Int32, Int8, int_M_M, Int32, Int8, Int32);
    scilab_fill_and(Int32, UInt8, int_M_M, Int32, UInt8, UInt32);
    scilab_fill_and(Int32, Int16, int_M_M, Int32, Int16, Int32);
    scilab_fill_and(Int32, UInt16, int_M_M, Int32, UInt16, UInt32);
    scilab_fill_and(Int32, Int32, int_M_M, Int32, Int32, Int32);
    scilab_fill_and(Int32, UInt32, int_M_M, Int32, UInt32, UInt32);
    scilab_fill_and(Int32, Int64, int_M_M, Int32, Int64, Int64);
    scilab_fill_and(Int32, UInt64, int_M_M, Int32, UInt64, UInt64);

    scilab_fill_and(Int32, ScalarInt8, int_M_S, Int32, Int8, Int32);
    scilab_fill_and(Int32, ScalarUInt8, int_M_S, Int32, UInt8, UInt32);
    scilab_fill_and(Int32, ScalarInt16, int_M_S, Int32, Int16, Int32);
    scilab_fill_and(Int32, ScalarUInt16, int_M_S, Int32, UInt16, UInt32);
    scilab_fill_and(Int32, ScalarInt32, int_M_S, Int32, Int32, Int32);
    scilab_fill_and(Int32, ScalarUInt32, int_M_S, Int32, UInt32, UInt32);
    scilab_fill_and(Int32, ScalarInt64, int_M_S, Int32, Int64, Int64);
    scilab_fill_and(Int32, ScalarUInt64, int_M_S, Int32, UInt64, UInt64);

    scilab_fill_and(ScalarInt32, Int8, int_S_M, Int32, Int8, Int32);
    scilab_fill_and(ScalarInt32, UInt8, int_S_M, Int32, UInt8, UInt32);
    scilab_fill_and(ScalarInt32, Int16, int_S_M, Int32, Int16, Int32);
    scilab_fill_and(ScalarInt32, UInt16, int_S_M, Int32, UInt16, UInt32);
    scilab_fill_and(ScalarInt32, Int32, int_S_M, Int32, Int32, Int32);
    scilab_fill_and(ScalarInt32, UInt32, int_S_M, Int32, UInt32, UInt32);
    scilab_fill_and(ScalarInt32, Int64, int_S_M, Int32, Int64, Int64);
    scilab_fill_and(ScalarInt32, UInt64, int_S_M, Int32, UInt64, UInt64);

    scilab_fill_and(ScalarInt32, ScalarInt8, int_S_S, Int32, Int8, Int32);
    scilab_fill_and(ScalarInt32, ScalarUInt8, int_S_S, Int32, UInt8, UInt32);
    scilab_fill_and(ScalarInt32, ScalarInt16, int_S_S, Int32, Int16, Int32);
    scilab_fill_and(ScalarInt32, ScalarUInt16, int_S_S, Int32, UInt16, UInt32);
    scilab_fill_and(ScalarInt32, ScalarInt32, int_S_S, Int32, Int32, Int32);
    scilab_fill_and(ScalarInt32, ScalarUInt32, int_S_S, Int32, UInt32, UInt32);
    scilab_fill_and(ScalarInt32, ScalarInt64, int_S_S, Int32, Int64, Int64);
    scilab_fill_and(ScalarInt32, ScalarUInt64, int_S_S, Int32, UInt64, UInt64);

    //uint32
    scilab_fill_and(UInt32, Int8, int_M_M, UInt32, Int8, UInt32);
    scilab_fill_and(UInt32, UInt8, int_M_M, UInt32, UInt8, UInt32);
    scilab_fill_and(UInt32, Int16, int_M_M, UInt32, Int16, UInt32);
    scilab_fill_and(UInt32, UInt16, int_M_M, UInt32, UInt16, UInt32);
    scilab_fill_and(UInt32, Int32, int_M_M, UInt32, Int32, UInt32);
    scilab_fill_and(UInt32, UInt32, int_M_M, UInt32, UInt32, UInt32);
    scilab_fill_and(UInt32, Int64, int_M_M, UInt32, Int64, UInt64);
    scilab_fill_and(UInt32, UInt64, int_M_M, UInt32, UInt64, UInt64);

    scilab_fill_and(UInt32, ScalarInt8, int_M_S, UInt32, Int8, UInt32);
    scilab_fill_and(UInt32, ScalarUInt8, int_M_S, UInt32, UInt8, UInt32);
    scilab_fill_and(UInt32, ScalarInt16, int_M_S, UInt32, Int16, UInt32);
    scilab_fill_and(UInt32, ScalarUInt16, int_M_S, UInt32, UInt16, UInt32);
    scilab_fill_and(UInt32, ScalarInt32, int_M_S, UInt32, Int32, UInt32);
    scilab_fill_and(UInt32, ScalarUInt32, int_M_S, UInt32, UInt32, UInt32);
    scilab_fill_and(UInt32, ScalarInt64, int_M_S, UInt32, Int64, UInt64);
    scilab_fill_and(UInt32, ScalarUInt64, int_M_S, UInt32, UInt64, UInt64);

    scilab_fill_and(ScalarUInt32, Int8, int_S_M, UInt32, Int8, UInt32);
    scilab_fill_and(ScalarUInt32, UInt8, int_S_M, UInt32, UInt8, UInt32);
    scilab_fill_and(ScalarUInt32, Int16, int_S_M, UInt32, Int16, UInt32);
    scilab_fill_and(ScalarUInt32, UInt16, int_S_M, UInt32, UInt16, UInt32);
    scilab_fill_and(ScalarUInt32, Int32, int_S_M, UInt32, Int32, UInt32);
    scilab_fill_and(ScalarUInt32, UInt32, int_S_M, UInt32, UInt32, UInt32);
    scilab_fill_and(ScalarUInt32, Int64, int_S_M, UInt32, Int64, UInt64);
    scilab_fill_and(ScalarUInt32, UInt64, int_S_M, UInt32, UInt64, UInt64);

    scilab_fill_and(ScalarUInt32, ScalarInt8, int_S_S, UInt32, Int8, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarUInt8, int_S_S, UInt32, UInt8, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarInt16, int_S_S, UInt32, Int16, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarUInt16, int_S_S, UInt32, UInt16, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarInt32, int_S_S, UInt32, Int32, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarUInt32, int_S_S, UInt32, UInt32, UInt32);
    scilab_fill_and(ScalarUInt32, ScalarInt64, int_S_S, UInt32, Int64, UInt64);
    scilab_fill_and(ScalarUInt32, ScalarUInt64, int_S_S, UInt32, UInt64, UInt64);

    //int64
    scilab_fill_and(Int64, Int8, int_M_M, Int64, Int8, Int64);
    scilab_fill_and(Int64, UInt8, int_M_M, Int64, UInt8, UInt64);
    scilab_fill_and(Int64, Int16, int_M_M, Int64, Int16, Int64);
    scilab_fill_and(Int64, UInt16, int_M_M, Int64, UInt16, UInt64);
    scilab_fill_and(Int64, Int32, int_M_M, Int64, Int32, Int64);
    scilab_fill_and(Int64, UInt32, int_M_M, Int64, UInt32, UInt64);
    scilab_fill_and(Int64, Int64, int_M_M, Int64, Int64, Int64);
    scilab_fill_and(Int64, UInt64, int_M_M, Int64, UInt64, UInt64);

    scilab_fill_and(Int64, ScalarInt8, int_M_S, Int64, Int8, Int64);
    scilab_fill_and(Int64, ScalarUInt8, int_M_S, Int64, UInt8, UInt64);
    scilab_fill_and(Int64, ScalarInt16, int_M_S, Int64, Int16, Int64);
    scilab_fill_and(Int64, ScalarUInt16, int_M_S, Int64, UInt16, UInt64);
    scilab_fill_and(Int64, ScalarInt32, int_M_S, Int64, Int32, Int64);
    scilab_fill_and(Int64, ScalarUInt32, int_M_S, Int64, UInt32, UInt64);
    scilab_fill_and(Int64, ScalarInt64, int_M_S, Int64, Int64, Int64);
    scilab_fill_and(Int64, ScalarUInt64, int_M_S, Int64, UInt64, UInt64);

    scilab_fill_and(ScalarInt64, Int8, int_S_M, Int64, Int8, Int64);
    scilab_fill_and(ScalarInt64, UInt8, int_S_M, Int64, UInt8, UInt64);
    scilab_fill_and(ScalarInt64, Int16, int_S_M, Int64, Int16, Int64);
    scilab_fill_and(ScalarInt64, UInt16, int_S_M, Int64, UInt16, UInt64);
    scilab_fill_and(ScalarInt64, Int32, int_S_M, Int64, Int32, Int64);
    scilab_fill_and(ScalarInt64, UInt32, int_S_M, Int64, UInt32, UInt64);
    scilab_fill_and(ScalarInt64, Int64, int_S_M, Int64, Int64, Int64);
    scilab_fill_and(ScalarInt64, UInt64, int_S_M, Int64, UInt64, UInt64);

    scilab_fill_and(ScalarInt64, ScalarInt8, int_S_S, Int64, Int8, Int64);
    scilab_fill_and(ScalarInt64, ScalarUInt8, int_S_S, Int64, UInt8, UInt64);
    scilab_fill_and(ScalarInt64, ScalarInt16, int_S_S, Int64, Int16, Int64);
    scilab_fill_and(ScalarInt64, ScalarUInt16, int_S_S, Int64, UInt16, UInt64);
    scilab_fill_and(ScalarInt64, ScalarInt32, int_S_S, Int64, Int32, Int64);
    scilab_fill_and(ScalarInt64, ScalarUInt32, int_S_S, Int64, UInt32, UInt64);
    scilab_fill_and(ScalarInt64, ScalarInt64, int_S_S, Int64, Int64, Int64);
    scilab_fill_and(ScalarInt64, ScalarUInt64, int_S_S, Int64, UInt64, UInt64);

    //uint64
    scilab_fill_and(UInt64, Int8, int_M_M, UInt64, Int8, UInt64);
    scilab_fill_and(UInt64, UInt8, int_M_M, UInt64, UInt8, UInt64);
    scilab_fill_and(UInt64, Int16, int_M_M, UInt64, Int16, UInt64);
    scilab_fill_and(UInt64, UInt16, int_M_M, UInt64, UInt16, UInt64);
    scilab_fill_and(UInt64, Int32, int_M_M, UInt64, Int32, UInt64);
    scilab_fill_and(UInt64, UInt32, int_M_M, UInt64, UInt32, UInt64);
    scilab_fill_and(UInt64, Int64, int_M_M, UInt64, Int64, UInt64);
    scilab_fill_and(UInt64, UInt64, int_M_M, UInt64, UInt64, UInt64);

    scilab_fill_and(UInt64, ScalarInt8, int_M_S, UInt64, Int8, UInt64);
    scilab_fill_and(UInt64, ScalarUInt8, int_M_S, UInt64, UInt8, UInt64);
    scilab_fill_and(UInt64, ScalarInt16, int_M_S, UInt64, Int16, UInt64);
    scilab_fill_and(UInt64, ScalarUInt16, int_M_S, UInt64, UInt16, UInt64);
    scilab_fill_and(UInt64, ScalarInt32, int_M_S, UInt64, Int32, UInt64);
    scilab_fill_and(UInt64, ScalarUInt32, int_M_S, UInt64, UInt32, UInt64);
    scilab_fill_and(UInt64, ScalarInt64, int_M_S, UInt64, Int64, UInt64);
    scilab_fill_and(UInt64, ScalarUInt64, int_M_S, UInt64, UInt64, UInt64);

    scilab_fill_and(ScalarUInt64, Int8, int_S_M, UInt64, Int8, UInt64);
    scilab_fill_and(ScalarUInt64, UInt8, int_S_M, UInt64, UInt8, UInt64);
    scilab_fill_and(ScalarUInt64, Int16, int_S_M, UInt64, Int16, UInt64);
    scilab_fill_and(ScalarUInt64, UInt16, int_S_M, UInt64, UInt16, UInt64);
    scilab_fill_and(ScalarUInt64, Int32, int_S_M, UInt64, Int32, UInt64);
    scilab_fill_and(ScalarUInt64, UInt32, int_S_M, UInt64, UInt32, UInt64);
    scilab_fill_and(ScalarUInt64, Int64, int_S_M, UInt64, Int64, UInt64);
    scilab_fill_and(ScalarUInt64, UInt64, int_S_M, UInt64, UInt64, UInt64);

    scilab_fill_and(ScalarUInt64, ScalarInt8, int_S_S, UInt64, Int8, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarUInt8, int_S_S, UInt64, UInt8, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarInt16, int_S_S, UInt64, Int16, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarUInt16, int_S_S, UInt64, UInt16, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarInt32, int_S_S, UInt64, Int32, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarUInt32, int_S_S, UInt64, UInt32, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarInt64, int_S_S, UInt64, Int64, UInt64);
    scilab_fill_and(ScalarUInt64, ScalarUInt64, int_S_S, UInt64, UInt64, UInt64);

    //boolean sparse
    scilab_fill_and(SparseBool, SparseBool, M_M, SparseBool, SparseBool, SparseBool);
    scilab_fill_and(Bool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_and(SparseBool, Bool, M_M, SparseBool, Bool, SparseBool);
    scilab_fill_and(ScalarBool, SparseBool, M_M, Bool, SparseBool, SparseBool);
    scilab_fill_and(SparseBool, ScalarBool, M_M, SparseBool, Bool, SparseBool);

#undef scilab_fill_and
}

// &&
InternalType* GenericShortcutAnd(InternalType* _pL)
{
    InternalType* pResult = NULL;
    switch (_pL->getType())
    {
        case InternalType::ScilabBool :
            isValueFalse(_pL->getAs<Bool>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabDouble :
            isValueFalse(_pL->getAs<Double>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt8 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt8 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt16 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt16 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt32 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt32 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabInt64 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabUInt64 :
            isValueFalse(_pL->getAs<Int8>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabSparse :
            isValueFalse(_pL->getAs<SparseBool>(), (Bool**)&pResult);
            break;
        case InternalType::ScilabSparseBool :
            isValueFalse(_pL->getAs<SparseBool>(), (Bool**)&pResult);
            break;
        default:
            // will return NULL
            break;
    }

    return pResult;
}

template<typename T>
void isValueFalse(T* _pL, types::Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            if ( !_pL->isComplex() || (_pL->getImg(i) == 0) )
            {
                *_pOut = new Bool(0); //false && something -> false
                return;
            }
        }
    }

    //call non shortcut operation
    *_pOut = NULL;
    return;
}

template<>
void isValueFalse(Double* _pL, Bool** _pOut)
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
                *_pOut = new Bool(0); //false && something -> false
                return;
            }
        }
    }

    //call non shortcut operation
    *_pOut = NULL;
    return;
}

template<>
void isValueFalse(Sparse* _pL, Bool** _pOut)
{
    if (_pL->nonZeros() != (size_t)_pL->getSize())
    {
        *_pOut = new Bool(0);
        return;
    }

    *_pOut = NULL;
    return;
}

template<>
void isValueFalse(SparseBool* _pL, Bool** _pOut)
{
    if (_pL->nbTrue() != (size_t)_pL->getSize())
    {
        *_pOut = new Bool(0);
        return;
    }

    *_pOut = NULL;
    return;
}

// &
InternalType* GenericLogicalAnd(InternalType* _pL, InternalType* _pR)
{
    InternalType *pResult = NULL;

    and_function bit_and = pAndfunction[_pL->getId()][_pR->getId()];
    if (bit_and)
    {
        pResult = bit_and(_pL, _pR);
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
InternalType* and_M_M(T *_pL, U *_pR)
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

    bit_and(_pL->get(), (long long)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    bit_and(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    bit_and(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    bit_and(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_M_E(T * /*_pL*/, U *_pR)
{
    return _pR;
}

template<class T, class U, class O>
InternalType* and_E_M(T *_pL, U * /*_pR*/)
{
    return _pL;
}

template<>
InternalType* and_E_M<Double, Bool, Bool>(Double* /* _pL */, Bool* _pR)
{
    return _pR;
}

template<>
InternalType* and_M_E<Bool, Double, Bool>(Bool* _pL, Double* /* _pR */)
{
    return _pL;
}

template<class T, class U, class O>
InternalType* and_I_S(T *_pL, U *_pR)
{
    return and_S_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* and_I_M(T *_pL, U *_pR)
{
    return and_M_S<U, T, O>(_pR, _pL);
}


template<class T, class U, class O>
InternalType* and_int_M_M(T *_pL, U *_pR)
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

    int_and(_pL->get(), (long long)_pL->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_int_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    int_and(_pL->get(0), (size_t)_pR->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_int_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    int_and(_pL->get(), (size_t)_pL->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* and_int_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    int_and(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

//boolean sparse
template<>
InternalType* and_M_M<SparseBool, SparseBool, SparseBool>(SparseBool* _pL, SparseBool* _pR)
{
    SparseBool* pOut = NULL;
    if (_pL->isScalar())
    {
        if (_pL->get(0, 0))
        {
            pOut = _pR;
        }
        else
        {
            pOut = new SparseBool(_pR->getRows(), _pR->getCols());
        }

        return pOut;
    }

    if (_pR->isScalar())
    {
        if (_pR->get(0, 0))
        {
            pOut = _pL;
        }
        else
        {
            pOut = new SparseBool(_pL->getRows(), _pL->getCols());
        }

        return pOut;
    }

    if (_pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
    {
        throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
    }

    return _pL->newLogicalAnd(*_pR);
}

template<>
InternalType* and_M_M<SparseBool, Bool, SparseBool>(SparseBool* _pL, Bool* _pR)
{
    SparseBool* pR = new SparseBool(*_pR);
    InternalType* pOut = and_M_M<SparseBool, SparseBool, SparseBool>(_pL, pR);
    if (pOut != pR)
    {
        delete pR;
    }

    return pOut;
}

template<>
InternalType* and_M_M<Bool, SparseBool, SparseBool>(Bool* _pL, SparseBool* _pR)
{
    SparseBool* pL = new SparseBool(*_pL);
    InternalType* pOut = and_M_M<SparseBool, SparseBool, SparseBool>(pL, _pR);
    if (pOut != pL)
    {
        delete pL;
    }
    return pOut;
}

