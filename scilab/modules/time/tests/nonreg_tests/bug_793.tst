// <-- Non-regression test for bug 793 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=793
//
// <-- Short Description -->
//    Getdate crashes scilab with negative arguments

err = execstr("getdate(-1)","errcatch");
if err <> 999 then pause,end
