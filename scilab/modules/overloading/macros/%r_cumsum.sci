// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a=%r_cumsum(a, orient, typ)
    // cumulative sum for rational matrics
    rhs = argn(2);
    select rhs
    case 1
        orient = "*";
    case 2
        if or(orient == ["native", "double"]) then
            orient = "*";
        end
    case 3
        if and(typ <> ["native", "double"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"cumsum", 3, "native", "double"));
        end
    end

    if size(orient, "*") <> 1 then
        if type(orient) == 10 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum", 2));
        else
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumsum", 2));
        end
    end

    dims = size(a);

    if type(orient) == 10 then
        orient = find(orient == ["m","*","r","c"]);
        if isempty(orient) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(size(dims,"*"))));
        end
        orient=orient-2;
    end

    if orient == -1 then //'m'
        orient = find(dims > 1, 1);
        if isempty(orient) then
            orient = 0;
        end
    end
    if orient<0 then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))));
    end
    
    select orient
    case 0 then // case "*"
        a = tril(ones(size(a,"*"), size(a, "*"))) * matrix(a, -1, 1);
        if simp_mode() then
            a = simp(a);
        end
        a = matrix(a, dims(1), dims(2));
    case 1 then // case 1 or 'r'
        a = [tril(ones(dims(1), dims(1))) zeros(dims(2), dims(2)); zeros(dims(1), dims(1)) tril(ones(dims(2), dims(2)))] * matrix(a, -1, 1);
        if simp_mode() then
            a = simp(a);
        end
        a = matrix(a, dims(1), dims(2));
    case 2 then // case 2 or 'c"
        a = [eye(dims(1), dims(1)) zeros(dims(2), dims(2)); eye(dims(1), dims(1)) eye(dims(2), dims(2))] * matrix(a, -1, 1);
        if simp_mode() then
            a = simp(a);
        end
        a = matrix(a, dims(1), dims(2));
    else
        a=a
    end

endfunction
