//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

//load("SCI/modules/demo_tools/macros/lib");

// Launch Scilab standard demo GUI
demo_gui();

// Get main list
frame1 = findobj("tag", "listbox_1");

// Select Xcos
allitems = frame1.string;
xcosItem = find(allitems==gettext("Scicos"));
frame1.value = xcosItem;

// Exec callback to display Xcos demos list
gcbo = frame1;
execstr(frame1.callback, "errcatch");

clear frame1 allitems xcosItem gcbo
