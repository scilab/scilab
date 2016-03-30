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


function [n,d]=cmndred(num,den)
    //Syntax: [num,den]=cmndred(num,den)
    //
    //Given the transfert matrix defined by num./den, cmndred computes
    //polynomial matrix n and a common denominator d such that
    //n/d=num./den
    //!
    [m1,n1]=size(num)
    d=1;for dk=matrix(den,1,m1*n1),d=lcm([d,dk]),end
    for l=1:m1
        for k=1:n1
            n(l,k)=num(l,k)*pdiv(d,den(l,k));
        end;
    end;
endfunction
