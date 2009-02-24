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

r = getpartlevel('cd c:\Program Files\scilab-5.1');
if r <>'1' then pause,end;

r = getpartlevel('cd(""c:\Program Files\scilab-5.1')
if r <>'1' then pause,end;

r = getpartlevel('printf(""hello"")');
if r <>'' then pause,end;

r = getpartlevel('g.children');
if r <> 'children' then pause,end

r = getpartlevel('1+myvariable');
if r <> 'myvariable' then pause,end