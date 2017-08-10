//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

cd(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);


tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"حريات وحقوق",
"תוכנית"];

ref =1:20;

for i = 1 : size(tab_ref,'*')
  pathnam = TMPDIR + filesep() + 'dir_' + tab_ref(i);
  filenam = pathnam + filesep() + 'file_' + tab_ref(i) + '.mat';
  fprintfMat(filenam,ref,'%f');
  r = fscanfMat(filenam);
  if or (r <> ref) then pause,end
end
