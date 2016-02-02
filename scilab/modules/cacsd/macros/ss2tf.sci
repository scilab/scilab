// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  1985-2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [h,num,den]=ss2tf(sl,rmax)
    // State-space to transfer function.
    // Syntax:
    //   h=ss2tf(sl)
    //   h=ss2tf(sl,'b')
    //   h=ss2tf(sl,rmax)
    //
    //   sl   : linear system (syslin list)
    //   h    : transfer matrix
    //   rmax : optional parameter controlling the conditioning in
    //          block diagonalization method is used (If 'b' is entered
    //          a default value is used)
    //   Method: By default, one uses characteristic polynomial and
    //   det(A+Eij)=det(A)+C(i,j) where C is the adjugate matrix of A
    //   Other method used : block-diagonalization (generally
    //   this gives a less accurate result).
    //
    //!

    if type(sl)==1|type(sl)==2 then
        h=sl
        return
    end
    if typeof(sl)<>"state-space" then
        error(msprintf(_("%s: Wrong type for input argument #%d: State-space form expected.\n"),"ss2tf",1));
    end
    //Handle special cases (no input, no output, no state)
    if sl.B==[] then h=sl.D;num=sl.D;den=eye(sl.D);return;end
    if sl.C==[] then h=sl.D;num=sl.D;den=eye(sl.D);return;end
    if size(sl.A,"*")==0 then //no state
        h=sl.D
        return
    end

    //Determine the rational fraction formal variable name
    domaine=sl.dt
    if type(domaine)==1 then var="z";end
    if domaine=="c" then var="s";end;
    if domaine=="d" then var="z";end;
    if domaine==[] then
        var="s";
        if type(sl.D)==2 then var=varn(sl.D);end
    end

    //Determine the algorithm
    [lhs,rhs]=argn(0);
    meth="p";
    if rhs==2 then
        if type(rmax)==10 then
            meth=part(rmax,1);
            if and(meth<>["p","b"]) then
                error(msprintf(_( "%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"ss2tf",1,"''p'',''b''"));
            end
            rhs=1;
        else
            meth="b";
        end
    end

    select meth
    case "b" // Block diagonalization + Leverrier method
        a=sl.A;
        [n1,n1]=size(a);
        z=poly(0,var);
        //block diagonal decomposition of the state matrix
        if rhs==1 then
            [a,x,bs]=bdiag(a);
        else
            [a,x,bs]=bdiag(a,rmax);
        end
        k=1;m=[];v=ones(1,n1);den=1;
        for n=bs' //loop on blocks
            k1=k:k-1+n;
            // Leverrier algorithm
            h=z*eye(n,n)-a(k1,k1);
            f=eye(n,n);
            for kl=1:n-1,
                b=h*f;
                d=-sum(diag(b))/kl;
                f=b+eye()*d;
            end
            d=sum(diag(h*f))/n;
            //
            den=den*d;
            l=[1:k-1,k+n:n1];
            if l<>[] then v(l)=v(l)*d;end
            m=[m,x(:,k1)*f];
            k=k+n;
        end

        if lhs==3 then
            h=sl.D,
            num=sl.C*m*diag(v)*(x\sl.B);
        else
            m=sl.C*m*diag(v)*(x \ sl.B)+sl.D*den;
            [m,den]=simp(m,den*ones(m))
            h=syslin(domaine,m,den)
        end

    case "p" then //Adjugate matrix method
        Den=poly(sl.A,var) //common denominator

        na=degree(Den);den=[];
        [m,n]=size(sl.D)
        c=sl.C
        for l=1:m //loop on outputs
            [m,i]=max(abs(c(l,:)));
            if m<>0 then
                ci=c(l,i)
                t=eye(na,na)*ci;
                t(i,:)=[-c(l,1:i-1), 1, -c(l,i+1:na)]
                al=sl.A*t;

                t=eye(na,na)/ci;
                t(i,:)=[c(l,1:i-1)/ci, 1, c(l,i+1:na)/ci]
                al=t*al;ai=al(:,i),
                b=t*sl.B

                for k=1:n //loop on inputs
                    al(:,i)=ai+b(:,k);
                    [nlk,dlk]=simp(poly(al,var),Den)
                    den(l,k)=dlk;
                    num(l,k)=-(nlk-dlk)*ci;
                end
            else
                num(l,1:n)=0*ones(1,n);
                den(l,1:n)=ones(1,n);
            end
        end
        if lhs==3 then
            h=sl.D;
        else
            w=num./den+sl.D;
            if type(w)==1 then
                h=w;   //degenerate case
            else
                h=syslin(domaine,w);
            end
        end
    end
endfunction
