//
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.
//

function demo_gui_navigate(catPath)
    // Ask the demo browser to open a category by name.
    // catPath is a category name, or a string vector for a nested path
    // (e.g. ["Xcos" "Standard demos"]).
    if isempty(get("scilab_demo_fig")) then
        return;
    end
    if isempty(get("demo_browser")) then
        return;
    end

    if size(catPath, "*") > 1 then
        catPath = matrix(catPath, 1, -1);
    end
    set("demo_browser", "data", struct("type", "navigate", "path", catPath));
endfunction
