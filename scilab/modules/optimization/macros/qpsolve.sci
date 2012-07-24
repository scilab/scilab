// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function [x, iact, iter, f]=qpsolve(Q,p,C,b,ci,cs,me)
    rhs = argn(2);
    if rhs <> 7
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "qsolve", 7));
    end
    Cb=[];bb=[];
    isCsparse=typeof(C)=='sparse'|ci<>[]|cs<>[]
    C(me+1:$,:)=-C(me+1:$,:);
    b(me+1:$)=-b(me+1:$);
    // replace boundary contraints by linear constraints
    Cb=[];bb=[];
    if ci<>[] then
        Cb=[Cb;speye(Q)]
        bb=[bb;ci]
    end
    if cs<>[] then
        Cb=[Cb;speye(Q)]
        bb=[bb;-cs]
    end
    C=[C;Cb];b=[b;bb]
    [x, iact, iter, f]=qp_solve(Q,-p,C',b,me)
endfunction

