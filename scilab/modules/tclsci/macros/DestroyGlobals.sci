// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function DestroyGlobals()
    global ged_current_figure

    if ~isempty(winsid()) & ~isempty(find(ged_current_figure==winsid())) then
        scf(ged_current_figure)
    end

    // ged is closed
    TCL_UnsetVar("sciGedIsAlive");

    clearglobal ged_current_figure
    clear ged_current_figure

    // disp("PASSE PAR DestroyGlobals Scilab");
endfunction
