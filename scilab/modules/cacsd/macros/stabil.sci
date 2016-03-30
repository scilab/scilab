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

function F=stabil(A,B,alfa)
    //
    //returns F such that A+B*F is stable if (A,B) is stabilizable.
    //Assignable poles are set to alfa(1),alfa(2),...
    //If (A,B) is not stabilizable a warning is displayed
    //and assignable poles are set to alfa(1),alfa(2),...
    // Default value for alfa is -1.
    //
    //K=stabil(Sys,alfa,Beta) returns K, a compensentor for Sys
    //such that (A,B)-controllable eigenvalues are set to
    //alfa and (C,A)-observable eigenvalues are set to Beta.
    // All assignable closed loop poles (which are given by the
    //eigenvalues of Aclosed=h_cl(Sys,K) are set to alfa(i)'s
    //and Beta(j)'s.
    //
    //Example:
    // Sys=ssrand(2,2,5,list('st',2,3,3));
    // A=Sys(2);B=Sys(3); F=stabil(A,B);
    // spec(A)
    //2 controllable modes 2 unstable uncontrollable modes
    //  and one stable uncontrollable mode
    //spec(A+B*F)
    //the two controllable modes are set to -1.
    //

    [LHS,RHS]=argn(0)
    if typeof(A)~="state-space" then
        [ns,nc,U,sl]=st_ility(syslin("c",A,B,[]));
        [nx,nx]=size(A);[nn,nu]=size(B);
        if ns<nx then
            warning(msprintf(gettext("%s: System not stabilizable (or detectable) => Stabilizing the stabilizable part.\n"),"stabil"));
        end
        if RHS==2 then
            alfa=-ones(1,nx);
        end
        if prod(size(alfa))==1 then
            alfa=alfa*ones(1,nx);
        end
        An=U'*A*U;Bn=U'*B;
        k=size(alfa,"*");
        if k < nc then
            alfa=[alfa,-ones(1,nc-k)];
        end
        F=-ppol(An(1:nc,1:nc),Bn(1:nc,:),alfa(1:nc));
        F=[F,zeros(nu,nx-nc)]*U';
    else
        //F=stabil(Sys,alfa,Beta);
        Sys=A;[A1,B1,C1,D1]=abcd(Sys);
        if RHS==1 then al=-1;be=-1;end
        if RHS==2 then al=B;be=-1;end
        if RHS==3 then al=B;be=alfa;end
        F=stabil(A1,B1,al);
        G=stabil(A1',C1',be);G=G';
        F=obscont(Sys,F,G);
    end
endfunction
