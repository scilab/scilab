// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

