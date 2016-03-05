// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function   [tree]=sci_cell2mat(tree)
    // M2SCI function
    // Conversion function for Matlab cell2mat()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    if is_empty(A) then
        tree.lhs(1).dims=list(0,0)
        tree.lhs(1).type=Type(Double,Real)
    elseif size(A.dims)==2
        if A.dims(1)<> -1 & A.dims(2)<> -1 then
            lhs_dims2=0;
            for i=1:A.dims(2)
                lhs_dims2=A.contents.data(i).dims(2) + lhs_dims2
            end
            lhs_dims1=0;
            for i=1:A.dims(1)
                lhs_dims1=A.contents.data(1+(i-1)*A.dims(2)).dims(1) + lhs_dims1
            end
            tree.lhs(1).dims=list(lhs_dims1,lhs_dims2)
            tree.lhs(1).type = A.contents.data(1).type
        else
            tree.lhs(1).dims=list(-1,-1)
            tree.lhs(1).type=Type(Unknown,Unknown)
        end
    else
        tree.lhs(1).dims=list(-1,-1)
        tree.lhs(1).type=Type(Unknown,Unknown)
    end
endfunction
