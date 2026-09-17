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

function out = %calendarDuration_string(dura)
    sec1 = 1000;
    min1 = sec1 * 60;
    hour1 = min1 * 60;

    function out = print_val(x, u)
        out = "";
        if x <> 0 then
            out = sprintf(" %d%s", x, u);
        end
    endfunction

    withY = %f;
    if dura.format == [] || dura.format == "ymdt" then
        withY = %t;
        y = dura.y + int(dura.m / 12)
        m = modulo(dura.m, 12)
    else
        out_y = "";
        m = dura.m + dura.y * 12
    end

    d = dura.t.duration;
    hh = int(d / hour1);
    mm = modulo(int(d / min1), 60);
    ss = modulo(int(d / sec1), 60);
    ms = modulo(d, 1000);
    hasTime = dura.t.duration <> 0 | (dura.y == 0 & dura.m == 0 & dura.d == 0);

    out = [];
    for c = 1:size(dura.y, 2)
        hasMS = or(modulo(dura.t(:, c).duration, 1000) <> 0);
        for r = 1:size(dura.y, 1)
            if withY then
                out_y = print_val(y(r, c), "y");
            end

            out_m = print_val(m(r, c), "mo");
            out_d = print_val(dura.d(r, c), "d");

            out_t = "";
            if hasTime(r, c) then

                if hasMS then
                    out_t = sprintf(" %dh %dm %.1fs", hh(r, c), mm(r, c), ss(r, c) + ms(r, c) / 1000);
                else
                    out_t = sprintf(" %dh %dm %ds", hh(r, c), mm(r, c), ss(r, c));
                end
            end

            out(r, c) = sprintf("%s%s%s%s", out_y, out_m, out_d, out_t);
        end
    end
endfunction
