//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function winId = atomsOpenProgressBar(msg, withValue)
    if (getscilabmode() <> "NWNI")
        if (withValue == %t)
            winId = waitbar(_("Atoms:")+" "+msg);
        else
            winId = progressionbar(_("Atoms:")+" "+msg);
        end
    else
        winId = -1
    end
endfunction
