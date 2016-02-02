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

function f=%p_c_r(m,f)
    // f= %p_c_r(m,f) <=> r= [m,f]  m:polynomial mat. r: rational mat.
    //!
    // Copyright INRIA
    f=rlist([m,f("num")],[ones(m),f("den")],f("dt"))
endfunction
