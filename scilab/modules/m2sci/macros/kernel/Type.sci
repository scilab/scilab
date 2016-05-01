// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Type_tlist=Type(vtype,property)
    // Create a new 'type' tlist for inference

    rhs=argn(2)
    if rhs<2 then
        Type_tlist=tlist(["type","vtype","property"],Unknown,Unknown)
        return
    end

    if and(vtype~=[Double,Boolean,String,Unknown,Sparse,Cell,Struct,Int,Handle]) then
        error(msprintf(gettext("%s is not yet implemented."),string(vtype)))
    end

    if typeof(property)=="list" then
        error(gettext("list of properties is not yet implemented."))
    elseif and(property~=[Real,Complex,Unknown]) then
        error(msprintf(gettext("wrong property %s."),string(property)))
    end

    Type_tlist=tlist(["type","vtype","property"],vtype,property)
endfunction
