// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function z = bit_op(x, y, fname)
    // Internal function: factorized code shared by bitor(), bitxor(), bitand()
    // fname: function name: single string among "bitor", "bitxor", and "bitand"

    // Check input arguments
    // =====================
    if ~isdef("fname","l") || ~or(fname==["bitor" "bitxor" "bitand"])
        msg = gettext("%s: unknown bitwise function ""%s"".\n")
        error(msprintf(msg, "bit_op", fname))
    end

    if x==[] | y==[]
        z = []
        return
    end

    if size(x,"*")>1 && size(y, "*")>1 && or(size(x)<>size(y)) then
        msg = gettext("%s: Arguments #%d and #%d: Same sizes expected.\n")
        error(msprintf(msg, fname, 1, 2))
    else
        if size(x,"*")==1
            x = x * ones(y)
        elseif size(y,"*")==1
            y = y * ones(x)
        end
    end

    msg = gettext("%s: Argument #%d: Integers >=0 expected.\n")
    if  (type(x)<>1  & type(x)<>8) || or(x<0) || (type(x)==1  & or((x-floor(x))<>0))
        error(msprintf(msg, fname, 1))
    end
    if  (type(y)<>1  & type(y)<>8) || or(y<0) || (type(y)==1  & or((y-floor(y))<>0))
        error(msprintf(msg, fname, 2))
    end

    // Casting
    // =======
    // If some decimal are provided, both x and y become decimal-encoded
    if type(x)~=type(y)
        if type(x)==1
            y = double(y)
        else
            x = double(x)
        end
    end
    // If only encoded integers are provided, we cast both x and y to the
    // widest provided encoding
    if type(x)==8 then
        //typeof : int8 < uint8 < int16 < uint16 < int32 < uint32 < int64 < uint64
        //inttype:   1      11      2       12       4       14       8       18
        // =>        1  <  1.5  <   2   <  2.5   <   4   <  4.5   <   8   <    8.5 :
        // Let's do  11 => 1.5, 12 => 2.5, 14 => 4.5, 18 => 8.5 :
        ix = inttype(x); if ix>10, ix = modulo(ix,10)+0.5, end
        iy = inttype(y); if iy>10, iy = modulo(iy,10)+0.5, end
        // Then, max() can work:
        i = max(ix, iy)
        if i~=fix(i)
            i = i + 9.5
        end
        x = iconvert(x, i)
        y = iconvert(y, i)
    end

    // Processing
    // ==========
    isDecimal = (type(x)==1)
    if isDecimal
        // We isolate and process only the 52 highest bits.
        // Other ones are below %eps, are not stored, and so are ignored.
        x = real(x)  // The possible imaginary part is ignored
        y = real(y)
        s = size(x)
        x = x(:)
        y = y(:)
        M = max([x y],"c")
        p = zeros(M)
        f = (M~=0)
        if or(f)
            p(f) = fix(log2(M(f)))-52
            p(p<0) = 0
        end
        twoP = 2 .^p
        x = uint64(x./twoP)
        y = uint64(y./twoP)
    end
    // ----------------
    select fname
    case "bitand"
        z = x & y
    case "bitor"
        z = x | y
    case "bitxor"
        z = (x | y) & ~(x & y)
    end
    // ----------------
    if isDecimal
        z = double(z) .* twoP
        z = matrix(z, s)
    end
endfunction
