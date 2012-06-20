// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8531 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8531
//
// <-- Short Description -->
// 
//  "edit" function failed to open a defined macro loaded on stack

// a Trick to overload call to editor in edit function
global resultPathfname
resultPath = "";
clearfun("editor");
warning('off')
function editor(fname)
  global('resultPathfname');
  resultPathfname = fname;
  clear('resultPathfname')
endfunction
warning('on')

exec("SCI/modules/core/tests/nonreg_tests/bug_8531.sci");
if ~isdef("BUG_8531") then pause, end

if execstr("edit(""BUG_8531"")", "errcatch") <> 0 then pause, end
if ~isfile(resultPathfname) then pause, end
result = mgetl(resultPathfname);
if grep(result, "function") == [] then pause, end
if grep(result, "endfunction") == [] then pause, end

deff('[x]=mymacro(y,z)',['a=3*y+1'; 'x=a*z+y'])
if execstr("edit(""mymacro"")", "errcatch") <> 0 then pause, end

clearglobal('resultPathfname');
