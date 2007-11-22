// <-- Non-regression test for bug 558 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=558
//
// <-- Short Description -->
//    mtlb_save reports an unnecessary warning. This is due to
//    line 57 of  SCI/macros/mtlb/mtlb_save.sci:
//
//      clear('rhs','lhs','kk','k','err','mtlb_thefile','or');
//
//    the last 'or' looks superfluous to me.


// exec("bug558.sce");
fileName="bug558.txt";
diary(fileName);
x = 20;
mtlb_save('bug558','x')
diary(0);

w=fileinfo(fileName);
if w(1) == 0 then
  affich_result(%F,558);
else
  affich_result(%T,558);
end;
 
mdelete(fileName);
mdelete('bug558.mat');
    
