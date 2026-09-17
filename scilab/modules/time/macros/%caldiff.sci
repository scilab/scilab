// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = %caldiff(d1, d2, c)
    // internal function used in caldiff and between
    [rows, cols] = size(d1);
    
    if d1 == d2 then
        out = caldays(zeros(rows, cols));
        return;
    end

    if c == "days" then
        d = (d2.date - d1.date) * 86400;
        t = (d2.time - d1.time);
        D = floor((d+t)/86400);
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
