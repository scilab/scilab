//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1985 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1985
//
// <-- Short Description -->

//    Scilab returns incorrect numerical result from simple computation.  This occurs
//    on an installation built from source, but does not occur on the Linux binary
//    distribution installed on he same machine nor in the Windows version.  No sign
//    noted during the build process of an error.  Reproduced on 2 machines, both
//    Fedora Core 5, one Pentium 4 with smp kernel, one AMD.

if  .18257419^2 + .36514837^2 + .54772256^2 + .73029674^2 - 1 > %eps then pause, end
