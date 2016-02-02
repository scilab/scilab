//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
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
// Convert to serial date number
//------------------------------------------------------------------------

function n=datenum(varargin)

    lhs=argn(1);
    rhs=argn(2);

    select rhs
    case 0
        Date = getdate();

        n = ymdhmns_to_scalar(Date(1),..
        Date(2),..
        Date(6),..
        Date(7),..
        Date(8),..
        Date(9));

        break

    case 1

        DateIn = varargin(1);

        // Checks

        if type(DateIn)<> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"datenum",1));
        end

        [nr,nc] = size(DateIn);

        if (nc <> 3) & (nc <> 6) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: m*3 matrix or a m*6 matrix expected.\n"),"datenum",1));
        end

        if min(DateIn(:,2))<1 | max(DateIn(:,2))>12 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be between %d and %d.\n"),"datenum",1,gettext("Month"),1,12));
        end

        if min(DateIn(:,3))<1 | max(DateIn(:,3))>31 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be between %d and %d.\n"),"datenum",1,gettext("Day"),1,31));
        end

        if nc == 6 then

            if min(DateIn(:,4))<0 | max(DateIn(:,4))>23 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be between %d and %d.\n"),"datenum",1,gettext("Hour"),0,23));
            end

            if min(DateIn(:,5))<0 | max(DateIn(:,5))>59 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be between %d and %d.\n"),"datenum",1,gettext("Minute"),0,59));
            end

            if min(DateIn(:,6))<0 | max(DateIn(:,6))>=60 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be between %d and %d.\n"),"datenum",1,gettext("Second"),0,59));
            end

        end

        if nc == 3 then
            n = ymdhmns_to_scalar(DateIn(:,1),DateIn(:,2),DateIn(:,3),zeros(nr),zeros(nr),zeros(nr));
        else
            n = ymdhmns_to_scalar(DateIn(:,1),DateIn(:,2),DateIn(:,3),DateIn(:,4),DateIn(:,5),DateIn(:,6));
        end
        break

    case 3

        YearIn  = varargin(1);
        MonthIn = varargin(2);
        DayIn   = varargin(3);

        // checks

        if (type(YearIn)<> 1) | (type(MonthIn)<> 1) | (type(DayIn)<> 1) then
            error(msprintf(gettext("%s: Wrong type for input arguments.\n"),"datenum"));
        end

        if (size(YearIn) <> size(MonthIn)) | (size(YearIn) <> size(DayIn)) then
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"datenum"));
        end

        if min(MonthIn)<1 | max(MonthIn)>12 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",2,1,12));
        end

        if min(DayIn)<1 | max(DayIn)>31 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",3,1,31));
        end

        [nr,nc]  = size(YearIn);
        n        = ymdhmns_to_scalar(YearIn,MonthIn,DayIn,zeros(nr,nc),zeros(nr,nc),zeros(nr,nc));

        break

    case 6

        YearIn  = varargin(1);
        MonthIn = varargin(2);
        DayIn   = varargin(3);
        HourIn  = varargin(4);
        MinIn   = varargin(5);
        SecIn   = varargin(6);

        // checks

        if  (type(YearIn) <> 1) | ..
            (type(MonthIn)<> 1) | ..
            (type(DayIn)  <> 1) | ..
            (type(HourIn) <> 1) | ..
            (type(MinIn)  <> 1) | ..
            (type(SecIn)  <> 1) then
            error(msprintf(gettext("%s: Wrong type for input arguments.\n"),"datenum"));
        end

        if  (size(YearIn) <> size(MonthIn)) | ..
            (size(YearIn) <> size(DayIn))   | ..
            (size(YearIn) <> size(HourIn))  | ..
            (size(YearIn) <> size(MinIn))   | ..
            (size(YearIn) <> size(SecIn))  then
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"datenum"));
        end

        if min(MonthIn)<1 | max(MonthIn)>12 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",2,1,12));
        end

        if min(DayIn)<1 | max(DayIn)>31 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",3,1,31));
        end

        if min(HourIn)<0 | max(HourIn)>23 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",4,0,23));
        end

        if min(MinIn)<0 | max(MinIn)>59 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",5,0,59));
        end

        if min(SecIn)<0 | max(SecIn)>=60 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"datenum",6,0,59));
        end

        n = ymdhmns_to_scalar(YearIn,MonthIn,DayIn,HourIn,MinIn,SecIn);

        break
    else
        error(msprintf(gettext("%s: Wrong number of input argument.\n"),"datenum"));
    end


endfunction


function scalaire=ymdhmns_to_scalar (annee,mois,jour,heure,mn,seconde)

    decimal_part = (seconde*(1/(24*3600)))+(mn*(1/(24*60)))+(heure*(1/24));

    // convert of month and day
    integer_part = jour + floor((mois * 3057 - 3007) / 100);

    // On retranche 1 si le mois est au dela de février
    integer_part = integer_part + ((mois < 3) - 1);

    // On retranche encore 1 si le mois est au dela de février et année non bissextile
    integer_part = integer_part + (((mois < 3)|(isLeapYear(annee))) -1);

    // Convertion des année
    leap_year_case     = annee * 365 + (annee / 4) - floor(annee / 100) + floor(annee / 400);
    not_leap_year_case = annee * 365 + floor(annee/4) + 1 - floor(annee / 100) + floor(annee / 400);

    leap_year_case(~isLeapYear(annee))    = 0;
    not_leap_year_case(isLeapYear(annee)) = 0;

    integer_part       = integer_part + leap_year_case + not_leap_year_case;

    scalaire = integer_part+decimal_part;

endfunction
