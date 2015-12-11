// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [a,b,c]=obsvss(a,b,c,tol)

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "constant" then
        if lhs<>3 then
            error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"),"obsvss",3)),
        end
        select rhs
        case 3 then
            tol = 100*%eps
        case 4 then ,
        else
            error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"obsvss",3,4))
        end;
    case "state-space" then
        if lhs<>1 then
            error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"),"obsvss",1)),
        end
        select rhs
        case 1 then
            tol=100*%eps
        case 2 then
            tol=b
        else
            error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"obsvss",1,2))
        end;
        [a,b,c,d,x0,dom]=a(2:7)
    else
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"obsvss",1))
        else
            error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"obsvss",1))
        end
    end;
    //
    [no,u]=contr(a',c',tol)
    u=u(:,1:no)
    a=u'*a*u;b=u'*b;c=c*u
    if lhs==1 then a=syslin(dom,a,b,c,d,u'*x0),end
endfunction
