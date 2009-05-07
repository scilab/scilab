// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

a = chdir(TMPDIR);
if ~a then pause, end

b = chdir(SCI);
if ~b then pause, end

c = chdir(pathconvert(SCI+"/nowhere"));
if c then pause, end
