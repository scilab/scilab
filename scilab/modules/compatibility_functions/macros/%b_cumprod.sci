// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a=%b_cumprod(varargin)
    a = varargin(1)
    dims = size(a);

    nargs = size(varargin);
    select nargs
    case 1
        d = "*";
        typ = "double";
    case 2
        if or(varargin(2) == ["native", "double"]) then
            d = "*";
            typ = varargin(2);
        else
            d = varargin(2);
            typ = "double";
        end
    case 3
        d = varargin(2);
        typ = varargin(3);
    else
        error(msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"),"cumprod", 1, 3));
    end

    // Check second argument : d
    select type(d)
    case 1
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod", 2))
        end
        if int(d) <> d | d < 0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: Integer >= %d expected.\n"),"cumprod", 2, 1))
        end
    case 10 
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",2))
        end
        if and(d<>["r","c","*","m"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "cumprod",2,"""*"",""r"",""c"",""m"""))
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumprod",2))
    end

    // Check third argument
    if type(typ)<>10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"cumprod",3))
    end

    if size(typ,"*")<>1 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",3))
    end
    
    if and(typ <> ["native" "double"]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"cumprod", 3, "native", "double"));
    end

    if typ == "native" then
        if type(d) == 10 then
            pos = [1,2,0,find(dims>1,1)];
            d=pos(find(d==["r","c","*","m"]))
        end
        select d
        case 0
            k=find(~a,1)
            if k<>[] then a(k:$) = %f,end
        case 1
            for k=1:dims(2)
                a(:,k)=cumprod(a(:,k),"native")
            end
        case 2
            for k=1:dims(1)
                a(k,:)=cumprod(a(k,:),"native")
            end
        else
            a = a
        end
    else
        a = cumprod(double(a), d)
    end
endfunction
