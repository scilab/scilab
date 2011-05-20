// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 4847 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4847
//
// <-- Short Description -->
// In uigetfile, there was no way to select files with a given pattern

cd TMPDIR
mkdir("TEST")
cd TEST
f=mopen("file10a.txt","w")
mclose(f)
f=mopen("file15a.txt","w")
mclose(f)
f=mopen("file20a.txt","w")
mclose(f)
f=mopen("anotherthing.txt","w")
mclose(f)
uigetfile("file*.txt",TMPDIR + "/TEST")

// only files with pattern file*.txt should be shew.