//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3022 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3022
//
// <-- Short Description -->
//    TCL_SetVar and TCL_GetVar had some trouble
//    with TCL thread synchro

TCL_CreateSlave("testMe");
TCL_SetVar("h","0", "testMe");
for i = 1:100000
  TCL_GetVar("h", "testMe");
  TCL_SetVar("h",string(i), "testMe");
end

if TCL_GetVar("h", "testMe") == 100000 then pause,end