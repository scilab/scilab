// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [a, ka] = setdiff(a, b, orien)
    // returns a values which are not in b

    // History:
    // * 2018 - S. Gougeon : orien="r"|"c" added, including the hypermat case
    // * 2019 - S. Gougeon : complex numbers supported

    [lhs, rhs] = argn();
    ka = []

    // ========================
    // CHECKING INPUT ARGUMENTS
    // ========================
    if rhs < 2 | rhs > 3 then
        msg = _("%s: Wrong number of input argument(s): %d or %d expected.\n")
        error(msprintf(msg, "setdiff", 2, 3))
    end
    typa = type(a)
    if ~or(typa == [1 4 5 6 8 10]) then
        msg = _("%s: Argument #%d: Unsupported type %s.\n")
        error(msprintf(msg, "setdiff", 1, typeof(a)))
    end

    if size(a,"*") <> 0 & size(b,"*") <> 0 & typa==10 & type(b) <> 10 then
        msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    if typa == 8 & type(b) == 8 & inttype(a) <> inttype(b) then
        msg = _("%s: Arguments #%d and #%d: Same integer types expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    // orien
    if ~isdef("orien","l") then
        orien = 0
    elseif orien == "r"
        orien = 1
    elseif orien == "c"
        orien = 2
    elseif orien ~= 1 & orien ~= 2
        msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
        error(msprintf(msg, "setdiff", 3, "''r'',''c'',1,2"))
    end
    // Trivial case, whatever is b
    if size(a,"*")==0
        return
    end
    //
    if orien==1 & size(b,"*")<>0 & size(a,2)~=size(b,2) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of columns expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    if orien==2 & size(b,"*")<>0 &  size(a,1)~=size(b,1) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of rows expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end

    // ==========
    // PROCESSING
    // ==========
    Complexes = (or(typa   ==[1 5]) && ~isreal(a)) | ..
                (or(type(b)==[1 5]) && ~isreal(b));

    // "r" or "c"
    // ==========
    if orien then
        if ndims(a) > 2 then
            a = serialize_hypermat(a, orien)
        end
        if ndims(b) > 2 then
            b = serialize_hypermat(b, orien)
        end
        if lhs > 1
            [a, ka] = unique(a, orien)
        else
            a = unique(a, orien)
        end
        if size(b,"*")==0
            return
        end
        b = unique(b, orien)
        if orien==2
            a = a.'
            b = b.'
        end
        if Complexes
            [c, kc] = gsort([a ; b], "lr", ["i" "i"], list(abs, atan))
        else
            [c, kc] = gsort([a ; b], "lr", "i")
        end
        k = find(and(c(1:$-1,:) == c(2:$,:), "c"))
        if k <> []
            a(kc([k k+1]),:) = []
            if lhs > 1
                ka(kc([k k+1])) = []
            end
        end
        if orien==2
            ka = matrix(ka, 1, -1)
            a = a.'
        end

    else
        // by element
        // ==========
        if lhs > 1
            [a, ka] = unique(a);
        else
            a = unique(a);
        end
        if size(b,"*")==0
            return
        end
        b = unique(b(:));
        if Complexes
            [x, k] = gsort([a(:) ; b], "g", ["i" "i"], list(abs, atan));
        else
            [x, k] = gsort([a(:) ; b], "g", "i");
        end
        e = find(x(2:$)==x(1:$-1));
        if e <> []
            a(k([e e+1])) = []
            if lhs > 1
                ka(k([e e+1])) = []
            end
        end
    end
endfunction

// ----------------------------------------------------------------------------

function h = serialize_hypermat(h, orien)
    if orien==1 then
        dims = 1:ndims(h)
        dims([1 2]) = [2 1]
        h = permute(h, dims)
        h = matrix(h, size(h,1), -1).'
    else
        h = matrix(h, size(h,1), -1)
    end
endfunction
