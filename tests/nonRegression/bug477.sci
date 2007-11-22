// <-- Non-regression test for bug 477 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=477
//
// <-- Short Description -->
//    Bug Report Id: 12044200362419444
//     %hm_maxi and %hm_mini are indeed currently defined with a _single_ return argument. IMHO they should be extended analogously to max, min and %hm_find, returning optionally either a vector of indices of the max/min.
//
//    On Scilab CVS with " %hm_maxi, %hm_mini " function
//    The Error Messages are:
//     -->a=hypermat([3,3,1],1:9);
//
//    -->[m]=max(a)                    
//     i  =
//
//     9.  
//
//    -->[m,i]=max(a)
//                 !--error    59 
//    incorrect # of outputs in the function
//    arguments are :
//     x         
//
//    -->[m,i,j,k]=max(a)
//                     !--error    78 
// ...


// bug477

mode (-1)
clear

correct=%F
a=hypermat([3,3,1],1:9)
[m]=max(a)                    
result2=execstr("[n,i]=max(a)","errcatch","n")
if result2==0 then
   result3=execstr("[m,i,j,k]=max(a)","errcatch","n")
   correct = ((m==n)&(result2==0)&(and(i==[3,3,1]))&(result3==78))
end
affich_result(correct, 477)

clear

//    The Error Messages are:
//    -->[m,i]=max(a)
//             !--error    59 
// incorrect # of outputs in the function
// arguments are :
//  x         



