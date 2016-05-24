// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %r_d_s(r, m)
    // r = r./m
    if size(m,"*")==0 then
        r = []
    else
       if isfield(r, "dt")
           r3 = r.dt
       else
           r3 = []
       end
       r = rlist(r.num ./ m, r.den.*ones(m), r3)
    end
endfunction
