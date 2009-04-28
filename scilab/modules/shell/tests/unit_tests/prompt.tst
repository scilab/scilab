// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
curprompt = prompt();
if curprompt <> ascii([45.    45.    62.]) then pause,end

[curprompt, pause_level] = prompt();
if curprompt <> ascii([45.    45.    62.]) then pause,end
if pause_level <> 0 then pause,end

prompt('-- My Prompt -->');
1+1;

ierr = execstr('[a,b,c] = prompt();','errcatch');
if ierr <> 78 then pause,end

ierr = execstr('[a,b] = prompt(1);','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('[a,b] = prompt(''1'');','errcatch');
if ierr <> 999 then pause,end
// =============================================================================

