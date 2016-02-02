// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
function x=erfinv(y)
    //   X = erfinv(Y) is the inverse error function for each element of Y.
    //   The inverse error function satisfies y = erf(x), for -1 <= y <= 1
    //   and -inf <= x <= inf.
    //   uses rational aproximation of erfinv on ]-1 -0.7[ [-0.7 0.7]  ]0.7 1[
    //   to obtain first aproximation then two newtion steps are used to
    //   refine the solution of the equation y-erf(x)=0
    //   coefficients of rational approximation are determined by solving non linear
    //   linear least square problems given tabulated values of x and erf(x).

    if ~isreal(y) then error(msprintf(gettext("%s: Wrong type for input argument #%d: Real expected.\n"),"erfinv",1));end;
    x = zeros(y)


    // Central range. -0.7 <= y <= 0.7
    //coefficient of the rational approximation of erfinv on this range
    a = [0.88622692374517353,-1.6601283962374516,0.92661860147244357,-0.14110320437680104];
    b = [-2.13505380615258078,1.46060340345661088,-0.33198239813321595,0.01197270616590528];
    //       a1*y + a2*y^3 + a3*y^5 + a4*y^7
    //  x= --------------------------------------
    //     1 + b1*y^2 + b2*y^4 + b3*y^6 + b4*y^8
    y0 = 0.7;
    k =find(abs(y)<=y0);
    if k<>[] then
        z = y(k) .* y(k);
        x(k) = y(k) .* (((a(4)*z+a(3)) .* z+a(2)) .* z+a(1)) ./ ((((b(4)*z+b(3)) .* z+b(2)) .* z+b(1)) .* z+1);
    end

    // Near end points of range.
    // Coefficients in rational approximations.

    c = [-1.994216456587148,-1.87267416351196,3.60874665878559364,1.82365845766309853];
    d = [3.74146294065960872,1.81848952562894617];


    // 0.7 < y < 1
    //       a1 + a2*z + a3*z^2 + a4*z^3
    //  x= ------------------------------ ; z = sqrt(-log(1-y)/2)
    //         1 + b1*z + b2*z^2

    k = find((y0<y)&(y<1));
    if k<>[] then
        z = sqrt(-log((1-y(k))/2));
        x(k) = (((c(4)*z+c(3)) .* z+c(2)) .* z+c(1)) ./ ((d(2)*z+d(1)) .* z+1);
    end

    // -1 < y < 0.7
    //      - a1 - a2*z - a3*z^2 - a4*z^3
    //  x= ------------------------------ ; z = sqrt(-log(1+y)/2)
    //         1 + b1*z + b2*z^2

    k = find((-y0>y)&(y>-1));
    if k<>[] then
        z = sqrt(-log((1+y(k))/2));
        x(k) = -(((c(4)*z+c(3)) .* z+c(2)) .* z+c(1)) ./ ((d(2)*z+d(1)) .* z+1);
    end

    // Two steps of Newton-Raphson correction to obtain full accuracy.
    // Without these steps, erfinv(y) would be about 3 times
    // faster to compute, but accurate to only about 6 digits.

    x = x-(erf(x)-y) ./ (2/sqrt(%pi)*exp(-x.^2));
    x = x-(erf(x)-y) ./ (2/sqrt(%pi)*exp(-x.^2));

    // Exceptional cases.
    x(find(y==-1))=-%inf;
    x(find(y==1))=%inf;
    x(find(abs(y)>1|isnan(x)))=%nan;

endfunction
