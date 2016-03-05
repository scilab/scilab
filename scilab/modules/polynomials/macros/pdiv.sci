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


function [R,Q]=pdiv(P1,P2)
    // Element wise euclidan division of a polynomial matrix
    // by a polynomial
    // This is just a loop for the primitive pppdiv
    //!
    [lhs,rhs]=argn(0);
    [n,m]=size(P1);
    [n1,m1]=size(P2);
    // Special case for constant matrices
    if type(P1)==1&type(P2)==1 then
        Q=P1./P2;R=0*P1;
        if lhs==1 then R=Q; end
        return;
    end
    R=[],Q=[]
    if n1==1 & m1==1 then
        for l=1:n,
            for k=1:m,
                [rlk,qlk]=pppdiv(P1(l,k),P2),R(l,k)=rlk;Q(l,k)=qlk;
            end;
        end
        if lhs==1 then R=Q;end

        return;
    end
    for l=1:n,
        for k=1:m,
            [rlk,qlk]=pppdiv(P1(l,k),P2(l,k)),R(l,k)=rlk;Q(l,k)=qlk;
        end;
    end
    if lhs==1 then R=Q; end
endfunction
