// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3905 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3905
//
// <-- Short Description -->
// regression fscanf

wMode = warning("query");
warning("off");

fln = TMPDIR + filesep() +'bug_3905.txt';
strs = ['txt1','txt2','txt3'];
mputl(strs, fln);

[fi, err] = file('open', fln, 'old');

if err <> 0 then pause,end

s1 = fscanf(fi,'%s');
if s1 <> 'txt1' then pause,end

s2 = fscanf(fi,'%s');
if s2 <> 'txt2' then pause,end

s3 = fscanf(fi,'%s');
if s3 <> 'txt3' then pause,end

file('close',fi);

ierr = execstr("fscanf(9999,''%s'');","errcatch");
if ierr <> 999 then pause,end

warning(wMode);