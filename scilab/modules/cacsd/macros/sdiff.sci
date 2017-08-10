// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function w=sdiff(z,d)
    // time serie diiferencing
    // W= (1-B)^d Z
    // where B is the backward-shift operator
    // and d the difrenecing order
    // z : a matrix of size(n1,n2) z(t)=z(:,t)

    [lhs,rhs]=argn(0)
    if rhs==1;d=1;end
    w=z;
    for i=1:d,[n1,n2]=size(w); w=w(:,2:n2)-w(:,1:(n2-1));end
endfunction
