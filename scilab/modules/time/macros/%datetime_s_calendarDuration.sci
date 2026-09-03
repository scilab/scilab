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

function out = %datetime_s_calendarDuration(dt, cd1)
    arguments
        dt
        cd1 {mustBeEqualDimsOrScalar(cd1, dt)}
    end

    ref_size = [1 1];
    if size(dt, "*") <> [0 1] then
        ref_size = size(dt);
    else
        ref_size = size(cd1);
    end

    //adjust dimensions
    scale = ones(ref_size(1), ref_size(2));
    cd1.y = cd1.y .* scale;
    cd1.m = cd1.m .* scale;
    cd1.d = cd1.d .* scale;
    cd1.t.duration = cd1.t.duration .* scale;

    dt.date = dt.date .* scale;
    dt.time = dt.time .* scale;

    out_y = dt.Year - cd1.y;
    out_m = dt.Month - cd1.m;

    idx = find(out_m > 12);
    while and(idx <> [])
        out_y(idx) = out_y(idx) + 1;
        out_m(idx) = out_m(idx) - 12;
        idx = find(out_m > 12);
    end

    idx = find(out_m < 1);
    while and(idx <> [])
        out_y(idx) = out_y(idx) - 1;
        out_m(idx) = 12 + out_m(idx);
        idx = find(out_m < 0);
    end

    out_d = min(dt.Day, eomday(out_y, out_m)) - cd1.d;
    out_t = dt.time*1000 - cd1.t.duration;

    d = datenum(out_y, out_m, out_d);
    out = mlist(["datetime", "date", "time", "format"], d, out_t / 1000, dt.format);

endfunction
