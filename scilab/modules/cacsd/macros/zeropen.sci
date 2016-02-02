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

function [Z,U]=zeropen(Sl)
    //Z = sE - F = zero pencil of Sl=[A,B,C,D]
    // With U row compression of [B;D] i.e, U*[B;D]=[0;*];
    //U*[-sI+A |B;   [ Z |0;
    //      C  |D] = [ * |*]

    s=poly(0,"s");
    [A,B,C,D]=abcd(Sl);
    BD=[B;D];
    [n,m]=size(BD);
    [U,rk]=rowcomp(BD);U=[U(rk+1:n,:);U(1:rk,:)];
    W=U*([-s*eye()+A;C])
    Z=W(1:n-rk,:);
endfunction
