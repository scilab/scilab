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

function [tree]=sci_rem(tree)
    // M2SCI function
    // Conversion function for Matlab rem()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_d_c and %b_d_b are not defined in Scilab
    [X,Y] = getrhs(tree)
    X = convert2double(X)
    Y = convert2double(Y)
    tree.rhs=Rhs_tlist(X,Y)

    // X./Y
    drd=Operation("./",tree.rhs,list())
    // fix(X./Y)
    fix_funcall=Funcall("fix",1,list(drd),list())
    // fix(X./Y).*Y
    drm=Operation(".*",list(fix_funcall,tree.rhs(2)),list())
    // X-fix(X./Y).*Y
    tree=Operation("-",list(tree.rhs(1),drm),tree.lhs)

    if is_a_scalar(X) then
        tree.out(1).dims=Y.dims
    elseif is_a_scalar(Y) then
        tree.out(1).dims=X.dims
    elseif is_a_vector(X) then
        tree.out(1).dims=X.dims
    elseif is_a_vector(Y) then
        tree.out(1).dims=Y.dims
    else
        tree.out(1).dims=X.dims
    end

    tree.out(1).type=Type(Double,Real)

endfunction
