// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2019 - Samuel GOUGEON
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

    // CHECKING INPUT ARGUMENTS
    // ========================
    if rhs < 2 | rhs > 3 then
        msg = gettext("%s: Wrong number of input argument(s): %d or %d expected.\n");
        error(msprintf(msg, "setdiff", 2, 3));
    end
    // Trivial case _whatever is b_
    if isempty(a)
        ka = []
        return
    end
    // orien
    if ~isdef("orien","l") then
        orien = 0
    elseif orien~="r" & orien~="c" & orien~=1 & orien~=2
        msg = gettext("%s: Argument #%d: Must be in the set {%s}.\n");
        error(msprintf(msg, "setdiff", 3, "''r'',''c'',1,2"));
    elseif orien=="c"
        orien = 2
    elseif orien=="r"
        orien = 1
    end
    if orien==1 & ~isempty(b) & size(a,2)~=size(b,2) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of columns expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    if orien==2 & ~isempty(b) & size(a,1)~=size(b,1) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of rows expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end

    // PROCESSING
    // ==========
    Complexes = (type(a)==1 && ~isreal(a)) | (type(b)==1 && ~isreal(b));
    // "r" or "c"
    // ----------
    if orien then
        if ndims(a) > 2 then
            a = serialize_hypermat(a, orien)
        end
        if ndims(b) > 2 then
            b = serialize_hypermat(b, orien)
        end
        if lhs > 1
            [a, ka] = unique(a, orien)
            if isempty(b)
                return
            end
            it = inttype(a)
            b = unique(b, orien)
            if orien==2
                a = a.'
                b = b.'
            end
            if Complexes
                [c, kc] = gsort([[a iconvert(ones(a(:,1)),it)] ;
                                 [b iconvert(ones(b(:,1))*2,it)]], ..
                                "lr", ["i" "i"], list(abs, atan))
            else
                [c, kc] = gsort([[a iconvert(ones(a(:,1)),it)] ;
                                 [b iconvert(ones(b(:,1))*2,it)]], "lr","i")
            end
            k = find(or(c(1:$-1,1:$-1)~=c(2:$,1:$-1),"c") & c(1:$-1,$)==1)
            if c($,$)==1
                k = [k size(c,1)]
            end
            ka = ka(kc(k))
            // a = a(ka,:) // in initial order
            a = c(k,1:$-1)
            if orien==2
                ka = matrix(ka, 1, -1)
                a = a.'
            end
        else
            a = unique(a, orien)
            if isempty(b)
                return
            end
            it = inttype(a)
            b = unique(b, orien)
            if orien==2
                a = a.'
                b = b.'
            end
            if Complexes
                c = gsort([[a iconvert(ones(a(:,1)),it)] ;
                           [b iconvert(ones(b(:,1))*2,it)]], ..
                           "lr", ["i" "i"], list(abs, atan))
            else
                c = gsort([[a iconvert(ones(a(:,1)),it)] ;
                           [b iconvert(ones(b(:,1))*2,it)]], "lr","i")
            end
            k = find(or(c(1:$-1,1:$-1)~=c(2:$,1:$-1),"c") & c(1:$-1,$)==1)
            if c($,$)==1
                k = [k size(c,1)]
            end
            // a = a(ka,:) // in initial order
            a = c(k,1:$-1)
            if orien==2
                a = a.'
            end
        end

    else
        // by element
        // ----------
        if lhs > 1
            [a,ka] = unique(a);
        else
            a = unique(a);
        end
        na = size(a,"*");
        if isempty(b)
            return
        end
        b = unique(b(:));
        if Complexes
            [x,k] = gsort([a(:); b], "g", ["i" "i"], list(abs, atan));
        else
            [x,k] = gsort([a(:); b], "g", "i");
        end
        d = find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
        if d <> [] then
            k([d;d+1]) = [];
        end

        keep = find(k <= na);
        a = a(k(keep));
        if lhs > 1
            ka = ka(k(keep))
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
