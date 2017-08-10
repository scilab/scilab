// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function h=trfmod(h,job)
    // hm=trfmod(h [,job])
    // To visualize the pole-zero structure of a SISO transfer function h
    //     job='p' : visualization of polynomials (default)
    //     job='f' : visualization of natural frequencies and damping
    //
    //!
    select typeof(h)
    case "rational" then
        if size(h("num"))<>[1 1] then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"trfmod",1))
        end
        flag="r"
    case "state-space" then
        if size(h("D"))<>[1 1] then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"trfmod",1))
        end
        flag="lss"
        den=real(poly(h("A"),"s"))
        na=degree(den)
        c=h(4)
        [m,i]=max(abs(c))
        ci=c(i)
        t=eye(h(2))*ci;t(i,:)=[-c(1:i-1), 1, -c(i+1:na)]
        al=h(2)*t;
        t=eye(h(2))/ci;t(i,:)=[c(1:i-1)/ci, 1, c(i+1:na)/ci]
        al=t*al;ai=al(:,i),
        b=t*h(3)
        al(:,i)=ai+b
        num=-(real(poly(al,"s"))-den)*ci
        h=syslin(h(7),num+h(5)*den,den);
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"trfmod",1))
    end


    //
    ft = format();
    format("v", 15);

    [lhs,rhs]=argn(0)
    if rhs==1 then job="p",end
    //
    if type(h("num"))==1 then h("num")=poly(h("num"),varn(h("den")),"c"),end
    if type(h("den"))==1 then h("den")=poly(h("den"),varn(h("num")),"c"),end

    var=varn(h("num")),nv=length(var);
    while part(var,nv)==" " then nv=nv-1,end;var=part(var,1:nv);

    fnum=polfact(h("num"))
    fden=polfact(h("den"))
    g=coeff(fnum(1))/coeff(fden(1))
    nn=prod(size(fnum))
    nd=prod(size(fden))
    //
    num=[]
    for in=2:nn
        p=fnum(in)
        if job=="p" then
            num=[num;pol2str(p)]
        else
            if degree(p)==2 then
                p=coeff(p)
                omeg=sqrt(p(1))
                xsi=p(2)/(2*omeg)
                num=[num;string(omeg)+"    "+string(xsi)]
            else
                num=[num;string(-coeff(p,0))]
            end
        end
    end
    //
    den=[];
    for id=2:nd
        p=fden(id)
        if job=="p" then
            den=[den;pol2str(p)]
        else
            if degree(p)==2 then
                p=coeff(p)
                omeg=sqrt(p(1))
                xsi=p(2)/(2*omeg)
                den=[den;string(omeg)+"    "+string(xsi)]
            else
                den=[den;string(-coeff(p,0))]
            end
        end
    end

    txt=[_("Gain :");string(g);_("Numerator :");num;_("Denominator :");den]

    id=[]
    if job=="p" then
        tit=[gettext("Irreducible Factors of transfer function (click below)")]
    else
        tit=[gettext("Irreducible Factors of transfer function natural frequency and damping factor (click below)")]
    end
    while id==[] then
        t=x_dialog(tit,txt)
        id=find(t==_("Denominator :"))
    end
    txt=t;

    tgain=txt(2)
    tnum=txt(4:id-1)
    tden=txt(id+1:prod(size(txt)))
    execstr(var+"=poly(0,''"+var+"'')")
    num=1
    for in=1:prod(size(tnum))
        txt=tnum(in)
        if length(txt)==0 then txt=" ",end
        if job=="p" then
            t=" ";
            for k=1:length(txt),
                tk=part(txt,k),
                if tk<>" " then t=t+tk,end
            end
            f=1;if t<>" " then f=evstr(t),end
        else
            if txt==part(" ",1:length(txt)) then
                f=1
            else
                f=evstr(txt)
                select prod(size(f))
                case 1 then
                    f=poly(f,var)
                case 2 then
                    f=poly([f(1)*f(1), 2*f(1)*f(2),1],var,"c")
                else
                    error(msprintf(gettext("%s: Incorrect answer.\n"),"trfmod"))
                end
            end
        end
        num=num*f
    end
    //
    den=1
    for id=1:prod(size(tden))
        txt=tden(id);
        if length(txt)==0 then txt=" ",end
        if job=="p" then
            t=" ";
            for k=1:length(txt),
                tk=part(txt,k),
                if tk<>" " then t=t+tk,end
            end
            f=1;if t<>" " then f=evstr(t),end
        else
            if txt==part(" ",1:length(txt)) then
                f=1
            else
                f=evstr(txt)
                select prod(size(f))
                case 1 then
                    f=poly(f,var)
                case 2 then
                    f=poly([f(1)*f(1), 2*f(1)*f(2),1],var,"c")
                else
                    error(msprintf(gettext("%s: Incorrect answer.\n"),"trfmod"))
                end
            end
        end
        den=den*f
    end
    x=evstr(tgain)/coeff(den,degree(den))
    h("num")=num*x
    h("den")=den/coeff(den,degree(den))
    format(ft(2),ft(1));
    if flag=="lss" then h=tf2ss(h),end
endfunction
