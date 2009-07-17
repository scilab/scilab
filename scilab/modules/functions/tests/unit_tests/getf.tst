// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
funcprot(0);
ierr = execstr('getf(''SCI/modules/graphics/macros/plot.sci'');','errcatch');
if ierr <> 0 then pause,end