// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [la,lb]=lattp(n,p,mat_cov)
    // See lattn for more information
    [l,d]=size(mat_cov);
    id=eye(d);
    [a,b]=lattn(n,0,mat_cov);
    a=a(n);
    b=b(n);
    z=poly(0,"z");
    la=list();
    lb=list();
    for j=0:p-1,
        r1=flipdim(mat_cov((j+1)*d+1:(j+n+2)*d,:), 1, d);
        r2=flipdim(mat_cov(j*d+1:(j+n+1)*d,:), 1, d);
        c1=coeff(a);
        c2=coeff(b);
        k=(c1*r1)*inv(c2*r2);
        hst=-inv(c1(:,n*d+1:(n+1)*d));
        r=k*hst;
        a=(id-r*z)*a-k*z*b;
        b=-hst*a;
        la(j+1)=a;
    end;
endfunction
