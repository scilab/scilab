// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function t=etime(t1,t0)

    // returns the time in seconds that has elapsed between vectors t1 and t0
    // t1 and t0 must be six elements long, in the format:
    //       t = [Year Month Day Hour Minute Second.Milliseconds]
    // or, 10 elements long, in the format returned by getdate()
    //       t = [Year Month ISO_8601_week_number Julian_day Week_Day Day Hour Minute Second Millisecond]
    // in this case ISO_8601_week_number Julian_day Week_Day are ignored
    // t1 and t0 can also be matrices having each line equal to a vector described above


    if type(t1)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"),"etime",1));
    end
    if type(t0)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"),"etime",2));
    end
    if and(size(t1,2)<>[6,10]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Must be between %d and %d.\n"),"etime",1,6,10));
    end
    if and(size(t0,2)<>[6,10]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Must be between %d and %d.\n"),"etime",2,6,10));
    end

    if size(t1,"*")<>size(t0,"*") then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"),"etime"), 1, 2);
    end

    if size(t1,2)==10 then
        t1(:,3:5)=[]
    end
    if size(t0,2)==10 then
        t0(:,3:5)=[]
    end

    // Cumulative days per month in a nonleap year
    cdm = [0,31,59,90,120,151,181,212,243,273,304,334]

    t=0

    for k=1:2
        if k==1 then
            tcur=t1
        else
            tcur=t0
        end

        Year=tcur(:,1)
        Month=tcur(:,2)
        Day=tcur(:,3)

        // Make sure month is in the range 1 to 12
        Month(Month<1)=1
        for kM=1:size(Month,"*")
            if Month(kM) > 12 then
                Year(kM) = Year(kM) + (Month(kM)-1)/12;
                Month(kM) = fix((Month(kM)-1) / 12) + 1;
            end
            CDM=cdm(Month(kM))
        end

        tsec = 365.*Year + ceil(Year/4) - ceil(Year/100) + ceil(Year/400) + CDM + Day;

        for kM=1:size(Month,"*")
            if Month > 2 then
                iy=fix(Year)
                if fix(iy/4).*4 == iy & fix(iy/100).*100 <> iy | fix(iy/400).*400 == iy then
                    tsec = tsec+1;
                end
            end
        end
        if k==1 then
            t=tsec
        else
            t=t-tsec
        end
    end
    t=86400*t+t1(:,4)*3600-t0(:,4)*3600+t1(:,5)*60-t0(:,5)*60+t1(:,6)-t0(:,6)
    if size(t1,2)==7 then
        t=t+(t1(:,7)-t0(:,7))*1e-3
    end
endfunction
