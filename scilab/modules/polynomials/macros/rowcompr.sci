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


function [X,rk,Ac]=rowcompr(A)
    //[X,rk,Ac]=rowcompr(A)
    //row compression of polynomial matrix A (toward the bottom)
    //X left polynomial unimodular base
    //rk=normal rank of A
    //Ac = X*A
    //Warning: elimination of neglected terms
    //!
    [n,m]=size(A);
    [Ac,U,rk]=htrianr(A');
    Ac=Ac';
    X=U';
    X=X(n:-1:1,:)
    Ac=Ac(n:-1:1,:)
endfunction
