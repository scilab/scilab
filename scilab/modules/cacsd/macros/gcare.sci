// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [X,F]=gcare(Sl)
    //[X,F]=gcare(Sl)
    //Generalized Control Algebraic Riccati Equation
    //X = solution , F = gain
    //!
    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"gcare",1))
    end
    if Sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"gcare",1))
    end

    [A,B,C,D]=abcd(Sl);
    S=eye()+D'*D;R=eye()+D*D';
    Si=inv(S);
    Ar=A-B*Si*D'*C;
    H=[Ar,-B*Si*B';
    -C'*inv(R)*C,-Ar'];
    X=ric_desc(H);
    F=-Si*(D'*C+B'*X)
endfunction
