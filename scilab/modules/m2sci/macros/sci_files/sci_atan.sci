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

function [tree]=sci_atan(tree)
    // File generated from sci_PROTO5.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab atan()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree

    A=getrhs(tree)
    A=convert2double(A)
    tree.rhs=Rhs_tlist(A)

    if tree.name=="atan" then
        set_infos(msprintf(gettext("If %s is outside [-1,1]\n   real part of output of %s will be the opposite of Matlab one."), strcat(expression2code(A), ""), strcat(expression2code(tree), "")),2)
    else
        set_infos(msprintf(gettext("If %s is outside [-1,1]\n   complex part of output of %s will be the opposite of Matlab one."), strcat(expression2code(A), ""), strcat(expression2code(tree), "")),2)
    end

    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(Double,Unknown)
endfunction
