// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3398 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3398
//
// <-- Short Description -->
// A bug in the completion:
// it removes the line. It should add it at the end instead instead.
// exec le<TAB>

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);

currentline = 'exec le';
r = getpartlevel(currentline);
if r <> 'le' then pause,end

if size(completion(r),'*') == [] then pause,end

r = completeline(currentline,'',getpartlevel(currentline),getfilepartlevel(currentline),%f);
if r <> currentline then pause,end

     