// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// -----------------------------------------------------------------------------
// TEST 1
// -----------------------------------------------------------------------------

file_1 = TMPDIR+'/test_binary_1.bin';

fid = mopen(file_1,'wb');

mput(1996,'l');
mput(1996,'i');
mput(1996,'s');
mput(98  ,'c');
mput(1996,'ul');
mput(1996,'ui');
mput(1996,'us');
mput(98  ,'uc');
mput(1996,'d');
mput(1996,'f');

// force little-endian
mput(1996,'ll');
mput(1996,'il');
mput(1996,'sl');
mput(98  ,'cl');
mput(1996,'ull');
mput(1996,'uil');
mput(1996,'usl');
mput(98  ,'ucl');
mput(1996,'dl');
mput(1996,'fl');

// force big-endian
mput(1996,'lb');
mput(1996,'ib');
mput(1996,'sb');
mput(98  ,'cb');
mput(1996,'ulb');
mput(1996,'uib');
mput(1996,'usb');
mput(98  ,'ucb');
mput(1996,'db');
mput(1996,'fb');

mputstr('Scilab');
mclose(fid);

fid = mopen(file_1,'rb');

if 1996<>mget(1,'l')  then pause,end
if 1996<>mget(1,'i')  then pause,end
if 1996<>mget(1,'s')  then pause,end
if   98<>mget(1,'c')  then pause,end
if 1996<>mget(1,'ul') then pause,end
if 1996<>mget(1,'ui') then pause,end
if 1996<>mget(1,'us') then pause,end
if   98<>mget(1,'uc') then pause,end
if 1996<>mget(1,'d')  then pause,end
if 1996<>mget(1,'f')  then pause,end

// force little-endian
if 1996<>mget(1,'ll')  then pause,end
if 1996<>mget(1,'il')  then pause,end
if 1996<>mget(1,'sl')  then pause,end
if   98<>mget(1,'cl')  then pause,end
if 1996<>mget(1,'ull') then pause,end
if 1996<>mget(1,'uil') then pause,end
if 1996<>mget(1,'usl') then pause,end
if   98<>mget(1,'ucl') then pause,end
if 1996<>mget(1,'dl')  then pause,end
if 1996<>mget(1,'fl')  then pause,end

// force big-endian
if 1996<>mget(1,'lb')  then pause,end
if 1996<>mget(1,'ib')  then pause,end
if 1996<>mget(1,'sb')  then pause,end
if   98<>mget(1,'cb')  then pause,end
if 1996<>mget(1,'ulb') then pause,end
if 1996<>mget(1,'uib') then pause,end
if 1996<>mget(1,'usb') then pause,end
if   98<>mget(1,'ucb') then pause,end
if 1996<>mget(1,'db')  then pause,end
if 1996<>mget(1,'fb')  then pause,end

if 'Scilab'<>mgetstr(6) then pause,end
mclose(fid);

// -----------------------------------------------------------------------------
// TEST 2
// An example with two files
// -----------------------------------------------------------------------------

file_2 = TMPDIR+'/test_binary_2.bin';
file_3 = TMPDIR+'/test_binary_3.bin';

fd2    = mopen(file_2,'wb');
fd3    = mopen(file_3,'wb');

mput(1996,'ull',fd2);
mput(1996,'ull',fd3);

mclose(fd2);
mclose(fd3);

fd2=mopen(file_2,'rb');
if 1996<>mget(1,'ull',fd2) then pause,end

fd3=mopen(file_3,'rb');
if 1996<>mget(1,'ull',fd3) then pause,end

mclose(fd2);
mclose(fd3);

// -----------------------------------------------------------------------------
// TEST 3
// An example with mseek
// -----------------------------------------------------------------------------

file_4 = TMPDIR+'/test_binary_4.bin';
fd4    = mopen(file_4,'wb');
for i=1:10
	mput(i,'d');
end
mseek(0);
mput(678,'d');
mseek(0,fd4,'end');
mput(932,'d');
mclose(fd4);

fd4     = mopen(file_4,'rb');
res     = mget(11,'d');
res1    = [1:11];
res1(1) = 678;
res1($) = 932;

if res1<>res then pause,end

mseek(0,fd4,'set');
// trying to read more than stored data
res1=mget(100,'d',fd4);
if res1<>res then pause,end

[a]=meof(fd4);
if a==0 then pause,end

mclearerr(fd4);
[a]= mclose(fd4);
if a<>0 then pause,end


// -----------------------------------------------------------------------------
// TEST 4
// Examples with arrays
// -----------------------------------------------------------------------------

file_5 = TMPDIR+'/test_binary_5.bin';

fid = mopen(file_5,'wb');
mput(1:100,'l');
mclose(fid);

fid = mopen(file_5,'rb');
X=mget(200,'l');
mclose(fid);

if size(X,'*')<> 100        then pause,end
if norm(X-(1:100))> 10*%eps then pause,end

file_6 = TMPDIR+'/test_binary_6.bin';
fid = mopen(file_6,'wb');
mput(1:100,'c');
mclose(fid);

fid = mopen(file_6,'rb');
X=mget(200,'c');
mclose(fid);

if size(X,'*')<> 100        then pause,end
if norm(X-(1:100))> 10*%eps then pause,end
