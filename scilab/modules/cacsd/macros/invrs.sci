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

function Sli=invrs(Sl,alfa);
    // Sli=invrs(Sl,alfa) computes Sli, the PSSD
    // inverse of PSSD Sl.
    //!
    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"invrs",1))
    end

    D=Sl.D;
    if type(D)==2 then
        s=poly(0,varn(D));
        Sl.D=horner(Sl.D,s+alfa);
    end
    Sl.A=Sl.A-alfa*eye(Sl.A); //Slnew(s)=Slold(s+alfa)

    [Sreg,Wss]=rowregul(Sl,0,0);
    if rcond(Sreg.D) >1.d-6 then
        Sli=invsyslin(Sreg)*Wss;
    else
        error(msprintf(gettext("%s: Square but singular system.\n"),"invrs"));
    end
    [Q,M]=pbig(Sli.A,0.001,"d");
    Sli=projsl(Sli,Q,M);//Remove poles at zero.

    if Sli.A~=[] then Sli.A=Sli.A+alfa*eye();end
    if type(Sli.D)==2 then
        Sli.D=horner(Sli.D,s-alfa);
    end
endfunction
