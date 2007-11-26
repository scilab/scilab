// <-- Non-regression test for bug 1198 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1198
//
// <-- Short Description -->
//    For the latest version of Scilab scilab-3.0-u-20050128 on a 
//    Windows 2000 platform.
//
//    The program crashes instead of reporting a sytax error.

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

G = syslin('c', [1 0 ; 0 1], [ 1; 1], [1 1]);
minss(G)("A");
B = lasterror();

if B <> 'invalid index' then pause,end
