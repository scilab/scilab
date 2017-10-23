// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 6911 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6911
//
// <-- Short Description -->
// Numerical HTML entities like "&#123;" were not supported in .sci heading comments
// to be processed with help_from_sci()

path = pathconvert(TMPDIR+"/bug_6911/help",%t,%t);
mkdir(path);
scifile = pathconvert(SCI+"/modules/helptools/tests/nonreg_tests/bug_6911_entities_in_sci_help.sci",%f,%t);
assert_checktrue(execstr("help_from_sci(scifile, path)", "errcatch")==0);
assert_checktrue(execstr("xmltohtml(path)", "errcatch")==0);
