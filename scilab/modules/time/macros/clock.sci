//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Return the current date and time as serial number
//------------------------------------------------------------------------

function [t, tz, Dst] = clock()
    t = getdate()
    t(9) = t(9) + t(10)/1000
    t([3:5 10]) = []

    // Time zone
    if argn(1) > 1 then
        tz = getdate(0)
        tz = tz(7) + tz(8)/60
        if tz > 12
            tz = tz - 24
        end
    end

    // Daylight saving time
    if argn(1) > 2 then
        tm = getdate()
        ts = getdate("s")
        dt = (datenum(tm([1 2 6 7 8 9])) - datenum([1970 1 1 0 0 0]))*24-ts/3600
        Dst = ceil(int((dt - tz)*100)/100)
    end
endfunction
