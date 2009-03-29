// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

n = 50;
nstr = strcat(string(1:n),' ');
fstr = strcat(strsubst(string(ones(1,n)),'1','%d'), ' ');

ierr = execstr("sscanf(1,''%d'')", "errcatch");
if ierr <> 999 then pause,end

[A1, A2, A3, A4, A5, A6, A7, A8] = sscanf(nstr, fstr);
ref = 1:8 ;
if or([A1, A2, A3, A4, A5, A6, A7, A8] <> ref) then pause,end

v = 'v';
args = strcat( v(ones(n,1)) + string(1:n)',',');
ierr = execstr('[' + args + '] = sscanf(nstr, fstr);','errcatch');
ref = 1:n ;
ierr = execstr('r = or([' + args + '] <> ref)','errcatch');
if ierr <> 0 then pause,end
if r <> %f then pause,end

n = 500;
nstr = strcat(string(1:n),' ');
fstr = strcat(strsubst(string(ones(1,n)),'1','%d'), ' ');
v = 'v';
args = strcat( v(ones(n,1)) + string(1:n)',',');
ierr = execstr('[' + args + '] = sscanf(nstr, fstr);','errcatch');
if ierr <> 999 then pause,end

n = 50;
nstr = strcat(string(1:n),' ');
fstr = strcat(strsubst(string(ones(1,n)),'1','%d'), ' ');

mputl(nstr,TMPDIR+'/fscanf.txt');
[A1, A2, A3, A4, A5, A6, A7, A8] = fscanf(TMPDIR+'/fscanf.txt',fstr);
ref = 1:8 ;
if or([A1, A2, A3, A4, A5, A6, A7, A8] <> ref) then pause,end

v = 'v';
args = strcat( v(ones(n,1)) + string(1:n)',',');
ierr = execstr('[' + args + '] = fscanf(TMPDIR+''/fscanf.txt'', fstr);','errcatch');
ref = 1:n ;
ierr = execstr('r = or([' + args + '] <> ref)','errcatch');
if ierr <> 0 then pause,end
if r <> %f then pause,end

n = 500;
nstr = strcat(string(1:n),' ');
fstr = strcat(strsubst(string(ones(1,n)),'1','%d'), ' ');
v = 'v';
args = strcat( v(ones(n,1)) + string(1:n)',',');
ierr = execstr('[' + args + '] = fscanf(TMPDIR+''/fscanf.txt'', fstr);','errcatch');
if ierr <> 999 then pause,end

//test format %i %d
if sscanf('123','%i')<>123 then pause,end
if sscanf('     123','%i')<>123 then pause,end
if sscanf('123','%2i')<>12 then pause,end
if sscanf('123','%0i')<>123 then pause,end
if sscanf('123','%5i')<>123 then pause,end
//test format %u
if sscanf('+123','%u')<>123 then pause,end
if sscanf(' 123','%2u')<>12 then pause,end
if sscanf('123','%0u')<>123 then pause,end
if sscanf('+123','%5u')<>123 then pause,end
//test format %e %f %g

if sscanf('123','%e')<>123 then pause,end
if sscanf('     123','%e')<>123 then pause,end
if sscanf('123','%2e')<>12 then pause,end
if sscanf('123','%0e')<>123 then pause,end
if sscanf('123','%5e')<>123 then pause,end

//test format %s
if sscanf('123','%s')<>'123' then pause,end
if sscanf('     123','%s')<>'123' then pause,end
if sscanf('123','%2s')<>'12' then pause,end
if sscanf('123','%0s')<>'123' then pause,end
if sscanf('123','%5s')<>'123' then pause,end

//test format %o
if sscanf('123','%o')<>83 then pause,end
if sscanf('     123','%o')<>83 then pause,end
if sscanf('123','%2o')<>10 then pause,end
if sscanf('123','%0o')<>83 then pause,end
if sscanf('123','%5o')<>83 then pause,end

//test format %x
if sscanf('123','%x')<>291 then pause,end
if sscanf('     123','%x')<>291 then pause,end
if sscanf('123','%2x')<>18 then pause,end
if sscanf('123','%0x')<>291 then pause,end
if sscanf('123','%5x')<>291 then pause,end

//test format %c
if sscanf('123','%c')<>'1' then pause,end
if sscanf('     123','%c')<>' ' then pause,end
if sscanf('123','%0c')<>'1' then pause,end

//test des format complexes 
if sscanf('123 4','%*s%s')<>'4' then pause,end
if sscanf('123 4','123%e')<>4 then pause,end
[a,b,c]=sscanf('xxxxx 4 test 23.45','xxxxx%i%s%e')
if a<>4|b<>'test'|c<>23.45 then pause,end

[a,b]=sscanf('123\n456','%e%e')
if a<>123|b<>456 then pause,end