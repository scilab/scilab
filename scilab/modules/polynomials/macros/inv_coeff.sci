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
