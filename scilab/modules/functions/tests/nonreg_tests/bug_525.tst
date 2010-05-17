// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 525 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=525
//
// <-- Short Description -->
//    Bug Report Id: 126632003824121343
//     error messages are strange to say least, scilab gets stymied after the second attempt. 
//
//    ps: I realize that execstr() is most likely responsible for all what I posted earlier today about ScilabEval on the newsgroup
//
//    On Scilab CVS with " execstr() " function
//    The Error Messages are:
//      -->execstr("function foo")             
//     !--error    17 
//    stack size exceeded! (Use stacksize function to increase it)
//    Memory used for variables :     17086
//    Intermediate memory needed:    982925
//    Total  memory available   :   1000001
//    in  execstr instruction    called by :  
//    execstr("function foo")
//
//
//    -->stacksize(2e6)
//
//    -->execstr("function foo")
// ...

stacksize(2e6);
execstr("function foo","errcatch");
str = lasterror();
if str<>msprintf(gettext("endfunction is missing.\n")) then pause,end

