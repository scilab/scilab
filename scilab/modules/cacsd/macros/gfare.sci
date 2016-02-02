// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Z,H]=gfare(Sl)
    //[Z,H]=gfare(Sl)
    //Generalized Filter Algebraic Riccati Equation
    //X = solution , F = gain
    //!
    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),..
        "gfare",1))
    end

    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"gfare",1))
    end
    if Sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"gfare",1))
    end

    [A,B,C,D]=abcd(Sl);
    S=eye()+D'*D;R=eye()+D*D';
    Si=inv(S);Ri=inv(R);
    Ar=A-B*Si*D'*C;
    H=[Ar',-C'*Ri*C;
    -B*Si*B',-Ar];
    Z=ric_desc(H);
    H=-(B*D'+Z*C')*Ri
endfunction
