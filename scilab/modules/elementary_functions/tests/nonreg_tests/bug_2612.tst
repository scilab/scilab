// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

ref = ["abc","abd";"aa","bxe"];
A=gsort(['abc','abd';'aa','bxe'],'rc');
if ref <> A then pause,end
