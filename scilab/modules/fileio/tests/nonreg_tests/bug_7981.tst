// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7981 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7981
//
// <-- Short Description -->
//  copyfile failed with localized filenames on linux
//

cd TMPDIR;
specialName = '中文';
dirName     = 'dir_'+specialName;
fileName    = 'file_'+specialName;
fileWithExt = 'file_'+specialName+'.ext';

mputl([specialName+'1';specialName+'2'],fileName);

if copyfile(fileName, fileWithExt) == 0 then pause,end;
if (fileinfo(fileWithExt) == []) then pause, end
if ~isfile(fileWithExt) then pause, end
deletefile(fileWithExt);

