function answer=buttondialog(mes,bt,icon)
// Button dialog 
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.1
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  sciGUI_init()
  //
  rhs=argn(2);
  doerror=%t;
  answer=0;
  if (rhs==0) then
    mes="This is an example"
    bt="quit"
    rhs=2;
  end
  if ((rhs==2)|(rhs==3)) then
    locicon="scilab"
    if ((rhs==3)&(type(icon)==10)) then
      locicon=icon;
    end
    if ((type(mes)==10)&(type(bt)==10)) then
      // do the work
      TCL_EvalStr('set mybutdialoganswertmp [sciGUIButtonDialog -1 ""'+mes+'"" ""'+bt+'"" '+locicon+']');
      while %t
	realtimeinit(0.1);realtime(0);realtime(0.1);
	TCL_EvalStr('set mybutdialogtmp [info exist mybutdialoganswertmp]');
	if TCL_GetVar('mybutdialogtmp')=='1' then break,end;
      end
      answer=TCL_GetVar('mybutdialoganswertmp');
      doerror=%f;
    end
  end

  if doerror then
    error(42);
  end

endfunction

