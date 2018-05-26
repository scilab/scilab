// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [R, Q] = pdiv(P1, P2)
    // Element wise euclidan division of a polynomial matrix
    // by a polynomial
    // This is just a loop for the primitive pppdiv
    //!

    // Special case for constant matrices
    if type(P1)==1 & type(P2)==1 then
        Q = P1 ./ P2
        R = zeros(P1)

    else
        scalarP2 = length(P2)==1
        s = size(P1)
        if size(s,2)>2
            P1 = P1(:)
            P2 = P2(:)
        end
        [n, m] = size(P1)
        R = zeros(P1)
        Q = zeros(P1)
        for l = 1:n
            for k = 1:m
                if scalarP2
                    [rlk, qlk] = pppdiv(P1(l,k), P2)
                else
                    [rlk, qlk] = pppdiv(P1(l,k), P2(l,k))
                end
                R(l,k) = rlk
                Q(l,k) = qlk
            end
        end
        if size(s,2)>2
            Q = matrix(Q, s)
            R = matrix(R, s)
        end
    end
    if argn(1)==1 then
        R = Q
    end
endfunction
