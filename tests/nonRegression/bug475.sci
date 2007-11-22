// <-- Non-regression test for bug 475 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=475
//
// <-- Short Description -->
//    Bug Report Id: 120442003624175150
//    m=min(a) and m=max(a) give correct results
//
//
//    On Scilab CVS with " min(), max() " function
//    The Error Messages are:
//                                                 !--error    59 
//    incorrect # of outputs in the function
//    arguments are :
//     x         
//
//    Commands:  a=hypermat([3,3,1],uint16(1:9)); [m,k]=max(a)
//
//    or
//
//     a=hypermat([3,3,1],uint16(1:9)); [m,k]=min(a)
//
//    Enrico Segre on Linux version RH9 distribution  with  gnome as window manager
//    Israel  July 24, 2003 at 17:51:50


// bug475

mode (-1)
clear

//correct=%F
a=hypermat([3,3,1],uint16(1:9))
result=execstr("[m,k]=max(a)","errcatch","n")  
if result == 0 then
   result=execstr("[m,k]=min(a)","errcatch","n")  
end
affich_result(result==0, 475)

clear

//    The Error Messages are:
//                                             !--error    59 
// incorrect # of outputs in the function




