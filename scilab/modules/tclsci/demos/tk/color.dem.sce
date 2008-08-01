//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

tkpath = get_absolute_file_path("color.dem.sce");

lines(0);
RGB=[];
RGB1=[0 0 0];
TCL_SetVar('rgb',tkpath+'rgb.txt');
TCL_EvalFile(tkpath+'tcolor')
while %t //wait for toplevel to disapear
  if or(RGB<>RGB1) then disp(RGB);RGB1=RGB;end
  TCL_EvalStr('set h [winfo exists .foo]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1);
end

TCL_UnsetVar('label1');TCL_UnsetVar('label2');TCL_UnsetVar('label3');
TCL_UnsetVar('red');TCL_UnsetVar('green');TCL_UnsetVar('blue');
TCL_UnsetVar('colorSpace');TCL_UnsetVar('color');TCL_UnsetVar('updating');
TCL_UnsetVar('autoUpdate');TCL_UnsetVar('name');
