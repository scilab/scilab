// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
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

function y = bitcmp(x,n)

    // BITCMP function
    //
    // Given an unsigned integer x, this function returns the unsigned integer
    // which is the integer corresponding to the complementary of the binary
    // form of x
    //
    // If the bits number of the x binary representation is less than the
    // bitmax number (8,16 or 32) then the bits '1' are added to the
    // complementary in order to have bitmax number (8, 16 or 32) for the
    // complementary
    //
    // for example for the type uint8 (bitmax=8), the complementary of '1101' is not '0010' but '11110010'
    // The integer n sets the bits max number
    // -Inputs :
    //  x : an unsigned integer
    //  n : a positive integer between 1 and the bitmax of the x type
    //
    // -Output :
    //  y : an unsigned integer
    //
    // P. Marechal, 5 Feb 2008
    //   - Add argument check

    // check number input argument
    rhs = argn(2);

    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"bitcmp",1));
    elseif (type(x) == 1) & (rhs == 1) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bitcmp",2));
    end

    // check type

    if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
        | (type(x)==8  & (inttype(x)<10)) ..
        | (type(x)<>1  & type(x)<>8) then

        error(msprintf(gettext("%s: Wrong input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitcmp",1));
    end

    if  (rhs == 2) & ( ..
        (type(n)==1  & (n-floor(n)<>0 | x<0)) ..
        | (type(n)==8  & (inttype(n)<10)) ..
        | (type(n)<>1  & type(n)<>8) ..
        | (size(n,"*")<>1) ) then

        error(msprintf(gettext("%s: Wrong input argument #%d: An unsigned integer expected.\n"),"bitcmp",2));
    end

    // check n value

    select inttype(x)
    case 0  then nmax = 52;
    case 11 then nmax = 8;
    case 12 then nmax = 16;
    case 14 then nmax = 32;
    end

    if rhs>1 then

        if (n>nmax) | (n<1) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"bitcmp",2,1,nmax));
        end

    else
        n = nmax;
    end

    // Algorithm
    // =========================================================================

    // empty matrix shortcut

    if isempty(x) then
        y = [];
        return;
    end

    // unit8, uint16 and uint32 shortcut

    if type(x)==8 then
        y = ~x;
        if rhs > 1 then
            select inttype(x)
            case 11 then y = y & uint8(  2^n - 1);
            case 12 then y = y & uint16( 2^n - 1);
            case 14 then y = y & uint32( 2^n - 1);
            end
        end
        return;
    end

    n = ones(x)*n;

    if type(x) == 1 then

        a     = 2^32;

        y_LSB = uint32( x - double(uint32(x/a)) * a ); // LSB Less Significant Bits
        y_MSB = uint32( x/a );                         // MSB Most Significant Bits

        y_LSB = ~y_LSB;
        y_MSB = ~y_MSB;

        if n <= 32 then
            y_LSB = y_LSB & uint32( 2^n - 1);
            y_MSB = uint32(0);
        else
            y_MSB = y_MSB & uint32( 2^(n-32) - 1);
        end

        y = double( a * y_MSB + y_LSB );
    end

endfunction
