//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function demo_xcos()

    bResumeSomeVariables = %t;
    // check if some variables used by demo_gui already exist
    if isdef("demo_gui_update") & isdef("resize_gui") then
        bResumeSomeVariables = %f;
    end

    // Launch Scilab standard demo GUI
    demo_gui();

    // Get main list
    frame1 = findobj("tag", "listbox_1");

    // Select Xcos
    allitems = frame1.string;
    xcosItem = find(allitems == gettext("Xcos"));
    frame1.value = xcosItem;

    // Exec callback to display Xcos demos list
    gcbo = frame1;
    execstr(frame1.callback, "errcatch");

    // resume some variables
    if bResumeSomeVariables then
        [demo_gui_update, resize_gui] = resume(demo_gui_update, resize_gui);
    end

endfunction

demo_xcos();
clear demo_xcos;
