// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7737 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7737
//
// <-- Short Description -->
// Apparently, a module to be installed is first downloaded without priorly
// checking that its archive is not already available in the archives dir. This
// can uselessly takes time, for heavy modules such that SIVP.

// To see all downloaded file, patch SCI/modules/atoms/macros/atoms_internals/atomsDownload.sci
// with the following line:
// mprintf("downloading %s ...\n",url_in);

atomsInstall(["module_lycee" "1.2"]);
atomsRemove(["module_lycee" "1.2"]);
atomsInstall(["module_lycee" "1.2"]);

// => module_lycee_1.2-1.bin.zip should be downloaded only one time
// Don't forget to restore SCI/modules/atoms/macros/atoms_internals/atomsDownload.sci :)
