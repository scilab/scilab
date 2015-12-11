//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6484 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6484
//
// <-- Short Description -->
// 'file' did not return filename of an opened fortran file

u = file('open',TMPDIR+'/foo.txt','unknown');
[a,b,c,d,e] = file(u);
if a <> u then pause,end
if b <> 'F' then pause,end
if c == '' then pause,end
if c <> pathconvert(TMPDIR + '/foo.txt',%f,%t) then pause,end
if d <> 3 then pause,end
if e <> 0 then pause,end
file('close', u);
