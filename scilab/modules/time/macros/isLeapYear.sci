// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
