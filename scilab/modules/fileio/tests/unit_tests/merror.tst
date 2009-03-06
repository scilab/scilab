// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

if merror() <> 1 then pause,end
if merror(1) <> 1 then pause,end

ierr = execstr('merror([1 1])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('merror(''toto'')','errcatch');
if ierr <> 999 then pause,end