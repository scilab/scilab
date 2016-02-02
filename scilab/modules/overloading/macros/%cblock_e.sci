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
function y=%cblock_e(varargin)
    x=varargin($)
    dims=[];
    for k=2:length(x)
        dims=[dims;size(getfield(k,x))];
    end
    m=dims(1,1) // common row number

    dims=cumsum([1 dims(:,2)']);
    n=dims($)-1 // number of columns
    y=mlist("cblock")

    j=varargin($-1)
    if type(j)==2|type(j)==129 then
        j=horner(j,n)
    elseif type(j)==4 then
        j=find(j)
    elseif and(size(j)==[-1,-1]) then
        j=1:n
    end
    if length(varargin)==2 then //y=x(i)
        if m<>1 then
            error(msprintf(_("%s: Syntax x(j) only allowed for row cblock''s.\n"),"cblock_e"));
        end
        i=1
    else //y=x(i,j)
        i=varargin(1)
    end

    first=%t

    for k=1:size(j,"*")
        jk=j(k)
        I=find(jk>=dims(1:$-1)&jk<dims(2:$))
        if I>1 then jk=jk-dims(I)+1,end
        v=getfield(I+1,x)
        if first then
            temp=v(i,jk)
            first=%f
        else
            if type(v)==type(temp) then
                temp=[temp v(i,jk)]
            else
                y=setfield(length(y)+1,temp,y)
                temp=v(i,jk)
            end
        end
    end
    if length(y)==1 then
        y=temp
    else
        y=setfield(length(y)+1,temp,y)
    end

endfunction
