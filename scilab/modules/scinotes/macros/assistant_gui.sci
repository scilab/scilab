// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systemes - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function assistant_gui()
    // Open the Scilab Assistant chat window.
    //
    // Calling Sequence
    //   assistant_gui()
    //
    // Description
    //   Opens an AI-powered assistant panel docked in the Scilab desktop.
    //   The assistant can execute Scilab code, inspect variables, search
    //   documentation, and capture figures.

    jimport org.scilab.modules.scinotes.SciNotes;
    ed = jinvoke(SciNotes, "getEditor");
    if ed == [] then
        // No SciNotes open — open one first
        editor();
        ed = jinvoke(SciNotes, "getEditor");
    end
    jinvoke(ed, "toggleAssistant");
endfunction
