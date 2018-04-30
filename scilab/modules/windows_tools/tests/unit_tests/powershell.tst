// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Allan CORNET
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unit tests for powershell()

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

if getos() == 'Windows' then
  cd('SCI/bin');
  r = powershell('ls');
  assert_checktrue(r~=[]);

  // Path with space & UTF-8 chars in command
  path = TMPDIR+"\mon répertoire";
  mkdir(path);
  mputl("Test bub 14355", path+"\test.txt");

  r = powershell("ls ''"+path+"''");
  assert_checktrue(r~=[]);
  assert_checktrue(grep(r,"/^Mode/","r")~=[]);
  rmdir(path,"s");

  // Date quoted format
  powershell("get-Date -format ''yyyy-MM-dd HH:mm''")
end
