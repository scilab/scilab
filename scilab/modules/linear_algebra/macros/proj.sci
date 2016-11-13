
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [y]=proj(x1,x2)
    //[y]=proj(x1,x2)  projection on x2 parallel to x1
    //F.D.
    //!
    [l,k]=size(x1);
    [w,n]=rowcomp(x1);w1=w(:,1:n);
    x1t=w(n+1:l,:);
    y=x2/(x1t*x2)*x1t
endfunction
