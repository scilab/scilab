// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11965 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11965
//
// <-- Short Description -->
// Export to SVG converted texts in shapes.


driver("SVG")
xinit(TMPDIR+"/plop.svg")
xtitle('this appears as bezier','x','y')
xend();

doc=xmlRead(TMPDIR+"/plop.svg");
xp=xmlXPath(doc, "//*[local-name()=''text'']");
strs=xmlAsText(xp);
xmlDelete(doc);

assert_checktrue(or(strs=="this appears as bezier"));
