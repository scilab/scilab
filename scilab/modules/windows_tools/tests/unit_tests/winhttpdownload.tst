// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

res = winhttpdownload('http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe',TMPDIR+'\scilab-4.1.2.exe');
if ~res then pause,end

[res1,res2]=winhttpdownload('http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe',TMPDIR+'\scilab-4.1.2.exe')
if ~res1 then pause,end
if (res2 <> 0) then pause,end

[res1,res2]=winhttpdownload('http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe_ERROR',TMPDIR+'\scilab-4.1.2.exe');
if res1 then pause,end
if res2 <> -1 then pause,end

[res1,res2]=winhttpdownload('http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe','aa:\scilab-4.1.2.exe');
if res1 then pause,end
if res2 <> -4 then pause,end

