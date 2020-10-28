// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function r = %printf_boolean(Format, varargin)
    r = varargin
    Format = strsubst(Format, "%%", "")
    [s, e, Fields] = regexp(Format, "/%.*?[diuoxXfeEgGcs]/")
    if grep(Fields, "$") <> [] then
        // numbered formats: reordering fields in order to know
        // the expected output format for each input argument.
        for f = Fields'
            [s,e,m,n] = regexp(f, "/%([0-9])\$/")
            tmp(evstr(n)) = f
        end
        Fields = tmp
    end
    for i = 1:length(varargin)
        if type(r(i)) == 4   // boolean
            if or(part(Fields(i),$)==["s" "c"])
                r(i) = string(r(i))
            else
                r(i) = bool2s(r(i))
            end
        end
    end
endfunction
