// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
