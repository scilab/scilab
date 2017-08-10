// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [dble]=convert2double(A)
    // This function converts input parameters to double to avoid to write overloading functions in Scilab

    lhs=argn(1)

    if A.vtype==Double then
        dble=A
        return
    end

    if only_double then
        dble=A
        dble.type=Type(Double,Unknown)
    else
        if A.vtype==String then
            lhslist=list();
            lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Real)))
            dble=Funcall("asciimat",1,Rhs_tlist(A),lhslist)
        elseif A.vtype==Boolean then
            lhslist=list();
            lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Real)))
            dble=Funcall("bool2s",1,Rhs_tlist(A),lhslist)
        elseif A.vtype==Unknown then
            lhslist=list();
            lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Unknown)))
            dble=Funcall("mtlb_double",1,Rhs_tlist(A),lhslist)
        else
            error(msprintf(gettext("%s is not yet implemented."),string(A.vtype)))
        end
    end
endfunction
