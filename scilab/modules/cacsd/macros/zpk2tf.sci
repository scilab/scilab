// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s=zpk2tf(sys)
    //zero pole gain to state space (simo system)

    if typeof(sys)<>"zpk" then
        error(msprintf(_("%s: Wrong type for input argument #%d: zero-pole-gain representation expected.\n"),"zpk2tf",1))
    end
    [ny,nu]=size(sys)
    siso=ny*nu==1

    if siso then
        s=zp2tf_siso(sys.Z{1},sys.P{1},sys.K,sys.dt)
    else
        s=[];
        for l=1:ny
            for k=1:nu
                s(l,k)=zp2tf_siso(sys.Z{l,k},sys.P{l,k},sys.K(l,k),sys.dt);
            end
        end
    end
endfunction

function s=zp2tf_siso(Z,P,K,dt)
    //remove poles and zeros at infinity
    Z(Z==%inf)=[];
    P(P==%inf)=[];
    if dt=="c" then var="s"; else var="z";end
    s=syslin(dt,(real(poly(Z,var,"r"))*K)/real(poly(P,var,"r")))
endfunction
