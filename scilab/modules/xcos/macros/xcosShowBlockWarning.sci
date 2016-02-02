//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function xcosShowBlockWarning(k, win)
    for i = k'
        if typeof(scs_m.objs(i)) == "Block" ..
            & size(scs_m.objs(i).doc) >= 1
            warnBlockByUID(scs_m.objs(i).doc(1), "ERROR");
        end
    end
endfunction
