// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 3924 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3924
//
// <-- Short Description -->
//    I used uicontrol with bad parameters, but uicontrol makes scilab hangs.


ierr = execstr("uicontrol(1,""style"",""text"",""string"",""Solution"",""position"",[603,30,192,120],""horizontalalignment"",""center"")", "errcatch");
msg = lasterror();
if ierr==0 | msg<>msprintf(gettext("%s: Wrong type for input argument #%d: A ''%s'' or a ''%s'' handle expected.\n"),"uicontrol", 1, "Figure", "Frame uicontrol") then pause; end

ierr = execstr("uicontrol(""Parent"",1,""style"",""text"",""string"",""Solution"",""position"",[603,30,192,120],""horizontalalignment"",""center"")", "errcatch");
msg = lasterror();
if ierr==0 | msg<>msprintf(gettext("Wrong value for ''%s'' property: A ''%s'' or ''%s'' handle expected.\n"), "Parent", "Figure", "Frame uicontrol") then pause; end

ierr = execstr("uicontrol(1)", "errcatch");
msg = lasterror();
if ierr==0 | msg<>msprintf(gettext("%s: Wrong type for input argument #%d: A graphic handle expected.\n"),"uicontrol", 1) then pause; end
  
