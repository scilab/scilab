// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
// Copyright (C) 2006 - INRIA - Pierre MARECHAL
// Copyright (C) 2011 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
//
// BIN2dec function
// Given str a binary string, this function returns the decimal number whose the
// binary representation is given by str
//
// -Input :
//    str : a string (or a vector/matrix of strings), containing only characters
//         '1' and '0'
// -Output :
//    y : a scalar/vector/matrix
//
// F.Belahcene

// check the type of input argument

// 2006-06-26 : Modified by Pierre MARECHAL
// Check length of given string ( must be 47 bits or less )
// =============================================================================

function y = bin2dec(str)

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bin2dec", 1));
    end

    if type(str)<>10
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"bin2dec",1));
    end

    // delete all spaces included in the str
    str = strsubst(str," ","");
    y = base2dec(str, 2);

endfunction
