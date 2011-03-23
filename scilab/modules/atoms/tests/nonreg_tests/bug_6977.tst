// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6977 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6977
//
// <-- Short Description -->
// In the presentation frame: Make http entries hyperlinked (with target="_blank").
// Presently, even a simple selection to copy/paste displayed URLs into the web
// browser is impossible.

atomsGui();

// Select a module which description conntains an URL or an email.
// The text should be hyperlinked and the default browser (or email client) should
// be opened
