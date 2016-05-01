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

function r=%sp_prod(a,d,typ)
    if argn(2)==1 then
        d="*"
    elseif argn(2)==2 then
        if argn(2)==2& or(d==["native","double"]) then
            d="*"

        end
    end
    if size(d,"*")<>1 then
        if type(d)==10 then
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"prod",2))
        else
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
        end
    end

    if type(d)==10 then
        d=find(d==["m","*","r","c"])
        if d==[] then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "prod",2,"""*"",""r"",""c"",""m"""))
        end
        d=d-2
    end
    dims=size(a);

    if d==-1 then //'m'
        d=find(dims>1,1)
        if d==[] then d=0,end
    end

    if d<0 then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "prod",2,"""*"",""r"",""c"",""m"",""native"",""double"""))
    end

    select d
    case 0 then
        if size(a, "*") <> nnz(a) then
            r = 0;
        end

        r=prod(full(a))
    case 1 then
        for k=1:dims(2)
            r(1,k)=prod(a(:,k))
        end
        r=sparse(r)
    case 2 then
        for k=1:dims(1)
            r(k,1)=prod(a(k,:))
        end
        r=sparse(r)
    else
        r=a
    end

endfunction
