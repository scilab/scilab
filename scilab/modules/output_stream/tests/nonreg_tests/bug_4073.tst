// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4073 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4073
//
// <-- Short Description -->
// mprintf does not display accent correctly

if 'אישח' <> sprintf('אישח') then pause,end
if 'אישח' <> sprintf('%s','אישח') then pause,end
disp('אישח')
mprintf('אישח');
mprintf('\n%s\n','אישח');
printf('אישח');
printf('\n%s\n','אישח');

printf("-- Crיation de [%s] (Macros) --\n",'test')