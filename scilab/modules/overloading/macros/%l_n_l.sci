// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=%l_n_l(l1,l2)
    //%l_n_l(l1,l2)  : l1<>l2
    //!
    n1=lstsize(l1)
    r=n1<>lstsize(l2)

    if ~r&n1>0 then
        r=%f(ones(1,n1))
        sel=%f(ones(1,n1))
        k1=definedfields(l1)
        k2=definedfields(l2)
        for i=intersect(k1,k2),
            if and(getfield(i,l1)==getfield(i,l2)) then r(i)= %t,end
        end
        s1=1:n1;s1(k1)=[];
        s2=1:n1;s2(k2)=[];
        k1=intersect(s1,s2)
        if k1<>[] then r(intersect(s1,s2))=%t,end
        r=~r
    end
endfunction
