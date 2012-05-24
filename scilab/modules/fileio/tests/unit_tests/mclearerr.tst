// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

ierr = execstr('mclearerr()','errcatch');
ierr = execstr('mclearerr(''toto'')','errcatch');
ierr = execstr('mclearerr([1 2])','errcatch');