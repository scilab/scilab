// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 657 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=657
//
// <-- Short Description -->
//    Strange result given with a combined command (max and int8 )
//    just after a wrong isdir instruction
//
//    Note that max(int8(10),-2.6) gives wrong result

test1 = max(int8(10),2);
err   = execstr('isdir(test1)','errcatch');
test2 = max(int8(10),2);
pwd();
test3 = max(int8(10),2);

if test1 <> 10  then pause,end
if err   == 0   then pause,end
if test2 <> 10  then pause,end
if test3 <> 10  then pause,end
