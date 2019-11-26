// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = blockdiag(varargin)
    //!
    // Call overload ?
    // ---------------
    callOverload = %f
    for i = 1:length(varargin)
        v = varargin(i);
        callOverload = callOverload | ..
         ( and(type(v)<>[1 2 4 5 6 8 10]) & and(typeof(v)<>["rational" "state-space"]))
        if callOverload
            oname = "%" + typeof(v, "overload") + "_blockdiag"
            if isdef(oname,"n")
                execstr("r = " + oname + "(varargin(:));")
                return
            else
                msg = _("Function not defined for given argument type(s),\n  Check arguments or define the overloading function %s().\n")
                error(msprintf(msg, oname))
            end
        end
    end
    //
    isss = %f;
    for i = 1:length(varargin)
        isss = isss | typeof(varargin(i))=="state-space"
        if isss, break, end
    end
    withSparse = %f
    for i = 1:length(varargin)
        withSparse = withSparse | issparse(varargin(i))
        if withSparse, break, end
    end
    r = varargin(1);
    if withSparse then
        r = sparse(r);
    end
    if type(r)==10 then
        padd = emptystr
    else
        padd = zeros
    end
    [m1, n1] = size(r);
    for k = 2:size(varargin)
        ak = varargin(k)
        [mk, nk] = size(ak);
        if mk*nk > 0
            if isss
                r = [r, padd(m1, nk); padd(mk, n1), ak]
            else
                r(m1+mk, n1+nk) = ak($,$)     // array extension
                if mk*nk > 1
                    r(m1+1:m1+mk, n1+1:n1+nk) = ak
                end
            end
            m1 = m1+mk
            n1 = n1+nk
        end
    end
endfunction
