//-------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [I, Names] = weekday(varargin)

    lhs = argn(1);
    rhs = argn(2);
    long = %f;
    lang = getlanguage();
    fname = "weekday";

    // CHECKING & PRE-PROCESSING PARAMETERS
    // ====================================
    if rhs > 5 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 0, 5))
    end
    if ~isdef("varargin", "l") then
        varargin = list();
    end
    if rhs>0 then
        i = 1;
        v = varargin(1);
        if type(v)==1
            // N, YMD, or Y
            if rhs==1 | type(varargin(2))~=1    // N or YMD
                if ndims(v)==2 & size(v,2)==3 & ..
                    and(v(:,2)>0 & v(:,2)<13 & v(:,3)>0 & v(:,3)<32)
                    // YMD
                    N = datenum(v(:,1), v(:,2), v(:,3))
                else
                    N = v
                end
                varargin(1) = null();
                i = 2;
            else  // Y  (,M, D)
                Y = v
                varargin(1) = null();
                i = 2;
                v = varargin(1)
                if type(v)~=1 | or(v<1) | or(v>12) | or(v~=fix(v))
                    msg = _("%s: Argument #%d: Integers in [1,12] expected.\n")
                    error(msprintf(msg, fname, 2))
                end
                if rhs==2 | type(varargin(2))~=1
                    msg = _("%s: Argument #%d missing: Integers in [1,31] expected.\n")
                    error(msprintf(msg, fname, 3))
                end
                M = v;
                varargin(1) = null()
                i = 3;
                v = varargin(1);
                if or(v<1) | or(v>31) | or(v~=fix(v))
                    msg = _("%s: Argument #%d: Integers in [1,31] expected.\n")
                    error(msprintf(msg, fname, 3))
                end
                D = v;
                varargin(1) = null();
                i = 4;
                if ~and(size(Y)==size(M)) | ~and(size(M)==size(D))
                    msg = _("%s: Arguments #%d to #%d: Same sizes expected.\n")
                    error(msprintf(msg, fname, 1, 3))
                end
                N = datenum(Y, M, D);
            end
        else
            if type(v)~=10
                msg = _("%s: Argument #%d: Numbers or texts expected.\n")
                error(msprintf(msg, fname, i))
            end
            if or(length(v)>5)  // => dates
                s = size(v);
                v = v(:);
                ok = %f;
                // Parsing against
                // DD-Mmm-YYYY | DD/MM/YYYY | YYYY-MM-DD | YYYY-MM-DD HH:MN:SS.ss
                // v = ["2001-12-21"        "2015-07-25 14:57:35" ..
                //      "2007-01-45 hdgtf"  "1875-04-17 23:57:04.145" ];
                tmp = grep(v, "|^\-{0,1}[0-9]{4}\-[0-9]{2}\-[0-9]{2}|","r");
                if and(tmp==(1:size(v,"*")))
                    // YYYY-MM-DD | YYYY-MM-DD HH:MN:SS.ss
                    // Triming the " HH:MN:SS.sss" trail
                    tmp = strcspn(v, " ");
                    v = strncpy(v, tmp);
                    v = csvTextScan(v, "-", [], "double");
                    ok = %t;
                else
                    if tmp~=[]
                        msg = _("%s: Argument #%d: All dates must have the same format.\n")
                        error(msprintf(msg, fname, 1))
                    end
                end
                if ~ok
                    //  Parsing against D/M/YYYY
                    // v = ["1/2/1234", "12/7/4321", "9/09/8754a", "08/04/457 abc"]
                    tmp = grep(v, "=^[0-9]{1,2}\/[0-9]{1,2}\/\-{0,1}[0-9]{1,4}(\s.*|)$=","r");
                    if and(tmp==(1:size(v,"*")))
                        // "D/M/YYYY" | "DD/M/YYY" | "D/MM/YYY" | "DD/MM/YYYY" | "D/M/YY abc"
                        // Triming the " blabla" trail
                        tmp = strcspn(v, " ");
                        v = strncpy(v, tmp);
                        v = csvTextScan(v, "/", [], "double");
                        v(:,[1 3]) = v(:,[3 1]);
                        ok = %t;
                    else
                        if tmp~=[]
                           msg = _("%s: Argument #%d: All dates must have the same format.\n")
                           error(msprintf(msg, fname, 1))
                        end
                    end
                end
                if ~ok
                    //  Parsing against enM/D/YYYY
                    // v = ["en2/1/1234", "en7/24/1958", "en12/09/8754a", "en08/04/457 abc"]
                    tmp = grep(v, "=^en[0-9]{1,2}\/[0-9]{1,2}\/\-{0,1}[0-9]{1,4}(\s.*|)$=i","r");
                    if and(tmp==(1:size(v,"*")))
                        //"enM/D/YYYY" |"enMM/D/YYY" |"enM/DD/YYY" |"enMM/DD/YYYY" |"enM/D/YY abc"
                        // Triming the prefix "en"
                        v = part(v, 3:$);
                        // Triming the " blabla" trail
                        tmp = strcspn(v, " ");
                        v = strncpy(v, tmp);
                        v = csvTextScan(v, "/", [], "double");
                        v(:,1:3) = v(:,[3 1 2]);
                        ok = %t;
                    else
                        if tmp~=[]
                           msg = _("%s: Argument #%d: All dates must have the same format.\n")
                           error(msprintf(msg, fname, 1))
                        end
                    end
                end
                if ~ok
                    //  Parsing against DD-Mmm-YYYY
                    // v = ["2-Dec-847", "07-Jun-2451", "07-JUN-2451", "15-Fav-527"]
                    //         ok            ok             nok            nok
                    tmp = grep(v, "=^[0-9]{1,2}\-(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\-[0-9]{1,4}$=","r");
                    if and(tmp==(1:size(v,"*")))
                        tmp = csvTextScan(v, "-", [], "string");
                        v = zeros(tmp);
                        v(:,[3 1]) = evstr(tmp(:,[1 3]));
                        M = ["Jan" "Feb" "Mar" "Apr" "May" "Jun" "Jul" "Aug" "Sep" "Oct" "Nov" "Dec"]
                        for i = 1:12
                            v(find(tmp(:,2)==M(i)),2) = i;
                        end
                        ok = %t;
                    else
                        if tmp~=[]
                           msg = _("%s: Argument #%d: All dates must have the same format.\n")
                           error(msprintf(msg, fname, 1))
                        end
                    end
                end
                if ~ok
                   msg = _("%s: Argument #%d: Wrong dates format.\n")
                   error(msprintf(msg, fname, 1))
                end
                if ~and(v(:,2)>0 & v(:,2)<13)
                    msg = _("%s: Argument #%d: Month indices expected in [1,12].\n")
                    error(msprintf(msg, fname, 1))
                end
                if ~and(v(:,2)>0 & v(:,2)<32)
                    msg = _("%s: Argument #%d: Day indices expected in [1,31].\n")
                    error(msprintf(msg, fname, 1))
                end
                N = datenum(v(:,1), v(:,2), v(:,3));
                N = matrix(N, s);
                varargin(1) = null();
                i = 2;
            end
        end
    end
    if ~isdef("N","l") then
        N = datenum();
    end

    // Parsing Options, if any:
    // -----------------------
    for o = varargin
        if type(o)~=10 | size(o,"*")>1
            msg = _("%s: Argument #%d: Single text word expected.\n")
            error(msprintf(msg, fname, i))
        end
        if o=="long"
            long = %t
        elseif o=="short"
        elseif length(o)==5
            if grep(o, "|^[a-z]{2}_[A-Z]{2}$|", "r")==[]
                msg = _("%s: Argument #%d: Wrong langage code.\n")
                error(msprintf(msg, fname, i))
            end
            lang = o;
        else
            msg = _("%s: Argument #%d: Unknown option.\n")
            error(msprintf(msg, fname, i))
        end
        i = i+1;

    end

    // PROCESSING
    // ==========
    // Day index
    I = pmodulo(floor(N-657435), 7)+1
    // Day name
    if %f  // only for gettext's parser
       [_("Sunday") _("Monday") _("Tuesday") _("Wednesday") _("Thursday") _("Friday") _("Saturday")];
       [_("Sun") _("Mon") _("Tue") _("Wed") _("Thu") _("Fri") _("Sat")];
    end
    if long then
        dayNames = ..
       ["Sunday" "Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday"]
    else
        dayNames = ["Sun" "Mon" "Tue" "Wed" "Thu" "Fri" "Sat"]
    end
    Names = matrix(dayNames(I), size(N))
        // Translation
    L = getlanguage();
    if lang~="en_US" then
        setlanguage(lang);
        Names = _(Names);
        setlanguage(L);
    end
endfunction
