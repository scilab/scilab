// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Type_tlist = Type(vtype,property)
    // Create a new 'type' tlist for inference
    // See also m2scideclare.sci

    rhs = argn(2)
    if rhs<2 then
        Type_tlist = tlist(["type","vtype","property"],Unknown,Unknown)
        return
    end

    if and(vtype~=[Boolean,Int,Double,Sparse,Polynomial,String,Handle,Function,Cell,Struct,Unknown]) then
        msg = gettext("vtype ''%s'' is not yet implemented.\n")
        error(msprintf(msg, getTypeName(vtype)))
    end

    if typeof(property)=="list" then
        error(gettext("List of properties is not yet implemented.\n"))
    else
        msg = gettext("Wrong property ''%s'' for vtype ''%s''.\n");
        select vtype
        case Boolean
            if and(property~=[Boolean, Sparse, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case Int
            if and(property~=["int8" "int16" "int32" "int64" "uint8" "uint16" "uint32" "uint64"]) & property <> Unknown then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case Double
            if and(property~=[Real, Complex, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case Sparse
            if and(property~=[Real, Complex, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
            // Sparse boolean are Boolean vtype with property = Sparse
        case Polynomial
            if and(property~=[Real, Complex, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case String
            if and(property~=[Char, String, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case Handle
            if and(property~=["Figure" "Axes" "Unknown"]) & property<>Unknown then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        case Function
            if and(property~=[Builtin, Macro, Unknown]) then
                error(msprintf(msg, getPropName(property), getTypeName(vtype)))
            end
        end
    end

    Type_tlist = tlist(["type","vtype","property"], vtype, property)
endfunction
