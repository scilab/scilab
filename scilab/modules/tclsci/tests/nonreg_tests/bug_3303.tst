// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3303 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3303
//
// <-- Short Description -->
// Shambolic error management or parsing in slave Tcl interpreters

// First command executed in main interpreter
if execstr("TCL_EvalStr(""wrongcommand"");", "errcatch")==0 then pause; end
msg = lasterror();
msg = strsubst(msg, ascii(9), "\n");
msg = sprintf("%s", msg);
msgref = ["TCL_EvalStr,  at line 1";"invalid command name ""wrongcommand""    while executing""wrongcommand"""];
if or(msg<>msgref) then pause; end

TCL_CreateSlave("slave");

// Command executed in slave: error message returned must not depend on previous error in main
if execstr("TCL_EvalStr(""wrongcommandinslave"",""slave"")", "errcatch")==0 then pause; end
msg = lasterror();
msg = strsubst(msg, ascii(9), "\n");
msg = sprintf("%s", msg);
msgref = ["TCL_EvalStr, invalid command name ""wrongcommandinslave"" at line 1";"invalid command name ""wrongcommandinslave""    while executing""wrongcommandinslave"""];
if or(msg<>msgref) then pause; end

// Second command executed in main interpreter
if execstr("TCL_EvalStr(""wrongcommand2"");", "errcatch")==0 then pause; end
msg = lasterror();
msg = strsubst(msg, ascii(9), "\n");
msg = sprintf("%s", msg);
msgref = ["TCL_EvalStr,  at line 1";"invalid command name ""wrongcommand2""    while executing""wrongcommand2"""];
if or(msg<>msgref) then pause; end

// Command executed again in slave: must give the same error message
if execstr("TCL_EvalStr(""wrongcommandinslave"",""slave"")", "errcatch")==0 then pause; end
msg = lasterror();
msg = strsubst(msg, ascii(9), "\n");
msg = sprintf("%s", msg);
msgref = ["TCL_EvalStr, invalid command name ""wrongcommandinslave"" at line 1";"invalid command name ""wrongcommandinslave""    while executing""wrongcommandinslave"""];
if or(msg<>msgref) then pause; end



