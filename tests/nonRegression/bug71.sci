// <-- Non-regression test for bug 71 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=71
//
// <-- Short Description -->
//    The macr2lst() function crashes when a macro including 
//    an "exit" call is given as parameter


// bug71

mode (-1)
clear

function result = bug()
// This part replay bug
   deff("foo","exit")
   result=execstr("macr2lst(foo)","errcatch","n")
   clear foo;
endfunction
affich_result(bug()==0, 71)

clear

// Error message :
//   !--error    37
//       incorrect function at line ******  
//
// The macr2lst() function crashes when a macro including
// an "exit" call is given as parameter

