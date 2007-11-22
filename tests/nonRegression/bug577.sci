// <-- Non-regression test for bug 577 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=577
//
// <-- Short Description -->
//    It can't save imaginary in MATLAB format (SCILAB 2.7.x and
//    cvs version)
//
//    I can reproduce the bug with integers data type not with imaginary

x=rand(5,5,100)+%i;
iCodeError= execstr('mtlb_save(''gain.mat'',''x'')','errcatch' );
lasterror()

if iCodeError ==0   then
	affich_result(%T,577)
else
	affich_result(%F,577)
end;
