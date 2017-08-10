//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

a = chdir(TMPDIR);
if(a <> %T) then pause, end

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);

b = chdir("dir_азеазея");
if(b <> %T) then pause, end
