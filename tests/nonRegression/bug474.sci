// <-- Non-regression test for bug 474 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=474
//
// <-- Short Description -->
//    Bug Report Id: 120442003624174827
//    k should be a 2 element vector!! like
//
//    -->a=matrix(1:9,3,3); [m,k]=min(a)       
//     k  =
//
//    !   1.    1. !
//     m  =
//
//        1.  
//
//    -->a=matrix(1:9,3,3); [m,k]=max(a)
//     k  =
//
//    !   3.    3. !
//     m  =
//
//        9.  
//
//
// ...

//correct=%F
a=matrix(1:9,3,3)
[mia,kia]=min(a) 
[maa,kaa]=max(a)

b=int16(matrix(1:9,3,3))
[mib,kib]=min(b)
[mab,kab]=max(b)

affich_result(mia==mib, 474.1)
affich_result(and(kia==kib), 474.2)
affich_result(maa==mab, 474.3)
affich_result(and(kaa==kab), 474.4)
