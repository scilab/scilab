// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function f = %s_d_r(n1, f2)
    // n1 ./ f2
    if size(n1,"*")==0 then
        f = []
    else
       if isfield(f2, "dt")
           r3 = f2.dt
       else
           r3 = []
       end
        f = rlist(n1.*f2("den"), ones(n1).*f2("num"), r3)
    end
endfunction
