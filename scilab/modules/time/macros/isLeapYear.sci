// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Return True if year is a leap year, otherwise False

// Si l'année est divisible par 4 et non par 100 => Année bissextile
// Si l'année est divisible par 400 => Année bissextile
// =============================================================================

function Rep=isLeapYear(year)

    lhs=argn(1);
    rhs=argn(2);

    if rhs==1 & type(year)==1 then
        Rep = ((modulo(year,100)<>0) & (modulo(year,4)==0)) | (modulo(year,400)== 0);
    else
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"isLeapYear",1));
    end

endfunction
