// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15282 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15282
//
// <-- Short Description -->
// When the function's name starts with "%", help_from_sci() wrongly used "%"
// in the xml:id and in the XML file name registered in master_help.xml

path = pathconvert(TMPDIR+"/bug_15282/help",%t,%t);
mkdir(path);
scifile = pathconvert(SCI+"/modules/helptools/tests/nonreg_tests/%bug_15282.sci",%f,%t);
assert_checktrue(execstr("help_from_sci(scifile, path)", "errcatch")==0);
assert_checktrue(execstr("xmltohtml(path)", "errcatch")==0);
