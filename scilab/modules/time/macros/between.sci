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

    if d1 == d2 then
        out = caldays(zeros(rows, cols));
        return;
    end

    if c == "days" then
        D = floor(days(d2 - d1));
        out = caldays(D);
        return
    end

    if c == "time" then
        dt = d2 - d1;
        out = calendarDuration(0, 0, 0, dt);
        return
    end

    nonNaNvalues = find(~(isnat(d1) | isnat(d2)));
    out = calendarDuration(%nan) * zeros(rows, cols);
    if isempty(nonNaNvalues) then
        return
    end

    newd1 = d1(nonNaNvalues);
    newd2 = d2(nonNaNvalues);

    sup = newd2 > newd1;
    idx = find(sup);
    idx2 = find(~sup);

    // year
    Y = newd2.Year - newd1.Year;
    tmp = newd1 + calyears(Y);

    mask = tmp(idx) > newd2(idx);
    if or(mask) then
        i = idx(mask);
        Y(i) = Y(i) - 1;
        tmp(i) = newd1(i) + calyears(Y(i));
    end

    mask = tmp(idx2) < newd2(idx2);
    if or(mask) then
        i = idx2(mask);
        Y(i) = Y(i) + 1;
        tmp(i) = newd1(i) + calyears(Y(i));
    end

    if c == "years" then
        out(nonNaNvalues) = calyears(Y);
        return;
    end

    // month    
    M = (newd2.Year - tmp.Year) * 12 + (newd2.Month - tmp.Month);
    candidate = tmp + calmonths(M);

    // Must never exceed d2
    mask = candidate(idx) > newd2(idx);
    if or(mask) then
        i = idx(mask);
        M(i) = M(i) - 1;
        candidate(i) = tmp(i) + calmonths(M(i));
    end

    mask = candidate(idx2) < newd2(idx2);
    if or(mask) then
        i = idx2(mask);
        M(i) = M(i) + 1;
        candidate(i) = tmp(i) + calmonths(M(i));
    end

    if c == "months" then
        out(nonNaNvalues) = calmonths(Y * 12 + M);
        out.format = "mdt";
        return;
    end

    tmp = candidate;

    // day
    D = floor(days(newd2 - tmp));
    D(idx2) = -D(idx2);

    // time
    dt = newd2 - (tmp + caldays(D));
    dt(idx2) = -1 * dt(idx2);

    out(nonNaNvalues) = calendarDuration(Y, M, D, dt);
endfunction
