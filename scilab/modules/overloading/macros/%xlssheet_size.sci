// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [m,n]=%xlssheet_size(sheet,opt)
    if argn(2)>1 then
        m=size(sheet.value,opt)
    else
        m=size(sheet.value)
        if argn(1)>1 then
            [m,n]=(m(1),m(2))
        end
    end
endfunction
