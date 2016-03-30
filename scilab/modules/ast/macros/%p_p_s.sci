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

function f=%p_p_s(p,s)
    // %p_p_s(p,s)  computes p^s for p polynomial matrix in special cases
    //!

    if s==[] then f=[],return,end
    if  or(imag(s)<>0)|or(int(s)<>s) then  error(msprintf(_("%s: Wrong type for input argument #%d: An integer matrix expected.\n"),"%p_p_s",2)),end
    [m,n]=size(p)
    [ms,ns]=size(s)
    if ms==1&ns==1 then
        if m==1|n==1 then //Element wise exponentiation p.^s with p vector
            if s<0 then
                if or(abs(coeff(p(:)))*ones(max(degree(p))+1,1)==0) then
                    error(27)
                end
                f = rlist(ones(p),p.^(-s),[])
            else // this case is in fact hard coded
                f=p.^s
            end
        elseif m==n then //square matrix exponentiation p^s
            if s<0 then
                f=invr(p),
                s=-s;
                f1=f;for k=2:s,f=f*f1;end
            else
                f=p
                for k=2:s,f=f*p;end
            end
        else
            error(20,1)
        end
    elseif ms==1|ns==1 then // Element wise exponentiation f.^s with f "scalar"
        if m<>1|n<>1 then error(43),end

        kp=find(s>=0)
        kn=find(s<0)
        num=ones(s)
        den=ones(s)
        num(kp)=p.^s(kp)
        if abs(coeff(p))*ones(degree(p)+1,1)==0 then
            error(27)
        end
        den(kn)=p.^(-s(kn))
        f = rlist(num,den,[])
    else
        error(43)
    end


endfunction
