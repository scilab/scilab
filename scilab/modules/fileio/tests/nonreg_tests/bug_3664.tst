// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3664 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3664
//
// <-- Short Description -->
//  an error occurs when a change directory operation implies a directory name with french accent letter ('é' for example).

curdir = pwd();
mkdir(TMPDIR,'cédéùà');
p = TMPDIR + filesep() + 'cédéùà';

if isdir(p) <> %t then pause,end

if cd(p) <> p then pause,end

if chdir(p) <> %T then pause,end

 
 
