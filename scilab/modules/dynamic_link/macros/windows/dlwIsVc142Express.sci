// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function bOK = dlwIsVc142Express()
    bOK = %f;
    vers = getVsWhereInformation();
    if isempty(vers) then
        return;
    end

    x = fieldnames(vers);
    x = find(x == "msvc142express");
    if isempty(x) then
        return;
    end

    bOK = %t;
endfunction
//=============================================================================
