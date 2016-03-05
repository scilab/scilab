// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=date()
    w=getdate();
    month=[gettext("Jan"), ..
    gettext("Feb"), ..
    gettext("Mar"), ..
    gettext("Apr"), ..
    gettext("May"), ..
    gettext("Jun"), ..
    gettext("Jul"), ..
    gettext("Aug"), ..
    gettext("Sep"), ..
    gettext("Oct"), ..
    gettext("Nov"), ..
    gettext("Dec")];
    s = msprintf("%02d-%s-%04d",w(6),month(w(2)),w(1));
endfunction
