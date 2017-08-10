// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %fun=createfun(t)
    //given t a string vector where first line gives the function calling
    //sequence and the following the function instructions, it returns et
    //Scilab function.
    // This is  mainly used by sci2exp
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "createfun", 1));
    end
    h=t(1)
    k=strindex(h,"=")
    if k==[] then k1=1,else k1=k(1),end
    k=strindex(h,"(")
    if k==[] then k2=length(h),else k2=k(1),end
    h=part(h,1:k1)+"%fun"+part(h,k2:length(h))
    deff(h,t(2:$)),
endfunction
