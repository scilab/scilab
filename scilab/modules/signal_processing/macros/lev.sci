// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// Copyright (C) INRIA - 1991 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [ar,sigma2,rc]=lev(r)
    //[ar,sigma2,rc]=lev(r)
    //Resolve the Yule-Walker equations:
    //
    //       |r(0)   r(1)   ... r(N-1)|| a(1) | |sigma2|
    //       |r(1)   r(0)   ... r(n-1)|| a(2) | |  0   |
    //       |  :      :    ...    :  ||   :  |=|  0   |
    //       |  :      :    ...    :  ||   :  | |  0   |
    //       |r(N-1) r(N-2) ...  r(0) ||a(N-1)| |  0   |
    //
    //using Levinson's algorithm.
    //  r      :Correlation coefficients
    //  ar     :Auto-Regressive model parameters
    //  sigma2 :Scale constant
    //  rc     :Reflection coefficients
    //!

    //get the size of the correlation vector

    rsize=max(size(r));
    r=matrix(r,1,rsize);

    //initialize levinson's algorithm

    ar=-r(2)/r(1);
    rc(1)=ar;
    sigma2=(1-ar*conj(ar))*r(1);

    //iterative solution to yule-walker equations

    for k=2:rsize-1,
        ak1(k)=-(r(k+1)+ar(1:k-1)'*r(k:-1:2)')/sigma2;
        rc(k)=ak1(k);
        ak1(1:k-1)=ar(1:k-1)+ak1(k)*conj(ar(k-1:-1:1));
        sigma2=(1-ak1(k)*conj(ak1(k)))*sigma2;
        ar=ak1;
    end,
endfunction
