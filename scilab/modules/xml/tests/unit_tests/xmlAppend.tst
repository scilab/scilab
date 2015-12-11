// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlDocument();
root = xmlElement(doc, "root");
doc.root = root;
for i=1:5
xmlAppend(doc.root, xmlElement(doc, "child_" + string(i)));
end;

assert_checkequal(doc.root.children(1).name,"child_1");
assert_checkequal(doc.root.children(2).name,"child_2");
assert_checkequal(doc.root.children(3).name,"child_3");
assert_checkequal(doc.root.children(4).name,"child_4");
assert_checkequal(doc.root.children(5).name,"child_5");

xmlDelete(doc);
