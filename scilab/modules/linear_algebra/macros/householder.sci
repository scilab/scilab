// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function u=householder(v,w)
    //Syntax
    //u=householder(v [,w])
    //Description
    //given 2 column vectors v w of same size householder(v,w) returns a unitary
    //column vector u, such that (eye-2*u*u')*v is proportional to w.
    //(eye-2*u*u') is the orthogonal Householder reflexion matrix
    //
    // w default value is eye(v). In this case vector (eye-2*u*u')*v is the
    // vector  eye(v)*(+-norm(v))
    [lhs,rhs]=argn(0)
    if rhs<2 then w=eye(v),end
    a=-sqrt((v'*v)/(w'*w))
    u=v+a*w
    u=u/norm(u)
endfunction
