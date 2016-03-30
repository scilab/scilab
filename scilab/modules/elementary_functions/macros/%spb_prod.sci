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

function r=%spb_prod(varargin)
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
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"prod",2))
        end
        if and(d<>["r","c","*","m"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "prod",2,"""*"",""r"",""c"",""m"""))
        end
        pos = [1,2,0,find(dims>1,1)];
        d=pos(find(d==["r","c","*","m"]))
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: string or scalar expected.\n"),"prod",2))
    end

    // Check third argument
    if type(typ)<>10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"prod",3))
    end

    if size(typ,"*")<>1 then
        error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"prod",3))
    end

    if and(typ <> ["native" "double"]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"prod", 3, "native", "double"));
    end
    
//    rhs = argn(2);
//    dims=size(a);
//    if rhs==1 then
//        typ="double";
//        d=0 //"*"
//    else
//        // call prod(a, d) or prod(a, d, typ)
//        // d must be a string or scalar -> check type and size
//        if and(type(d)<> [1, 10]) then
//            error(msprintf(_("%s: Wrong type for input argument #%d: string or scalar expected.\n"),"prod",2))
//        end
//
//        if size(d,"*")<>1 then
//            if type(d)==10 then
//                error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"prod",2))
//            else
//                error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
//            end
//        end
//
//        // call prod(a, d) with d = "native" or "double"
//        if rhs == 2 & or(d==["native","double"]) then
//            typ=d;
//            d=0 //"*"
//        else
//            // If d is a string, d = "m", "*", "r" or "c"
//            // Else d is an integer > 0
//            if type(d)==10 then
//                d=find(d==["m","*","r","c"])
//                if d==[] then
//                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
//                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
//                end
//                d=d-2
//            else
//                if d<0 then
//                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
//                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
//                end
//            end
//
//            if d==-1 then //'m'
//                d=find(dims>1,1)
//                if d==[] then d=0,end
//            end
//
//            // call prod(a, d, typ)
//            // typ must be a string, equal to "native" or "double"
//            if rhs == 3  then
//                if type(typ)<>10 then
//                    error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"prod",3))
//                end
//
//                if size(typ,"*")<>1 then
//                    error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"prod",3))
//                end
//
//                if and(typ <> ["native", "double"])  then
//                    error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"prod", 3, "native", "double"));
//                end
//            else
//                typ="double"
//            end
//        end
//    end

    select d
    case 0 then
        ij=spget(a)
        r=size(ij,1)==prod(dims)
        if typ<>"native" then r=bool2s(r),end
    case 1 then
        for k=1:dims(2)
            r(1,k)=prod(a(:,k),typ)
        end
        r=sparse(r)
    case 2 then
        for k=1:dims(1)
            r(k,1)=prod(a(k,:),typ)
        end
        r=sparse(r)
    else
        r=a
        if typ<>"native" then r=bool2s(r),end
    end

endfunction
