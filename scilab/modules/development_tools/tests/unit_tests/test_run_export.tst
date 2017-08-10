// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- CLI SHELL MODE -->
//exec modules/development_tools/macros/test_run.sci;

// Checks that this file is launched in nwni mode.
//test_run("development_tools",["checkequal","checktrue","checkfalse"],["show_diff","show_error"],"/tmp/results.xml");

targetXML=TMPDIR+"/plop.xml";
test_run(["boolean","xml"],[],["show_diff","show_error"],targetXML);

assert_checktrue(isfile(targetXML));
xmlElem=xmlRead(targetXML);
assert_checktrue(xmlElem.root.children.size == 2);

assert_checkequal(xmlElem.root.children(1).name, "testsuite");
assert_checktrue(evstr(xmlElem.root.children(1).attributes.tests) > 0);
assert_checkequal(xmlElem.root.children(1).children.size, evstr(xmlElem.root.children(1).attributes.tests));

assert_checkequal(xmlElem.root.children(1).children(1).name, "testcase");
assert_checktrue(length(xmlElem.root.children(1).children(1).attributes.name) > 0);

assert_checkequal(xmlElem.root.children(2).name, "testsuite");
assert_checktrue(evstr(xmlElem.root.children(2).attributes.tests) > 0);
assert_checkequal(xmlElem.root.children(2).children.size, evstr(xmlElem.root.children(2).attributes.tests));

assert_checkequal(xmlElem.root.children(2).children(1).name, "testcase");
assert_checktrue(length(xmlElem.root.children(2).children(1).attributes.name) > 0);
listModule = xmlXPath(xmlElem,"//testsuites/testsuite");
assert_checkequal(listModule.size, 2);

test_run(["time"],["datenum"],["show_diff","show_error"], targetXML);
doc=xmlRead(targetXML);
listModule = xmlXPath(doc,"//testsuites/testsuite");
// Check that the append worked
assert_checkequal(listModule.size, 3);

xmlDelete(doc);
