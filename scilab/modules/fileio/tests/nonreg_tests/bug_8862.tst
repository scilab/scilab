//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2013 - Alexander Kindyakov
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 8862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8862
//
// <-- Short Description -->

// Trying to read binary file by mget function. 
// I want to read a uint64 bits data, then I using "mget(1,'ul',fid)".
// I found it only reads 32bits not 64bits. the mget(1,'l',fid) does same got 32bits.

// <-- Scilab error message -->
// mget reading 64bits got 32bits

bf = mopen(TMPDIR+'/foo','wb');
ierr = execstr('mput(1996,''l'',bf);','errcatch');
if ierr <> 0 then pause,end
ierr = execstr('mput(1996,''ul'',bf);','errcatch');
if ierr <> 0 then pause,end
ierr = execstr('mput(10000000000,''l'',bf);','errcatch');
if ierr <> 0 then pause,end
ierr = execstr('mput(10000000000,''ul'',bf);','errcatch');
if ierr <> 0 then pause,end
mclose(bf);

bf= mopen(TMPDIR+'/foo','rb');
ierr = execstr('a = mget(1,''l'',bf);','errcatch');
if ierr <> 0 | a <> 1996 then pause,end
ierr = execstr('a = mget(1,''ul'',bf);','errcatch');
if ierr <> 0 | a <> 1996 then pause,end
ierr = execstr('a = mget(1,''l'',bf);','errcatch');
if ierr <> 0 | a <> 10000000000 then pause,end
ierr = execstr('a = mget(1,''ul'',bf);','errcatch');
if ierr <> 0 | a <> 10000000000 then pause,end
mclose(bf);
