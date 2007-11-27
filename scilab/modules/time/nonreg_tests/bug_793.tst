// <-- Non-regression test for bug 793 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=793
//
// <-- Short Description -->
//    Getdate crashes scilab with negative arguments

if or(getdate(-1) <> [0 0 0 0 0 0 0 0 0 0]) then pause,end
