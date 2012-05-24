// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - allan.cornet@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2889 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2889
//

mprintf(' %s\n', "%exxxx");
mprintf(' %s\n', "xx%exx");
mprintf(' %s\n', "xxxx");
mprintf(' %s\n', "%nyyyy");


if sprintf(' %s\n', "%exxxx") <> " %exxxx" then pause,end
if sprintf(' %s\n', "xx%exx") <> " xx%exx" then pause,end
if sprintf(' %s\n', "xxxx") <> " xxxx" then pause,end
if sprintf(' %s\n', "%nyyyy") <> " %nyyyy" then pause,end

                    