// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function i = mtlb_uint64(x)
    // Emulation function for Matlab int64()

    imax = uint64(%inf)    //  18446744073709551615
    if type(x)==4 then
        i = uint64(x*1)
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
        i = uint64(x);
        i(bmin) = imin;
        i(bmax) = imax;
    end
endfunction
