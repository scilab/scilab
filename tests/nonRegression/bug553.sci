// <-- Non-regression test for bug 553 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=553
//
// <-- Short Description -->
//    Bug Report Id: 0327120031024153915
//    Funny behavior when an interpreted function has a function argument that is called within a loop -- the loop exits after the first pass.
//
//    I don't know if it's related, but I just discovered that normal loop termination clears the loop variable:
//
//        -->n=12;
//         n =
//           12.
//
//        -->for n=1:3; disp(n); end
//           1.
//           2.
//           3.
//
//        -->n
//          !--error  4
//        undefined variable : n
//
//    Is this correct? I was expecting n == 3.
//    On Scilab 2.7.2 with " loopbug.sci, shown below " function
// ...


//exec("bug553.sci")
 

n=12;
 

 stat=execstr("for n=1:3; disp(n); end","errcatch");


 if stat == 0 then
 
 affich_result(%T,553);
 
 else
 affich_result(%F,553);
 
 end;
    
