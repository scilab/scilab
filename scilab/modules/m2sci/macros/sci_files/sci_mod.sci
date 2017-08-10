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

function [tree]=sci_mod(tree)
    // M2SCI function
    // Conversion function for Matlab mod()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_r_c and %b_r_b are not defined in Scilab
    [X,Y] = getrhs(tree)
    X = convert2double(X)
    Y = convert2double(Y)
    tree.rhs=Rhs_tlist(X,Y)

    tree.name="pmodulo"

    if is_a_scalar(X) then
        tree.lhs(1).dims=Y.dims
    elseif is_a_scalar(Y) then
        tree.lhs(1).dims=X.dims
    elseif X.dims==Y.dims then
        tree.lhs(1).dims=X.dims
    else
        tree.lhs(1).dims=allunknown(X.dims)
    end
    tree.lhs(1).type=Type(Double,Real)
endfunction
