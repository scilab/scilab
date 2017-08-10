//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5487 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5487
//
// <-- Short Description -->
// Missing space in a macro was causing the first message to be included
// into the documentation

docFile = help_from_sci(SCI+"/modules/helptools/tests/nonreg_tests/bug_5487.sci",TMPDIR);
fd_r = mopen(docFile,'rt');
content = mgetl(fd_r);

wrongString = "<varlistentry><term>a=1+2</term></varlistentry>";
position = grep(content, wrongString);
if position <> [] then pause,end
