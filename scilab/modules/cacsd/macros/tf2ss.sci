// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [sl]=tf2ss(h,tol)
    // Transfer function to state-space.
    //Syntax : sl=tf2ss(h [,tol])
    // h = transfer matrix
    // sl = linear system in state-space representation (syslin list)
    //!

    [lhs,rhs]=argn(0)

    if type(h)<=2 then
        sl=syslin([],[],[],[],h);
        return;
    end
    [num,den]=h(["num","den"]);
    //
    [nd,md]=size(den)
    a=[];b=[];c=[];d=[];n1=0; // s=[]
    for k=1:md
        for l=1:nd
            [r,q]=pdiv(num(l,k),den(l,k));
            dk(l)=q;
            num(l,k)=r;
        end
        if nd<>1 then
            [nk,pp]=cmndred(num(:,k),den(:,k));
        else
            pp=den(k);nk=num(k);
        end;

        slk=cont_frm(nk,pp);
        [ak,bk,ck,dk1]=slk(2:5);
        // [s sk]
        [n2,m2]=size(bk);
        if n2<>0 then
            a(n1+1:n1+n2,n1+1:n1+n2)=ak;
            b(n1+1:n1+n2,k)=bk;
            c=[c ck];
            n1=n1+n2;
        else
            if n1<>0 then b(n1,k)=0;end
        end;
        d=[d dk];
    end;

    if degree(d)==0 then d=coeff(d),end

    if n1<>0 then
        nrmb=norm(b,1);
        nrmc=norm(c,1);
        fact=sqrt(nrmc*nrmb);
        b=b*fact/nrmb;
        c=c*fact/nrmc;
        atmp=a;
        [a,u]=balanc(a);
        //next lines commented out to fix bug 3796
        //   if rcond(u)< %eps*n1*n1*100
        //     nn=size(a,1);u=eye(nn,nn);a=atmp;
        //   end

        //apply transformation u without matrix inversion
        [k,l]=find(u<>0) //get the permutation
        u=u(k,l);c=c(:,k)*u; b=diag(1 ./diag(u))*b(k,:);

        if rhs<2 then
            [no,u]=contr(a',c',%eps);
        else
            [no,u]=contr(a',c',tol);
        end
        u=u(:,1:no);
        a=u'*a*u;b=u'*b;c=c*u;
        sl=syslin(h("dt"),a,b,c,d);
    else
        sl=syslin(h("dt"),[],[],[],d)
    end
endfunction
