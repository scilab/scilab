// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=set_infos(t,level)

    // Add warning and error messages obtained while translating to display it in the end of file translation
    global("m2sci_infos")

    t(1)="L."+string(nblines)+": "+t(1)

    select level
    case 0 then
    case 1 then
        m2sci_infos(level)=%t
        t="! "+t
    case 2 then
        m2sci_infos(level)=%t
        t="!! "+t
    end
    m2sci_info(t,verbose_mode);
endfunction
