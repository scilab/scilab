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
