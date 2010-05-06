// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

consolestatus = 'off';
tkpath = get_absolute_file_path("scroll.dem.sce");

TCL_EvalFile(tkpath+'cscroll')

if getos() == 'Windows' then
   consolestatus = consolebox();
   if consolestatus == 'off' then
     consolebox("on");
   end
end

while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .cscroll]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1);
end

if getos() == 'Windows' then
   consolebox(consolestatus);
end

