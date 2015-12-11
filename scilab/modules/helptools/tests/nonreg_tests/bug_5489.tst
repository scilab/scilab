//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5489 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5489
//
// <-- Short Description -->
// Bad syntax is the export

docFile = help_from_sci(SCI+"/modules/helptools/tests/nonreg_tests/bug_5489.sci",TMPDIR);
fd_r = mopen(docFile,'rt');
content = mgetl(fd_r);

wrongString = "<varlistentry><term>Calixte Denizet</term></varlistentry>";
position = grep(content, wrongString);
// We should get a simplelist
if position <> [] then pause,end
