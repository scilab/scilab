
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

function [S]=sqroot(Q)
    Q1=(Q+Q')/2;
    if norm(Q1-Q,1) > 100*%eps then
        warning(msprintf(gettext("%s: Wrong size for input argument #%d: Symmetric expected"),"sqroot",1));
    end
    tt=min(spec(Q1));
    if tt <-10*%eps then
        warning(msprintf(gettext("%s: Wrong value for input argument #%d: Not semi-definite positive"),"sqroot",1));
    end
    if norm(Q,1) < sqrt(%eps) then S=[];return;end
    [u,S,v,rk]=svd(Q);
    S=v(:,1:rk)*sqrt(S(1:rk,1:rk));
    if norm(imag(Q1),1) <1.d-8 then S=real(S);;end
endfunction
