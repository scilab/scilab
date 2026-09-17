// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = between(d1, d2, c)
    // between - calendar difference between two datetime
    arguments
        d1 {mustBeA(d1, "datetime")}
        d2 {mustBeA(d2, "datetime"), mustBeEqualDimsOrScalar(d2, d1)}
        c {mustBeA(c, "string"), mustBeScalar} = "default"
    end

    if c <> "default" then
        values = ["years", "months", "days", "time"];
        c = convstr(c);
        if and(c <> values) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "between", 3, sci2exp(values)));
        end
    end

    s1 = size(d1);
    s2 = size(d2);
    ref_size = s1;

    if size(d1, "*") == 1 then
        ref_size = s2;
    end
    
    rows = ref_size(1);
    cols = ref_size(2);

    d1.date = ones(rows, cols) .* d1.date;
    d1.time = ones(rows, cols) .* d1.time;

    d2.date = ones(rows, cols) .* d2.date;
    d2.time = ones(rows, cols) .* d2.time;

    out = %caldiff(d1, d2, c)

endfunction
