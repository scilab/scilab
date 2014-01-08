// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13174 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13174
//
// <-- Short Description -->
// Crash after element removal.
//

doc = xmlReadStr("<root><a>Hello</a><b>Scilab</b><a>World</a></root>");
xmlRemove(doc.root.children(1));
assert_checkequal("Scilab", doc.root.children(1).content);
assert_checkequal("World", doc.root.children(2).content);
xmlDelete(doc)

doc = xmlReadStr("<root><a>Hello</a><b>Scilab</b><a>World</a></root>");
xmlRemove(doc.root.children(2));
assert_checkequal("Hello", doc.root.children(1).content);
assert_checkequal("World", doc.root.children(2).content);
xmlDelete(doc)

