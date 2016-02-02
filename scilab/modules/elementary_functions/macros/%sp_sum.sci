// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a=%sp_sum(a,d,typ)
    rhs=argn(2)
    if rhs==1 then
        d="*"
    elseif rhs==2 then
        if argn(2)==2& or(d==["native","double"]) then
            d="*"
        end
    end
    if size(d,"*")<>1 then
        if type(d)==10 then
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"sum",2))
        else
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"sum",2))
        end
    end

    [ij s dims] = spget(a);

    if type(d)==10 then
        d=find(d==["m","*","r","c"])
        if d==[] then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "sum",2,"""*"",""r"",""c"",""m"""))
        end
        d=d-2
    end

    if d==-1 then
        //sum(x,'m'), determine the summation direction
        d=find(dims>1,1)
        if d==[] then d=0,end
    end

    if d<0 then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "sum",2,"""*"",""r"",""c"",""m"",""native"",""double"""))
    end

    select d
    case 0 then
        //sum of all elements
        a=sum(s)
    case 1 then
        a=sparse(ones(1,dims(1))*a);
    case 2 then
        a=sparse(a*ones(dims(2),1));
    else
        a=a
    end
endfunction
