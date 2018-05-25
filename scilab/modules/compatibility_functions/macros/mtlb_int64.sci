// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function i = mtlb_int64(x)
    // Emulation function for Matlab int64()

    imin = int64(-%inf)   // -9223372036854775808
    imax = int64(%inf)    //  9223372036854775807
    if type(x)==4 then
        i = int64(x*1)
    else
        if type(x)==1
            if ~isreal(x)
                x = real(x)
            end
            x(isnan(x)) = 0
        end
        x = round(x);
        bmin = x <= imin;
        bmax = x >= imax;
        i = int64(x);
        i(bmin) = imin;
        i(bmax) = imax;
    end
endfunction
