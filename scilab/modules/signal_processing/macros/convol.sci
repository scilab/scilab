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
function [y,e1]=convol(h,x,e0)
    //  convol - convolution
    //%SYNTAX
    //  [y]=convol(h,x)
    //  [y,e1]=convol(h,x,e0)     (for use with overlap add method)
    //%PARAMETERS
    //  x,h       :input sequences (h is a "short" sequence, x a "long" one)
    //  e0        : old tail to overlap add (not used in first call)
    //  y         : output of convolution
    //  e1        : new tail to overlap add (not used in last call)
    //%DESCRIPTION
    //  calculates the convolution y= h*x of two discrete sequences by
    //  using the fft.  overlap add method can be used.
    //%USE OF OVERLAP ADD METHOD
    //  For x=[x1,x2,...,xNm1,xN]
    //  First call : [y1,e1]=convol(h,x1)
    //  Subsequent calls : [yk,ek]=convol(h,xk,ekm1)
    //  Final call : [yN]=convol(h,xN,eNm1)
    //  Finally y=[y1,y2,...,yNm1,yN]
    //!

    [lhs,rhs]=argn(0)
    n=prod(size(x))
    m=prod(size(h))
    m1=2^(int(log(n+m-1)/log(2))+1)
    x(m1)=0;h(m1)=0
    if norm(imag(x))==0&norm(imag(h))==0 then
        y=real(fft(fft(matrix(x,1,m1),-1).*fft(matrix(h,1,m1),-1),1))
    else
        y=fft(fft(matrix(x,1,m1),-1).*fft(matrix(h,1,m1),-1),1)
    end
    if lhs+rhs==5 then,
        e0(n)=0;//update carried from left to right
        e1=y(n+1:n+m-1)
        y=y(1:n)+e0

    elseif lhs+rhs==4 then
        if rhs==2 then
            e1=y(n+1:n+m-1)
            y=y(1:n) //initial update
        else
            e0(n+m-1)=0 //final update
            y=y(1:n+m-1)+e0
        end,

    else
        y=y(1:n+m-1) //no update
    end
endfunction
