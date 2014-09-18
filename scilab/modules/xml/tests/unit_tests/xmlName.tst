// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlReadStr("<root><foo a=""123"" b=""456"" c=""789""/></root>")

assert_checkequal(xmlName(doc.root.children(1)),"foo");
assert_checkequal(xmlName(doc.root.children(1).attributes),["a" "b" "c"]);
assert_checkequal(xmlName(xmlXPath(doc,"//foo/@*")),["a" "b" "c"]);
assert_checkequal(xmlName(xmlXPath(doc,"//foo")),"foo");

xmlDelete(doc);