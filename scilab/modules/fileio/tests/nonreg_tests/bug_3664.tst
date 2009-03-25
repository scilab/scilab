// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 3664 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3664
//
// <-- Short Description -->
//  an error occurs when a change directory operation implies a directory name with french accent letter ('é' for example).

curdir = pwd();
mkdir(TMPDIR,'cédéùà');
if cd(TMPDIR + filesep() + 'cédéùà') <> TMPDIR + filesep() + 'cédéùà' then pause,end
cd(curdir);

if chdir(TMPDIR + filesep() + 'cédéùà') <> %T then pause,end