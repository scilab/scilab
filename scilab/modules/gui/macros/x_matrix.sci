// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = x_matrix(mes, x)

    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "x_matrix", 2));
    end

    if and(type(x) <> [1 4 8 10]) then
        error(msprintf(_("%s: Argument #%d: Booleans, numbers, or text expected.\n"), "x_matrix", 2));
    end

    // Encoding
    // --------
    select type(x)
        case 1
            isReal = %T
            rx = real(x)
            sr = string(rx)
            sr(rx>=0) = ascii(160) + sr(rx>=0)
            sr = strsubst(justify(sr,"l"), ascii(160), " ")
            if ~isreal(x,0) then
                isReal = %f
                im = imag(x)
                si = string(abs(im)) + "i"
                im = im >= 0
                si(im) = "+ " + si(im)
                si(~im) = "- " + si(~im)
                y = sr + " " + justify(si,"l")
                y = strcat(y, ",  ", "c")
            else
                y = strcat(sr, ", ", "c")
            end
        case 4  // booleans
            y = strcat(string(x), "  ","c")
        case 8  // integer
            y = string(x)
            y = strcat(justify(y, "r"), ", ", "c")
        case 10 // strings
            y = strsubst(x, """", """""")
            y = strsubst(y, "''", "''''")
            y = """" + y + """"
            y = strcat(justify(y, "l"),", ","c")
            if mes <> [] & stripblanks(mes) <> ""
                mes = [mes ; ""]
            end
            mes = [mes ; _("<html>(quotes <b>"" ''</b> inside texts must be doubled)")]
    end

    // Modal prompt
    // ------------
    y = x_dialog(mes, y);

    // Decoding
    // --------
    if y <> [] then
        select type(x)
        case 1
            if ~isReal then
                y = strsubst(y, " ", "")
                fn = tempname()
                csvWrite(y, fn, ",");
                y = csvRead(fn, ",")
            else
                y = evstr(y)
            end
        case 4
            y = strsubst(stripblanks(y), "T", "%T")
            y = strsubst(y, "F", "%F")
            y = "[" + strcat(y, ";") + "]"
            y = evstr(y)
        case 8
            y = iconvert(evstr(y), inttype(x))
        case 10
            y = "[" + strcat(y, "; ") + "]"
            y = evstr(y)
        end
    end
endfunction
