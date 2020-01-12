// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = mtlb_rand(a)
    // Emulation function for rand() Matlab function

    // Save current mode for random generator
    randType = rand("info");
    // Switch to uniform distribution
    rand("uniform");

    if and(size(a)==[1 1]) then
        r = rand(a, a)
    else
        tmp = list()
        for k=1:size(a,"*")
            tmp(k) = a(k)
        end
        r = rand(tmp(1:$))
    end

    // Restore rand mode
    rand(randType);

endfunction
