//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

file3=TMPDIR + '/test3.bin';
fd1= mopen(file3, 'wb');
for i=1:10
    mput(i, 'd');
end 

mseek(0);
mput(678, 'd');
mseek(0, fd1, 'end');
mput(932, 'd');
mclose(fd1);
fd1 = mopen(file3, 'rb');
res = mget(11, 'd');
res1 = [1:11];
res1(1) = 678;
res1($) = 932;
if res1 <> res then pause,end;

mseek(0, fd1, 'set');
res1 = mget(100, 'd', fd1);
if res1 <> res then pause,end;
meof(fd1);
mclearerr(fd1);
mclose(fd1);

ierr = execstr('mseek(0,0,""set"")','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mseek(0,5,""set"")','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mseek(0,6,""set"")','errcatch');
if ierr <> 999 then pause,end

