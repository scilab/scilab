// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
