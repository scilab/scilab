// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [P]=inv_coeff(c,d,name)
    // inverse function of coeff
    rhs=argn(2);
    if rhs <= 2 ; name = "x";end

    [n,m]=size(c);
    if rhs <= 1 ; d = (m/n-1) ; end
    if d==m-1 then
        P=[];
        for l=1:n, P=[P;poly(c(l,:),name,"coeff")];end
        return,
    end
    if modulo(m,d+1) <> 0 then
        error(msprintf(_("%s: incompatible input arguments %d and %d\n"),"inv_coeff",1,2))
    end
    p=poly(0,name);
    P=p.^(0:d)';
    P=c*(P.*.eye(m/(d+1),m/(d+1)))
endfunction
