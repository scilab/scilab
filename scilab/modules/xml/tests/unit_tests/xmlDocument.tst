// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlDocument("TMPDIR/foo.xml");
root = xmlElement(doc, "root");
root.attributes.attr = "value";

assert_checkequal(root.line,0);

root.children(1) = "<a>hello</a>";
root.children(2) = xmlElement(doc, "b");
root.children(2).attributes.id = "123";
root.children(2).content = " world";

doc.root = root;
xmlDump(doc);

assert_checkequal(doc.root.children(1).name,"a");
assert_checkequal(doc.root.children(1).content,"hello");
assert_checkequal(doc.root.children(2).name,"b");
assert_checkequal(doc.root.children(2).content," world");
xmlWrite(doc);
xmlDelete(doc);

doc2 = xmlRead("TMPDIR/foo.xml");
assert_checkfalse(execstr("doc2.line","errcatch") == 0);
assert_checkequal(doc2.root.line,2);
assert_checkequal(doc2.root.children(1).line,3);
xmlDelete(doc2);
