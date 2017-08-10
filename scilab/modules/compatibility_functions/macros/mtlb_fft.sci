// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=mtlb_fft(x,n,job)
    // Translation function for fft() Matlab function

    [lhs,rhs]=argn(0)
    if size(x,"*")==0 then
        r=[]
        return
    end
    if rhs<2 then n=[],end
    if rhs==3 then //row or column-wise fft
        select job
        case 1 then //row-wise
            if n<>[] then //pad or truncate
                s=size(x,1)
                if s>n then //truncated fft
                    x=x(1:n,:)
                elseif s<n then //padd with zeros
                    x(n,:)=0
                end
            end
            r=[]
            for xk=x
                r=[r fft(xk,-1)]
            end
        case 2 then //column-wise
            if n<>[] then //pad or truncate
                s=size(x,2)
                if s>n then //truncated fft
                    x=x(:,1:n)
                elseif s<n then //padd with zeros
                    x(:,n)=0
                end
            end
            r=[]
            for k=1:size(x,1)
                r=[r;fft(x(k,:),-1)]
            end
        end
    else
        if min(size(x))==1 then  //fft of a vector
            if n<>[] then //pad or truncate
                s=size(x,"*")
                if s>n then //truncated fft
                    x=x(1:n)
                elseif s<n then //padd with zeros
                    x(n)=0
                end
                r=fft(x,-1)
                if s==1 then
                    r=r.'
                end
            else
                r=fft(x,-1)
            end
        else //row-wise fft
            if n<>[] then //pad or truncate
                s=size(x,1)
                if s>n then //truncated fft
                    x=x(1:n,:)
                elseif s<n then //padd with zeros
                    x(n,:)=0
                end
            end
            r=[]
            for xk=x
                r=[r fft(xk,-1)]
            end
        end
    end
endfunction
