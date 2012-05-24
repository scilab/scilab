// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4277 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4277
//
// Short description:
// strsubst returns strange results if accented characters and 
// character class are associated in the pattern to match
//
//==============================================================
subject = "subject with the é accentued letter";
if strsubst(subject,'/é/','e','r') <> 'subject with the e accentued letter' then pause,end
if strsubst(subject,'/[é]/','e','r')<> 'subject with the e accentued letter' then pause,end
if strsubst(subject,'/[c]/','e','r') <> 'subjeet with the é accentued letter' then pause,end
//==============================================================
