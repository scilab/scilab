// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

    C=cell(size(s,1),1);

    for i=1:size(s,1)
        c(i,1).entries=strcat(s(i,:));
    end

endfunction
