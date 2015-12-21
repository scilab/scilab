//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=dir('SCI\bin');
f1=a(2);
r1=newest(f1);
clear a f1 r1

realtimeinit(2);
realtime(0);

for k=1:4
	realtime(k);
	mputl("",TMPDIR+"/newest_"+string(k));
end

if newest([]) <> [] then pause,end
if newest()   <> [] then pause,end

if newest("SCI/etc/scilab.start") <> 1 then pause,end
if newest("SCI/nofile.txt")       <> 1 then pause,end

if newest(TMPDIR+"/newest_1",TMPDIR+"/newest_2",TMPDIR+"/newest_3",TMPDIR+"/newest_4") <> 4 then pause,end
if newest(TMPDIR+"/newest_2",TMPDIR+"/newest_3",TMPDIR+"/newest_4",TMPDIR+"/newest_1") <> 3 then pause,end
if newest(TMPDIR+"/newest_3",TMPDIR+"/newest_4",TMPDIR+"/newest_1",TMPDIR+"/newest_2") <> 2 then pause,end
if newest(TMPDIR+"/newest_4",TMPDIR+"/newest_1",TMPDIR+"/newest_2",TMPDIR+"/newest_3") <> 1 then pause,end

if newest([TMPDIR+"/newest_1" TMPDIR+"/newest_2" TMPDIR+"/newest_3" TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/newest_2" TMPDIR+"/newest_3" TMPDIR+"/newest_4" TMPDIR+"/newest_1"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_3" TMPDIR+"/newest_4" TMPDIR+"/newest_1" TMPDIR+"/newest_2"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_4" TMPDIR+"/newest_1" TMPDIR+"/newest_2" TMPDIR+"/newest_3"] ) <> 1 then pause,end

if newest([TMPDIR+"/newest_1" TMPDIR+"/newest_2" ; TMPDIR+"/newest_3" TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/newest_2" TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" TMPDIR+"/newest_1"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_3" TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" TMPDIR+"/newest_2"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_4" TMPDIR+"/newest_1" ; TMPDIR+"/newest_2" TMPDIR+"/newest_3"] ) <> 1 then pause,end

if newest([TMPDIR+"/newest_1" ; TMPDIR+"/newest_2" ; TMPDIR+"/newest_3" ; TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/newest_2" ; TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" ; TMPDIR+"/newest_1"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" ; TMPDIR+"/newest_2"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" ; TMPDIR+"/newest_2" ; TMPDIR+"/newest_3"] ) <> 1 then pause,end


if newest(TMPDIR+"/newest_1",TMPDIR+"/no_file",TMPDIR+"/newest_3",TMPDIR+"/newest_4") <> 4 then pause,end
if newest(TMPDIR+"/no_file",TMPDIR+"/newest_3",TMPDIR+"/newest_4",TMPDIR+"/newest_1") <> 3 then pause,end
if newest(TMPDIR+"/newest_3",TMPDIR+"/newest_4",TMPDIR+"/newest_1",TMPDIR+"/no_file") <> 2 then pause,end
if newest(TMPDIR+"/newest_4",TMPDIR+"/newest_1",TMPDIR+"/no_file",TMPDIR+"/newest_3") <> 1 then pause,end

if newest([TMPDIR+"/newest_1" TMPDIR+"/no_file" TMPDIR+"/newest_3" TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/no_file" TMPDIR+"/newest_3" TMPDIR+"/newest_4" TMPDIR+"/newest_1"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_3" TMPDIR+"/newest_4" TMPDIR+"/newest_1" TMPDIR+"/no_file"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_4" TMPDIR+"/newest_1" TMPDIR+"/no_file" TMPDIR+"/newest_3"] ) <> 1 then pause,end

if newest([TMPDIR+"/newest_1" TMPDIR+"/no_file" ; TMPDIR+"/newest_3" TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/no_file" TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" TMPDIR+"/newest_1"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_3" TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" TMPDIR+"/no_file"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_4" TMPDIR+"/newest_1" ; TMPDIR+"/no_file" TMPDIR+"/newest_3"] ) <> 1 then pause,end

if newest([TMPDIR+"/newest_1" ; TMPDIR+"/no_file" ; TMPDIR+"/newest_3" ; TMPDIR+"/newest_4"] ) <> 4 then pause,end
if newest([TMPDIR+"/no_file" ; TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" ; TMPDIR+"/newest_1"] ) <> 3 then pause,end
if newest([TMPDIR+"/newest_3" ; TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" ; TMPDIR+"/no_file"] ) <> 2 then pause,end
if newest([TMPDIR+"/newest_4" ; TMPDIR+"/newest_1" ; TMPDIR+"/no_file" ; TMPDIR+"/newest_3"] ) <> 1 then pause,end

if execstr('newest(''rrr'',''SCI\bin\libscilab.dll'',3)','errcatch') <> 999 then pause,end
