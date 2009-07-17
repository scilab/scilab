// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
