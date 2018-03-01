// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 13039 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13039
//
// <-- Short Description -->
// The "File => Save as in" action opened the file browser in the parent directory of the targeted one.

// * In File => Working directories, check that at least one directory is defined.
//   If not, add one.
edit linspace
// * Try to save linspace.sci in one of the registered directories, with "File => Save as in.."

// CHECKING:
// When the file browser appears, the content of the targeted directory must be displayed,
//  not the content of the parent directory.
