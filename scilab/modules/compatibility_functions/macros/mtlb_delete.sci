// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=mtlb_delete(y)
    // Emulation function for Matlab delete()

    if type(y)==10 then
        mdelete(y);
    elseif type(y)==9 then
        delete(y);
    else
        error(msprintf(gettext("%s: This feature has not been implemented.\n"),"mtlb_delete"));
    end

endfunction


