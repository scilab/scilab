//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
cd(TMPDIR);
[a1,b1] = diary('log1.txt');
if (a1 == 0) then pause,end

[a2,b2] = diary('log2.txt');
if (a2 == 0) then pause,end

diary(a1,'close')
diary(a2,'close')
// =============================================================================
ierr = execstr("diary(10000,''close'')","errcatch");
if ierr <> 999 then pause,end

[a2,b2] = diary('log2.txt');
if (a2 == 0) then pause,end

diary('log2.txt','close')

ierr = execstr("diary(''log2.txt'',''close'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
[a1,b1] = diary(TMPDIR + '/pause.txt');
diary(TMPDIR + '/pause.txt','on');
diary(TMPDIR + '/pause.txt','resume');
diary(TMPDIR + '/pause.txt','off');
diary(TMPDIR + '/pause.txt','pause');
// =============================================================================
diary(a1,'on');
diary(a1,'resume');
diary(a1,'off');
diary(a1,'pause');
// =============================================================================
ierr = execstr("diary(1000,''on'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary(1000,''off'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary(1000,''resume'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary(1000,''pause'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
diary(a1,'close')
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/pause.txt'',''pausewitherror'');", "errcatch");
if ierr <> 999 then pause,end
// =============================================================================
a = diary(TMPDIR + '/log1.txt');
b = diary(TMPDIR + '/log2.txt');

if ~diary(a,'exists') then pause,end
if ~diary(b,'exists') then pause,end

if ~diary(TMPDIR + '/log1.txt','exists') then pause,end
if ~diary(TMPDIR + '/log2.txt','exists') then pause,end

diary(TMPDIR + '/log1.txt','close')
diary(TMPDIR + '/log2.txt','close')

mdelete(TMPDIR + '/log1.txt');
mdelete(TMPDIR + '/log2.txt');
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate'',''new'',''prefix=YY-MM-DD hh:mm:ss'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate'',''new'',''prefix=YYYY-MM-DD hh:mm:ss'')","errcatch");
if ierr <> 0 then pause,end
if ( fileinfo(TMPDIR + '/diarydate') == []) then pause,end
diary(TMPDIR + '/diarydate','close')
mdelete(TMPDIR + '/diarydate');
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate2'',''new'',''prefix=U'')","errcatch");
if ierr <> 0 then pause,end
if ( fileinfo(TMPDIR + '/diarydate2') == []) then pause,end
diary(TMPDIR + '/diarydate2','close')
mdelete(TMPDIR + '/diarydate2');
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate2'',''new'',[''prefix=U'',''prefix-only-commands''])","errcatch");
if ierr <> 0 then pause,end
if ( fileinfo(TMPDIR + '/diarydate2') == []) then pause,end
diary(TMPDIR + '/diarydate2','close')
mdelete(TMPDIR + '/diarydate2');
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate2'',''new'',''filter)command'')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate2'',''new'',''filter=command'')","errcatch");
if ierr <> 0 then pause,end
if ( fileinfo(TMPDIR + '/diarydate2') == []) then pause,end
diary(TMPDIR + '/diarydate2','close')
mdelete(TMPDIR + '/diarydate2');
// =============================================================================
ierr = execstr("diary(TMPDIR + ''/diarydate2'',''new'',''filter=output'')","errcatch");
if ierr <> 0 then pause,end
if ( fileinfo(TMPDIR + '/diarydate2') == []) then pause,end
diary(TMPDIR + '/diarydate2','close')
mdelete(TMPDIR + '/diarydate2');
// =============================================================================
ierr = execstr("diary('''','''','''')","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary([],[],[])","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("diary([],''list'',[])","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
names = [];
ids = [];
[a0, b0] = diary(TMPDIR + '/checkname');
for i=1:100
  [a,b] = diary(TMPDIR + '/checkname','new');
  ids = [ids, a];
  names = [names, b];
end
if size(ids,'*') <> 100 then pause,end

l = ls(TMPDIR+'/checkname*');
if size(l,'*') <> 101 then pause,end

if and(diary(ids,'exists')) <> %t then pause,end

diary(ids, 'close')
diary(a0, 'close')

for i=0:99
  mdelete(TMPDIR + '/checkname' + '_' + string(i));
end

mdelete(TMPDIR + '/checkname');
// =============================================================================
r = [];
for i = 1:100
  r = [r, diary(TMPDIR + '/test'+string(i)+'.diary','new');];
  if ( fileinfo(TMPDIR + '/test'+string(i)+'.diary') == []) then pause,end
end

if (size(r,'*') <> 100) then pause,end

for i = 1:100
	diary(i,'close');
	mdelete(TMPDIR + '/test'+string(i)+'.diary');
	if ( fileinfo(TMPDIR + '/test'+string(i)+'.diary') <> []) then pause,end
end
// =============================================================================

