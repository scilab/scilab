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

result=xmlDump(doc);
result_ref = ["<?xml version=""1.0""?>";"<root attr=""value"">";"  <a>hello</a>";"  <b id=""123""> world</b>";"</root>"];
assert_checkequal(result, result_ref);

result=xmlDump(doc,%f);
result_ref=["<?xml version=""1.0""?>";"<root attr=""value""><a>hello</a><b id=""123""> world</b></root>"];
assert_checkequal(result, result_ref);
