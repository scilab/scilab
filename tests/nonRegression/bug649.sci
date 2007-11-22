// <-- Non-regression test for bug 649 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=649
//
// <-- Short Description -->
//    scilab crashes under windows
//    under linux, Scilab loops forever, and it uses the whole CPU
//    resource.


// exec("bug649.sci");

x=rand(5,5,100)+%i;
iCodeError= execstr('mtlb_save gain.mat x','errcatch' );

affich_result(%T,649)
