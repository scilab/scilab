//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2010 - DIGITEO - Allan CORNET
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

function demo_xcos()

    bResumeSomeVariables = %t;
    // Check if some variables used by demo_gui already exist
    if isdef("demo_gui_update") then
        bResumeSomeVariables = %f;
    end

    // Launch Scilab standard demo GUI
    demo_gui();

    // Get main list
    frame1 = findobj("tag", "listbox_1");

    // Select Xcos
    allitems = frame1.string;
    xcosItem = grep(allitems, gettext("Xcos"));
    frame1.value = xcosItem;

    // Exec callback to display Xcos demos list
    gcbo = frame1;
    execstr(frame1.callback, "errcatch");

    // Resume some variables
    if bResumeSomeVariables then
        demo_gui_update = resume(demo_gui_update);
    end

endfunction

demo_xcos();
clear demo_xcos;
