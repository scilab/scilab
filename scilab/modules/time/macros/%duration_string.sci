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

function out = %duration_string(dura)
    sec1 = 1000;
    min1 = sec1 * 60;
    hour1 = min1 * 60;
    day1 = hour1 * 24;
    year1 = day1 * 365.2425;

    out = [];
    if dura.format <> [] then //user format
        form = dura.format;
        if length(form) == 1 then //special values to count in "units"
            select form
            case "y"
                val = dura.duration / year1;
            case "d"
                val = dura.duration / day1;
            case "h"
                val = dura.duration / hour1;
            case "m"
                val = dura.duration / min1;
            case "s"
                val = dura.duration / sec1;
            else
                error(msprintf(gettext("%s: Wrong format: Options {%s, %s, %s, %s, %s} expected.\n"), "%duration_string", "y", "d", "h", "m", "s"));
            end

            for c = 1:size(dura.duration, 2)
                for r = 1:size(dura.duration, 1)
                    out(r, c) = sprintf("%0.3f %s", val(r, c), form);
                end
            end
        else
            hasMS = %f;
            nbSstr = "%s.%03d";
            nbS = 1;
            if grep(form, "/\.S+$/", "r") then
                hasMS = %t;
                nbS = length(strindex(form, "S"));
                nbSstr = "%s.%0" + string(nbS) + "d";
                nbS = 10^(nbS-3);
            end

            form = strsubst(form, "/\.S+$/", "", "r");
            d = dura.duration;
            select form
            case "dd:hh:mm:ss"
                dd = int(d / day1);
                hh = modulo(int(d / hour1), 24);
                mm = modulo(int(d / min1), 60);
                ss = modulo(int(d / sec1), 60);
            case "hh:mm:ss"
                hh = int(d / hour1);
                mm = modulo(int(d / min1), 60);
                ss = modulo(int(d / sec1), 60);
            case "mm:ss"
                mm = int(d / min1);
                ss = modulo(int(d / sec1), 60);
            case "hh:mm"
                hh = int(d / hour1);
                mm = modulo(int(d / min1), 60);
            end
            
            for c = 1:size(d, 2)
                for r = 1:size(d, 1)
                    select form
                    case "dd:hh:mm:ss"
                        out(r, c) = sprintf("%02d:%02d:%02d:%02d", dd(r, c), hh(r, c), mm(r, c), ss(r, c));
                    case "hh:mm:ss"
                        out(r, c) = sprintf("%02d:%02d:%02d", hh(r, c), mm(r, c), ss(r, c));
                    case "mm:ss"
                        out(r, c) = sprintf("%02d:%02d", mm(r, c), ss(r, c));
                    case "hh:mm"
                        out(r, c) = sprintf("%02d:%02d", hh(r, c), mm(r, c));
                    end

                    if hasMS && form <> "hh:mm" then
                        out(r, c) = sprintf(nbSstr, out(r, c), modulo(d(r, c), 1000) * nbS);
                    end
                end
            end
        end
    else
        d = dura.duration;
        h = int(d / hour1);
        m = int((d - h * hour1) / min1);
        s = int((d - h * hour1 - m * min1) / sec1);
        S = modulo(d, 1000);

        for c = 1:size(d, 2)
            hasMS = or(modulo(d(:, c), 1000));
    
            for r = 1:size(d, 1)
                out(r, c) = sprintf("%02d:%02d:%02d", h(r, c), m(r, c) , s(r, c));
    
                if hasMS then
                    if S(r, c) then
                        out(r, c) = sprintf("%s.%03d", out(r, c), S(r, c));
                    else
                        out(r, c) = sprintf("%s    ", out(r, c), S(r, c));
                    end
                end
            end
        end
    end
endfunction
