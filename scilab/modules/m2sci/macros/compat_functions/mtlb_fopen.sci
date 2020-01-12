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

function [fid,mess]=mtlb_fopen(filename,permission)
    // Emulation function for fopen() Matlab function

    [lhs,rhs]=argn()

    [fid,err]=mopen(filename,permission,0)
    if err<0 then
        fid=-1
        mess=msprintf(gettext("%s: Cannot open file ''%s''.\n"),"mtlb_fopen",filename);
    else
        mess=""
    end

endfunction

