//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//------------------------------------------------------------------------
function listcal = calendar(varargin)
    lhs=argn(1);
    rhs=argn(2);

    c=[0,0,0];

    select rhs
    case 0
        ct=getdate();
        c=[ct(1),ct(2),1]
        break
    case 2
        Y=varargin(1);
        M=varargin(2);
        if (size(Y) == [1,1]) & (size(M) == [1,1]) then
        else
            error(msprintf(gettext("%s: Wrong type for input arguments: Must be scalars.\n"),"calendar"));
        end
        if (M < 1) | (M > 12) then error(msprintf(gettext("%s: Wrong value for input argument: Must be between %d and %d.\n"),"calendar",1,12));,end
        c=[Y,M,1];
        break
    else
        error(msprintf(gettext("%s: Wrong number of input arguments.\n"),"calendar"));
    end


    mths = [gettext("Jan"); ..
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

    mth = mths(c(:,2),:);
    cal=Calendar(c(1),c(2));
    listcal=list(sprintf("%s %d",mth,c(1)),gettext("   M      Tu     W      Th     F     Sat     Sun"),cal);
endfunction
//------------------------------------------------------------------------
