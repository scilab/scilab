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

function [f,x,g]=leastsq(imp,fun,varargin)

    //                                                        n     p
    //   min sum (fun(x).^2)    where fun is a function from R  to R
    //     x
    //
    // [f]=fun(x) computes the value f of the function at the point x
    // and the gradient g of f at x g(i,j)=Dfi/dxj
    if type(imp)<>1 then
        varargin(0)=fun
        fun=imp
        imp=0
    end

    if type(fun)==15 then
        fn=fun(1);params=fun;params(1)=null()
    else
        fn=fun;params=list()
    end

    Dfun=varargin(1),kr=1;
    if type(Dfun)==10 then //the 'b' keyword or the jacobian entry point name
        if Dfun=="b" & size(varargin) >= 3 then
            if type(varargin(2))==1 & type(varargin(3))==1 then
                J=%f //bounds specification
            else
                J=%t //jacobian
            end
        else
            J=%t //jacobian
        end
    elseif type(Dfun)==11|type(Dfun)==13 then
        J=%t  //Jacobian provided
    elseif type(Dfun)==15 then
        error(msprintf(gettext("%s: Jacobian cannot be a list, parameters must be set in fun."),"leastsq"));
    else
        J=%f;
    end
    if J then, varargin(1)=null(), end  // to correct bug 1219 (bruno, 22 feb 2005)
    kr=1

    if varargin(kr)=="b" then kr=kr+3,end
    x0=varargin(kr)

    if type(fn)==10 then //hard coded function given by its name
        if size(params)==0 then
            error(msprintf(gettext("%s: With hard coded function, user must give output size of fun."),"leastsq"));
        end
        m=params(1);params(1)=null()
        n=size(x0,"*")
        // foo(m,nx,x,params,f)
        deff("f=fn(x)","f=call(''"+fn+"'',"+..
        "m,1,''i'',n,2,''i'',x,3,''d'',"+..
        "pars,4,''d'',''out'',["+string(m)+",1],5,''d'')")

        pars=[];
        for k=1:size(params)
            p=params(k)
            pars=[pars;p(:)]
        end
        params=list()

    end

    if J then //jacobian given
        if type(Dfun)==10 then //form function to call hard coded external
            // dfoo(m,nx,x,params,g)
            deff("g=Dfun(x)","g=call(''"+Dfun+"'',"+..
            "m,1,''i'',n,2,''i'',x,3,''d'',"+..
            "pars,4,''d'',''out'',["+string(m)+","+string(n)+"],5,''d'')")
        end
    else
        if params==list() then
            deff("g=Dfun(x)","g=numderivative(fn,x)")
        else
            deff("g=Dfun(x,varargin)","g=numderivative(list(fn,varargin(:)),x)")
        end
    end

    if params==list() then
        deff("[f,g,ind]=%opt(x,ind)",[
        "ff=fn(x);gf=Dfun(x)"
        "f=sum(ff.^2)"
        "g=2*(gf''*ff(:))"])
    else
        deff("[f,g,ind]=%opt(x,ind)",[
        "ff=fn(x,params(:));gf=Dfun(x,params(:))"
        "f=sum(ff.^2)"
        "g=2*(gf''*ff(:))"])
    end

    [f,x,g]=optim(%opt,varargin(:),imp=imp)
endfunction

