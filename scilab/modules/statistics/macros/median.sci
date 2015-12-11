// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//


function y=median(x,orient)
    //
    // NOTES
    //    - modified by farid.belahcene:the case when x is an hypermatrix
    //    - new syntaxes: median(x,'m') and median(x,dim)

    [lhs, rhs] = argn(0);

    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"),"median",1,2));
    end

    if argn(2)<2 then
        orient=0;
    else
        if orient=="r" then
            orient=1
        elseif orient=="c" then
            orient=2
        elseif orient=="*" then
            orient=0
        elseif orient=="m" then
            orient=find(size(x)>1,1)
            if orient==[] then orient=1,end
        else
            if type(orient)<>1|size(orient,"*")<>1|~isreal(orient)|orient<=0 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'',''%s'' or a positive number expected.\n"),"median",2,"r","c","m")),
            end
        end
    end

    if orient==0 then
        if x==[] then y=%nan,return,end
        n=size(x,"*");
        x=gsort(x(:),"g","i")
        if 2*int(n/2)==n then
            y = (x(n/2)+x(n/2+1))/2;
        else
            y = x((n+1)/2);
        end
    else
        if x==[] then y=[],return,end
        if orient>ndims(x) then y=x; return;  end
        xsize=size(x);
        if xsize(orient)==1 then  y=x; return;  end
        orient_above_size=xsize(orient+1:$);N=prod(orient_above_size)
        orient_below_size=xsize(1:orient-1);M=prod(orient_below_size)
        orient_size=xsize(1:orient);P=prod(orient_size)
        y=[];
        n=xsize(orient)
        for k=1:N
            for i=1:M
                ytemp=gsort(x(i+(0:n-1)*M+(k-1)*P),"r","i"); ytemp = ytemp(:);
                if 2*int(n/2)==n then
                    y = [y (ytemp(n/2,:)+ytemp(n/2+1,:))/2];
                else
                    y = [y ytemp((n+1)/2,:)];
                end
            end
        end
        xsize(orient)=1;
        y=matrix(y,xsize);
    end

endfunction
