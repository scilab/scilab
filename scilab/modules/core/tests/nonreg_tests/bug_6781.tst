// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6781 -->
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6781
//
// <-- Short Description -->
// Variables defined in a library are not recognized in compiled macros

//create a library of variables
Dir = TMPDIR + "/bug_6781/";
mkdir(Dir);
a = 1;
b = 2;
warning("off");
save(Dir + "a.bin", a);
save(Dir + "b.bin", b);
warning("on");
mputl(["a";"b"], Dir + "names");
testlib = lib(Dir);

clear a b
if a<>1 then pause,end
if b<>2 then pause,end


clear a b
deff("y=test", "y=a+b","n")
if test()<>3 then pause,end

comp(test)
clear a b
if test()<>3 then pause,end
