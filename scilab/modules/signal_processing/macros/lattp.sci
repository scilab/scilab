// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [la,lb]=lattp(n,p,cov)
    // See lattn for more information
    [l,d]=size(cov);
    id=eye(d);
    [a,b]=lattn(n,0,cov);a=a(n);b=b(n);
    z=poly(0,"z");la=list();lb=list();
    jd=jmat(n+1,d);
    for j=0:p-1,
        r1=jd*cov((j+1)*d+1:(j+n+2)*d,:);
        r2=jd*cov(j*d+1:(j+n+1)*d,:);
        c1=coeff(a);c2=coeff(b);
        k=(c1*r1)*inv(c2*r2);
        hst=-inv(c1(:,n*d+1:(n+1)*d));
        r=k*hst;
        a=(id-r*z)*a-k*z*b;
        b=-hst*a;
        la(j+1)=a;
    end;
endfunction
