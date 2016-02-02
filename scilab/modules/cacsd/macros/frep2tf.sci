// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// Copyright (C) ENPC -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [best_h,best_w]=frep2tf(frq,repf,dg,dom,tols,weight)
    // iterative use of frep2tf_b jpc fd 1997

    // Copyright INRIA
    [lhs,rhs]=argn(0);
    if rhs <= 3 then dom="c" ; end
    if rhs <= 4 then
        rtol=1.e-2; atol=1.e-4, N=10;
    else
        rtol=tols(1);atol=tols(2);N=tols(3);
    end
    if dom==[] then dom="c";end
    if dom=="d" then dom=1;end
    if rhs <=5  then
        [h,err]=frep2tf_b(frq,repf,dg,dom);
        best_w = [];
    else
        [h,err]=frep2tf_b(frq,repf,dg,dom,weight);
        best_w = weight;
    end
    best_h = h ;
    for k=1:N
        if dom=="c" then
            //    weight=(1)./abs(freq(h('den'),1,%i*frq*2*%pi));
            weight=(1)./horner(h("den"),%i*frq*2*%pi);
        else
            weight=(1)./horner(h("den"),exp(dom*%i*frq*2*%pi));
        end
        [h,err1]=frep2tf_b(frq,repf,dg,dom,weight);
        if ( (abs(err-err1) < rtol *err & err > err1 )| err1 < atol) then break;end;
        if err1 < err then best_err = err1 ; best_h = h; end
        err=err1;
        mprintf(gettext("%s: Iteration %s, error=%s.\n"), "frep2tf", part(string(k+1),1:5), string(err1));
    end
endfunction

function [h,err]=frep2tf_b(frq,repf,dg,dom,weight)
    // steer, jpc, fd 1997 (Nov)
    //============================
    [lhs,rhs]=argn(0);
    // test the system type 'c' 'd' or dt
    if rhs <= 3 then dom="c" ; end
    if rhs <= 4 then weight=[];end
    if dom==[] then dom="c";end
    if dom=="d" then dom=1;end
    n=size(frq,"*");
    if dom=="c" then
        w=2*%i*%pi*matrix(frq,n,1);
    else
        w=exp(2*%i*%pi*dom*matrix(frq,n,1));
    end
    //initialization
    m=2*dg
    //We compute the linear system to be solved:
    //w(k)=%i* frq(k)*2pi
    //for k=1,n  sum(a_i*(w(k))^i,i=1,dg)
    //		-repf(k)*sum(b_i*(w(k))^i,i=1,dg) = 0
    //with sum x_i = 1
    //building Van der monde matrix ( w_i^j ) i=1,n j=0:dg-1
    a1=w.*.[ones(1,dg)];
    //0.^0 is not accepted in Scilab....
    a1=[ones(n,1),a1.^(ones(n,1).*.[1:(dg)])];
    a2=a1; for k=1:n; a2(k,:)= -repf(k)*a2(k,:);end
    a=[a1,a2];
    // Computing constraints
    // We impose N(i wk) - repfk D(i wk) =0 for k=imax
    // as follows:
    // N(i wk) = repfk*(1+%i*b)
    // D(i wk) = 1+%i*b
    // L*[x;b]=[repfk;1]
    // Least squ. pb is  min norm of [A,0] [x;b]
    //  under constraint         L*[x;b]=[repfk;1]
    [rmax,imax]=max(abs(repf))
    L2=a(imax,1:dg+1);
    L=[zeros(L2),L2,%i;
    L2,zeros(L2),repf(imax)*%i];
    BigL=[real(L);imag(L)]
    c=[1;repf(imax)];
    Bigc=[real(c);imag(c)];
    [ww,dim]=rowcomp(BigL);
    BigL=ww*BigL;Bigc=ww*Bigc;
    BigL=BigL(1:dim,:);Bigc=Bigc(1:dim,:);

    a=[a,zeros(size(a,1),1)];
    // auto renormalization : if weight is not given
    if dom == "c" then
        if weight==[] then
            nn= sqrt(sum(abs(a).^2,"c"))+ones(n,1);
            a=a./(nn*ones(1,size(a,2)));
        end
    end
    // user given renormalization
    if weight<>[] then
        if size(frq,"*")<>size(weight,"*") then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same numbers of elements expected.\n"),"frep2tf",1,5))
        end
        w1=weight(:)*ones(1,size(a,2));
        a= w1.*a;
    end
    BigA=[real(a);imag(a)];
    // Constraints BigL x =Bigc
    //
    x=LSC(BigA,BigL,Bigc);
    x=x(1:$-1);

    h=syslin(dom,poly(x(1:dg+1),"s","c"),poly([x((dg+2):$)],"s","c"))
    if lhs==2 then
        repf1=repfreq(h,frq);
        err = sum(abs(repf1(:)-repf(:)))/n;
    end
endfunction

function [x]=LSC(A,L,c)
    // Ax=0 Least sq. + Lx = c
    [W,rk]=colcomp(L);
    LW=L*W;
    Anew=A*W
    A1=Anew(:,1:($-rk))
    A2=Anew(:,($-rk+1:$));
    x2=inv(LW(:,$-rk+1:$))*c
    b= -A2*x2
    x1=A1\b
    x=W*[x1;x2]
endfunction
