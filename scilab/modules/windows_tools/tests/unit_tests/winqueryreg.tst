// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr = execstr('winqueryreg()','errcatch');
if ierr <> 77 then pause,end

ierr = execstr('winqueryreg(''name'', ''HKEY_CURRENT_USER'')','errcatch');
if ierr <> 999 then pause,end

mousechar = winqueryreg('name', 'HKEY_CURRENT_USER','control panel\mouse');
if size(mousechar,'*') <> 18 then pause,end

cpu = winqueryreg('HKEY_LOCAL_MACHINE','HARDWARE\DESCRIPTION\System\CentralProcessor\0\','ProcessorNameString');
if length(cpu) == 0 then pause,end

