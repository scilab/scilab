// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
