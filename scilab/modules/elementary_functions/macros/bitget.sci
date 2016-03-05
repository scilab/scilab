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

function y = bitget(x,pos)

    // BITGET function
    // Given an unsigned integer x, this function returns an unsigned integer
    // (0 or 1) which is the bit number 'pos' from the representation binary of x
    // if x=uint8(19) and pos=2 then bitget returns the 2th bit of the binary form of 19 ('10011') which is 1
    // -Inputs :
    //  x : an unsigned integer
    // pos : a positive integer between 1 and the bitmax of the x type
    //
    // -Output :
    //  y : an unsigned integer
    //
    // F.Belahcene

    // P. Marechal, 5 Feb 2008
    //   - Add argument check

    // Check input arguments
    // =========================================================================

    // check number input argument

    rhs = argn(2);

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bitget",2));
    end

    // case empty matrix

    if isempty(x)
        if ~isempty(pos) & prod(size(pos))<>1
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"bitget"));
        else
            y=[]
            return
        end
    end

    // check size

    if (size(x,"*")>1) & (size(pos,"*")>1) & (or(size(x)<>size(pos))) then
        error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"bitget"));
    end

    // check type

    if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
        | (type(x)==8  & (inttype(x)<10)) ..
        | (type(x)<>1  & type(x)<>8) then

        error(msprintf(gettext("%s: Wrong input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitget",1));
    end

    if    (type(pos)==1  & (pos-floor(pos)<>0 | pos<0)) ..
        | (type(pos)==8  & (inttype(pos)<10)) ..
        | (type(pos)<>1  & type(pos)<>8) then

        error(msprintf(gettext("%s: Wrong input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitget",2));
    end

    // check pos value

    select inttype(x)
    case 0  then posmax = 52;
    case 11 then posmax = 8;
    case 12 then posmax = 16;
    case 14 then posmax = 32;
    end

    if (pos>posmax) | (pos<1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"bitget",2,1,posmax));
    end

    // Algorithm
    // =========================================================================

    if size(pos,"*") == 1;
        pos  = ones(x)  * pos;
    end

    if size(x,"*") == 1;
        x    = ones(pos) * x;
    end

    if type(x)==8 then

        select inttype(x)

        case 11 then
            mask = uint8(2^(pos-1));
            y    = uint8(1 * ((x & mask) > 0));
            return;

        case 12 then
            mask = uint16(2^(pos-1));
            y    = uint16(1 * ((x & mask) > 0));
            return;

        case 14 then
            mask = uint32(2^(pos-1));
            y    = uint32(1 * ((x & mask) > 0));
            return;
        end

    else

        // type == 1

        a     = 2^32;
        mask  = uint32(zeros(pos));
        ytemp = uint32(zeros(pos));

        if or(pos<=32) then
            mask( pos<=32 )  = uint32( 2^(pos(pos<=32) -1 ));
            ytemp( pos<=32 ) = uint32( x(pos<=32) - double(uint32(x(pos<=32)/a)) * a ); // permet de récupérer les 32 bits de poid faible
        end

        if or(pos>32) then
            mask( pos>32  )     = uint32( 2^(pos(pos>32) -32 -1 ));
            ytemp( pos> 32 )    = uint32( x(pos> 32)/a); // permet de récupérer les 32 bits de poid fort
        end

        y = 1 * ((ytemp & mask) > 0);

    end

endfunction
