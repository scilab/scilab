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


function gama=dhnorm(Sl,tol,gamamax)
    //discrete-time case
    // included in h_norm!
    [lhs,rhs]=argn(0);
    if rhs==1 then tol=0.000001;gamamax=10000000;end
    if rhs==2 then gamamax=1000;end
    gamamin=sqrt(%eps);
    n=0;
    while %T
        gama=(gamamin+gamamax)/2;n=n+1;
        if n>1000 then warning(msprintf(gettext("%s: More than %d iterations.\n"),"dhnorm",1000));return;end
        if dhtest(Sl,gama) then
        gamamax=gama; else gamamin=gama
        end
        if (gamamax-gamamin)<tol then return;end
    end

endfunction
function ok=dhtest(Sl,gama)
    //test if discrete hinfinity norm of Sl is < gama
    [A,B,C,D]=Sl(2:5);B=B/sqrt(gama);C=C/sqrt(gama);D=D/gama;
    R=eye()-D'*D;
    [n,n]=size(A);Id=eye(n,n);Z=0*Id;
    Ak=A+B*inv(R)*D'*C;
    e=[Id,-B*inv(R)*B';Z,Ak'];
    Aa=[Ak,Z;-C'*inv(eye()-D*D')*C,Id];
    [As,Es,w,k]=schur(Aa,e,"d");
    //Testing magnitude 1 eigenvalues.
    [al,be]=spec(As,Es);
    finite=find(abs(be)>0.00000001);
    finite_eigen=al(finite)./be(finite);
    bad=find( abs(abs(finite_eigen)-1) < 0.0000001);
    if bad<>[] then ok=%f;return;end
    //if k<>n then ok=%f;return;end
    ws=w(:,1:n);
    x12=ws(1:n,:);
    phi12=ws(n+1:2*n,:);
    if rcond(x12) > 1.d-6 then
        X=phi12/x12;
        z=eye()-B'*X*B
        ok= min(real(spec(z))) > -%eps
    else
    ok=%t;end
endfunction
