// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - 2016 INRIA -
//
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K,X]=lqe(P21,Q_ww,R_vv,S_wv)
    if typeof(P21)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"lqe",1))
    end

    if P21.dt == [] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Time domain must be ''c'' or ''d''.\n"),"lqe",1))
    end

    if argn(2)>1 then //[K,X]=lqe(P21,Q_ww,R_vv,S_wv)
        [ny,nu,nx]=size(P21)
        if or(size(Q_ww)<>nx) then
            error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),...
                "lqe",2,nx,nx))
        end

        if or(size(R_vv)<>ny) then
            error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),...
            "lqe",2,ny,ny))
        end

        if norm(Q_ww.'-Q_ww,1)>100*%eps*norm(Q_ww,1) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqe",2))
        end

        if norm(R_vv.'-R_vv,1)>100*%eps*norm(R_vv,1) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqe",3))
        end

        if argn(2)<4 then
            S_wv=zeros(nx,ny);
        elseif or(size(S_wv)<>[nx,ny]) then
            error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "lqe",4,nx,ny))
        end

        [kk,X]=lqr(P21',Q_ww,R_vv,S_wv')
    else
        [kk,X]=lqr(P21');
    end

    K=kk';
endfunction
