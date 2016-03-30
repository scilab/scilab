// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [X,Y]=mtlb_meshdom(x,y)
    [lhs,rhs]=argn()
    x=matrix(x,1,-1);
    y=matrix(y,-1,1);
    X=ones(y)*x
    Y=y($:-1:1)*ones(x)
endfunction
