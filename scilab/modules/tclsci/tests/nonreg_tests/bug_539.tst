// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 539 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=539
//
// <-- Short Description -->
//    Bug Report Id: 030612003103172113
//    Normally, as a procedure exists with an error, the message  reports the calling tree, such as
//
//    -->function foo(); a_wrong_string; endfunction
//
//    -->foo()
//     !--error     4
//    undefined variable : a_wrong_string
//    at line       2 of function foo                      called by :
//    foo()
//
//
//    Here instead the "called by" is the last scilab command, which is pretty confusing.  My claim is that the error message in the scilab window is wrong, and that is due to ScilabEval(). The use of  scipad is just a quick way of accessing ScilabEval(), in order to evidence the bug; however scipad/execute would certyainly gain if the bug is corrected.
//    On Scilab CVS with " ScilabEval " function
//    The Error Messages are:
//     --> a_wrong_string,return,
//       !--error     4 
//    undefined variable : a_wrong_string
//    in  execstr instruction    called by :
//    disp(" a_wrong_string"),return,
// ...

function foo(); a_wrong_string; endfunction

tcl_script=['toplevel .w1'
'button .w1.b -text ""Click here to see a new Scilab Graphic Window""\'
'  -command {ScilabEval ""foo()""}'
'pack .w1.b ']
mputl(tcl_script,TMPDIR+'/test.tcl')
// Execute the tcl script
TCL_EvalFile(TMPDIR+'/test.tcl')
