// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = bitset(x,pos,v)

    // INRIA 2008 - Pierre MARECHAL
    //
    // BITSET function
    // Set bit at specified position

    // Check input arguments
    // =========================================================================

    // check number input argument

    rhs = argn(2);

    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"bitset",2));
    end

    // case empty matrix
    if isempty(x)
        if ~isempty(pos) & prod(size(pos))<>1
            error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitset"));
        else
            y=[]
            return
        end
    end

    // check v value
    if rhs == 3 & or(v <> 0 & v <> 1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: 0 or 1 expected.\n"),"bitset",3));
    elseif rhs == 2 then
        v = ones(x);
    end

    // check size
    if or(size(x) <> size(pos)) | or(size(v) <> size(x)) then
        error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitset"));
    end

    // check type
    if (type(x) == 1  & (or(x-floor(x) <> 0) | or(x < 0))) ..
        | (type(x) == 8  & (inttype(x) < 10)) ..
        | (typeof(x) == "hypermat" & (or(x-floor(x) <> 0) | or(x < 0))) ..
        | (type(x) <> 1 & type(x) <> 8 & typeof(x) <> "hypermat") then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar/matrix/hypermatrix of unsigned integers expected.\n"),"bitset",1));
    end

    if (type(pos) == 1  & (or(pos-floor(pos) <> 0) | or(pos < 0))) ..
        | (type(pos) == 8  & (inttype(pos) < 10)) ..
        | (typeof(pos) == "hypermat" & (or(pos-floor(pos) <> 0) | or(pos < 0))) ..
        | (type(pos) <> 1 & type(pos) <> 8 & typeof(pos) <> "hypermat") then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar/matrix/hypermatrix of unsigned integers expected.\n"),"bitset",2));
    end

    // check pos value
    select inttype(x(1))
    case 0  then posmax = 52;
    case 11 then posmax = 8;
    case 12 then posmax = 16;
    case 14 then posmax = 32;
    end

    if or(pos > posmax) | or(pos < 1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"bitset",2,1,posmax));
    end

    // Algorithm
    // =========================================================================

    if size(pos,"*") == 1;
        pos  = ones(x)*pos;
    end

    if size(x,"*") == 1;
        x    = ones(pos)*x;
    end

    vZero = find(v == 0);
    vOne = find(v == 1);
    sz = size(x);

    if type(x(1)) == 8 then

        select inttype(x(1))
        case 11 then mask = uint8(2^(pos(:)-1));
        case 12 then mask = uint16(2^(pos(:)-1));
        case 14 then mask = uint32(2^(pos(:)-1));
        end

        mask = matrix(mask, sz);
        y(vZero) = x(vZero) & (~mask(vZero));
        y(vOne) = x(vOne) | mask(vOne);
        y = matrix(y, sz);

        return;

    else
        // type == 1
        a     = 2^32;
        mask  = uint32(zeros(pos));

        y_MSB  = uint32(zeros(pos));
        y_LSB  = uint32(zeros(pos));

        y_LSB = uint32( x - double(uint32(x/a)) * a ); // LSB Less Significant Bits
        y_MSB = uint32( x/a );                         // MSB Most Significant Bits
        yMSB  = y_MSB;
        yLSB  = y_LSB;

        if or(pos<=32) then
            mask(pos<=32) = uint32(2^(pos(pos<=32) -1));
            yLSB = y_LSB(pos<=32);
            ymask = mask(pos<=32);
            // v == 0
            yLSB(vZero) = yLSB(vZero) & (~ymask(vZero));
            // v == 1
            yLSB(vOne) = yLSB(vOne) | ymask(vOne);
            yLSB = matrix(yLSB, sz);
        end

        if or(pos>32) then
            mask(pos>32) = uint32(2^(pos(pos>32) -32 -1));
            yMSB = y_MSB(pos>32);
            ymask = mask(pos>32);
            yMSB(vZero) = yMSB(vZero) & (~ymask(vZero));
            yMSB(vOne) = yMSB(vOne) | ymask(vOne);
            // v == 0
            yMSB(vZero) = yMSB(vZero) & (~ymask(vZero));
            // v == 1
            yMSB(vOne) = yMSB(vOne) | ymask(vOne);
            yMSB = matrix(yMSB, sz);
        end
        y = double(a * yMSB + yLSB);
    end

endfunction
