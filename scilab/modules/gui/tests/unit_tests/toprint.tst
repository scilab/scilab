// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// =============================================================================
// unit tests for toprint function
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// interactive test since it send to a printer some data.
//

ierr = execstr("toprint()", "errcatch");
if ierr <> 77 then pause, end

ierr = execstr("toprint(1, 2, 3)", "errcatch");
if ierr <> 77 then pause, end

ierr = execstr("toprint(1000)", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Figure with figure_id %d does not exist."), "toprint", 1000) then pause, end

warning('off');
ierr = execstr("r = toprint(""file does not exist"")", "errcatch");
warning('on');
if ierr <> 0 then pause, end
if r <> %F then pause, end

ierr = execstr("toprint(4000,4000);", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Figure with figure_id %d does not exist."), "toprint", 4000) then pause, end

scf(4000);
plot3d();

ierr = execstr("toprint(4000,4000);", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong type for input argument #%d.\n"), "toprint", 2) then pause, end

ierr = execstr("toprint(4000, ""wrong value"");", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong input argument #%d: ''%s'' or ''%s'' expected.\n"), "toprint", 2, "pos", "gdi") then pause, end

ierr = execstr("r = toprint(4000);",  "errcatch");
if ierr <> 0 then pause, end
if r <> %T then pause, end

ierr = execstr("r = toprint(4000, ""pos"");",  "errcatch");
if ierr <> 0 then pause, end
if r <> %T then pause, end

ierr = execstr("r = toprint(4000, ""gdi"");",  "errcatch");
if ierr <> 0 then pause, end
if r <> %T then pause, end

ierr = execstr("r = toprint(SCI + ""/modules/gui/etc/gui.start"");",  "errcatch");
if ierr <> 0 then pause, end
if r <> %T then pause, end

txt = ["Test", "toprint primitive"];
pageHeader = "Scilab page header";
if toprint(txt, pageHeader) <> %t then pause, end


