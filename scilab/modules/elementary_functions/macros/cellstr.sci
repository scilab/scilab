// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function   c=cellstr(s)

    // This function converts a string array (matrix or a vector) into a cell of strings.
    // Input : s, vector,matrix of strings
    // Output : c, cell of strings components
    // size(c,1)=size(s,1) and size(c,2)=1
    // F.B

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"cellstr",1));
    end

    // The input argument size must be less or equal to 2
    if size(size(s),"*")>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Must be <= %d.\n"),"cellstr",1,2));
    end

    // The input argument must be a string
    if typeof(s)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of string expected.\n"),"cellstr",1));
    end

    c=cell(size(s,1),1);

    for i=1:size(s,1)
        c{i,1} = strcat(s(i,:));
    end

endfunction
