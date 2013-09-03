// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
