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

if 'àéùç' <> sprintf('àéùç') then pause,end
if 'àéùç' <> sprintf('%s','àéùç') then pause,end
disp('àéùç')
mprintf('àéùç');
mprintf('\n%s\n','àéùç');
printf('àéùç');
printf('\n%s\n','àéùç');

printf("-- Création de [%s] (Macros) --\n",'test')