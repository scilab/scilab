// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlDocument("TMPDIR/foo.xml");
doc.root = xmlElement(doc, "root");
a = xmlElement(doc, "a");
b = xmlElement(doc, "b");
ns_scilab = xmlNs(a, "scilab", "http://www.scilab.org");
ns_balics = xmlNs(a, "balics", "http://gro.balics.www");
xmlAddNs(b, ns_scilab, ns_balics);

doc.root.children(1) = a;
doc.root.children(2) = b;

tmp=xmlGetNsByPrefix(doc.root.children(1), "scilab");
assert_checktrue(tmp.prefix=="scilab");
assert_checktrue(tmp.href=="http://www.scilab.org");
tmp=xmlGetNsByPrefix(doc.root.children(2), "scilab");
assert_checktrue(tmp.prefix=="scilab");
assert_checktrue(tmp.href=="http://www.scilab.org");

tmp=xmlGetNsByPrefix(doc.root.children(1), "balics");
assert_checktrue(tmp.prefix=="balics");
assert_checktrue(tmp.href=="http://gro.balics.www");
tmp=xmlGetNsByPrefix(doc.root.children(2), "balics");
assert_checktrue(tmp.prefix=="balics");
assert_checktrue(tmp.href=="http://gro.balics.www");

assert_checktrue(grep(xmlDump(doc),"http://www.scilab.")<>[]);
assert_checktrue(grep(xmlDump(doc),"http://gro.ba")<>[]);

xmlDelete(doc);

doc = xmlReadStr("<root><a xmlns:scilab=""http://www.scilab.org""><b>Hello </b><scilab:c>World</scilab:c></a></root>");
c = doc.root.children(1).children(2);
a=xmlGetNsByHref(c, "http://www.scilab.org");
assert_checkequal(a.prefix,"scilab");
assert_checkequal(a.href,"http://www.scilab.org");
xmlDelete(doc);
