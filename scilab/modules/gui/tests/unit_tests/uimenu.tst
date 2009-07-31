// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- TEST WITH GRAPHIC -->

// Simple example with Checked menu
h = uimenu("parent", gcf(), "label", "parent");
h1 = uimenu("parent", h, "label", "child1", "checked", "off");
h2 = uimenu("parent", h, "label", "child2");

// Checked menu which becomes a parent menu
h = uimenu("parent", gcf(), "label", "parent");
h1 = uimenu("parent", h, "label", "child1");
h2 = uimenu("parent", h, "label", "child2", "checked", "off");
h3 = uimenu("parent", h1, "label", "subchild1", "checked", "off");
h4 = uimenu("parent", h2, "label", "subchild1", "checked", "on");

h = uimenu("label","test");
if h.parent<>gcf() then pause,end
if h.enable<>"on" then pause,end
if h.label<>"test" then pause,end
if h.visible<>"on" then pause,end
if h.callback<>"" then pause,end
if h.callback_type<>0 then pause,end
if h.checked<>"off" then pause,end
if h.tag<>"" then pause,end

f=scf();
h = uimenu("Parent", f, ..
    "Enable","off",..
    "Label", "test", ..
    "Visible","off",..
    "Callback","disp(1)",..
    "Checked", "on", ..
    "Tag", "hello");
if h.parent<>f then pause,end
if h.enable<>"off" then pause,end
if h.label<>"test" then pause,end
if h.visible<>"off" then pause,end
if h.callback<>"disp(1)" then pause,end
if h.callback_type<>0 then pause,end
if h.checked<>"on" then pause,end
if h.tag<>"hello" then pause,end

