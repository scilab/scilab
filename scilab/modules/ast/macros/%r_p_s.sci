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

function f=%r_p_s(f,s)
    // %r_p_s(p,n) computes f^s for f a rational matrix s scalar or vector
    //!

    if s==[] then f=[],return,end
    if or(imag(s)<>0)|or(int(s)<>s) then error(_("Invalid exponent: expected real exponents.")),end
    s = real(s)
    [m,n]=size(f("num"))
    [ms,ns]=size(s)
    if ms==1&ns==1 then
        if m==1|n==1 then //Element wise exponentiation f.^s with f vector
            if s<0 then
                num=f("num")
                if or(abs(coeff(num(:)))*ones(max(degree(num))+1,1)==0) then
                    error(_("Division by zero"))
                end
                s=-s
                f=rlist(f("den").^s,num.^s,f("dt"))
            else
                f=rlist(f("num").^s,f("den").^s,f("dt"))
            end
        elseif m==n then //square matrix exponentiation f^s
            if s==0 then f=eye(m,n),return,end
            if s<0 then f=invr(f),s=-s,end
            f1=f;for k=2:s,f=f*f1;end
        else
        msg = _("%s: Argument #%d: Square matrix expected.\n")
           error(sprintf(msg, "power" , 1));
        end
    elseif ms==1|ns==1 then // Element wise exponentiation f.^s with f "scalar"
        if m<>1|n<>1 then error(_("Not implemented")),end
        kp=find(s>=0)
        kn=find(s<0)
        num=ones(s)
        den=ones(s)
        num(kp)=f("num").^s(kp)
        den(kp)=f("den").^s(kp)
        if abs(coeff(f("num")))*ones(degree(f("num"))+1,1)==0 then
            error(_("Division by zero"))
        end
        num(kn)=f("den").^(-s(kn))
        den(kn)=f("num").^(-s(kn))
        f=rlist(num,den,[])
    elseif m==1&n==1 then // Element wise exponentiation f.^s with f "scalar"
        kp=find(s>=0)
        kn=find(s<0)
        num=ones(s)
        den=ones(s)
        num(kp)=f("num").^s(kp)
        den(kp)=f("den").^s(kp)
        p=1/f
        num(kn)=p("num").^(-s(kn))
        den(kn)=p("den").^(-s(kn))
        f=rlist(num,den,[])
    else
        error(_("Invalid exponent"))
    end

endfunction
