// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- INTERACTIVE TEST -->

uigetcolor()
uigetcolor("My title")
uigetcolor([200 200 200])
uigetcolor("My title", [200 200 200]);
uigetcolor(200, 200, 200);
uigetcolor("My title", 200, 200, 200);

[R, G, B] = uigetcolor()
[R, G, B] = uigetcolor("My title")
[R, G, B] = uigetcolor([200 200 200])
[R, G, B] = uigetcolor("My title", [200 200 200]);
[R, G, B] = uigetcolor(200, 200, 200);
[R, G, B] = uigetcolor("My title", 200, 200, 200);
