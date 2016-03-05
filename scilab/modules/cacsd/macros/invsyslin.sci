// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function it=invsyslin(t)

    if typeof(t)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"invsyslin",1))
    end

    [p,m]=size(t.D);
    if p <> m then
        warning(msprintf(gettext("%s: Wrong size for input argument #%d: Square system expected.\n"),"invsyslin",1)),
    end
    //
    d=pinv(t.D);
    it=syslin(t.dt,t.A-t.B*d*t.C,t.B*d,-d*t.C,d,t.X0);
endfunction
