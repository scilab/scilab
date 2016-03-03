// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2015 - Samuel GOUGEON : http://bugzilla.scilab.org/13810
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [u, hm] = householder(v,w)

    fname = "householder"
    [lhs,rhs] = argn(0)

    // Example = demo
    // --------------
    if rhs==0 then
        disp("householder() example: Reflect an object using the Householder matrix")
        [funs, path] = libraryinfo(whereis("householder"));
        editor(path+"householder.sce")
        exec(path+"householder.sce", -1)
        return
    end

    // CHECKING INPUT PARAMETERS
    // -------------------------
    if rhs<2 then
        w = eye(v)
    end
    if typeof(v(:))~="constant"
        msg = _("%s: Wrong type for argument %d: Decimal or complex numbers expected.\n")
        error(msprintf(msg, fname, 1))
    end
    if typeof(w(:))~="constant"
        msg = _("%s: Wrong type for argument %d: Decimal or complex numbers expected.\n")
        error(msprintf(msg, fname, 2))
    end
    if length(find(size(v)>1))>1 then
        msg = _("%s: Wrong size for input argument #%d: Column vector expected.\n")
        error(msprintf(msg, fname, 1))
    end
    if length(find(size(w)>1))>1 then
        msg = _("%s: Wrong size for input argument #%d: Column vector expected.\n")
        error(msprintf(msg, fname, 2))
    end
    if length(v)~=length(w) then
        msg = _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n")
        error(msprintf(msg, fname, 1, 2))
    end
    v = v(:)
    w = w(:)

    // PROCESSING
    // ----------
    if v==[] | w==[] then
        u = []
        hm = []
        return
    end
    a = sqrt((v'*v) / (w'*w))

    // Are v and w colinear?
    colinear = %t
    kn = find(w==0)
    if kn~=[] then
        colinear = colinear & and(v(kn)==0)
    end
    k = find(w~=0)
    if colinear & k~=[] then
        tmp = v(k)./w(k)
        colinear = colinear & and(tmp==tmp(1))
    end

    // v and w coliear and reals
    if colinear & isreal(v) & isreal(w) then
        if tmp(1)<0 // v and w are opposite
            u = v
        else  // v and w in the same direction
            // => u is in the hyperplane orthogonal to v, and then u'*v==0
            u = zeros(v)
            if kn~=[]
                u(kn(1)) = 1
            else
                if length(u)>1
                    u(1:2) = [1 ; -w(1)/w(2)]
                else
                    u = %nan
                end
            end
        end
    else
        // v and w are not colinear or are complex
        if or(v~=a*w)   // v and w not colinear
            u = v - a*w
        else
            u = v + a*w
        end
    end
    try
        u = u / norm(u)
    end
    if lhs>1 then
        hm = eye() - 2*u*u'
    end
endfunction
