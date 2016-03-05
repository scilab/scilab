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

function [archap,la,lb,sig,resid]=armax(r,s,y,u,b0f,prf)
    // [la, lb, sig, resid] = armax(r, s, y, u, [b0f, prf])
    // armax identification
    // Compute the coefficients of a n-dimensional ARX
    //   A(z^-1)y= B(z^-1)u + sig*e(t)
    //           e(t) is a white noise of n-dimensional Id variance
    //           sig is a n by n matrix
    //           A(z)= 1+a1*z+...+a_r*z^r; ( r=0 => A(z)=1)
    //           B(z)= b0+b1*z+...+b_s z^s ( s=-1 => B(z)=0)
    // Method:
    //     Cfre : Eykhoff (trends and progress in system identification) page 96
    //     Introducing z(t)=[y(t-1),..,y(t-r),u(t),...,u(t-s)] and
    //     oeff = [-a1,..,-ar,b0,...,b_s], we obtain
    //     y(t)= coef* z(t) + sig*e(t)
    //     and the algorithm is finding coef which minimizes
    //     sum_{t=1}^N ( [y(t)- coef'z(t)]^2)
    //
    // Input:
    //     y: output process y(ny, n); ny: dimension,
    //                                 n: sample size
    //     u: input process u(nu, n); nu: dimension
    //                                n: sample size
    //     r and s: auto-regression orders r >=0 and s >=-1
    //     b0f is optional parameter. By default, b0f is 0 and it
    //     means that this parameter must be identified. If b0f is 1, then
    //     b0f is supposed to be zero and is not identified.
    //     prf is optional parameter for display control.
    //         if prf = 1, a display is done (the default value)
    // Output:
    //     la is the list(a,a+eta,a-eta); eta: estimated standard deviation
    //                                a=[Id,a1,a2,...,ar] where ai: ny-by-ny matrix
    //     lb is the list(b,b+etb,b-etb); etb: estimated standard deviation
    //                               b=[b0,.....,b_s] where bi is nu-by-nu matrix
    //     sig is the estimated standard deviation of noise
    //     resid=[ sig*e(t0),....];
    //     t0=max(max(r,s)+1,1));
    //
    // Example:
    //      Enter the command [a,b,sig,resid]=armax();
    //      to see an example in dimension 1.
    // Auteur: J-Ph. Chancelier ENPC Cergrene
    //!
    // Copyright INRIA
    [lhs,rhs]=argn(0)
    if rhs==0,write(%io(2),"/ / y_t = 0.2*u_{t-1}+0.01*e(t)");
        write(%io(2)," rand(''normal''); u=rand(1,1000);");
        write(%io(2)," y=arsimul([1],[0,0.2],1,0.01,u);");
        write(%io(2)," [archap,a,b,sig,resid]=armax(0,1,y,u)");
        write(%io(2),"/ / we must find a=1,b=[0,0.2]''");
        rand("normal"),u=rand(1,1000);
        y=arsimul([1],[0,0.2],1,0.01,u);
        [archap,la,lb,sig,resid]=armax(0,1,y,u,1);
        return
    end
    if rhs<=5,prf=1;end
    if rhs<=4,b0f=0;end
    [ny,n2]=size(y)
    [nu,n2u]=size(u)
    // Compute zz matrix as
    // zz(:,j)=[ y(t-1),...,y(t-r),u(t),...,u(t-s)]', with  t=t0-1+j
    // zz can be computed from t = t0
    t0=max(max(r,s)+1,1);
    if r==0;if s==-1;error(msprintf(gettext("%s: Wrong value for input arguments: If %s and %s nothing to identify.\n"),"armax","r==0","s==-1"))
    end;end
    z=[];
    if r>=1;for i=1:r,z=[z ; y(:,t0-i:(n2-(i)))];end;end
    if s>=-1;for i=b0f:s,z=[z ; u(:,t0-i:(n2-(i)))];end;end
    zz= z*z';
    zy= z*y(:,t0:n2)';
    // Rank test
    [nzl,nzc]=size(zz);
    k=rank(zz);
    if k<>nzl then
        warning(msprintf(gettext("%s: %s is numerically singular.\n"),"armax","z*z''"));
    end;
    pv=pinv(zz);
    coef=(pv*zy)';
    // The residual noise
    resid=y(:,t0:n2) - coef*z;
    // The variance of the residual noise
    sig2= resid*resid'/(n2-t0+1)
    // The standard deviation
    sig=sqrtm(sig2);
    a=[eye(ny,ny),-coef(:,1:r*ny)];
    if b0f==0 then
        b=coef(:,r*ny+1:(s+1)*nu+r*ny);
    else
        b=[0*ones(ny,nu),coef(:,r*ny+1:r*ny+s*nu)];
    end
    // For the SISO systems, the estimated standard deviation is added.
    // It is to be done for the MIMO
    if ny == 1,
        dve=sqrt(diag(sig*pv,0))';
        la=list(a,a+[0,dve(1:r)],a-[0,dve(1:r)]);
        if b0f==0 then
            lb=list(b,b+dve(r+1:r+s+1),b-dve(r+1:r+s+1)),
        else
            lb=list(b,b+[0,dve(r+1:r+s)],b-[0,dve(r+1:r+s)]);
        end
    else
        la=a;lb=b;
    end
    // If prf = 1, the display is done
    //si prf vaut 1 on donne un display
    archap=armac(a,b,eye(ny,ny),ny,nu,sig);

    if prf==1;
        if ny==1,
            [nla,nca]=size(la(2));
            mprintf(gettext("%s: Standard deviation of the estimator %s:\n"),"armax","a");
            form_a="(5x,"+string(nca)+"(f7.5,1x))";
            write(%io(2),la(2)-a,form_a);
            if nu<>0 then
                mprintf(gettext("%s: Standard deviation of the estimator %s:\n"),"armax","b");
                [nlb,ncb]=size(lb(2));
                write(%io(2),lb(2)-b,"(5x,"+string(ncb)+"(f7.5,1x))");
            end
        end
    end
endfunction
