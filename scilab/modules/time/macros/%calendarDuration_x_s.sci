// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function out = %calendarDuration_x_s(cd1, coef)
    arguments
        cd1
        coef {mustBeEqualDimsOrScalar(coef, cd1)}
    end

    out_m = (cd1.y*12 + cd1.m) .* coef;
    out_y = int(out_m /12);
    out_m = modulo(out_m, 12);
    out_d = cd1.d .* coef;

    out_t = cd1.t .* coef;
    out = mlist(["calendarDuration", "y", "m" "d", "t", "format"], out_y, out_m, out_d, out_t, cd1.format);
endfunction
