// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 4543 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4543
//
// <-- Short Description -->
// 
// Waitbar command seemed not to work properly with message define by a string.

winId = waitbar(0);

iLast = 20000;

// Proper operation
for i=1:iLast
  waitbar(i/iLast,"%",winId);
end
delete(winId);

mes = 'a';
ierr = execstr('waitbar(0/iLast,mes,winId);','errcatch');
if ierr <> 999 then pause,end

// we create a new waitbar
winId = waitbar(0);

for i=1:iLast
  waitbar(i/iLast,mes,winId);
end
delete(winId);