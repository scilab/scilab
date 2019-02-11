// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
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

    rhs = argn(2);

    // CHECKING INPUT ARGUMENTS
    // ========================
    if rhs < 2 | rhs > 3 then
        msg = gettext("%s: Wrong number of input argument(s): %d or %d expected.\n");
        error(msprintf(msg, "setdiff", 2, 3));
    end
    // Trivial case _whatever is b_
    if a==[]
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
    if orien==1 & size(a,2)~=size(b,2) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of columns expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    if orien==2 & size(a,1)~=size(b,1) then
        msg = _("%s: Arguments #%d and #%d: Same numbers of rows expected.\n")
        error(msprintf(msg, "setdiff", 1, 2))
    end
    
    // PROCESSING
    // ==========
    // "r" or "c"
    // ----------
    if orien then
        it = inttype(a)
        if ndims(a)>2 then
            a = serialize_hypermat(a, orien)
        elseif orien==2
            a = a.'
        end
        // Trivial case
        if b == [] then
            ka = 1:size(a,orien);
            if orien==1
                ka = ka'
            end
            return
        end
        if ndims(b)>2 then
            b = serialize_hypermat(b, orien)
        elseif orien==2
            b = b.'
        end
        [a, ka] = unique(a, "r")
        b = unique(b, "r")
        [c, kc] = gsort([[a iconvert(ones(a(:,1)),it)] ;
                         [b iconvert(ones(b(:,1))*2,it)]], "lr","i")
        k = find(or(c(1:$-1,1:$-1)~=c(2:$,1:$-1),"c") & c(1:$-1,$)==1)
        if c($,$)==1
            k = [k size(c,1)]
        end
        ka = ka(kc(k))
        // a = a(ka,:) // in initial order
        a = c(k,1:$-1)
        if orien==2
            ka = ka'
            a = a.'
        end
    else
        // by element
        // ----------
        [a,ka] = unique(a);
        na = size(a,"*");
    
        b = unique(b(:));
    
        [x,k] = gsort([a(:); b], "g", "i");
        d = find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
        if d <> [] then
            k([d;d+1]) = [];
        end
    
        keep = find(k <= na);
        a = a(k(keep));
        ka = ka(k(keep));
    end
endfunction
// ----------------------------------------------------------------------------
function h = serialize_hypermat(h, orien)
    if orien==1 then
        dims = 1:ndims(h)
        dims([1 2]) = [2 1]
        h = permute(h, dims)
    end
    h = matrix(h, size(h,1), -1).'
endfunction
