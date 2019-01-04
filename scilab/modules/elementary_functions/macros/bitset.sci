// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = bitset(x, pos, v)
    fname = "bitset"
    rhs = argn(2)

    // CHECKING AND FORMATTING INPUT ARGUMENTS
    // =======================================
    if rhs < 2 | rhs > 3 then
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, fname, 2, 3))
    end

    // Check x
    // -------
    if x==[]
        y = []
        return
    end
    if  and(type(x)<>[1 8]) | (type(x)==1 & (~isreal(x)|or((x-floor(x))<>0))) | or(x<0)
        msg = gettext("%s: Argument #%d: Non-negative real integers expected.\n")
        error(msprintf(msg, fname, 1))
    end

    // Check pos
    // ---------
    if ~isdef("pos","l") | pos==[] then     // do nothing, keep it as is
        y = x
        return
    end
    if  (type(pos)<>1  & type(pos)<>8) |  ..
        (type(pos)==1  & (or((pos-floor(pos))<>0) | ~isreal(pos))) | or(pos<1)
        msg = gettext("%s: Argument #%d: integers > 0 expected.\n")
        error(msprintf(msg, fname, 2))
    else
        // max value
        icode = inttype(x)
        select modulo(icode,10)
        case 0  then posmax = 1024  // log2(number_properties("huge"))
        case 1 then posmax = 8
        case 2 then posmax = 16
        case 4 then posmax = 32
        case 8 then posmax = 64
        end
        if icode>0 & icode<10      // Signed integers
            posmax = posmax-1      // => sign bit reserved
        end
        if or(pos > posmax)
            msg = gettext("%s: Argument #%d: Integers <= %d expected.\n")
            error(msprintf(msg, fname, 2, posmax))
        end
    end
    // Check consistency between x and pos sizes
    sx = size(x)
    ndx = ndims(x)
    sp = size(pos)
    ndp = ndims(pos)
    sameBits = ndp<ndx | ~and(sx(1:ndx)==sp(1:ndx)) // Same bits for all x components
    if sameBits then
        if ~isscalar(pos) & ~isvector(pos)
            msg = gettext("%s: Arguments #%d and #%d: Incompatible sizes.\n")
            error(msprintf(msg, fname, 1, 2))
        end
        if ~isdef("v","l") | size(v,"*")==1
            pos = unique(pos)
            // we can't do that if an array v is provided, because then
            // its length must match the pos one.
        else
            if length(pos)>posmax
                msg = gettext("%s: Argument #%d is longer than %d. Please check against duplicate values.\n")
                error(msprintf(msg, fname, 2, posmax))
            end
        end
    else // ndp>=ndx & and(sx(1:ndx)==sp(1:ndx))
        pos = matrix(pos,[sx -1])
        // duplicate values along a range can't be removed nor easily canceled.
        // So, no check. Never mind.
    end

    // Check v
    // -------
    // At the end, v must be either a vector with the pos's length (sameBits case)
    // or an array with same sizes as pos (element-wise case)
    if ~isdef("v","l") || v==[] then
        v = ones(pos)
    else
        if  (type(v)<>1  & type(v)<>8) || or(v~=0 & v~=1)
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, fname, 3, "0,1"))
        end
        if prod(size(v)(1:min(ndims(v),ndims(pos)))) == 1 // scalar or hypercolumn cases
            v = v .*. ones(pos)
        elseif prod(size(v)(1:ndims(pos))) ~= length(pos)
            msg = gettext("%s: Arguments #%d and #%d: Incompatible sizes.\n")
            error(msprintf(msg, fname, 2, 3))
        end
    end

    // PROCESSING
    // ==========
    if sameBits then
        nBitLayers = length(pos)
        template = ones(x)
    else
        nBitLayers = size(pos,ndx+1)
        // Extraction mask:
        str = strcat(emptystr(1,ndx)+":,")+"iL"
    end
    Pos = pos
    V = v
    y = x
    //
    for iL = 1:nBitLayers
        if sameBits
            pos = template*Pos(iL)
            v   = template*V(iL)
        else
            execstr("pos = Pos("+str+"); v = V("+str+")")
        end
        vZero = (v == 0)
        vOne  = (v == 1)
        x = y
        if type(x)==8 then
            mask =  2 .^ iconvert((pos-1), inttype(x))
            y(vZero) = x(vZero) & (~mask(vZero))
            y(vOne) = x(vOne) | mask(vOne)
            y = matrix(y,sx)
        else
            b = bitget(x, pos)
            tmp = vOne & b==0
            if or(tmp)
                y(tmp) = y(tmp) + 2 .^(pos(tmp)-1)
            end
            tmp = vZero & b==1
            if or(tmp)
                y(tmp) = y(tmp) - 2 .^(pos(tmp)-1)
            end
        end
    end
endfunction
