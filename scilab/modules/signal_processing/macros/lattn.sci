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

function [la,lb]=lattn(n,p,mat_cov)
    //[la,lb]=lattn(n,p,cov)
    //macro which solves recursively on n (p being fixed)
    //the following system (normal equations), i.e. identifies
    //the AR part(poles) of a vector ARMA(n,p) process
    //
    //                        |Rp+1 Rp+2 . . . . . Rp+n  |
    //                        |Rp   Rp+1 . . . . . Rp+n-1|
    //                        | .   Rp   . . . . .  .    |
    //                        |                          |
    //    |I -A1 -A2 . . .-An|| .    .   . . . . .  .    |=0
    //                        | .    .   . . . . .  .    |
    //                        | .    .   . . . . .  .    |
    //                        | .    .   . . . . . Rp+1  |
    //                        |Rp+1-n.   . . . . . Rp    |
    //
    //    where {Rk;k=1,nlag} is the sequence of empirical covariances
    //
    //   n   : maximum order of the filter
    //   p   : fixed dimension of the MA part. If p is equal to -1,
    //       : the algorithm reduces to the classical Levinson recursions.
    //   cov : matrix containing the Rk(d*d matrices for
    //       : a d-dimensional process).It must be given the
    //       : following way:
    //
    //                        |  R0 |
    //                        |  R1 |
    //                    cov=|  R2 |
    //                        |  .  |
    //                        |  .  |
    //                        |Rnlag|
    //
    //   la  : list-type variable, giving the successively calculated
    //       : polynomials (degree 1 to degree n),with coefficients Ak
    //!


    if argn(2)<>3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"lattn",3))
    end
    if type(n)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"lattn",1))
    end
    if  size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"lattn",1))
    end
    if  n<0|n<>round(n) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),"lattn",1))
    end
    if type(p)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"lattn",2))
    end
    if  size(p,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"lattn",2))
    end
    if  p<-1|p<>round(p) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in the interval [%s, %s].\n"),"lattn",2,"-1","%inf"))
    end

    if type(mat_cov)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"lattn",3))
    end

    [l,d]=size(mat_cov);
    if d>l then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A tall matrix expected.\n"),"lattn",3))
    end


    a=eye(d);
    b=eye(d);
    z=poly(0,"z");
    la=list();
    lb=list();
    no=p-n-1;
    cv=mat_cov;

    if -no >= floor(l/d) then
        error(msprintf(gettext("%s: Wrong values for input arguments #%d and #%d.\n"),"lattn",1, 2))
    end

    if no<0,
        for j=1:-no,cv=[mat_cov(j*d+1:(j+1)*d,:)';cv];end;
        p=p-no;
    end

    if (p + 2 + (n-1)) * d >  size(cv,"r") then
        error(msprintf(gettext("%s: Wrong values for input arguments #%d and #%d.\n"),"lattn",1, 2))
    end

    for j=0:n-1,
        r1=flipdim(cv((p+1)*d+1:(p+2+j)*d,:), 1, d);
        r2=flipdim(cv(p*d+1:(p+1+j)*d,:), 1, d);
        r3=flipdim(cv((p-1-j)*d+1:p*d,:), 1, d);
        r4=flipdim(cv((p-j)*d+1:(p+1)*d,:), 1, d);
        c1=coeff(a);
        c2=coeff(b);
        k1=(c1*r1)*inv(c2*r2);
        k2=(c2*r3)*inv(c1*r4);
        a1=a-k1*z*b;
        b=-k2*a+z*b;
        a=a1;
        la(j+1)=a;
        lb(j+1)=b;
    end;
endfunction
