// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);

currentline = 'cd /home/sy';
r = completeline(currentline,'sylvestre',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd /home/sylvestre' then pause,end

currentline = 'cd d:\GIT-scilab-';
r = completeline(currentline,'d:\GIT-scilab-branch',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd d:\GIT-scilab-branch' then pause,end
