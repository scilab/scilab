// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
