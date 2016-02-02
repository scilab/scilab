// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function t=cat_code(a,b)

    // Catenate two parts of code (called by tree2code)
    // Input:
    // - a: original code
    // - b: code to add to a
    // Output:
    // - t: catenation of a and b

    if a==[] then
        t=b
    elseif b==[] then
        t=a
    elseif stripblanks(b)=="" then // b=="" then add a new line
        t=[a;""];
    else
        t=[a(1:$-1);a($)+b(1);b(2:$)]
    end
endfunction
