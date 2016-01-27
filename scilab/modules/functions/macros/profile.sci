// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2013 - Samuel GOUGEON : bug #13146 fixed
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function count = profile(fun)
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "profile", 1));
    end
    if type(fun)==13 then
        lst = macr2lst(fun)
    else
        lst = fun
    end
    i = 5
    if lst(4)(1)=="31"
        i = 6
    end
    if lst(i)(1)<>"25" then
        error(msprintf(gettext("%s: The function has not been built for profiling"), "profile"))
    end
    count = get_profile(lst,4)
    count = count(1:$-1,:)
    count(:,2) = count(:,2)/1000000
endfunction
