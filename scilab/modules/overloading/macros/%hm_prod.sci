// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a = %hm_prod(varargin)
    a = varargin(1)
    dims = size(a);
    tm = type(a.entries)

    nargs = size(varargin);
    select nargs
    case 1
        d = 0;
        if tm == 8 then
            typ = "native";
        else
            typ = "double";
        end
    case 2
        if or(varargin(2) == ["native", "double"]) then
            d = 0;
            typ = varargin(2);
        else
            d = varargin(2);
            if tm == 8 then
                typ = "native";
            else
                typ = "double";
            end
        end
    case 3
        d = varargin(2);
        typ = varargin(3);
    else
        error(msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"),"prod", 1, 3));
    end

    // Check second argument : d
    select type(d)
    case 1
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod", 2))
        end
        if int(d) <> d | d < 0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: Integer >= %d expected.\n"),"prod", 2, 1))
        end
    case 10 
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",2))
        end
        if and(d<>["r","c","*","m"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "prod",2,"""*"",""r"",""c"",""m"""))
        end
        pos=[1,2,0,find(dims>1,1)];
        d=pos(find(d==["r","c","*","m"]))
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"prod",2))
    end

    // Check third argument
    if type(typ)<>10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"prod",3))
    end

    if size(typ,"*")<>1 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",3))
    end

    if and(typ <> ["native" "double"]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"prod", 3, "native", "double"));
    end

    if d == 0 then // '*'
        a=prod(a.entries, "*", typ), dims;
    elseif d > size(dims,"*") then
        //requested summation direction exceeds array dims, return the array, converted
        //to double if necessary.
        if typ == "double" & or(tm == [4 8]) then
            a.entries=double(a.entries),
        end
        a=a
    else
        //permute the array dimension to put the selected dimension first
        p=1:size(dims,"*");
        p([1,d])=p([d,1]);
        a=matrix(permute(a,p),dims(d),-1)
        a=prod(a,1,typ);
        //permute back
        if d==size(dims, "*") then
            dims=dims(1:$-1)
            p(1) = []
        else
            dims(d)=1
        end
        a=permute(matrix(a,dims(p)),p)
    end
endfunction
