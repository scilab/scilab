// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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


