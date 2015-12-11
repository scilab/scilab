// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
