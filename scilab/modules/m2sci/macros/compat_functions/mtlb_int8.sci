// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function i = mtlb_int8(x)
    // Emulation function for Matlab int8()

    if type(x)==4 then
        i = int8(x*1)
    else
        if type(x)==1
            if ~isreal(x)
                x = real(x)
            end
            x(isnan(x)) = 0
        end
        i = int8(max(min(round(x),127),-128))
    end
endfunction
