// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2017 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function z = bitget(x, pos)
/*
   P. Marechal, 5 Feb 2008
   - Add argument check
   S. Gougeon - 2017:
    - Bug 2694: Positive signed encoded integers were not supported
    - new uint64 and int64 integers were not supported
    - decimal numbers: pos was limited to 52 instead of log2(number_properties("huge")) == 1024
    - decimal numbers x > 2^52 with pos < log2(x)-52 returned 0 instead of %nan
    - Several bits from each component of an array
*/
    fname = "bitget"

    // CHECKING ARGUMENTS
    // ==================
    rhs = argn(2)
    if rhs~=2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n");
        error(msprintf(msg, fname, 2));
    end

    // case empty matrix
    if x==[] | pos==[]
        z = [];
        return
    end

    // check for positive integers
    msg = gettext("%s: Argument #%d: Non-negative integers expected.\n")
    if  (type(x)<>1  & type(x)<>8) || or(x<0) || ..
        (type(x)==1  & (or((x-floor(x))<>0) | or(imag(x)~=0)))
        error(msprintf(msg, fname, 1));
    elseif type(x)==1
        x = real(x);
    end
    if  (type(pos)<>1  & type(pos)<>8) || or(pos<0) || ..
        (type(pos)==1  & (or((pos - floor(pos))<>0) | or(imag(pos)~=0)))
        error(msprintf(msg, fname, 2));
    elseif type(pos)==1
        pos = real(pos);
    end

    // check sizes
    fromEach = length(x)>1 & length(pos)>1 & (or(size(x)<>size(pos)));
    if size(x,"*") == 1
        x = ones(pos) * x;
    end
    if size(pos,"*") == 1
        pos = ones(x) * pos;
    end

    // check pos value
    icode = inttype(x)
    select modulo(icode, 10)
    case 0  then posmax = 1024  // log2(number_properties("huge"))
    case 1 then posmax = 8
    case 2 then posmax = 16
    case 4 then posmax = 32
    case 8 then posmax = 64
    end
    if icode>0 & icode<10 then  // Signed integers
        posmax = posmax - 1     // => sign bit reserved
    end
    if or(pos>posmax) | or(pos<1) then
        msg = _("%s: Wrong value for input argument #%d: Must be between %d and %d.\n")
        error(msprintf(msg, fname, 2, 1, posmax));
    end

    // PROCESSING
    // ==========
    if fromEach then
        masks = 2.^(pos-1);
        [X,B] = ndgrid(x(:), masks);
        z = bool2s(bitand(X,B)==B);
        if type(x)==1
            [X,B] = ndgrid(x(:), pos);
            below_eps = B<=(log2(X)-52);
            z(below_eps) = %nan;
        else
            z = iconvert(z, inttype(x));
        end
    else
        if  type(x)==8
            mask = iconvert(2 .^(pos-1), inttype(x));
            z = iconvert(1 * ((x & mask) > 0),inttype(x));
        else
            tmp = x ./ (2 .^pos);
            z = bool2s((tmp - fix(tmp))>=0.5);
            below_eps = pos<=(log2(x)-52);
            z(below_eps) = %nan;
        end
    end
endfunction

