//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//------------------------------------------------------------------------

function varargout = calendar(varargin)

    c = [0,0,0];

    select argn(2)
    case 0
        ct = getdate();
        c = [ct(1),ct(2),1]
        break
    case 2
        Y = varargin(1);
        M = varargin(2);
        msg = gettext("%s: Argument #%d: Scalar (1 element) expected.\n");
        if length(Y)>1
            error(msprintf(msg,"calendar", 1));
        end
        if length(M)>1
            error(msprintf(msg,"calendar", 2));
        end
        if (M < 1) | (M > 12) then
            msg = gettext("%s: Argument #%d: Must be in the interval [%d, %d].\n")
            error(msprintf(msg, "calendar", 2, 1, 12));
        end
        c = [Y, M, 1];
        break
    else
        msg = gettext("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "calendar", 0, 2));
    end


    months = [gettext("Jan"); ..
    gettext("Feb"); ..
    gettext("Mar"); ..
    gettext("Apr"); ..
    gettext("May"); ..
    gettext("Jun"); ..
    gettext("Jul"); ..
    gettext("Aug"); ..
    gettext("Sep"); ..
    gettext("Oct"); ..
    gettext("Nov"); ..
    gettext("Dec")];

    month = months(c(:,2), :);
    cal = Calendar(c(1), c(2));
    dayNames = gettext("Mon  Tue  Wed  Thu  Fri  Sat  Sun")
    //!\\ Glyphs for ja, zh, .. are not monospaced, even in the Monospaced font
    // .po translations have been tuned and tested for alignments with Monospaced 12.
    Title = sprintf("%s %d", month, c(1))
    if ~argn(1) then
        k = vectorfind(cal, zeros(1,7), "r")
        cal(k,:) = []
        t = matrix(msprintf("%d\n",cal(:)), -1, 7)
        t(t=="0") = ""
        t = strcat(justify(t, "r"), "   ", "c");
        Title = blanks((length(t(2))-length(Title))/2) + Title
        t = strsubst(["" ; Title ; dayNames ; t], " ", ascii(160))  // non-breakable spaces
        mprintf(" %s\n", t)
    else
        varargout = list(list(Title, dayNames, cal));
    end
endfunction
