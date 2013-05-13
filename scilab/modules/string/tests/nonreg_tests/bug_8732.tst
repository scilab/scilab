// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8732 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8732
//
// <-- Short Description -->
// strchr did not work with a non-ASCII character needle.
//
// =============================================================================
v = "世界您好";
c = "您";
if strchr(v, c) <> "您好" then pause, end
if strrchr(v, c) <> "您好" then pause, end
// =============================================================================
v = "азеаея";
c = "з";
if strchr(v, c) <> "зеаея" then pause, end
if strrchr(v, c) <> "зеаея" then pause, end
// =============================================================================
v = "азеаея";
c = "е";
if strchr(v, c) <> "еаея" then pause, end
if strrchr(v, c) <> "ея" then pause, end
// =============================================================================
v = "ハロー・ワールド";
c = "ド";
if strchr(v, c) <> "ド" then pause, end
if strrchr(v, c) <> "ド" then pause, end
// =============================================================================
v = "תוכנית";
c = "י";
if strchr(v, c) <> "ית" then pause, end
if strrchr(v, c) <> "ית" then pause, end
// =============================================================================
if strchr("This is a sample string with accent é&à", "é") <> "é&à" then pause, end
if strrchr("This is a sample string with accent é&à", "é") <> "é&à" then pause, end
// =============================================================================
