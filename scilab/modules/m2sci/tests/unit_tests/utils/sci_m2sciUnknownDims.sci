// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright DIGITEO - 2009 - Vincent COUVERT
// Copyright INRIA - 2002-2004 - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_m2sciUnknownDims(tree)
    // Variable dimensions set to Unknown
    nbdims=size(tree.dims)
    newdims=list()
    for k=1:nbdims
        newdims(k)=-1
    end
    tree.lhs(1).dims=newdims
    tree.lhs(1).type=tree.rhs(1).type
endfunction


