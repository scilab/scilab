// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4029 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4029
//
// <-- Short Description -->
// Windows Vista x64 detected as "Vista" with [a,b]=getos() and not as "Vista x64" by scilab x64
// scilab x86 detect without error "Vista x64"

// <-- INTERACTIVE TEST -->
// install scilab x64 on Vista x64
// 
// [a,b] = getos();
// if b <> 'Vista x64' then pause,end

// install scilab x86 on Vista x64
// 
// [a,b] = getos();
// if b <> 'Vista' then pause,end
