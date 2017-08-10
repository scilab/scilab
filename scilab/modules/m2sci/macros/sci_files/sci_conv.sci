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

function [tree]=sci_conv(tree)
    // M2SCI function
    // Conversion function for Matlab conv()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_conv()

    if rhs==2 then // conv(u,v)
        [A,B]=getrhs(tree) // conv(u,v)
    elseif rhs==3 then
        [A,B,shape]=getrhs(tree)
    end
    if is_real(A)&is_real(B) then
        tree.lhs(1).type=Type(Double,Real)
    elseif is_complex(A)|is_complex(B) then
        tree.lhs(1).type=Type(Double,Complex)
    else
        tree.lhs(1).type=Type(Double,Unknown)
    end
    if and([A.dims(1),B.dims(1)]==1)|(A.dims(2)==SupToOne&B.dims(2)==SupToOne)|(A.dims(2)>1&B.dims(2)>1) then  //A and B row vectors
        tree.lhs(1).dims=list(1,Unknown)
    elseif and([A.dims(2),B.dims(2)]==1)|(A.dims(1)==SupToOne&B.dims(1)==SupToOne)|(A.dims(1)>1&B.dims(1)>1) then // A and B column vectors
        tree.lhs(1).dims=list(Unknown,1)
    else
        tree.lhs(1).dims=list(Unknown,Unknown)
    end
endfunction
