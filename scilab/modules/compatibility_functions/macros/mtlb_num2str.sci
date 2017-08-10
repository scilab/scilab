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

function [str] = mtlb_num2str(x, f)
    // Emulation function for Matlab num2str()

    rhs = argn(2);

    if rhs == 1 then // num2str(x)
        if isempty(x) then
            str = "";
        else
            str = string(x);
        end
    else
        if type(f) == 10 & (size(f, "*") == 1) then
            str = msprintf(f, x);
        else
            error(msprintf(gettext("%s: Not yet implemented.\n"), "mtlb_num2str"));
        end
    end

endfunction


