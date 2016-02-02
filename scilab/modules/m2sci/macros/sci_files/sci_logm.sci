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

function [tree]=sci_logm(tree)
    // Generic conversion function
    // M2SCI function
    // Conversion function for Matlab logm()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    tree.rhs=Rhs_tlist(A)

    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(Double,Unknown)
    if size(tree.lhs)==2
        tree.lhs(2)=null()
        set_infos(gettext("second output argument will be ignored."),2);
    end
endfunction




