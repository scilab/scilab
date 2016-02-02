// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [hst]=fsfirlin(hd,flag)
    //<hst>=fsfirlin(hd,flag)
    //macro for the design of FIR, linear phase filters
    //using the frequency sampling technique
    //  hd   : vector of desired frequency response samples
    //  flag : is equal to 1 or 2,
    //         according to the choice of type 1 or type 2 design
    //  hst  : vector giving the approximated continuous response
    //         on a dense grid of frequencies
    //!


    n1=prod(size(hd));//filter length
    if int(n1/2)==n1/2,n=2*n1;else,n=2*n1+1;end;//even or odd length
    scd=sincd(n,flag);//calculates the function Sin(N*x)/Sin(x)
    hst=hd(1)*scd(4*n+1:6*n+1);
    eps=(-1)**(n-1);
    for j=1:n1-1,hst=hst+hd(j+1)*[scd(-4*j+4*n+1:-4*j+6*n+1)+..
    eps*scd(4*j+1:4*j+2*n+1)];end;
endfunction
