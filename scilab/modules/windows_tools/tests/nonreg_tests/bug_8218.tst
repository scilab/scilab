// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8218 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8218
//
// <-- Short Description -->
// file association with filename accentued characters did not work
//

// install a binary version with file association enabled

tab_ref = [
"dédé",
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق"];

// Creates files to check if we can open files

tab_ref = tab_ref + '.sce';

cd TMPDIR;
disp('Go to directory with Windows Explorer: ' + TMPDIR);

for i = 1:size(tab_ref, "*")
  mputl('if you see this text it is OK', tab_ref(i));
end

//click on localized filenames
// it must be opened by scilab

