// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
