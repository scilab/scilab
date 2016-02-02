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


function [Y,rk,Ac]=colcompr(A);
    //[Y,rk,Ac]=colcompr(A);
    //column compression of polynomial matrix A
    //(left compression)
    //Y = right unimodular base
    //rk = normal rank of A
    //Ac = A*Y
    //see rowcompr
    //!
    [m,n]=size(A);
    [Ac,Y,rk]=htrianr(A);
endfunction
