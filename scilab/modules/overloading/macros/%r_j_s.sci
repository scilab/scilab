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

function f=%r_j_s(f,s)
    //  p.^s for p rational matrix
    //!
    if s==[] then f=[],return,end
    if  or(imag(s)<>0)|or(int(s)<>s) then error(_("Invalid exponent: expected real exponents.")),end
    s = real(s)
    [m,n]=size(f)
    [ms,ns]=size(s)
    if ms==1&ns==1 then
        if s<0 then
            num=f("num")
            if or(abs(coeff(num(:)))*ones(max(degree(num))+1,1)==0) then
                error(_("Division by zero"))
            end
            s=-s
            f=rlist(f("den").^s,f("num").^s,f("dt"))
        else
            f=rlist(f("num").^s,f("den").^s,f("dt"))
        end
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
    elseif ms==m&ns==n then  // Element wise exponentiation
        s=s(:);
        kp=find(s>=0)
        kn=find(s<0)
        num=f("num")(:);
        den=f("den")(:);
        num(kp)=num(kp).^s(kp)
        den(kp)=den(kp).^s(kp)

        if or(abs(coeff(num(kn)))*ones(max(degree(num(kn)))+1,1)==0) then
            error(_("Division by zero"))
        end
        num(kn)=den(kn).^(-s(kn))
        den(kn)=num(kn).^(-s(kn))
        f=rlist(matrix(num,n,m),matrix(den,n,m),[])
    else
        error(_("Invalid Exponent"))
    end

endfunction
