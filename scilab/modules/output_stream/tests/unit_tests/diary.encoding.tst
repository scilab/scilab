//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
  diary(TMPDIR + filesep() + tab_ref(i) + '.diary');
  1+1;
  diary(0);
  if (fileinfo(TMPDIR + filesep() + tab_ref(i) + '.diary') == []) then pause,end
end  
