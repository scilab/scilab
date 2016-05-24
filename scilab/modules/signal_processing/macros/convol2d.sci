// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function y=convol2d(h,x)
    //  convol2d - 2-D convolution
    //%SYNTAX
    //  y=convol2d(h,x)
    //%PARAMETERS
    //  x,h       :input matrices
    //  y         : output of convolution
    //%DESCRIPTION
    //  calculates the 2-D convolution y= h*x of two discrete sequences by
    //  using the fft.
    if argn(2)<2 then
        error(msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"convol2d",2))
    end

    if type(h)<>1 then
        error(msprintf(_("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"),"convol2d",1))
    end
    if type(x)<>1 then
        error(msprintf(_("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"),"convol2d",2))
    end
    if isempty(h) then
        y=zeros(x);
        return
    end
    if isempty(x) then
        y=zeros(h);
        return
    end
    //inline fft2d function definition (fft2 does not manage inverse fft)
    function y=fft2d(x,d)
        [mx,nx]=size(x)
        y=fft(fft(x,d,mx,1),d,nx,mx)
    endfunction
    [mx,nx]=size(x);
    [mh,nh]=size(h);
    //use  power of 2 dimensions for efficiency
    m1=2^(int(log(mx+mh-1)/log(2))+1);
    n1=2^(int(log(nx+nh-1)/log(2))+1);
    //m1=mx+mh-1;
    //n1=nx+nh-1;
    x(m1,n1)=0;
    h(m1,n1)=0;
    y=fft2d(fft2d(x,-1).*fft2d(h,-1),1);
    if isreal(h,0)&isreal(x,0) then
        y=real(y);
    end
    y=y(1:(mx+mh-1),1:(nx+nh-1));
endfunction
