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

function [tree]=sci_str2num(tree)
    // M2SCI function
    // Conversion function for Matlab str2num()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    tree.name="evstr"
    tree.lhs(1).type=Type(Double,Real)
endfunction