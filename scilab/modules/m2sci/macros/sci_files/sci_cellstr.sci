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

function  [tree]=sci_cellstr(tree)
    // M2SCI function
    // Conversion function for Matlab cellstr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    tree.rhs=Rhs_tlist(A)
    Aknowndim=%t // Aknowndim is a boolean, true if all dimennsions of A are known
    tree.lhs(1).dims=list()
    for i=1:size(A.dims)
        if A.dims(i)==-1 then
            Aknowndim=%f
            break
        end
    end
    if ~Aknowndim then
        tree.lhs(1).dims(1)=-1
        tree.lhs(1).dims(2)=1
    else
        tree.lhs(1).dims(1)=A.dims(1)
        tree.lhs(1).dims(2)=1
    end
    tree.lhs(1).type=Type(Cell,Unknown)
endfunction
