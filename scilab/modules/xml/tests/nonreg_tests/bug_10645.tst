// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10645 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10645
//
// <-- Short Description -->
// File encoding could not be given as argument in xmlRead.
//

assert_checkerror("doc=xmlRead(""SCI/modules/xml/tests/nonreg_tests/bug_10645.xml"")",[],999);
doc = xmlRead("SCI/modules/xml/tests/nonreg_tests/bug_10645.xml","iso-8859-1");
assert_checkequal(doc.root.content, "Clément");
xmlDelete(doc);