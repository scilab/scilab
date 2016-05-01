//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// Return day of week
// Maurice Kraitchik Algorithm
//------------------------------------------------------------------------

function [N,S] = weekday(D,form)

    lhs=argn(1);
    rhs=argn(2);

    // checkings

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"weekday",1,2));
    end

    if type(D) <> 1 then
        error(msprintf(gettext("%s: Wrong type  for input argument #%d: Real constant matrix expected.\n"),"weekday",1));
    end

    if (rhs==2) & (form <> "long") & (form <> "short") then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"weekday",2,"long","short"));
    end

    // some definitions

    common_year  = [0,31,59,90,120,151,181,212,243,273,304,334,365];
    leap_year    = [0,31,60,91,121,152,182,213,244,274,305,335,366];
    week_numbers = [7,1,2,3,4,5,6];

    week_strings_short = [gettext("Sat"), ..
    gettext("Sun"), ..
    gettext("Mon"), ..
    gettext("Tue"), ..
    gettext("Wed"), ..
    gettext("Thu"), ..
    gettext("Fri")];

    week_strings_long = [gettext("Saturday"), ..
    gettext("Sunday")    , ..
    gettext("Monday")    , ..
    gettext("Tuesday")   , ..
    gettext("Wednesday") , ..
    gettext("Thursday")  , ..
    gettext("Friday")    , ..
    gettext("Jan")];


    if rhs==2 then
        if form == "long" then
            week_strings = week_strings_long;
        else
            week_strings = week_strings_short;
        end
    else
        week_strings = week_strings_short;
    end


    // Algorithm

    [nr,nc]  = size(D);

    if nc == 1 then
        common_year = common_year';
        leap_year   = leap_year';
    end

    month_day_mat   = ones(nr,nc);

    m               = ones(nr,nc);
    y               = ones(nr,nc);
    d               = ones(nr,nc);

    Y        = floor(D/365.2425);
    temp     = D - (365.0*Y + ceil(0.25*Y)- ceil(0.01*Y) + ceil(0.0025*Y));

    mask     = find(temp <= 0);
    if ~isempty(mask)
        Y(mask)  = Y(mask) - 1;
        D(mask)  = D(mask) - (365.0*Y(mask) + ceil(0.25*Y(mask)) - ceil(0.01*Y(mask)) + ceil(0.0025*Y(mask)));
        D(~mask) = temp(~mask);
    else
        D = temp;
    end

    M = int(D/29);
    idx_leap_year = isLeapYear(Y);

    if ~isempty(M(idx_leap_year))
        month_day_mat(idx_leap_year)  = leap_year(M(idx_leap_year)+1);
    end
    if ~isempty(M(~idx_leap_year))
        month_day_mat(~idx_leap_year) = common_year(M(~idx_leap_year)+1);
    end

    if ~isempty(M( D>month_day_mat )) then
        M( D>month_day_mat ) = M( D>month_day_mat )+1;
    end

    if ~isempty(M(idx_leap_year))
        month_day_mat(idx_leap_year)  = leap_year(M(idx_leap_year));
    end
    if ~isempty(M(~idx_leap_year))
        month_day_mat(~idx_leap_year) = common_year(M(~idx_leap_year));
    end

    d = D - month_day_mat;

    m(M==1) = 13;
    m(M==2) = 14;

    if ~isempty(Y( M==1 | M==2 )) then
        y( M==1 | M==2 ) = Y( M==1 | M==2 ) - 1;
    end
    
    if ~isempty(M( M<>1 & M<>2 )) then
        m( M<>1 & M<>2 ) = M( M<>1 & M<>2 );
    end
    if ~isempty(Y( M<>1 & M<>2 )) then
        y( M<>1 & M<>2 ) = Y( M<>1 & M<>2 );
    end

    n =  modulo(  (d + floor(2*m) + floor(3*(m+1)/5) + y + floor(y/4) - floor(y/100) + floor(y/400) + 2)  ,  7  );

    N = matrix( week_numbers(n+1) , nr, nc );
    S = matrix( week_strings(n+1) , nr, nc );

endfunction
