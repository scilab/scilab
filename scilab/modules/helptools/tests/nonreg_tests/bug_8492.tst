// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 8492 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8492
//
// <-- Short Description -->
// When the documentation was generated, error messages were not displayed in the console.

tmpfile = TMPDIR + "/macroscreen.sci";
mputl(["function mymacro()"
       "// Performs stuff."
       "//"
       "// Description"
       "// The following is a stuff:"
       "//"
       "//  <unknownTag>"
       "//  foo"
       "//  </unknownTag>"
       "//"
       "endfunction"], tmpfile);
cd(TMPDIR);
[helptxt,demotxt]=help_from_sci("macroscreen");
testdir = fullfile(TMPDIR,"screendir");
mkdir(testdir);
xmlfile = fullfile(testdir,"macroscreen.xml");
fd = mopen(xmlfile,"w");
mputl(helptxt,fd);
mclose(fd);
tbx_build_help("ScreenTest",testdir);

// An error message like "The tag screen is not handled." must be displayed in the console