// <-- Non-regression test for bug 542 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=542
//
// <-- Short Description -->
//    Bug Report Id: 031202003109123244
//     bitwise operations among integers used to be defined until last time I needed them. How come they aren't anymore?
//    On Scilab CVS with " &, | " function
//    The Error Messages are:
//                       !--error    43 
//    not implemented in scilab....
//
//    Commands:  int8(7) & int8(8)
//    Enrico SEGRE on Linux version RH9 distribution  with   as window manager
//    Israel  
//    November 9, 2003 at 12:32:44


//exec("/home/huynh/poubelle/testNonReg/bug542.sce")
 

ierror=execstr("int8(7) & int8(8)","errcatch");

 if ierror == 0 then
 
 affich_result(%T,542);
 
 else
 affich_result(%F,542);
 
 end;
    
