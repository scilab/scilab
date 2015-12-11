// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
