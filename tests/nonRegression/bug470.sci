// <-- Non-regression test for bug 470 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=470
//
// <-- Short Description -->
//    Bug Report Id: 120442003624162135
//    %i_i_i is undefined because /macros/percent/%i_i_i.sci is WRONG and defines function %i_i_s() instead. Changing s->i there (and eventually the comments) should fix it.
//
//    On Scilab CVS with " %i_i_i " function
//    The Error Messages are:
//                                        !--error     4 
//    undefined variable : %i_i_i                  
//
//    Commands: a=int8(ones(2,2)); a(1,1)=uint8(2)
//
//    Enrico Segre on Linux version RH9 distribution  with   as window manager
//    Israel  July 24, 2003 at 16:21:35


// bug470

mode (-1)
clear

// correct=%F
result=execstr("a=int8(ones(2,2))","errcatch","n")
if result == 0 then
   result=execstr("a(1,1)=uint8(2)","errcatch","n")
end
affich_result(result==0, 470)

clear

// %i_i_i is undefined because /macros/percent/%i_i_i.sci is WRONG 
// and defines function %i_i_s() instead. 
// Changing s->i there (and eventually the comments) should fix it.
//
//The Error Messages are:
//    !--error     4 
// undefined variable : %i_i_i     




